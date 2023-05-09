FROM ubuntu:latest

RUN apt-get update -y
RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get install -y tzdata
RUN apt-get install fish gcc-11 libstdc++-11-dev g++-11 cmake ccls clang-tidy software-properties-common git gcovr -y
RUN apt-add-repository ppa:ubuntu-elisp/ppa
RUN apt-get update -y
RUN apt-get install emacs-snapshot-nox -y
COPY emacs /root/.emacs
RUN emacs -l /root/.emacs --batch
RUN ln -s /usr/bin/g++-11 /usr/bin/g++