# Linux Character Device Driver

A simple Linux character device driver `/dev/mosvector` that returns 'Hello, World!'

## TL;DR

```shell
make && make install
sudo modprobe mosvector
cat /dev/mosvector
sudo modprobe -r mosvector
```
