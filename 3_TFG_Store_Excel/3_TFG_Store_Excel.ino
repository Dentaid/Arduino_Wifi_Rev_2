// ----------LIBRARIES--------------
#include "DFRobot_BloodOxygen_S.h"  // Required libraries
#include "Wire.h"


// --------CONSTANTS (won't change)---------------
const int ExcelInterval = 50; // Number of millisecs between Excel data

//------------ VARIABLES (will change)---------------------
unsigned long currentMillis = 0;    // Stores the value of millis() in each iteration of loop()
unsigned long previousExcelMillis = 0;   // Will store last time the Excel was updated

float HR;
float SpO2; // Variables to acquire the sensor data
float Temp;
int item = 0;


// ----------I2C--------------
#define I2C_COMMUNICATION  //use I2C for communication

// **********SpO2**************
#define I2C_ADDRESS    0x57
DFRobot_BloodOxygen_S_I2C MAX30102(&Wire ,I2C_ADDRESS);


// ----------SETUP--------------
void setup()
{
  Serial.begin(9600); // Initialize the serial communication:
  delay(2000);

  //****************SpO2****************
  while (false == MAX30102.begin()){
    Serial.println("SpO2 init failed, Please try again!");
    delay(1000);
  }
  Serial.println("SpO2 init success!");
  Serial.println("SpO2 start measuring...");
  MAX30102.sensorStartCollect();   // Initialize the SpO2 sensor

  //****************TIMER****************
  previousExcelMillis = millis(); // Initilialize the "timer"

  //****************EXCEL****************
  Serial.println("CLEARDATA"); // Clear Data -- This command deletes the registered data
  Serial.println("CLEARSHEET"); //Clear sheet -- This command deletes all the data form the ActiveSheet 
  Serial.println("LABEL,Date,Hour,Seconds,Item,SpO2,HR,Board Temp.");
  Serial.println("RESETTIMER");
}


// ----------LOOP--------------
void loop()
{
  currentMillis=millis();
  if(currentMillis<=122000){  // If passes 2 minutes, it stops
    Excel_func();
  }
}


// ----------EXCEL--------------
void Excel_func()
{
  //****************SpO2****************
  if(currentMillis - previousExcelMillis >= ExcelInterval) {  //Uploads the values every 50 milliseconds
    previousExcelMillis += ExcelInterval;

    Serial.print("DATA,DATE,TIME,");
    
    Serial.print(String((float)(currentMillis-2000)/1000)+",");

    item += 1;
    Serial.print(String(item)+",");

    MAX30102.getHeartbeatSPO2();

    SpO2=MAX30102._sHeartbeatSPO2.SPO2;
    Serial.print(String(SpO2)+",");

    HR=MAX30102._sHeartbeatSPO2.Heartbeat;
    Serial.print(String(HR)+",");

    Temp=MAX30102.getTemperature_C();
    Serial.println(String(Temp));    
  }
}
