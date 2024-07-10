# MCU Test for PineTime

This is a test of the FOTA for PineTime using Zephyr and nRF Connect. It's based on the Blinky example. I am using two build configurations to test:
- pinetime_devkit0
- nrf52dk_nrf52832

The goal is to update a PineTime DevKit OTA from the nrfConnect mobile app.


## PineTime Errors/Resolutions

One problem is that the PineTime board enables the 'xt25fb32' flash NOR device on the spi1 bus by default in the devicetree, and this will cause a linker error is a result of the SPI driver not being included in the bootloader build.

Assuming you don't want to use this FLASH NOR device in your bootloader, you can create a devicetree overlay to disable the device for mcuboot. To apply a devicetree overlay for the child image, you can create a folder named 'chilld_image' in your project root with the 'mcuboot.overlay' file. Folder structure should be as follows (Permanent configuration changes to child images):

blinky_ble/child_image/
└── mcuboot.overlay

```
/delete-node/ &xt25fb32;

&spi1 {
    status = "disabled"; 
};
```