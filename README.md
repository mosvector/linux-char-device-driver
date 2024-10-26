# Linux Character Device Driver

A simple Linux character device driver `/dev/mosvector` that returns 'Hello, World!'

## TL;DR

```shell
make && make install
sudo modprobe mosvector
cat /dev/mosvector
sudo modprobe -r mosvector
```

## Dev Environment

```shell
$ uname -a
Linux RaspberryPi 6.6.31+rpt-rpi-2712 #1 SMP PREEMPT Debian 1:6.6.31-1+rpt1 (2024-05-29) aarch64 GNU/Linux
$ sudo apt install raspberrypi-kernel-headers
$ make --version
GNU Make 4.3
Built for aarch64-unknown-linux-gnu
$ gcc --version
gcc (Debian 12.2.0-14) 12.2.0
```
