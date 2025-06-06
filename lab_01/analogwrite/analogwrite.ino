#define LED_PIN D5
#define BUTTON_PIN D6

int brightness = 0;  // LED brightness level (0-255)
int fadeAmount = 5;  // Step size for increasing/decreasing brightness

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Use internal pull-up
}

void loop() {
    if (digitalRead(BUTTON_PIN) == LOW) { // Button pressed
        brightness += fadeAmount;  // Increase brightness
        if (brightness > 255) brightness = 255; // Limit max brightness
    } else { // Button released
        brightness -= fadeAmount;  // Decrease brightness
        if (brightness < 0) brightness = 0; // Limit min brightness
    }

    analogWrite(LED_PIN, brightness);  // Apply brightness level
    delay(30); // Small delay to make dimming effect smooth
}
