/* https://github.com/earlephilhower/arduino-pico
  https://docs.arduino.cc/hardware/nano-rp2040-connect
  https://github.com/arduino/ArduinoCore-mbed/blob/master/libraries/USBHID/src/USBKeyboard.h */

/* Arduino Mbed OS RP2040 boards */

#include "PluggableUSBHID.h"
#include "USBKeyboard.h"
#include <Arduino.h>

/* HW pinout */
#define HW_ENCODER_DT         20
#define HW_ENCODER_CLK        21
#define HW_ENCODER_SW         19

#define HW_BUTTON_PREW        6
#define HW_BUTTON_STOP        5
#define HW_BUTTON_NEXT        3
#define HW_BUTTON_A           2
#define HW_BUTTON_MUTE        4
#define HW_BUTTON_C           7

/* Global definition */
#define SW_VERSION            "1.0.0"
#define LOOP_REFRESH_TIME     100      ///< ms
#define DIRECTION_INC         true
#define DIRECTION_DEC         false
#define BUTTON_DEFAULT_STATE  1

#define ENCODER_DEC           false
#define ENCODER_INC           true

/* Global function */
void ProcessEncoder();
void TestButton();

/* Global Variable */
int EncSteps = 0;
bool EncStatus = false;
bool EncProcess = true;

bool ButtonProcEnc = false;
bool ButtonProcStop = false;
bool ButtonProcMute = false;
bool ButtonProcNext = false;
bool ButtonProcPrew = false;
bool ButtonProcA = false;
bool ButtonProcC = false;

USBKeyboard Keyboard;

void setup() {
  Serial.begin(115200);
  Serial.print("Start MCU! ");
  Serial.println(SW_VERSION);
  pinMode(LED_BUILTIN, OUTPUT);

  /* configuration encoder*/
  pinMode(HW_ENCODER_DT, INPUT);
  pinMode(HW_ENCODER_CLK, INPUT);
  pinMode(HW_ENCODER_SW, INPUT);
  attachInterrupt(HW_ENCODER_DT, ProcessEncoder, CHANGE );

  /* configuration buttons */
  pinMode(HW_BUTTON_PREW, INPUT_PULLUP);
  pinMode(HW_BUTTON_STOP, INPUT_PULLUP);
  pinMode(HW_BUTTON_NEXT, INPUT_PULLUP);
  pinMode(HW_BUTTON_A, INPUT_PULLUP);
  pinMode(HW_BUTTON_MUTE, INPUT_PULLUP);
  pinMode(HW_BUTTON_C, INPUT_PULLUP);
  gpio_pull_up(HW_BUTTON_PREW);
  gpio_pull_up(HW_BUTTON_STOP);
  gpio_pull_up(HW_BUTTON_NEXT);
  gpio_pull_up(HW_BUTTON_A);
  gpio_pull_up(HW_BUTTON_MUTE);
  gpio_pull_up(HW_BUTTON_C);

  //Keyboard.printf("Hello world\n\r");
}

void loop() {
  delay(LOOP_REFRESH_TIME);

  /* process encoder data */
  if (EncProcess == false) {
    EncProcess = true;
    Serial.print("Encoder [");
    Serial.print(EncSteps);
    Serial.print("]:");
    if (EncStatus == ENCODER_DEC) {
      Keyboard.media_control(KEY_VOLUME_DOWN);
      Serial.println("volume down");

    } else if (EncStatus == ENCODER_INC) {
      Keyboard.media_control(KEY_VOLUME_UP);
      Serial.println("volume up");
    }
  }

  /* process encoder button */
  if ((digitalRead(HW_ENCODER_SW) != BUTTON_DEFAULT_STATE) && (!ButtonProcEnc)) {
    Serial.println("Play/Pause");
    Keyboard.media_control(KEY_PLAY_PAUSE);
    ButtonProcEnc = true;
  } else {
    ButtonProcEnc = false;
  }

  /* process button */
  if ((digitalRead(HW_BUTTON_STOP) != BUTTON_DEFAULT_STATE) && (!ButtonProcStop)) {
    Serial.println("Stop");
    Keyboard.media_control(KEY_STOP);
    ButtonProcStop = true;
  } else {
    ButtonProcStop = false;
  }

  /* process button */
  if ((digitalRead(HW_BUTTON_MUTE) != BUTTON_DEFAULT_STATE) && (!ButtonProcMute)) {
    Serial.println("Mute");
    Keyboard.media_control(KEY_MUTE);
    ButtonProcMute = true;
  } else {
    ButtonProcMute = false;
  }

  /* process button */
  if ((digitalRead(HW_BUTTON_NEXT) != BUTTON_DEFAULT_STATE) && (!ButtonProcNext)) {
    Serial.println("Next track");
    Keyboard.media_control(KEY_NEXT_TRACK);
    ButtonProcNext = true;
  } else {
    ButtonProcNext = false;
  }

  /* process button */
  if ((digitalRead(HW_BUTTON_PREW) != BUTTON_DEFAULT_STATE) && (!ButtonProcPrew)) {
    Serial.println("Previous track");
    Keyboard.media_control(KEY_PREVIOUS_TRACK);
    ButtonProcPrew = true;
  } else {
    ButtonProcPrew = false;
  }

  /* process button */
  if ((digitalRead(HW_BUTTON_A) != BUTTON_DEFAULT_STATE) && (!ButtonProcA)) {
    Serial.println("Button A");
    //Keyboard.key_code_three('M', KEY_CTRL , KEY_SHIFT  );
    ButtonProcA = true;
  } else {
    ButtonProcA = false;
  }

  /* process button */
  if ((digitalRead(HW_BUTTON_C) != BUTTON_DEFAULT_STATE) && (!ButtonProcC)) {
    Serial.println("Mute meet");
    Keyboard.key_code('d', KEY_CTRL);
    ButtonProcC = true;
  } else {
    ButtonProcC = false;
  }

}

void ProcessEncoder() {
  if (digitalRead(HW_ENCODER_CLK) == digitalRead(HW_ENCODER_DT)) {
    EncSteps++;
    EncStatus = ENCODER_INC;
  } else {
    EncSteps--;
    EncStatus = ENCODER_DEC;
  }
  EncProcess = false;
}

void TestButton() {
  if (digitalRead(HW_BUTTON_PREW) != BUTTON_DEFAULT_STATE) {
    Serial.println("HW_BUTTON_PREW");
  } else if (digitalRead(HW_BUTTON_STOP) != BUTTON_DEFAULT_STATE) {
    Serial.println("HW_BUTTON_STOP");
  } else if (digitalRead(HW_BUTTON_NEXT) != BUTTON_DEFAULT_STATE) {
    Serial.println("HW_BUTTON_NEXT");
  } else if (digitalRead(HW_BUTTON_A) != BUTTON_DEFAULT_STATE) {
    Serial.println("HW_BUTTON_A");
  } else if (digitalRead(HW_BUTTON_MUTE) != BUTTON_DEFAULT_STATE) {
    Serial.println("HW_BUTTON_MUTE");
  } else if (digitalRead(HW_BUTTON_C) != BUTTON_DEFAULT_STATE) {
    Serial.println("HW_BUTTON_C");
  } else if (digitalRead(HW_ENCODER_SW) != BUTTON_DEFAULT_STATE) {
    Serial.println("HW_ENCODER_SW");
  }
}

/* EOF */
