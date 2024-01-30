#define SensorPin A0
#define NUM_SAMPLES 60
int samples[NUM_SAMPLES];
int currentIndex = 0;
unsigned long lastReadingTime = 0;
float averageValue = 0;
float slope = 100.0;
float intercept = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_SAMPLES; i++) {
    samples[i] = 0;
  }
}

void loop() {
  // Read the analog value
  float voltage, vol_water_cont;
  voltage = (float(analogRead(SensorPin))/1023.0)*1;

  // Save the value to the array
  samples[currentIndex] = voltage;
  currentIndex++;

  // If we've filled up the array, calculate the average value
  if (currentIndex == NUM_SAMPLES) {
    float sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
      sum += samples[i];
    }
    averageValue = sum / NUM_SAMPLES;
    currentIndex = 0;
  }

  // If a minute has passed, print the average value
  unsigned long currentTime = millis();
  if (currentTime - lastReadingTime >= 60000) {
    lastReadingTime = currentTime;
    vol_water_cont = ((averageValue)*slope)+intercept;
    Serial.print("Average voltage over the past minute: ");
    Serial.print(averageValue, 2);
    Serial.print(" V, Theta_v: ");
    Serial.print(vol_water_cont, 2);
    Serial.println(" cm^3/cm^3");
  }
  delay(500); // slight delay between readings
}
