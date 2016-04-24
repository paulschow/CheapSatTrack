/*  Servo Recorder
This software has no warranty either real or implied.  Use at
your own risk.

From 
http://forums.trossenrobotics.com/tutorials/how-to-diy-128/get-position-feedback-from-a-standard-hobby-servo-3279/

Based in part on the servo and smoothing tutorials on www.arduino.cc

Records and plays back servo positions using a servo
modified with feedback.

The feedback connects to analog pin 5

The following commands are supported:
a records the current servo position
b releases the servo to record the position
c plays back the recorded position
d calibrates the end points of the servo

Every servo should be calibrated when first started to get more accurate
end points.

The minimum and maximum pulse width will vary with different servo models.  If the calibration routine does not work correctly then look up the minimum and maximum pulse widths for your servo and use them in the servo.attach lines like so:
servo.attach(9,minimumPulseWidth,maximumPulseWidth);
*/

#include <Servo.h>

Servo servo;

int angle = 90;
int position = 90;
int feedbackPin = 0;
int val = 0;
int calVal[] = {191, 1011};  // initial cal values
int calStartPos = 0;
int final = 90;

#define NUMREADINGS 10      // Number of readings to take for smoothing


int readings[NUMREADINGS];                // the readings from the analog input
int index = 0;                            // the index of the current reading
int total = 0;                            // the running total
int average = 0;                          // the average

void setup()
{
  Serial.begin(9600);
  //analogReference(EXTERNAL);   // Use the external analog reference  
}

void loop()
{
 static int v = 0;
  position = analogRead(feedbackPin);  
  position = smooth(position);

  if ( Serial.available()) {
    char ch = Serial.read();

    switch(ch) {
      case '0'...'9':
        v = v * 10 + ch - '0';
        break;
        
      case 'a':   //  record the position of the pot  
        servo.detach();    
        Serial.print("Recording position: ");
        Serial.println(position);      
        final = position;  
        v = 0;
        break;
        
      case 'b':  //  stop playing
        Serial.println("Stop");
        servo.detach();    
        v = 0;
        break;
        
      case 'c':   // play            
        angle = map(final, calVal[0], calVal[1], 0, 180);
        Serial.print("playing: ");
        Serial.println(angle);        
        servo.attach(9);  
        servo.write(angle);  
        v = 0;
        break;
        
        
      case 'd' :  // calibrate      
        Serial.println("calibrating");
        servo.attach(9);
        //servo.write(1);
        servo.writeMicroseconds(500);
        delay(5000);  // wait 1 second for servo to reach the position                
        calVal[0] = analogRead(feedbackPin);  
        //servo.write(180);
        servo.writeMicroseconds(3000);
        delay(5000);
        calVal[1] = analogRead(feedbackPin);
        Serial.print("Cal values: ");
        Serial.print(calVal[0]);
        Serial.print(",");
        Serial.println(calVal[1]);
        v = 0 ;
        break;      
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
