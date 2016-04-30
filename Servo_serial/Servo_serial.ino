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
int elfeedbackPin = 0;
int rofeedbackPin = 5;
int val = 0;
int rocalVal[] = {80, 445};  // initial cal values
int elcalVal[] = {80, 445};  // initial cal values

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
  //roservo.writeMicroseconds(1000);
} 

void loop() {
    position = analogRead(elfeedbackPin);  
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
          xmap = map(x, 0, 180, 700, 2300);
          //Serial.println(xmap);
          elservo.writeMicroseconds(xmap);
          Serial.println("Adjusting Elevation");
          delay(1000);
          Serial.print("Elevation adjusted to: ");
          Serial.println(analogRead(elfeedbackPin));
        }
        else if (str == "r"){
           Serial.println(x);
           //x = Serial.parseInt();
          // rotation servo
          xmap = map(x, 0, 180, 500, 2400);
          roservo.writeMicroseconds(xmap);
          Serial.println("Adjusting Rotation");
          delay(1000);
          Serial.print("Rotation adjusted to: ");
          Serial.println(analogRead(rofeedbackPin));
        }
        else if (str == "cale"){
          // calibrate
           //Serial.println(x);
           //x = Serial.parseInt();
          // rotation servo
          Serial.println("Calibrating elevation");
          elservo.writeMicroseconds(500);
          delay(5000);  // wait 5 seconds for servo to reach the position                
          elcalVal[0] = analogRead(elfeedbackPin);  
          //servo.write(180);
          elservo.writeMicroseconds(3000);
          delay(5000);
          elcalVal[1] = analogRead(elfeedbackPin);
          Serial.print("Elevation cal values: ");
          Serial.print(elcalVal[0]);
          Serial.print(",");
          Serial.println(elcalVal[1]);
        }
        else if (str == "calr"){
          // calibrate roation
           //Serial.println(x);
           //x = Serial.parseInt();
          // rotation servo
          Serial.println("Calibrating rotation");
          roservo.writeMicroseconds(500);
          delay(5000);  // wait 5 seconds for servo to reach the position                
          rocalVal[0] = analogRead(rofeedbackPin);  
          //servo.write(180);
          roservo.writeMicroseconds(3000);
          delay(5000);
          rocalVal[1] = analogRead(rofeedbackPin);
          Serial.print("Roation cal values: ");
          Serial.print(rocalVal[0]);
          Serial.print(",");
          Serial.println(rocalVal[1]);
        }
        else if (str == "ec"){
          // move elevation using cal values
          Serial.println("Adjusting Elevation using cal values to: ");
          Serial.println(x);
          //x = Serial.parseInt();
          // elevation servo
          // map the range of angles to the servo
          xmap = map(x, 0, 180, elcalVal[0], elcalVal[1]);
          //Serial.println(xmap);
          xmap2 = map(xmap, elcalVal[0], elcalVal[1], 500, 2400);
          elservo.writeMicroseconds(xmap2);
          //Serial.println(xmap2);
          delay(1000);
          Serial.print("Elevation adjusted to: ");
          //Serial.println(analogRead(elfeedbackPin));
          Serial.println(map(analogRead(elfeedbackPin), elcalVal[0], elcalVal[1], 0, 180));
        }
        else if (str == "er"){
          // move rotation using cal values
          Serial.print("Adjusting Rotation using cal values to: ");
          Serial.println(x);
          //x = Serial.parseInt();
          // elevation servo
          // map the range of angles to the servo
          xmap = map(x, 0, 180, rocalVal[0], rocalVal[1]);
          //Serial.println(xmap);
          xmap2 = map(xmap, rocalVal[0], rocalVal[1], 500, 2400);
          roservo.writeMicroseconds(xmap2);
          //Serial.println(xmap2);
          delay(1000);
          Serial.print("Rotation adjusted to: ");
          Serial.println(map(analogRead(rofeedbackPin), elcalVal[0], elcalVal[1], 0, 180));
        }
        else if (str == "fullr"){
            int d;
            int dr = 1200;
            // rotate one degree at a time
            Serial.println("Rotating");
            roservo.writeMicroseconds(500);
            delay(2000);
            for ( d = 500; d < 2300; d++ ) {
            roservo.writeMicroseconds(d);
            delay(10);
                if( d % 100 == 0){  
                  elservo.writeMicroseconds(dr);
                    dr = dr + 100;
                }
            }
            Serial.println("Done");
        }
    }
  }


int smooth(int data) {
    total -= readings[index];               // subtract the last reading
    readings[index] = analogRead(elfeedbackPin); // read from the sensor
    total += readings[index];               // add the reading to the total
    index = (index + 1);                    // advance to the next index

    if (index >= NUMREADINGS)               // if we're at the end of the array...
    index = 0;                            // ...wrap around to the beginning

    val = total / NUMREADINGS;          // calculate the average
    return val;
}
