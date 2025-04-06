# STM32-based Smart Home Conditioning System
Team: Zheng Zhang,3036381242; Yi Zhao,3036410445
## Introduction
This project developed an indoor smart home system to collect data such as temperature, humidity, brightness, etc. And control desk lamps and fans and other devices to provide users with a better home experience. The key technologies involved include STM32 microcontrollers, various sensors for environmental monitoring, and relay modules for device control. In addition, on the web side, streamlit is used to visualize and analyze the collected data, while integrating artificial intelligence to enhance the user experience and promote health.
##  Prerequisites:
### Hardware
**Required knowledge:**

1. Basic knowledge of C programming 
2. Fundamentals of microcontroller programming

**Tools:**

1. STM32 IDE (Keil MDK) 
2. Serial debugging assistant software (XCOM) 

**Equipment：**

1. STM32 microcontroller (STM32F103ZE) 
2. Temperature and humidity sensor (DHT11) 
3. Light intensity sensor (Photoresistor) 
4. Fan module 
5. LED light 
6. Jumper wires 
7. LCD Display Mode

### Web
##  Step-by-Step Guide
### Hardware
<a name="ole_link5"></a>**1. Hardware Connection**

1. Temperature and Humidity Sensor Connection 

Connect to the sensor dock reserved by the stm32f103ze motherboard

1. Light Intensity Sensor Connection 

Use the photoresistor that comes with the stm32f103ze motherboard

1. Fan Control 

Fan <a name="ole_link7"></a>interface FI connected to microcontroller PA8

Fan interface BI connected to microcontroller GND

Fan interface VCC <a name="ole_link8"></a>connected to microcontroller 5V

Fan interface GND connected to microcontroller GND

1. LED Connection 

LED <a name="ole_link9"></a>interface connected to microcontroller PA7

The other LED interface connected to microcontroller GND

1. <a name="ole_link6"></a>Other Connection 

The power supply port of the microcontroller is connected to the computer for power supply and serial port monitoring

Connect ST-Link to program recording

6 LCD Connection 

LCD is connected to the microcontroller through the microcontroller reserved interface

**2. Hardware Connection**

1\. System Initialization

2\. Fan motor initialization

3\. DHT11 Temperature/Humidity Sensor Initialization and Reading

4\. Environmental Control Logic

5\. Display Implementation

**3. Compilation and Download**

1. Compile the project in STM32 IDE
1. Connect STM32 to the computer via ST-Link
1. Download the program to STM32

**4. Debugging and Parameter Adjustment**

1. Open the serial debugging assistant and set the baud rate to 115200
1. Observe the temperature, humidity, and light intensity data
1. Adjust the TEMP\_THRESHOLD and LIGHT\_THRESHOLD values according to actual conditions

### Web

## Conclusion
##  References
https://streamlit.io/
##  Acknowledgement
### Contribution Statement 
||  Zheng Zhang   | Yi Zhao  |
|  :----: | :----: |:----: |
|Code ownership| [hardware](https://github.com/Yeyeyolo/AIOT-Tutorial-Writing/tree/main/aiot-hardware)| [web](https://github.com/Yeyeyolo/AIOT-Tutorial-Writing/tree/main/aiot-web) |
|| 单元格  | 单元格 |
