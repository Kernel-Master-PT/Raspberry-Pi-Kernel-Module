
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

# Load Kernel Module
## Insert Kernel Module
```
insmod <MODULE_NAME>.ko
```
## List Kernel Modules
```
lsmod
```
## Remove Kernel Module
```
rmmod <MODULE_NAME>
```

# GPIO Subsystem Observation
## Install gpiod
```
sudo apt install gpiod
```

