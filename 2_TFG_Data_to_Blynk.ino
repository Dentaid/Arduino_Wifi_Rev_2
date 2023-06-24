// ----------LIBRARIES--------------
#include "DFRobot_BloodOxygen_S.h"  // Required libraries
#include "Wire.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>


// --------CONSTANTS (won't change)---------------
char ssid[] = "AP Monitoring";  // Your WiFi credentials.
char pass[] = "12345678";


//------------ VARIABLES (will change)---------------------
BlynkTimer timer; //Timer

float HR; // Variables to acquire the sensor data
float SpO2;


// ----------I2C--------------
#define I2C_COMMUNICATION  //use I2C for communication

// **********SpO2**************
#define I2C_ADDRESS    0x57
DFRobot_BloodOxygen_S_I2C MAX30102(&Wire ,I2C_ADDRESS);


// ----------Blynk-------------- 
/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL5UCL6CaKS"
#define BLYNK_TEMPLATE_NAME         "SpO2 and HR Monitoring"
#define BLYNK_AUTH_TOKEN            "3sxZCCs0wfNahzGdHjh5W0Xu1i-GIn02"


// ----------SETUP--------------
void setup()
{
  Serial.begin(115200);  // initialize the serial communication:

  //****************BLYNK****************//
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println(" and Blynk server...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);  // You can also specify server:
                                              // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
                                              // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);
  Serial.println("Connected!");

  //****************SpO2****************
  while (false == MAX30102.begin()){
    Serial.println("SpO2 init failed, Please try again!");
    delay(1000);
  }
  Serial.println("SpO2 init success!");
  Serial.println("SpO2 start measuring...");
  MAX30102.sensorStartCollect();  // Initialize the SpO2 sensor
  
  //****************TIMER****************
  // Setup a function to be called every half a second
  timer.setInterval(500L, myTimerEvent);
}


// ----------LOOP--------------
void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}


// ----------TIMER_EVENT--------------
// This function sends Arduino's up time every second to Virtual Pin 0 and 1.
void myTimerEvent()
{
  MAX30102.getHeartbeatSPO2();
  SpO2=MAX30102._sHeartbeatSPO2.SPO2;
  HR=MAX30102._sHeartbeatSPO2.Heartbeat;

  Blynk.virtualWrite(V0, SpO2);
  Blynk.virtualWrite(V1, HR);
}
