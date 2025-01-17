# Getting Started Guide for STM32U5 IoT Discovery Kit with AWS

## Introduction

This project demonstrates how to integrate modular FreeRTOS software with hardware enforced security to help secure updatable cloud connected applications. The project is pre-configured to run on the STM32U585 IoT Discovery Kit and connect to AWS.

The [STM32U585 IoT Discovery kit](https://www.st.com/en/evaluation-tools/b-u585i-iot02a.html) comes with 2048 KB of Flash memory, 786 kB of RAM and is based on Arm Cortex®-M33.

The STM32U5 IoT Discovery Kit is equipped with a Wi-Fi and Bluetooth module, microphones, a temperature and humidity sensor, a magnetometer, an accelerometer and gyroscope, a pressure sensor, as well as Time-of-Flight (ToF) and gesture-detection sensors.

The board also comes with 512-Mbit octal-SPI Flash memory, 64-Mbit octal-SPI PSRAM, 256-Kbit I2C EEPROM, as well as ARDUINO Uno V3, STMod+, and Pmod expansion connectors, plus an expansion connector for a camera module, and STLink-V3E embedded debugger.

The following project folder consists of a **Non-TrustZone version(b_u585i_iot02a_ntz)** of the project and **TF-M enabled version(b_u585i_iot02a_tfm)** of the project. The following shows the steps to connect the STM32U585 IoT Discovery kit to AWS IoT core and utilize the FreeRTOS OTA Update service. The demo connects to AWS IoT using the WiFi module. It then uses the coreMQTT-Agent library to enable multiple concurrent tasks to share a single MQTT connection. These tasks publish sensor data, and demonstrate use of the Device Shadow and Device Defender AWS IoT services.

## Hardware Description

https://www.st.com/en/microcontrollers-microprocessors/stm32u5-series.html

##  User Provided items

A USB micro-B cable

## Clone the repository and submodules

Using your favorite unix-like console application, run the following commands to clone and initialize the git repository and its submodules.
 If using Microsoft Windows, path length limitations may apply so it is recommended to clone the respository to the root of a drive to minimize the path length of each file.

```
git clone https://github.com/FreeRTOS/lab-iot-reference-stm32u5.git
git -C lab-iot-reference-stm32u5 submodule update --init
```

## Set up your Development Environment

Download and install [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html#get-software) Version 1.9.0.
Download and install the latest version of [Python](https://www.python.org/downloads/).
During the installation process for python, make sure to tick the boxes to install pip and add python to path.
To install python libraries using pip, navigate to the repository (C:\lab-iot-reference-stm32u5\tools) and type:

```
pip install -r requirements.txt
```
The above command will install the following packages-boto3,requests,pyserial,cryptography and black required for the build.

Install [AWS CLI](https://docs.aws.amazon.com/cli/latest/userguide/getting-started-install.html).

Create an [IAM user](https://docs.aws.amazon.com/IAM/latest/UserGuide/id_users_create.html).

Type :

```
aws configure
```
on a command prompt terminal. Fill in the AWS Access Key ID, AWS Secret Access Key, Default output format and Region as show below:

 <img width="371" alt="12" src="https://user-images.githubusercontent.com/44592967/153652474-eaa0f45e-654f-4eb0-986e-edce6d1af53f.PNG">

Optional: A serial terminal like [TeraTerm](https://osdn.net/projects/ttssh2/releases/)

## Set up your hardware

![image](https://user-images.githubusercontent.com/44592967/162077566-531f1bf3-d974-44ef-9409-06df1615cfd0.png)

Connect the ST-LINK USB port (USB STLK / CN8) to the PC with USB cable.  The USB STLK port is located to the right of the MXCHIP module in the above figure. It is used for power supply, programming the application in flash memory, and interacting with the application with virtual serial COM port.

## Running the TFM-Enabled and Non-TrustZone projects

For getting the Non Trust Zone project up and running follow the README.md in
Projects\b_u585i_iot02a_ntz by clicking [here](Projects/b_u585i_iot02a_ntz/Readme.md).

For getting the Trust Zone project up and running follow the README.md in
Projects/b_u585i_iot02a_tfm by clicking [here](Projects/b_u585i_iot02a_tfm/Readme.md).

## Component Licensing

Source code located in the *Projects*, *Common*, *Middleware/AWS*, and *Middleware/FreeRTOS* are available under the terms of the MIT License. See the LICENSE file for more details.

Other libraries located in the *Drivers* and *Middleware* directories are available under the terms specified in each source file.
