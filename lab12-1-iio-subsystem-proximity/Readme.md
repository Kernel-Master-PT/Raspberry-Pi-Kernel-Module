
# Put Device Tree on device with overlay
## Compile Deveice Tree Blob
```
dtc -@ -I dts -O dtb -o <BLOB_NAME>.dtbo <FILE_NAME>.dts
```

## Put Blob in `/boot` directory
```
cp <BLOB_NAME>.dtbo /boot/overlays/
```

## Modify Boot Configuration
```
vi /boot/config.txt
---
+dtoverlay=arduino-pinctrl #arduino-pinctrl is device tree node
```

## Check Device Tree on Device
```
dtc -I fs /proc/device-tree -O dts
```

# Load IIO Module
## Load IIO Dependency Module
```
sudo modprobe industrialio-triggered-buffer
```
## Load iio-constant Module
```
sudo insmod iio-constant.ko
```

# Observation
## sysfs
```
ls /sys/bus/iio/devices/iio\:device1
---
in_proximity0_input  in_proximity1_input  in_proximity2_input  in_proximity3_input  in_proximity4_input  in_proximity5_input  name  of_node  power  subsystem  uevent
```

## Show Values
```
cat in_proximity{0..5}_input
---
9876
9876.001234
9876.001234 dB
9876 1234 
8.003241491
0.000000000
```
