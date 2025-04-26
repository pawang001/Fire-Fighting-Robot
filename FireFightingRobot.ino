#include <Servo.h>  // Include Servo library

Servo myservo;      // Create a Servo object

int pos = 0;        // Variable to store servo position
int motor_speed = 110;  
boolean fire = false; // Fire detection flag

// Sensor and Motor pin definitions
#define Left 9      // Left sensor
#define Right 10    // Right sensor
#define Forward 8   // Front sensor
#define LM1 2       // Left motor forward
#define LM2 7       // Left motor backward
#define RM1 4       // Right motor forward
#define RM2 12      // Right motor backward
#define pump 6      // Pump control pin

void setup() {
  // Set sensor and motor pins
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
  pinMode(Forward, INPUT);
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  pinMode(pump, OUTPUT);

  // Set initial motor speed using PWM pins (use pins 3 and 5)
  analogWrite(3, motor_speed); 
  analogWrite(5, motor_speed);

  // Attach servo to pin 11
  myservo.attach(11);
  myservo.write(90); // Initialize servo to 90 degrees (neutral position)
}

void put_off_fire() {
  delay(300);  // Small delay before activating the pump
  
  // Stop motors while putting off fire
  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, HIGH);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, HIGH);

  // Activate pump
  digitalWrite(pump, HIGH);
  delay(500); // Wait for pump to activate
  
  // Sweep the servo motor from 0 to 180 degrees
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(20); 
  }

  // Sweep back from 180 to 0 degrees
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(20);
  }

  // Turn off pump
  digitalWrite(pump, LOW);
  
  // Reset the servo to neutral (90 degrees)
  myservo.write(90);

  // Set fire flag to false
  fire = false;
}

void loop() {
  myservo.write(90);  // Set servo to neutral

  // Move forward if no obstacle is detected
  if (digitalRead(Left) == 1 && digitalRead(Right) == 1 && digitalRead(Forward) == 1) {
    digitalWrite(LM1, LOW); // Move both motors forward
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW);
  }  

  // Stop and handle fire if front sensor detects an obstacle
  else if (digitalRead(Forward) == 0) {
    digitalWrite(LM1, LOW); // Stop both motors
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW);
    fire = true; // Set fire flag to true
  }

  // Turn left if left sensor detects an obstacle
  else if (digitalRead(Left) == 0) {
    digitalWrite(LM1, HIGH); // Stop left motor (turn left)
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW); // Move right motor forward
  }

  // Turn right if right sensor detects an obstacle
  else if (digitalRead(Right) == 0) {
    digitalWrite(LM1, HIGH); // Move left motor forward
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);  // Stop right motor (turn right)
    digitalWrite(RM2, HIGH);
  }

  // Small delay to ensure smooth operation
  delay(300);

  // Check if the fire flag is set to true and call fire handling function
  if (fire == true) {
    put_off_fire();
  }
  else {
    // Make sure the pump is off if there's no fire
    digitalWrite(pump, LOW);
  }
}
