// William Denson
// 12-01-24
// This final project utilizes a capacitive "hall effect" on a rotating
// platter for medicine distribution. A smoothing capacitor picks up 
// coupling between two aluminum sheets while a counter keeps track of
// revolutions for patient tracking of medicine dosages. 

#include <CapacitiveSensor.h>                                 // Utilized GitHub repository source: https://github.com/PaulStoffregen/CapacitiveSensor
CapacitiveSensor capSensor = CapacitiveSensor(4,2);

// Motor constant and global variables.
const int controlPin1 = 8;
const int controlPin2 = 3;
const int enablePin = 9;
const int directionSwitchPin = 13;
const int onOffSwitchStateSwitchPin = 5;
const int potPin = A0;

int onOffSwitchState = 0;
int previousOnOffSwitchState = 0;
int directionSwitchState = 0;
int previousDirectionSwitchState = 0;

int motorEnabled = 0;
int motorSpeed = 0;
int motorDirection = 1;

// Capacitive coupling constant and global variables.
int threshold = 65;
const int ledPin = 12;

// Counter variable
int connectionMade = 0;

void setup() {
  // Various pinModes are function templates that allow a user 
  // to pass in digital inputs to respective pin numbers.

  //------------------------------------------------------
  // Capaciatance function code:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  //------------------------------------------------------
  // Motor function code:
  pinMode(directionSwitchPin, INPUT);
  pinMode(onOffSwitchStateSwitchPin, INPUT);
  pinMode(controlPin1, OUTPUT);
  pinMode(controlPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  digitalWrite(enablePin, LOW);
}

void loop() {
  //------------------------------------------------------
  // Switch priming for motor:
  onOffSwitchState = digitalRead(onOffSwitchStateSwitchPin);
  delay(1);                                                       // Delay of (1) changed from an incremental value of 10
                                                                  // to accomidate motor button signal detection.
  directionSwitchState = digitalRead(directionSwitchPin);
  motorSpeed = analogRead(potPin)/4;

  // digitalWrite function template utilized.
  if(onOffSwitchState != previousOnOffSwitchState){
    if(onOffSwitchState == HIGH){
      motorEnabled = !motorEnabled;
    }
  }

  if(directionSwitchState != previousDirectionSwitchState){
    if(directionSwitchState == HIGH){
      motorDirection = !motorDirection;
    }
  }

  if(motorDirection == 1){
    digitalWrite(controlPin1, HIGH);
    digitalWrite(controlPin2, LOW);
  }
  else{
    digitalWrite(controlPin1, LOW);
    digitalWrite(controlPin2, HIGH);
  }

  if(motorEnabled == 1){
    analogWrite(enablePin, motorSpeed);
  }
  else{
    analogWrite(enablePin, 0);
  }

  previousDirectionSwitchState = directionSwitchState;
  previousOnOffSwitchState = onOffSwitchState;

  //------------------------------------------------------
  // capacitiveSensor object function continually monitors
  // the sensor voltage levels for an activation value to 
  // sensorValue which is in a long number format.
  long sensorValue = capSensor.capacitiveSensor(30);          
  // Serial.println(sensorValue);                                     // Original project code printed sensor values but this is not needed
                                                                      // for production purposes; this would be private data in a class object.


  if(sensorValue > threshold){
    digitalWrite(ledPin, HIGH);

    // If a connection is made with the capacitive circuit, connectionMade is incremented.
    ++connectionMade;
    // The serial monitor window will continually display the connectionMade increment count. 
    Serial.println("Current medicine count and platter increment");
    Serial.println(connectionMade);
  }
  else{
    digitalWrite(ledPin, LOW);
  }

  // The loop is delayed 10 units to re-read the sensorValue. This causes a slight lag for reading
  // the push button switch for the motor to initial start up. Discrete times of engaging the button
  // may not line up with the delay of 10 miliseconds. 
                                                   

}
