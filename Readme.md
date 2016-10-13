# MAVKit

## build

```shell
mkdir build
cd build
cmake ..
make
```
## how to use it ?

Once you've built it, simply run **./mavkit**

Options are not implemented yet, please look into the main code to choose between UDP and serial implementation.

The UDP and Serial constructors allow you to choose which IP, port or UART you want to speak with.


```
lucas@lucas-desktop:~/projects/mavlink_utils/mavkit/build$ ./mavkit
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
