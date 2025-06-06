#define LED_PIN D5 //digital pin for LED
#define BUTTON_PIN D6 //digital pin for input button

void setup(){
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT); //using external pull-up resistance
}

void loop(){
  //read digital state of input button
  if(digitalRead(BUTTON_PIN) == LOW){   //Input button pressed due to pull-up resistance
    digitalWrite(LED_PIN, HIGH);  //LED turn ON
  }
  else{
    digitalWrite(LED_PIN, LOW);   //LED turn OFF
  }
}