#include <Adafruit_DotStar.h>
#include <Keyboard.h>

#define BUTTON_PIN 2
#define REQ_PIN 1
#define CLK_PIN 3
#define DATA_PIN 4

Adafruit_DotStar strip = Adafruit_DotStar(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

int i = 0;
int j = 0;
int k = 0;
int signCh = 8;
int sign = 0;
int decimal;
float dpp;
int units;

byte mydata[14];
String value_str;
long value_int; //was an int, could not measure over 32mm
float value;

void req_on() {
  // Should pre-condition REQ low, then assert low, without ever transitioning through a HIGH state.
  pinMode(REQ_PIN, INPUT_PULLDOWN);
  pinMode(REQ_PIN, OUTPUT);
  digitalWrite(REQ_PIN, LOW);
}

void req_off() {
  // Set REQ as high-z (off)
  pinMode(REQ_PIN, INPUT);
}

void sbcRead() {
  req_on(); // generate set request
  for( i = 0; i < 13; i++ ) {
    k = 0;
    for (j = 0; j < 4; j++) {
      while( digitalRead(CLK_PIN) == LOW) {
      } // hold until clock is high
      while( digitalRead(CLK_PIN) == HIGH) {
      } // hold until clock is low
      bitWrite(k, j, (digitalRead(DATA_PIN) & 0x1));
    }
    mydata[i] = k;
  }
  sign = mydata[4];
  value_str = String(mydata[5]) + String(mydata[6]) + String(mydata[7]) + String(mydata[8] + String(mydata[9] + String(mydata[10]))) ;
  decimal = mydata[11];
  units = mydata[12];

  value_int = value_str.toInt();
  if (decimal == 0) dpp = 1.0;
  if (decimal == 1) dpp = 10.0;
  if (decimal == 2) dpp = 100.0;
  if (decimal == 3) dpp = 1000.0;
  if (decimal == 4) dpp = 10000.0;
  if (decimal == 5) dpp = 100000.0;

  value = value_int / dpp;

  if (sign == 0) {
    Serial.println(value,decimal);
  }
  if (sign == 8) {
    Serial.print("-"); Serial.println(value,decimal);
  }

  req_off();
  delay(100);
}

void setup() {
  strip.begin();
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  req_off();
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DATA_PIN, INPUT_PULLUP);
  Keyboard.begin();
}

char latestReading [11];

bool armed = false;

void loop() {
  Serial.println(armed);
  if ( armed && (!digitalRead(BUTTON_PIN)) ) {
    armed = false; // Make sure we only take one reading per button press.
    // Button is pressed (low)
    strip.setPixelColor(0, 64, 0, 0); strip.show(); //red
    sbcRead();
    Keyboard.println(value, decimal);
  } else {
    if (digitalRead(BUTTON_PIN)) {
      // Button is released.
      armed = true; // Re-arm now that we've seen a released button.
    }
    strip.setPixelColor(0, 0, 64, 0); strip.show(); //green
  }
}