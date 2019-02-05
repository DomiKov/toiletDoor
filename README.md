# toiletDoor
People say that necessity is the mother of invention. Not sure, it is laziness and this project is a proof of that.

The idea is that you will not have to check the bathroom physically to know whether it is ocuppied or not. Simply check the Node-Red UI to see whether the bathroom is occupied or not. This is great if you live with a lot of people and have a somewhat large house. The project is very simple and also quite quick to make.

What will you need?

This project is using a NodeMCU ESP8266 (https://www.lightinthebox.com/en/p/nodemcu-esp8266-lua-wifi-internet-development-board_p6142020.html) and a Raspberry Pi 3 (but you can also use a Raspberry Zero with wi-fi connection), which has Node-Red running on it. Both the NodeMCU and RPi are connected to a wi-fi and are communicating with each other using MQTT. You will also need some sort of 5V power supply for the NodeMCU. I was using a phone charger with micro USB port for testing. You can use a powerbank as well. 

How it works:

The idea is very simple. If the bathroom door is closed, the bathroom is occupied. If it is open, the bathroom isn't occupied. To determine whether the bathroom doors are closed, a magnetic reed switch will be used. The circuit for the switch cannot get any simpler. 
It is just one 10k ohm resistor and one magnetic switch (like this one https://www.ebay.com/itm/Normally-Open-Closed-Magnetic-Switch-Door-Sensor-Alarm-Home-Window-Contact-Reed-/122892186998). 

How to install:

First, you need to setup your raspberry. I am using the latest release of Raspbian Stretch at the time of writing this (5th February 2019). You need to install Node-Red, unless it has already been installed. If you are not familiar with it, check some tutorials on how to use it. The official Node-Red website is a good source to begin with. You will also need to install mosquitto on your RPi (https://randomnerdtutorials.com/how-to-install-mosquitto-broker-on-raspberry-pi/). Set the Node-Red to start when the RPi boots and setup a static IP address for the RPi as well. Then import the flow (Node-Red-bathroom.txt) into the Node-Red. 

The next step is to load your NodeMCU with the necessary code (Bathroom_door_ArduinoIDE.txt). Use Arduino IDE (or something else if you want, that is compatible with the NodeMCU) to upload the code in the repository. Don't forget to change the necessary things in the code (Wi-Fi name, Password, Server IP and etc.). You will need to download some things as well. The ESP8266 board manager; Adafruit ESP8266 library; Adafruit MQTT library and Adafruit SleepyDog. When you have everything ready, upload the code to the NodeMCU and connect the Switch with a resistor to the NodeMCU. For testing purposes, it is good to use a breadboard and if everything is working, make the circuit on a small veroboard. 

If you have set up and uploaded everything correctly, you should be able to see the bathroom door status via Node-Red UI. If not, try to debug. Use serial monitor in Arduino IDE and debug node in the Node-Red.

Security:

To incorporate a little bit of securit into the MQTT messaging, you can set a username and a password for the RPi mosquitto broker. 

First do this: sudo mosquitto_passwd -c /etc/mosquitto/passwd YourUsername

You will be asked to enter a password for the username you chose.

Second, include this in the mosquitto.conf file. 

allow_anonymous false
password_file /etc/mosquitto/passwd

To access this file, write: sudo nano /etc/mosquitto/mosquitto.conf

This will prohibit others to publish messages or subscribe to your MQTT broker (RPi) unless they provide the right username and password, which means that this username and password will have to be the same on the NodeMCU side.

Conclusion:

Feel free to use this as an inspiration for other fun projects. I plan to expand this by some sort of control from the RPi side (e.g. a buzzer that will be triggered via Node-Red and buzz in the bathroom for a while to let someone in the bathroom know that you are in a hurry and need to use it). Or maybe adding a humidity/temperature sensor. You could also use this as a template for other rooms/things in your house/work/etc.

If you need any help with setting this up, feel free to contact me at my e-mail: dominik.kovacik18@gmail.com; where you also detail your problem. I will try to get to you as soon as I can. 

