//custom keyboard code, created by 8bit-meiko.nl, GPL v2 licenced
#include "Keyboard.h" //include arduino depencency

#define COOLDOWN_TIME 20 //cooldown time before we send a repress of the trigger
#define BUTTON_AMOUNT 6 //max button amount
uint32_t button_ms; //millisecond software time variable
struct button { //button datablock
  bool pressed;
  uint8_t pin;
  uint8_t cooldown;
  char action;
} buttons[BUTTON_AMOUNT]; //make it the max button amount, please.

void set_button(uint8_t id, uint8_t pin, char action) { // set a button's properties
  buttons[id].pressed = 0;
  buttons[id].pin = pin;  
  buttons[id].action = action;
  pinMode(buttons[id].pin,INPUT_PULLUP);
}
void setup_keyboard() { //setup code for the buttons
  set_button(0,1,'w');
  set_button(1,2,'a');
  set_button(2,3,'s');
  set_button(3,4,'d');
  set_button(4,5,'q');
  set_button(5,6,'e');
  Serial.begin(9600);
  Keyboard.begin();
  button_ms = millis();
}
void poll_keyboard() { //poll check, preferably call every millisecond
  if (millis() - button_ms > 0) { //only trigger if its a new millisecond
    button_ms = millis(); //store new millisecond
    uint8_t i = 0; //temp variable for buttons to check
    while (i < BUTTON_AMOUNT) { //nested while loop for checking buttons
      if (buttons[i].cooldown > 0) {
        buttons[i].cooldown--; //subtract one from the cooldown timer
      } else {
        if (buttons[i].pressed == 0) { //if not pressed status stored, do keypress
          if (digitalRead(buttons[i].pin) == LOW) {
            buttons[i].cooldown = COOLDOWN_TIME;
            Keyboard.write(buttons[i].action); 
            buttons[i].pressed = 1;
          }
        } else { //pressed status is true, on finished cooldown, reset button press to host os
          buttons[i].pressed = 0;
          Keyboard.release(buttons[i].action);
        }
      }
      i++; //increment button checker by 1 before we end the nested while loop
    }
  }
}
void setup() {  // initialize the buttons' inputs:
  setup_keyboard();
}
void loop() { // runs as infinite loop
  poll_keyboard();
}
