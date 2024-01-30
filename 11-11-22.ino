/*

  Capacitive Soil Moisture Sensor  

*/

#define SensorPin A0

float sensorValue = 0;

float slope = 0.1; // slope from linear fit

float intercept = 0; // intercept from linear fit

unsigned long lastAverageTime = 0;
unsigned long averagePeriod = 10000; // average period in milliseconds
float averageVoltage = 0;
int sampleCount = 0;

int sensorState = 0 ; // beginning state to the 

void setup() {
  Serial.begin(9600);
}

void loop() {
  unsigned long now = millis();

  // read the sensor voltage
  sensorValue = analogRead(SensorPin);
  float voltage = (sensorValue / 1023.0) * 5.1; // convert to voltage

  // add the voltage reading to the average
  averageVoltage += voltage;
  sampleCount++;

  // check if it's time to calculate the average
  if (now - lastAverageTime >= averagePeriod) {
    // calculate the average voltage and reset the count
    float average = averageVoltage / sampleCount;
    sampleCount = 0;
    averageVoltage = 0;

    Serial.print("Voltage: ");
    Serial.print(average); // print the averaged voltage
    Serial.print(" V, Theta_v: ");

    // we need to learn a better way to transform the voltage reading to a volumetric water content 
    // 10% vwc
    if ((average > 1) && (average < 2)) {
      slope = 0.21; // slope from linear fit
      intercept = -0.23; // intercept from linear fit
    }
    // 20% vwc 
    if (average > 2 && average < 2.6) {
      slope = 0.17; // slope from linear fit
      intercept = -0.23; // intercept from linear fit
    }
    // 30% vwc 
    if (average > 2.65 && average < 3) {
      slope = 0.195; // slope from linear fit
      intercept = -0.23; // intercept from linear fit
    }

    // calculate the volumetric water content
    float vwc = (average * slope) + intercept;

    // make sure the vwc is not negative
    if (vwc < 0) {
      Serial.println(0.00);
    } else {
      Serial.print(vwc,4);
      Serial.print("  ");
      Serial.print(vwc*100,4);
      Serial.println("%");

    }

    lastAverageTime = now; // record the time of the last average calculation
  }

  delay(1); // delay between voltage readings
}
