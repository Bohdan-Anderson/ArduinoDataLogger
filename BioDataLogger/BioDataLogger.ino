/*BioMapping Data Logger: Mobile Experience Lab,
 Symon Oliver. Bohdan Anderson. Myles Borins. Andrei Vasili*/

//Does not support .JSON file type as the extension violates the .123 limit of the current version

/*INIT GLOBAL VARIABLES*/
char buff[50];

int resetOpenLog = 2;

int partNum = 01; //This should come from Node or Python
String partIn; //Participant Initials

int dataPin =  12; //Status LED connected to digital pin 13
int statusPin = 9; //Indicates state of logging
int bright = 0;    // how bright the LED is
int fade = 2;    // how many points to fade the LED by
int buttonState = 0;
int entryNum = 0;

String cbOpen = "{";
String cbClose = "}";

int sensorVal;

int sensorPin = A0;    // select the input pin for the potentiometer

const int buttonPin = 6;     // the number of the pushbutton pin

boolean complete = false;


/*SETUP*/
void setup() 
{
  pinMode(dataPin, OUTPUT);
  pinMode(statusPin, OUTPUT);
  pinMode(resetOpenLog, OUTPUT);
  pinMode(buttonPin, INPUT);

  Serial.begin(9600); //9600bps is default for OpenLog

  //Reset OpenLog
  digitalWrite(resetOpenLog, LOW);
  delay(100);
  digitalWrite(resetOpenLog, HIGH);

  //Wait for OpenLog to respond with '<' to indicate it is alive and recording to a file
  while(1) {
    if(Serial.available())
      if(Serial.read() == '<') break;
  }

  //Works with Arduino v1.0
  Serial.write(26);
  Serial.write(26);
  Serial.write(26);

  //Wait for OpenLog to respond with '>' to indicate we are in command mode
  while(1) {
    if(Serial.available())
      if(Serial.read() == '>') break;
  }

  //Old way
  sprintf(buff, "new bio.txt\r");
  Serial.print(buff); //\r in string + regular print works with older v2.5 Openlogs

  //Wait for OpenLog to return to waiting for a command
  while(1) {
    if(Serial.available())
      if(Serial.read() == '>') break;
  }

  sprintf(buff, "append bio.txt\r");
  Serial.print(buff);

  //Wait for OpenLog to indicate file is open and ready for writing
  while(1) {
    if(Serial.available())
      if(Serial.read() == '<') break;
  }

  digitalWrite(resetOpenLog, LOW);

  delay(2000); //Wait a second for OpenLog to init

  digitalWrite(resetOpenLog, HIGH);

  Serial.println();

  Serial.print(cbOpen);
  Serial.print('"');
  Serial.print("breath");
  Serial.print('"');
  Serial.print(":");
  Serial.print(cbOpen);

  digitalWrite(resetOpenLog, LOW);
}

void loop()
{ 
  delay(500);

  if(digitalRead(dataPin) == 0) //Turn the status LED on/off as we go
    digitalWrite(dataPin, HIGH);
  else
    digitalWrite(dataPin, LOW);

  // read the value from the sensor:
  sensorVal = analogRead(sensorPin); 

  Serial.print('"');
  Serial.print(entryNum);
  Serial.print('"');
  Serial.print(": ");
  Serial.print(cbOpen);
  Serial.print('"');
  Serial.print("SensorValue");
  Serial.print('"');
  Serial.print(": ");
  Serial.print(sensorVal);
  Serial.print(cbClose);
  Serial.print(", ");

  if(digitalRead(dataPin) == 0) //Turn the status LED on/off as we go
    digitalWrite(dataPin, HIGH);
  else
    digitalWrite(dataPin, LOW);

  delay(500);
  
    entryNum ++;
    

  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {

    digitalWrite(dataPin, LOW);

    complete = true;

    digitalWrite(dataPin, LOW);
    Serial.print('"');
    Serial.print(entryNum);
    Serial.print('"');
    Serial.print(": ");
    Serial.print(cbOpen);
    Serial.print('"');
    Serial.print("SensorValue");
    Serial.print('"');
    Serial.print(": ");
    Serial.print(sensorVal);
    Serial.print(cbClose);
    Serial.println(cbClose+cbClose);
  }

  //Fade status LED to indicate that data collection has stopped
  while(complete){
    // fade in from min to max in increments of 5 points:
    for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) { 
      // sets the value (range from 0 to 255):
      analogWrite(statusPin, fadeValue);         
      // wait for 30 milliseconds to see the dimming effect    
      delay(30);                            
    } 

    // fade out from max to min in increments of 5 points:
    for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=10) { 
      // sets the value (range from 0 to 255):
      analogWrite(statusPin, fadeValue);         
      // wait for 30 milliseconds to see the dimming effect    
      delay(30);                            
    } 
  }
}



