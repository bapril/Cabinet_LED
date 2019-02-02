/*  Simple LED light controller to go under the cabinets in my kitchen. 
    Intended to be loaded on an adafruit trinket/any small arduino should would work. */

// Using the on-board LED pin for simplicity in testing/debugging. 
#define LED_PIN 1
// Any GPIO pin.
#define BUTTON_PIN 0
// How long we wait between cycles.
#define DELAY 10
// How far each fade step takes us
#define FADE_RATE 3
// How long to wait for DEBOUNCE
#define DEBOUNCE 50

// Define States
#define UP 0
#define DOWN 1
#define UP_D 2

// Variables will change:
long brightness = 0; // Easier to catch that we've rolled over high or low. 
int fade = FADE_RATE;
int state;                   // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, brightness);
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);
  int delta = 0;
  if(reading){ // Button Pressed
    if (reading != lastButtonState) { // Pressed now, up before. 
      lastDebounceTime = millis();
      state = DOWN;
    } else { // Pressed now, pressed before. 
      if ((millis() - lastDebounceTime) > DEBOUNCE) {
        delta += fade;
      }
    }
  } else { //Button Up. 
    if (reading != lastButtonState) { //up now, pressed before. 
      lastDebounceTime = millis();
      state = UP;
    } else { //up now, up before. 
      if(state == UP){
        if ((millis() - lastDebounceTime) > DEBOUNCE) {
          state = UP_D;
          if(fade == FADE_RATE){
            fade = 0 - FADE_RATE;
          } else {
            fade = FADE_RATE;
          }
        }
      }
    }
  }
  if(delta){
    brightness = brightness + delta;
    if(brightness > 255) brightness = 255 ;
    if(brightness < 0) brightness = 0 ;
    analogWrite(LED_PIN, brightness);
  }
  delay(DELAY);
  lastButtonState = reading;
}
