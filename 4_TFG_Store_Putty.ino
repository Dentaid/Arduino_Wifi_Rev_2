// --------CONSTANTS (won't change)---------------
const int ECGLOPlusPin =  10;      // The pin numbers for the LO + and LO -
const int ECGLOMinusPin = 11;

const int PuttyInterval = 1; // Number of millisecs between Putty data


//------------ VARIABLES (will change)---------------------
unsigned long currentMillis = 0;    // Stores the value of millis() in each iteration of loop()
unsigned long previousPuttyMillis = 0;   // Will store last time the Putty was updated

unsigned long item = 0;


// ----------SETUP--------------
void setup()
{
  Serial.begin(115200); // Initialize the serial communication:
  delay(2000);

  //****************ECG****************
  pinMode(ECGLOPlusPin, INPUT); // Setup for leads off detection LO +
  pinMode(ECGLOMinusPin, INPUT); // Setup for leads off detection LO -

  //****************TIMER****************
  previousPuttyMillis = millis(); // Initilialize the "timer"
}


// ----------LOOP--------------
void loop()
{
  currentMillis=millis();
  if(currentMillis<62000){  // If passes 1 minute, it stops
    Putty_func();
  }
}


// ----------PUTTY--------------
void Putty_func()
{
  if(currentMillis - previousPuttyMillis >= PuttyInterval) { //Uploads the values every 1 millisecond approximately
    previousPuttyMillis += PuttyInterval;
    
    Serial.print(String((float)(currentMillis-2000)/1000)+";");

    item += 1;
    Serial.print(String(item)+";");
    //****************ECG****************
    if((digitalRead(ECGLOPlusPin) == 1)||(digitalRead(ECGLOMinusPin) == 1)) { // Initialize the ECG sensor
      Serial.println("!");
    }
    else{
      Serial.println(analogRead(A0));
    }     
  }
}
