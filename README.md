
# Xsens MTi ROS Driver and Ntrip Client

This code was based on the official ROS Driver and tested on MTi-680.
#### Note: the Pvt Data needs to be enabled: MT Manager - Device Settings - Output Configuration - GNSS Data , select "Pvt Data", click "Apply"

## Changes made to the MTi ROS Driver:

 - add ntrip_util.h and ntrip_util.cpp under lib/xspublic/xscommon
 - add nmeapublisher.h under src/messagepublisher folder, to send GPGGA message, /nmea rostopic.

change:
 - lib/xspublic/xscontroller/iointerface.h, line 138, change to PO_OneStopBIt for PO_XsensDEfaults.
 - lib/xspublic/xscommon/threading.cpp, line 387 to 408, change the threading behavior, this will be useful for ubuntu 22 OS.

## Ntrip_Client
The Ntrip_client subscribes to the /nmea rostopic from MTi ROS Node, and wait until it gets data, it will send GPGGA to the Ntrip Caster(Server) every 10 seconds.

User needs to change the ntrip.launch for their own credentials/servers/mountpoint. 

## How to use:
clone the source file to your catkin_ws
```
cd ~/catkin_ws
pushd src/xsens_ros_mti_driver/lib/xspublic && make && popd
catkin_make
```
and then open two terminals:
```
roslaunch xsens_mti_driver xsens_mti_node.launch
```
and then
```
roslaunch ntrip ntrip.launch
```

