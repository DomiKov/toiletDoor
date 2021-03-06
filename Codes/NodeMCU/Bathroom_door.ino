#include <ESP8266WiFi.h> 
#include "Adafruit_MQTT.h" 
#include "Adafruit_MQTT_Client.h" 
/************************* WiFi Access Point *********************************/ 
#define WLAN_SSID       "Your SSID" 
#define WLAN_PASS       "Your PW"

#define MQTT_SERVER      "Your RPi IP addres"   // Best to make your RPi IP addres static (otherwise the NODE
                  // might not connect as the RPi may have a different IP
#define MQTT_PORT        1883                    
#define MQTT_USERNAME    "Your Username"  //Optional
#define MQTT_PASSWORD    "Your Password"  //Optional

#define BUTTON_PIN  D4              // Pin connected to the magnetic switch (D4)
#define BUZZER_PIN  D5      // Pin connected to the buzzer (D5)
/************ Global State ******************/ 

// Create an ESP8266 WiFiClient class to connect to the MQTT server. 
WiFiClient client; 
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details. 
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD); 
/****************************** Feeds ***************************************/ 
// Setup a feed called 'bathroom_door' for publishing. 

// Notice MQTT paths follow the form: <username>/feeds/<feedname> 
Adafruit_MQTT_Publish bathroom_door = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/bathroom/door"); 
// Setup a feed called 'bathroom_buzzer' for subscribing to changes. 

Adafruit_MQTT_Subscribe bathroom_buzzer = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "/bathroom/buzzer"); 
/*************************** Sketch Code ************************************/ 
void MQTT_connect(); 
void setup() { 
 Serial.begin(115200); 
 delay(10); 
 
 // Setup button as an input (with external pull down). 
pinMode(BUTTON_PIN, INPUT); 

 Serial.println(F("RPi-ESP-MQTT")); 
 // Connect to WiFi access point. 
 
 Serial.println(); Serial.println(); 
 Serial.print("Connecting to "); 
 Serial.println(WLAN_SSID); 
 WiFi.begin(WLAN_SSID, WLAN_PASS); 
 while (WiFi.status() != WL_CONNECTED) { 
   delay(500); 
   Serial.print("."); 
 } 
 Serial.println(); 
 Serial.println("WiFi connected"); 
 Serial.println("IP address: "); Serial.println(WiFi.localIP()); 
 // Setup MQTT subscription for bathroom_buzzer feed. 
 mqtt.subscribe(&bathroom_buzzer);
} 

uint32_t x=0; 

void loop() { 
  // Check if the button has been pressed by looking for a change from high to 
 // low signal (with a small delay to debounce). 
 int button_first = digitalRead(BUTTON_PIN); 
 // Ensure the connection to the MQTT server is alive (this will make the first 
 // connection and automatically reconnect when disconnected).  See the MQTT_connect 
 MQTT_connect(); 
 // this is our 'wait for incoming subscription packets' busy subloop 
 // try to spend your time here 
 // Here its read the subscription 
 
 Adafruit_MQTT_Subscribe *subscription; 
 while ((subscription = mqtt.readSubscription())) { 
   if (subscription == &bathroom_buzzer) { 
     char *message = (char *)bathroom_buzzer.lastread; 
     Serial.print(F("Got: ")); 
     Serial.println(message); 
        // Check if the message was ON, OFF, or TOGGLE. 
     if (strncmp(message, "ON", 2) == 0) { 
       // Turn the LED on. 
       digitalWrite(LED_PIN, HIGH); 
     } 
     else if (strncmp(message, "OFF", 3) == 0) { 
       // Turn the LED off. 
       digitalWrite(LED_PIN, LOW); 
     } 
   } 
 } 

 delay(20); 
 //; 
 int button_second = digitalRead(BUTTON_PIN); 
 if ((button_first == LOW) && (button_second == HIGH)){ 
   // Button was pressed!   
   Serial.println("Door Closed."); 
   bathroom_door.publish("Occupied"); 
 } 
 else if ((button_first == HIGH) && (button_second == LOW)){
    Serial.println("Door Open."); 
    bathroom_door.publish("Vacant"); 
 }
} 
// Function to connect and reconnect as necessary to the MQTT server. 

void MQTT_connect() { 
 int8_t ret; 
 // Stop if already connected. 
 if (mqtt.connected()) { 
   return; 
 } 
 Serial.print("Connecting to MQTT... "); 
 uint8_t retries = 10; 
 while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected 
      Serial.println(mqtt.connectErrorString(ret)); 
      Serial.println("Retrying MQTT connection in 5 seconds..."); 
      mqtt.disconnect(); 
      delay(5000);  // wait 5 seconds 
      retries--; 
      if (retries == 0) { 
        // basically die and wait for WDT to reset me 
        while (1); 
      } 
 } 
 Serial.println("MQTT Connected!"); 
} 
