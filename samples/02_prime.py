print("Checking prime numbers from 3 to 10000")
number = 3
check = 2
for number in list(range(3, 100000)):
    for check in list(range(2, number)):
        if number % check == 0:
            break
    else:
        print(number)

