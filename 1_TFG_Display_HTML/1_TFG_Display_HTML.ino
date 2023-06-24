// ----------LIBRARIES--------------
#include "DFRobot_BloodOxygen_S.h"  // Required libraries
#include "Wire.h"
#include <SPI.h>
#include <WiFiNINA.h>


// --------CONSTANTS (won't change)---------------
const int SpO2Interval = 4000; // Number of millisecs between SpO2 data

char ssid[] = "AP Monitoring";
char pass[] = "12345678"; // Connection constants
int keyIndex = 0;
WiFiServer server(80);


//------------ VARIABLES (will change)---------------------
unsigned long currentMillis = 0;    // Stores the value of millis() in each iteration of loop()
unsigned long previousSpO2Millis = 0;   // Will store last time the SpO2 was updated
int status = WL_IDLE_STATUS;

float HR;
float SpO2; // Variables to acquire the sensor data
float Temp;


// ----------I2C--------------
#define I2C_COMMUNICATION  // Use I2C for communication

// **********SpO2**************
#define I2C_ADDRESS    0x57
DFRobot_BloodOxygen_S_I2C MAX30102(&Wire ,I2C_ADDRESS);


// ----------SETUP--------------
void setup() 
{
  Serial.begin(115200);  // Initialize the serial communication:

  //****************CONNECTION****************
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: "); // Waits until the connection is made
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(2000);
  }
  server.begin();

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();  // Prints the SSID and IP
  Serial.print("IP Address: ");
  Serial.println(ip);
  delay(2000);

  //****************SpO2****************
  while (false == MAX30102.begin()){
    Serial.println("SpO2 init failed, Please try again!");
    delay(1000);
  }
  Serial.println("SpO2 init success!");
  Serial.println("SpO2 start measuring...");
  MAX30102.sensorStartCollect();  // Initialize the SpO2 sensor

  //****************TIMER****************
  previousSpO2Millis = millis();  // Initilialize the "timer"

}


// ----------LOOP--------------
void loop() 
{
  currentMillis=millis();
  HTML_func();  
}


// ----------HTML--------------
void HTML_func()
{
  WiFiClient client = server.available();   
  if (client) { 
    Serial.println("new client");           
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        if (c == '\n') {  
          if (currentLine.length() == 0) {
            if(currentMillis - previousSpO2Millis >= SpO2Interval) {//The sensor updates the data every 4 seconds
              previousSpO2Millis += SpO2Interval;
              MAX30102.getHeartbeatSPO2();
              SpO2=MAX30102._sHeartbeatSPO2.SPO2;
              HR=MAX30102._sHeartbeatSPO2.Heartbeat;
              Temp=MAX30102.getTemperature_C();
            } //Updates the webpage every 4 seconds, giving time to update the values of the sensors
              client.println("<html><head><meta name='viewport' content='width=device-width, user-scalable=no'><meta http-equiv='refresh' content='4' charset='UTF-8'><title>Adrián Amatriain - ESIEE</title><style>body {font-family: monospace;background-color: rgb(20,21, 38);}h1 {color: white;text-align: center;margin: 50px;font-size: 24px;}.sensor{color: #90c4e8;}.barraTemp{height: 200px;width: 200px;margin: auto;border-radius: 50%;overflow: hidden;}.cont{height: 100px;width: 200px;}.circulo_centro {width: 150px;height: 150px;background-color: rgb(20,21, 38);border-radius: 50%;margin: auto;position: relative;margin: -175px auto 50px;}.rectangulo{width: 200px;height: 100px;background-color: rgb(20,21, 38);position: relative;margin: -125px auto 50px;}</style></head><body><h1>HR and SpO2 Monitoring</h1><h1 class='sensor'>HR <img src='https://i0.wp.com/agelessinvesting.com/wp-content/uploads/2019/02/Resting-Heart-Rate-Chart-By-Age-For-Men-and-Women-min.png?resize=560%2C598&ssl=1' alt='Resting Heart Rate Chart' style='position:absolute; top:130; left:950;width:500px;height:500px;''> </h1><div class='barraTemp' style='transform: rotate("+String(map(HR,-1,160,0,180))+"deg);'><div class='cont' style='background-color: rgb(143, 136, 136);'></div><div class='cont' style='background-color: rgb(253,0,0);''></div></div><div class='circulo_centro'></div><div class='rectangulo'></div><h1 style='font-size: 22px; position: relative; margin: -125px auto 50px;'>"+HR+" bpm</h1><h1 class='sensor'>SpO2</h1><div class='barraTemp' style='transform: rotate("+String(map(SpO2,-1,100,0,180))+"deg);'><div class='cont' style='background-color: rgb(143, 136, 136);'></div><div class='cont' style='background-color: rgb(253,0,0);'></div></div><div class='circulo_centro'></div><div class='rectangulo'></div><h1 style='font-size: 22px; position: relative; margin: -125px auto 50px;'>"+SpO2+" %</h1><h1 class='sensor'>Board Tª</h1><div class='barraTemp' style='transform: rotate("+String(map(Temp,20,40,0,180))+"deg);'><div class='cont' style='background-color: rgb(143, 136, 136);'></div><div class='cont' style='background-color: rgb(253,0,0);'></div>   </div><div class='circulo_centro'></div><div class='rectangulo'></div><h1 style='font-size: 22px; position: relative; margin: -125px auto 50px;'>"+Temp+" °C</h1></body></html>");           
            break;  
          }
          else {
            currentLine = "";
          }
        } 
        else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("client disconnected");
  }
}
