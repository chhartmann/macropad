#include <WS2812.h>
#include <Serial.h>

int copyButton = 11; // copy
int pasteButton = 17; // paste
int cutButton = 16; // cut

#define NUM_BUTTON 3
static const uint8_t buttonPins[] = {11, 17, 16};
bool buttonPressed[NUM_BUTTON] = {0};
char* buttonMsgs = "BTN_COPY\0\0BTN_PASTE\0BTN_CUT\0";

#define NUM_LEDS 3
#define COLOR_PER_LEDS 3
#define NUM_BYTES (NUM_LEDS*COLOR_PER_LEDS)
#define SIZE_RCV_STR 16

uint8_t ledTargetColor[NUM_BYTES] = {0};
uint8_t ledCurrentColor[NUM_BYTES] = {0};
bool ledFading[NUM_LEDS] = {false};
char recvStr[SIZE_RCV_STR];
uint8_t recvStrPtr = 0;
bool stringComplete = false;
int8_t fadePercent = 100;
int8_t fadeDirection = -1;

uint8_t hex2byte(char* hex) {
  uint8_t byte = 0;
  if (hex[0] < 'A') {
    byte = (hex[0] - '0') << 4;
  } else {
    byte = (hex[0] - 'A' + 10 ) << 4;
  }
  if (hex[1] < 'A') {
    byte |= (hex[1] - '0');
  } else {
    byte |= (hex[1] - 'A' + 10 );    
  }
  return byte;
}


void setup() {

  pinMode(34, OUTPUT); //Possible to use other pins.
  for (uint8_t i = 0; i < NUM_LEDS; ++i) {
    set_pixel_for_GRB_LED(ledTargetColor, i, 0, 0, 0); // R G B
  }
  neopixel_show_P3_4(ledTargetColor, NUM_BYTES);
}

void loop() {

  while (USBSerial_available()) {
    char serialChar = USBSerial_read();
    if ((serialChar == '\n') || (serialChar == '\r') ) {
      recvStr[recvStrPtr] = '\0';
      recvStrPtr = 0;
      USBSerial_flush();
      stringComplete = true;
      break;
    } else {
      recvStr[recvStrPtr] = serialChar;
      recvStrPtr++;
      if (recvStrPtr == SIZE_RCV_STR) {
        recvStrPtr = 0;
        USBSerial_flush();
        USBSerial_println("RCV_ERR");
        break;
      }
    }
  }

  if (stringComplete == true) {
    stringComplete = false;
    if (strlen(recvStr) == 10) {
      // SET1FFFFFF
      // SET2FFFFFF
      // SET3FFFFFF
      set_pixel_for_GRB_LED(ledTargetColor, recvStr[3] - '1', hex2byte(&recvStr[4]), hex2byte(&recvStr[6]), hex2byte(&recvStr[8]));
      neopixel_show_P3_4(ledTargetColor, NUM_BYTES);
      USBSerial_println("SET_OK");
    } else if (strlen(recvStr) == 7) {
      // FADE000
      // FADE111
      for (uint8_t i = 0; i < NUM_LEDS; ++i) {
        ledFading[i] = (recvStr[4 + i] == '1');
      }
      USBSerial_println("FADE_OK");
    } else {
      USBSerial_println("RET_ERR");
    }
  }

  fadePercent += fadeDirection * 4;
  if (fadePercent <= 0) {
    fadePercent = 0;
    fadeDirection = 1;
  }
  if (fadePercent >= 100) {
    fadePercent = 100;
    fadeDirection = -1;
  }
  for (uint8_t i = 0; i < NUM_LEDS; ++i) {
    if (ledFading[i]) {
      for (uint8_t c = 0; c < COLOR_PER_LEDS; ++c) {
        ledCurrentColor[i*COLOR_PER_LEDS + c] = ((int)ledTargetColor[i*COLOR_PER_LEDS + c]) * fadePercent / 100;
      }
      neopixel_show_P3_4(ledCurrentColor, NUM_BYTES);
    } 
  }

  for (uint8_t i = 0; i < NUM_BUTTON; ++i) {
    int buttonState = digitalRead(buttonPins[i]);
    if (!buttonState && buttonPressed[i]) {
      USBSerial_println(&buttonMsgs[i*10]);
    }
    buttonPressed[i] = buttonState;
  }

  delay(30);
}
