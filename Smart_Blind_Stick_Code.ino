#define TRIG_PIN 9        // Ultrasonic Trig pin
#define ECHO_PIN 10       // Ultrasonic Echo pin
#define BUZZER_PIN 8      // Buzzer pin
#define SOS_BUTTON 2      // SOS button pin
#define WATER_SENSOR A0   // Analog pin for Soil Moisture Sensor
#define LED_PIN 7         // LED indicator for water alert

bool buzzerState = false;  // To track buzzer state from moisture detection

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(SOS_BUTTON, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(9600); // Start serial communication
}

void loop() {
    // --- Obstacle Detection ---
    long duration;
    float distance;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * 0.034 / 2;

    if (distance > 0 && distance <= 20) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm - CRITICAL! Very close to obstacle!");
} else if (distance > 20 && distance <= 60) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm - Obstacle detected careful!");
} else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
}


    // Default to buzzer off
    digitalWrite(BUZZER_PIN, LOW);

    // Obstacle proximity alert
    if (distance > 0 && distance <= 20) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(100);
        digitalWrite(BUZZER_PIN, LOW);
        delay(100);
    } else if (distance > 20 && distance <= 50) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(300);
        digitalWrite(BUZZER_PIN, LOW);
        delay(300);
    } else if (distance > 50 && distance <= 80) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(500);
        digitalWrite(BUZZER_PIN, LOW);
        delay(500);
    }

    // --- Water Detection ---
    int moistureValue = analogRead(WATER_SENSOR);
    int moisturePercent = map(moistureValue, 1023, 0, 0, 100); // 0 = dry, 100 = wet

    if (moisturePercent <40){
        Serial.println("Water detected!");
        digitalWrite(LED_PIN, HIGH);
        buzzerState = true;
    } else {
        Serial.println("Dry surface");
        digitalWrite(LED_PIN, LOW);
        buzzerState = false;
    }

    // --- SOS Button ---
    if (digitalRead(SOS_BUTTON) == LOW) {
        Serial.println("SOS Activated!");
        tone(BUZZER_PIN, 2000);  // 2kHz emergency tone
        delay(500);
        noTone(BUZZER_PIN);
    } else if (buzzerState) {
        digitalWrite(BUZZER_PIN, HIGH);
    } else {
        digitalWrite(BUZZER_PIN, LOW);
    }

    delay(500);  // Slight delay before next loop
}
 