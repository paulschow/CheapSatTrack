#include <Servo.h> 

Servo elservo;
Servo roservo;

String str;
int x;
int xmap;
int xmap2;
int inByte = 800;         // incoming serial byte
int angle = 90;
int position = 90;
int feedbackPin = 0;
int val = 0;
int calVal[] = {80, 445};  // initial cal values

#define NUMREADINGS 10      // Number of readings to take for smoothing


int readings[NUMREADINGS];                // the readings from the analog input
int index = 0;                            // the index of the current reading
int total = 0;                            // the running total
int average = 0;                          // the average



void setup() 
{ 
  Serial.begin(9600);
  Serial.setTimeout(200);
  elservo.attach(9);
  roservo.attach(8);
  roservo.writeMicroseconds(1000);
} 

void loop() {
    position = analogRead(feedbackPin);  
    position = smooth(position);
    
      if (Serial.available() > 0) {
        str = Serial.readStringUntil(';');
        x = Serial.parseInt();
        Serial.println(str);
        if (str == "e"){
          Serial.println(x);
          //x = Serial.parseInt();
          // elevation servo
          // map the range of angles to the servo
          //xmap = map(x, 0, 180, 700, 2300);
          //Serial.println(xmap);
          elservo.writeMicroseconds(x);
          Serial.println("Adjusting Elevation");
          delay(1000);
          Serial.print("Elevation adjusted to: ");
          Serial.println(analogRead(feedbackPin));
        }
        else if (str == "r"){
           Serial.println(x);
           //x = Serial.parseInt();
          // rotation servo
          xmap = map(x, 0, 180, 500, 2400);
          roservo.writeMicroseconds(xmap);
          Serial.println("Adjusting Rotation");
        }
        else if (str == "cale"){
          // calibrate
           //Serial.println(x);
           //x = Serial.parseInt();
          // rotation servo
          Serial.println("Calibrating elevation");
          elservo.writeMicroseconds(500);
          delay(5000);  // wait 5 seconds for servo to reach the position                
          calVal[0] = analogRead(feedbackPin);  
          //servo.write(180);
          elservo.writeMicroseconds(3000);
          delay(5000);
          calVal[1] = analogRead(feedbackPin);
          Serial.print("Cal values: ");
          Serial.print(calVal[0]);
          Serial.print(",");
          Serial.println(calVal[1]);
        }
        else if (str == "ec"){
          // move elevation using cal values
          Serial.println("Adjusting Elevation using cal values");
          Serial.println(x);
          //x = Serial.parseInt();
          // elevation servo
          // map the range of angles to the servo
          xmap = map(x, 0, 180, calVal[0], calVal[1]);
          Serial.println(xmap);
          xmap2 = map(xmap, calVal[0], calVal[1], 500, 2400);
          elservo.writeMicroseconds(xmap2);
          Serial.println(xmap2);
          delay(1000);
          Serial.print("Elevation adjusted to: ");
          Serial.println(analogRead(feedbackPin));
        }
    }
  }


int smooth(int data) {
    total -= readings[index];               // subtract the last reading
    readings[index] = analogRead(feedbackPin); // read from the sensor
    total += readings[index];               // add the reading to the total
    index = (index + 1);                    // advance to the next index

    if (index >= NUMREADINGS)               // if we're at the end of the array...
    index = 0;                            // ...wrap around to the beginning

    val = total / NUMREADINGS;          // calculate the average
    return val;
}
