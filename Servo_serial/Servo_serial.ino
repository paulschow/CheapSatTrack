#include <Servo.h> 

Servo elservo;
Servo roservo;

String str;
int x;
int inByte = 800;         // incoming serial byte


void setup() 
{ 
  Serial.begin(9600);
  Serial.setTimeout(50);
  elservo.attach(9);
  roservo.attach(8);
  elservo.writeMicroseconds(800);
  delay(5000);
  roservo.writeMicroseconds(1000);
} 

void loop() {
      if (Serial.available() > 0) {
        str = Serial.readStringUntil(';');
        x = Serial.parseInt();
        Serial.println(str);
        Serial.println(x);
        if (str == "e"){
          // elevation servo
          elservo.writeMicroseconds(x);
          Serial.println("Adjusting Elevation");
        }
        else if (str == "r"){
          // rotation servo
          roservo.writeMicroseconds(x);
          Serial.println("Adjusting Rotation");
        }
    }
  } 
