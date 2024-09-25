// EOG Filter - BioAmp EXG Pill
// https://github.com/upsidedownlabs/BioAmp-EXG-Pill

#define SAMPLE_RATE 50
#define INPUT_PIN A0
#define INPUT_PIN1 A1
char direction = 'S';
char lastDirection = 'S'; // To track the last direction sent

#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial BTSerial(2,3);

void setup() {
  // Serial connection begin
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  // Calculate elapsed time
  static unsigned long past = 0;
  unsigned long present = micros();
  unsigned long interval = present - past;
  past = present;

  // Run timer
  static long timer = 0;
  timer -= interval;

  // Sample
  if (timer < 0) {
    timer += 3000000 / SAMPLE_RATE;
    float sensor_value1 = analogRead(INPUT_PIN);
    float sensor_value2 = analogRead(INPUT_PIN1);
    float signal1 = EOGFilter1(sensor_value1);
    float signal2 = EOGFilter2(sensor_value2);

    // Print filtered signals for debugging
    Serial.print("Signal1:");
    Serial.print(signal1);
    Serial.print(',');
    Serial.print("Signal2:");
    Serial.println(signal2);
    //Serial.println(direction);
   // BTSerial.write(direction);
     //Send direction only if it has changed
    if (direction != lastDirection) {
      BTSerial.write(direction);
      Serial.println(direction);
      lastDirection = direction; // Update last direction sent
    }
  }
}

// Band-Pass Butterworth IIR digital filter for Sensor 1
float EOGFilter1(float input) {
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - 0.02977423 * z1 - 0.04296318 * z2;
    output = 0.09797471 * x + 0.19594942 * z1 + 0.09797471 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - 0.08383952 * z1 - 0.46067709 * z2;
    output = 1.00000000 * x + 2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.92167271 * z1 - 0.92347975 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.96758891 * z1 - 0.96933514 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }

  // Adjust output and direction based on the filtered signal
  if (output >= -150 && output <= 150) {
    output = 0;
  }

  // Check for each condition independently
  else if (output > 500 || output < -500) {
    direction = 'S';  // Still when output exceeds 500 or falls below -500
  }
  else if (output > 150) {
    direction = 'F';  // Forward
  }
  else if (output < -150) {
    direction = 'B';  // Backward
  }

  return output;
}

// Band-Pass Butterworth IIR digital filter for Sensor 2
float EOGFilter2(float input) {
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - 0.02977423 * z1 - 0.04296318 * z2;
    output = 0.09797471 * x + 0.19594942 * z1 + 0.09797471 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - 0.08383952 * z1 - 0.46067709 * z2;
    output = 1.00000000 * x + 2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.92167271 * z1 - 0.92347975 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.96758891 * z1 - 0.96933514 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }

  // Adjust output and direction based on the filtered signal
  if (output >= -150 && output <= 150) {
    output = 0;
    
  }

  // Check for each condition independently
  else if (output > 500 || output < -500) {
    direction = 'S';  // Still when output exceeds 500 or falls below -500
  }
 else if (output > 150) {
    direction = 'L';  // Left
  }
 else if (output < -150) {
    direction = 'R';  // Right
  }

  return output;
}
