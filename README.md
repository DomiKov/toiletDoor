# toiletDoor
A school project that could be utilized at home.

People say that necessity is the mother of invention. Not ture, it is laziness and this project is a proof of that.

The idea is that you will not have to check the bathroom physically to know whether it is ocuppied or not. Simply use your raspberry to run a programme that will tell you the bathroom status. The main reason for doing this is that the house where I live is relatively large and it has 6 tennats with just 1 bathroom. This way I will not have to walk down all the way from my room to the bathroom only to find out that one of my roommates is taking a shower. 

First you have to pull the Toilet door folder as it has both the RPI and AVR codes. Then you have to upload the AVR to your Atmega. Note:This project is using Atmega328P. If you are using a different Atmega, you will most probably have to make some changes to the code. 
You can also use an Arduino board as it also utilizes the Atmega328 chip. If necessary I can upload a file with instructions on how to use Arduino with Atmel Studio. 

Once you have uploaded the code to your Atmega, connect everything the LEDs (pay in mind that you also have to connect an appropriate resistor with the LED). To evaluate the door state / event, I was using a magnetic reed switch. This could be also applied in a real-life scenario, however, a button or a regular switch can be used for demonstration purposes. Do not forget to add a pull-down resistor to the switch. 

With the LED's and switches connected, you should be able to try out the functionality of the programme. If the doors are open (both switches are off) no LED is turned on. If the doors are closed (door-switch is on) the door-led is turned on. If the doors are locked (both door-switch and lock-switch is on), both the door-led and lock-led are turned on. 
Note: In a real-life application, the lock-switch would not work. A different solution would have to be implemented.
Note #2: The software does not account for the situation where the doors are opened, yet still locked. This is due to the fact that in real-life application, this should not be possible. 

To make the raspberry part work, you will need to install a bcm2835.h library. This library allows, among other things, to access the SPI on the raspberry pi. It can be downloaded from here:http://www.airspayce.com/mikem/bcm2835/index.html by following the instructions on the website. The website also has several examples, including examples for SPI usage. Additionally, if you are having problems with making the SPI work on your RPi, it may be due to the fact that the SPI is disabled on your RPi. By typing sudo raspi-config you should be able to access the configuration panel of your raspberry. In there, you will have to enable your SPI in Interface options. 

Once you have the library installed, you will have to copy the Toiletdoor_rpi.c code into your raspberry. You will need to first compile the code in order to make it run. If you keep the same file name, the compilation command should look like so:

gcc -o Toiletdoor_rpi Toiletdoor_rpi.c -l bcm2835

The last step is to connect the Atmega with the RPi. The connections should be as follows:

RPi                                        AVR
GPIO 10 - SPI0_MOSI                         PINB3 - MOSI
GPIO 9  - SPI0_MISO                         PINB4 - MISO
GPIO 11 - SPI0_SCLK                         PINB5 - SCK
GPIO 8  - SPI0_CS0_N                        PINB2 - SS'
GND                                         GND         - Grounds have to be common. 

Once this is connected, you should be able to receive the door status at on the RPi by executing the programme on the rasberry pi:

sudo ./Toiletdoor_rpi


As of now, the only real-life application would be the door-open / door-close status. Unfortunately, I have not made it work wirelessly yet. Once I will, I will update this repository. Hopefully, it will also be running on Node-RED at that time. 

