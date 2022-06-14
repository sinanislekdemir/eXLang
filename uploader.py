import glob
import sys
import threading
from time import sleep
from typing import List

import click
import serial

socket = None
active = True
ready = False
device = ''


def serial_ports() -> List[str]:
    """Get list of serial ports."""
    if sys.platform.startswith("win"):
        ports = ["COM%s" % (i + 1) for i in range(256)]
    elif sys.platform.startswith("linux") or sys.platform.startswith("cygwin"):
        ports = glob.glob("/dev/tty[A-Za-z]*")
    elif sys.platform.startswith("darwin"):
        ports = glob.glob("/dev/tty.*")
    else:
        raise EnvironmentError("Unsupported platform")

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result


def reader():
    """Reader thread."""
    global ready
    global device
    buffer = ""
    while active:
        if socket is not None:
            while socket.in_waiting:
                data = socket.read(size=1)
                data_s = data.decode("ascii", errors="ignore")
                print(data_s, end="")
                buffer += data_s

            if buffer.endswith("\n"):
                buffer = buffer.replace("\r", "")
                if 'ATMEL' in buffer:
                    device = 'ATMEL'
                if "ESP32" in buffer:
                    device = 'ESP32'
                if "End program with a single dot" in buffer:
                    ready = True
                buffer = ""
            sys.stdout.flush()


def writer():
    """Writer thread."""
    global active
    while active:
        c = input("")
        if socket is not None:
            socket.write(bytes(c + "\n", "ascii"))
        if c == "quit":
            active = False


@click.command("upload")
@click.option("--port", default="", help="Device address")
@click.option("--filename", help="File to upload")
def upload(port: str, filename: str):
    """Upload file to device."""
    global socket
    global active
    global ready

    r = threading.Thread(target=reader)
    r.start()
    if port == "":
        port = serial_ports()[0]

    socket = serial.Serial(port=port)
    print("Waiting for connection")
    print("Sending comment signal")
    socket.write(bytes("#\n", "ascii"))
    print("Waiting...")
    while not ready:
        sleep(0.1)

    print(f"Connected device [{device}]")

    count = 0
    for fname in filename.split(","):
        if fname == "":
            continue
        print(f"Sending file [{fname}] on port [{port}]")
        data = ""
        with open(fname, "r") as f:
            data = f.read()

        for line in data.splitlines():
            socket.write(bytes(line + "\n", "ascii"))
            socket.flush()
            sleep(0.10)
            if line == "---":
                sleep(1)
        socket.write(bytes("\n.\n", "ascii"))
        socket.flush()
        print("Program sent")
        count += 1
    num_progs = 4
    if device == 'ESP32':
        num_progs = 8
    for i in range(count, num_progs):
        socket.write(bytes(".\n", "ascii"))
        socket.flush()
    w = threading.Thread(target=writer)
    w.start()
    r.join()
    w.join()


@click.command("list")
def list_ports():
    """List serial ports."""
    print("Serial ports:")
    for s in serial_ports():
        print(s)


@click.group()
def entry_point():
    """Click entrypoing placeholder."""
    pass


entry_point.add_command(list_ports)
entry_point.add_command(upload)
if __name__ == "__main__":
    entry_point()
