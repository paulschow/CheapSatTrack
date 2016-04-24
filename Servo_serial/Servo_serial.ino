#include <Servo.h> 

Servo elservo;
Servo roservo;

String str;
int x;
int xmap;
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
        //x = Serial.parseInt();
        Serial.println(str);
        if (str == "e"){
          Serial.println(x);
          x = Serial.parseInt();
          // elevation servo
          // map the range of angles to the servo
          xmap = map(x, 0, 180, 700, 2300);
          elservo.writeMicroseconds(xmap);
          Serial.println("Adjusting Elevation");
        }
        else if (str == "r"){
           Serial.println(x);
           x = Serial.parseInt();
          // rotation servo
          xmap = map(x, 0, 180, 700, 2300);
          roservo.writeMicroseconds(xmap);
          Serial.println("Adjusting Rotation");
        }
    }
  } 
