#include <SR04.h>
#include <Servo.h>

Servo s1;
SR04 sr04 = SR04(5, 4);
int motorpinRight = 11;
int motorpinLeft = 10;
int threshold = 0;

void setup() {
  
  pinMode(motorpinRight, OUTPUT);
  pinMode(motorpinLeft, OUTPUT);
  s1.attach(6);

}

void loop() {

  // braitenberg + habituation
  
  int soundLeft = analogRead(A1);
  int soundRight = analogRead(A0);

  // using the map function to map the threshold values to 
  // the values used to substract the delay.
  int d_delay = map(threshold, 0, 1000, 0, 600);

  // if the sound surpasses the threshold, orient the robot
  // opposite to the sound source.
  if (soundLeft > threshold) {
      digitalWrite(motorpinRight, LOW);
      digitalWrite(motorpinLeft, HIGH);
      threshold = threshold + 100;
      delay((600 - d_delay));
      }
   else if (soundRight > threshold) {
      digitalWrite(motorpinRight, HIGH);
      digitalWrite(motorpinLeft, LOW);
      threshold = threshold + 100;
      delay((600 - d_delay));
      }
   else {
    
    // bang-bang controller
    
    digitalWrite(motorpinRight, HIGH);
    digitalWrite(motorpinLeft, HIGH);

    // if distance is less that 35cm, halt the motors
    // and take measurements from both sides of the robot
    // in order to choose which side is the clearest.
     if (sr04.Distance() < 35){
       digitalWrite(motorpinRight, LOW);
       digitalWrite(motorpinLeft, LOW);
        
       s1.write(0);
       delay(1000);
       int rightDist = sr04.Distance();
       delay(1000);
       s1.write(180);
       delay(1000);
       int leftDist = sr04.Distance();
       delay(1000);
       s1.write(90);
        
       if (rightDist > leftDist){
         digitalWrite(motorpinRight, LOW);
         digitalWrite(motorpinLeft, HIGH);
         delay(600);
        }
       else if (rightDist < leftDist){
         digitalWrite(motorpinRight, HIGH);
         digitalWrite(motorpinLeft, LOW);
         delay(600);
        }
      }
   }
}
