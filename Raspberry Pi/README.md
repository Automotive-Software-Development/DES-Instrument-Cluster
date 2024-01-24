# Raspberry Pi Setup

- ### Download Raspbian
    Rasbian Lite(Server) 64-Bit : latest version but update raspi-firmware version 5.15.84

- ### Update the Raspberrypi Firmware
    ```
    sudo apt-get update
    sudo apt-get dist-upgrade
    sudo reboot
    sudo rpi-update (firmware_version commit full SHA)
    sudo reboot
    ```

- ### Install Dependencies
    ```
    sudo apt update
    sudo apt install \
        gcc \
        v4l-utils \
        i2c-tools \
        raspi-config \
        python3-dev \
        python3-setuptools \
        libopencv-dev \
        python3-pip \
    sudo apt upgrade
    sudo reboot
    ```

- ### Enable i2c and camera
    Use the raspi-config tool to enable the following peripherals:

    i2c: Interface Options > I2C
    Camera: Interface Options > Camera
    Afterwards, reboot:
    ```
    sudo reboot
    ```

- ### Install libraries
    1. #### bcm2835
        ``` 
        wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.60.tar.gz
        tar zxvf bcm2835-1.60.tar.gz
        cd bcm2835-1.60/
        sudo ./configure
        sudo make
        sudo make check
        sudo make install
        ```

    1. #### wiringpi
        ```
        #Open the Raspberry Pi terminal and run the following command
        cd
        
        sudo apt-get install wiringpi
        #For Raspberry Pi systems after May 2019 (earlier than that can be executed without), an upgrade may be required:
        wget https://project-downloads.drogon.net/wiringpi-latest.deb
        sudo dpkg -i wiringpi-latest.deb

        gpio -v
        # Run gpio -v and version 2.52 will appear, if it doesn't it means there was an installation error

        # Bullseye branch system using the following command:
        git clone https://github.com/WiringPi/WiringPi
        cd WiringPi
        . /build
        sudo gpio -v
        # Run gpio -v and version 2.70 will appear, if it doesn't it means there was an installation error
        ```

    1. #### python
        ```
        sudo apt-get update
        sudo apt-get install python3-pip
        sudo apt-get install python3-pil
        sudo apt-get install python3-numpy
        sudo pip3 install RPi.GPIO
        sudo pip3 install spidev
        sudo pip3 install python-can
        ```

- ### Preparation
    1. #### Insert the module into Raspberry Pi, and then modify the config.txt file:
        ```
        sudo nano /boot/config.txt
        ```

    1. #### Append these statements to the file:
        ```
        dtparam=spi=on
        dtoverlay=mcp2515-can1,oscillator=16000000,interrupt=25
        dtoverlay=mcp2515-can0,oscillator=16000000,interrupt=23
        dtoverlay=spi-bcm2835-overlay
        dtparam=spi=on
        dtoverlay=spi1-3cs
        dtoverlay=mcp251xfd,spi0-0,interrupt=25
        dtoverlay=mcp251xfd,spi1-0,interrupt=24
        ```

    1. #### Save and exit, then restart your Pi:
        ```
        sudo reboot
        ```

    1. #### After restart, check if initialize successfully:
        ```
        sudo dmesg | grep spi
        ```
    
    1. #### Set Up CAN Interfaces
        ```
        sudo ip link set can0 up type can bitrate 1000000
        sudo ip link set can1 up type can bitrate 1000000
        sudo ifconfig can0 txqueuelen 65536
        sudo ifconfig can1 txqueuelen 65536
        ```
    
    1. #### Verify Network Interfaces
    
        ```
        ifconfig
        ```
    
    1. #### Install can-utils (Optional)
    
        ```
        sudo apt-get install can-utils
        ```
    
    1. #### Test CAN Communication
    
        ```
        candump can0
        // If above line is not working, then do
        candump can1
        ```

- ### Install Piracer-py package
    ```
    pip install piracer-py
    ```

- ### 7.9-inch Touchable Display

    1. #### Download and enter the Waveshare-DSI-LCD driver folder

        ``` 
        git clone https://github.com/waveshare/Waveshare-DSI-LCD
        cd Waveshare-DSI-LCD
        ```

    1. #### Enter uname -a in the terminal to view the kernel version and cd to the corresponding file directory

        ```
        cd 5.15.84
        ```

    1. #### Please check the bits of your system, enter the 32 directory for 32-bit systems, and enter the 64 directory for 64-bit systems

        ```
        cd 64
        ```

    1. #### Enter your corresponding model command to install the driver, pay attention to the selection of the I2C DIP switch

        ```
        #7.9inch DSI LCD 400×1280 driver：
        sudo bash ./WS_xinchDSI_MAIN.sh 79 I2C0
        ```

    1. #### Wait for a few seconds, when the driver installation is complete and no error is prompted, restart and load the DSI driver and it can be used normally

        ```
        sudo reboot
        ```