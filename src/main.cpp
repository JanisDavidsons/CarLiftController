#include <Arduino.h>
#include <Bounce2.h>

#define UP_BUTTON 7
#define DOWN_BUTTON 6

#define PHASE_SWAP_RELAY_1_1 A5
#define PHASE_SWAP_RELAY_1_2 A4
#define PHASE_SWAP_RELAY_2_1 A3
#define PHASE_SWAP_RELAY_2_2 A2
#define LEFT_MOTOR_RELAY A1
#define RIGHT_MOTOR_RELAY A0

#define LEFT_UPPER_END_SWITCH 9
#define LEFT_BOTTOM_END_SWITCH 10
#define RIGHT_UPPER_END_SWITCH 12
#define RIGHT_BOTTOM_END_SWITCH 13
#define LEFT_SYNC_SWITCH 11
#define RIGHT_SYNC_SWITCH 8

struct ButtonConfig {
  Bounce2::Button button;
  int pin;
  const char* name;
};

ButtonConfig buttons[] = {
  {Bounce2::Button(), UP_BUTTON, "Up Button"},
  {Bounce2::Button(), DOWN_BUTTON, "Down Button"},
  {Bounce2::Button(), LEFT_UPPER_END_SWITCH, "Left Upper End Switch"},
  {Bounce2::Button(), LEFT_BOTTOM_END_SWITCH, "Left Bottom End Switch"},
  {Bounce2::Button(), RIGHT_UPPER_END_SWITCH, "Right Upper End Switch"},
  {Bounce2::Button(), RIGHT_BOTTOM_END_SWITCH, "Right Bottom End Switch"},
  {Bounce2::Button(), LEFT_SYNC_SWITCH, "Left Sync Switch"},
  {Bounce2::Button(), RIGHT_SYNC_SWITCH, "Right Sync Switch"}
};

void setupButtons() {
    for (auto &btn : buttons) {
        pinMode(btn.pin, INPUT);
        btn.button.attach(btn.pin, INPUT);
        btn.button.interval(5);
        btn.button.setPressedState(LOW);
    }
}

void setupRelays() {
    int relays[] = {LEFT_MOTOR_RELAY, RIGHT_MOTOR_RELAY,
                    PHASE_SWAP_RELAY_1_1, PHASE_SWAP_RELAY_1_2,
                    PHASE_SWAP_RELAY_2_1, PHASE_SWAP_RELAY_2_2};
    
    for (int relay : relays) {
        pinMode(relay, OUTPUT);
        digitalWrite(relay, HIGH);
    }
}

void setup() {
    Serial.begin(9600);
    setupRelays();
    setupButtons();
}

void loop() {
    for (auto &btn : buttons) {
        btn.button.update();
    }

    bool upButtonPressed = buttons[0].button.pressed();
    bool upButtonReleased = buttons[0].button.released();
    bool downButtonPressed = buttons[1].button.pressed();
    bool downButtonReleased = buttons[1].button.released();
    bool bothButtonsPressed = buttons[0].button.isPressed() && buttons[1].button.isPressed();
    
    bool upperLimitReached = buttons[2].button.isPressed() || buttons[4].button.isPressed();
    bool lowerLimitReached = buttons[3].button.isPressed() || buttons[5].button.isPressed();
    bool holdingUpButton = buttons[0].button.isPressed();
    bool holdingDownButton = buttons[1].button.isPressed();

    if (bothButtonsPressed) {
        Serial.println("Both buttons pressed");
        digitalWrite(PHASE_SWAP_RELAY_1_1, HIGH);
        digitalWrite(PHASE_SWAP_RELAY_1_2, HIGH);
        digitalWrite(PHASE_SWAP_RELAY_2_1, HIGH);
        digitalWrite(PHASE_SWAP_RELAY_2_2, HIGH);
        return;
    }

    if (upperLimitReached && holdingUpButton)
    {
      Serial.println("Upper Limit reached");
      digitalWrite(PHASE_SWAP_RELAY_1_1, HIGH);
      digitalWrite(PHASE_SWAP_RELAY_1_2, HIGH);
      digitalWrite(PHASE_SWAP_RELAY_2_1, HIGH);
      digitalWrite(PHASE_SWAP_RELAY_2_2, HIGH);
      return;
    }

    if (lowerLimitReached && holdingDownButton)
    {
      Serial.println("Lower Limit reached");
      digitalWrite(PHASE_SWAP_RELAY_1_1, HIGH);
      digitalWrite(PHASE_SWAP_RELAY_1_2, HIGH);
      digitalWrite(PHASE_SWAP_RELAY_2_1, HIGH);
      digitalWrite(PHASE_SWAP_RELAY_2_2, HIGH);
      return;
    }
    

    if (upButtonPressed) {
        Serial.println("going up");
        digitalWrite(PHASE_SWAP_RELAY_1_1, LOW);
        digitalWrite(PHASE_SWAP_RELAY_1_2, LOW);
    } else if (upButtonReleased) {
        digitalWrite(PHASE_SWAP_RELAY_1_1, HIGH);
        digitalWrite(PHASE_SWAP_RELAY_1_2, HIGH);
    }

    if (downButtonPressed) {
        Serial.println("going down");
        digitalWrite(PHASE_SWAP_RELAY_2_1, LOW);
        digitalWrite(PHASE_SWAP_RELAY_2_2, LOW);
    } else if (downButtonReleased) {
        digitalWrite(PHASE_SWAP_RELAY_2_1, HIGH);
        digitalWrite(PHASE_SWAP_RELAY_2_2, HIGH);
    }

    for (int i = 2; i < 8; i++) {
      if (buttons[i].button.isPressed()) {
          Serial.println(String(buttons[i].name) + " activated");
      }
  }
}
