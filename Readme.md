# MAVKit

## Introduction

MAVkit is a C++ toolbox for **MAVLink 2.0** in command line.

 It can **display**, **redirect**, **broadcast**, **log**, **replay** data from and to different types of interface (**Serial**, **UDP**, **TCP**, **File**).

MAVkit is also a framework for MAVLink programmers.
It is built on a modular architecture where each module can be used independently inside other projects.
In that case, it provides a simple MAVLink block that can be replaced according to communication hardware.

Designed with low latency in mind, it keeps IO buffers safe from overflows (leading to data loss) by using a dual thread processing loop per interface (fast read and process).

## Build

### Submodule update
>Note : Be sure to use the following recursive command, there is two submodule levels in mavlink.

```Shell
git submodule update --init --recursive
```

### Compilation
***Dependencies*** : Boost + MAVLink usual dependencies

```shell
mkdir build
cd build
cmake ..
make
```

## How it works ?

Mavkit is organised in modules called **messengers**. A messenger can receive and send messages to others in a fully concurrent way.

Each messenger can be either a serial link, an UDP socket, a log file,...

you have to specify at least one messenger (the first) for the role of **master**.
This particular one will be duplex connected to every other messenger created then.
On the contrary, secondary messengers won't be connected between them.

Here is an example of two MAVkit instances, composed of respectively 4 and 2 messengers and linked between them through UDP. Note that masters are connected to all other messengers, while those are not linked between them.

<img src="/docs/mavkit_interfaces.jpg" />

## How to use it ?

Once you've built it, you can run MAVKit from command line by specifying messengers one after the other.

The first messenger will take on the role of master.

### Serial
***Arguments :***
* ***device*** -> path of a tty device.
* ***baudrate*** -> baudrate.

ex : `--tty /dev/ttyUSB0 57600`

### UDP server
***Arguments :***
* ***port*** -> listening port.

ex : `--udp_server 14550`

### UDP client
***Arguments :***
* ***IP address*** -> target IP address.
* ***port*** -> target port.

`--udp_client 192.168.1.10 14550`

### TCP server
***Arguments :***
* ***port*** -> listening port.

ex : `--tcp_server 14550`

### TCP client
***Arguments :***
* ***IP address*** -> target IP address.
* ***port*** -> target port.

`--tcp_client 192.168.1.10 14550`

### Logger
Save received messages in .raw and .ts file (.raw contains the mavlink raw data, .ts contains timestamp for each message).

***No arguments.***

`--log`

### File
Replay content of .raw and .ts files.

***Arguments :***
* ***file prefix*** -> logs are made of two files (.raw and .ts), both should be located on the same place for mavkit to find them. Example : dir/log will look for dir/log.raw and dir/log.ts files.
* ***speed multiplier*** -> floating point multiplier for the reading speed.
* ***starting time*** -> time to start in the file in seconds.

ex : `--file ../myLog 2.0 50`

### display
Output messages to stdout.

***No arguments.***

`--display`

---
You can combine almost as many messengers as you want in command line.

Example :

```Shell
./mavkit --tty /dev/ttyACM0 57600 --udp_server 14550 --display --log --tcp_client 127.0.0.1 14551
```
