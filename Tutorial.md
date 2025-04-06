# STM32-based Smart Home Conditioning System
Team:  Zheng Zhang <3036381242> 
Yi Zhao <3036410445>
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
Python Environment 

##  Step-by-Step Guide
### Hardware
<a name="ole_link5"></a>**1. Hardware Connection**

1. Temperature and Humidity Sensor Connection 

Connect to the sensor dock reserved by the stm32f103ze motherboard

2. Light Intensity Sensor Connection 

Use the photoresistor that comes with the stm32f103ze motherboard

3. Fan Control 

Fan <a name="ole_link7"></a>interface FI connected to microcontroller PA8

Fan interface BI connected to microcontroller GND

Fan interface VCC <a name="ole_link8"></a>connected to microcontroller 5V

Fan interface GND connected to microcontroller GND

4. LED Connection 

LED <a name="ole_link9"></a>interface connected to microcontroller PA7

The other LED interface connected to microcontroller GND

5. <a name="ole_link6"></a>Other Connection 

The power supply port of the microcontroller is connected to the computer for power supply and serial port monitoring

Connect ST-Link to program recording

6. LCD Connection 

LCD is connected to the microcontroller through the microcontroller reserved interface

**2. Compilation and Download**

1. Compile the project in STM32 IDE
1. Connect STM32 to the computer via ST-Link
1. Download the program to STM32

**3. Debugging and Parameter Adjustment**

1. Open the serial debugging assistant and set the baud rate to 115200
2. Observe the temperature, humidity, and light intensity data
3. Adjust the TEMP\_THRESHOLD and LIGHT\_THRESHOLD values according to actual conditions

### Web

IMPORT:
```Python
import streamlit as st
import pandas as pd
import plotly.express as px
import dashscope
from dashscope import Generation
import os
import requests
```



*Export the exported data to CVS format using python for easy subsequent reading  
*The streamlit framework is used to complete the display of temperature, humidity and brightness of the web front-end, and the optimal indoor environment is set according to the data for analysis and comparison  
*Access to the Hong Kong Observatory's public API enables real-time outdoor temperature and humidity collection for easy comparison with indoor  
*Access AI to form a chat bot, which is convenient for users to answer questions and give reasonable suggestions in real time when using the system  


USE AI MODEL:

```Python
api_key=os.getenv("DASHSCOPE_API_KEY"),
base_url="https://dashscope.aliyuncs.com/compatible-mode/v1",
```


## Conclusion

In this project, we developed a smart home system to enhance the user experience through real-time monitoring of environmental data and intelligent control of devices. The STM32 single chip microcomputer is the core of the system, which integrates temperature, humidity and light intensity sensors for environmental monitoring. The relay module controls lamps, fans and other equipment to help users create a more comfortable living environment.  
We use the Streamlit framework to create an intuitive Web front end that displays environmental data and compares it to ideal indoor conditions to help users understand the state of the environment. In addition, the system is connected to the Hong Kong Observatory's public API to collect real-time outdoor environment data, which is convenient for users to compare with the indoor environment, so as to make more reasonable home adjustments. The introduction of artificial intelligence further improves the function of the system. AI can answer user questions in real time and provide personalized recommendations based on environmental data and user preferences. This interactive feature not only improves user satisfaction, but also helps users improve their living environment through timely suggestions and reminders.  
Overall, this smart home system combines the advantages of hardware and software to provide users with a responsive and easy-to-use environment. In the future, the types of sensors and AI functions can be further optimized to achieve real-time data transmission and improve the convenience and practicality of the system.  

##  References
Use streamlit for front-end presentation:
https://streamlit.io/

Use the Hong Kong Observatory API：
https://data.weather.gov.hk/weatherAPI/opendata/weather.php?dataType=rhrread&lang=en

Use QWEN-7B model:
https://bailian.console.aliyun.com/?spm=5176.29597918.nav-v2-dropdown-menu-0.d_main_0_6.1eda7ca0LHZlDW&scm=20140722.M_10793437._.V_1#/model-market/detail/qwen-7b-chat


##  Acknowledgement
### Contribution Statement 
||  Zheng Zhang   | Yi Zhao  |
|  :----: | :----: |:----: |
|Code ownership| [hardware](https://github.com/Yeyeyolo/AIOT-Tutorial-Writing/tree/main/aiot-hardware)| [web](https://github.com/Yeyeyolo/AIOT-Tutorial-Writing/tree/main/aiot-web) |
|video| Shoot | Edit |
|Report| Hardware part | Others |
|Github | - | Build and Improve|
