#define LED_PIN D5 //digital pin for LED
#define BUTTON_PIN D6 //digital pin for input button

bool ledState = false;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debouncedDelay = 50;  //50ms debounce interval

void setup(){
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); //using internal pull-up resistance
}

void loop(){
  //read digital state of input button
  bool reading = digitalRead(BUTTON_PIN);
  
  //check if the button state has changed
  if(reading != lastButtonState){ 
    lastDebounceTime = millis();  //  Reset the debounce timer
  }

  //  if the button state has remainedstable for the debounce interval
  if((millis() - lastDebounceTime) > debouncedDelay){
    if (reading == LOW){
      ledState = !ledState;   //  Toggle LED state on button press
      digitalWrite(LED_PIN, ledState);
    }
  }

  lastButtonState = reading;  //  Update the last known state
}