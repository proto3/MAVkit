# MAVKit

## build

```shell
mkdir build
cd build
cmake ..
make
```

## How it works ?

Mavkit is organised in modules called interfaces. An interface can receive and send messages to other interfaces in a fully concurrent way.

Each interface can be either a serial link, an UDP socket, a log file,...

you have to specify at least one interface that will be called ***master interface***.
This particular one will be duplex connected to every other interface created then.
On the contrary, secondary interfaces won't be connected between them.

Here is an example of two mavkit instances, composed of respectively 4 and 2 interfaces and linked between them through UDP. Note that masters are connected to all other interfaces, while those are not linked between them.

<img src="/docs/mavkit_interfaces.jpg" />

## how to use it ?

Once you've built it, you can run MAVKit from command line by specifying interfaces one after the other.
The first interface will take on the role of master.

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
You can combine almost as many interfaces as you want with the power of command line.

```Shell
./mavkit --tty /dev/ttyACM0 57600 --upd_server 14550 --display --log --tcp_client 127.0.0.1 14551
```
