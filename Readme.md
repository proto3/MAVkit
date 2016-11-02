# MAVKit

## build

```shell
mkdir build
cd build
cmake ..
make
```
## how to use it ?

Once you've built it, you can run MAVKit with either a serial device path or an IP.

```shell
# Example
./mavkit 192.168.1.xxx
# or
./mavkit /dev/ttyUSB0
```
***Note: in IP mode the port is not accessible yet through command line, please look at MavlinkUDP constructor.***

This is what messages display looks like. Only a few messages are handled, but one can add new messages easily.

```
Ready
heartbeat
    type:            6
    autopilot:       8
    base_mode:       0
    custom_mode:     0
    system_status:   0
    mavlink_version: 3
----------------------
param request list
----------------------
heartbeat
    type:            6
    autopilot:       8
    base_mode:       0
...
```
