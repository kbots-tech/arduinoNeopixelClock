
#include <DHT.h>

#include <Wire.h>
#include <ds3231.h>
#include <Adafruit_NeoPixel.h>
#include <IRremote.h>
int dat1;
int RECV_PIN = 2 ;
IRrecv irrecv(RECV_PIN);
decode_results results;
#define LED_PIN    9
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 30
// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel backLED(24, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel closetLight(5, 6, NEO_GRB + NEO_KHZ800);
struct ts t;
int lampC = 1;
int lampC2= 1;
int rMin, gMin, bMin;
int rHour, gHour, bHour;
int ran1, ran2, ran3;
int ranA, ranB, ranC;
int rSec, gSec, bSec;
int tMin1, tMin2;
int hourNum1, hourNum2, hourNum3;
int hour;
int min;
bool onOff;
bool whiteLamp;
bool redLamp;
bool blueLamp;
bool greenLamp;
bool tempShow;
bool backLight;
bool closetLamp;
bool rainBowClock;
bool lampClock;
bool pallete;
int count;
int prevBrightness;
int pos = 0;
int cCount = 1;
int backCount = 1;

#define SOLENLOID 8

//Array For Accurate RGB colors
const uint8_t PROGMEM gamma8[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

void setup() {
  attachInterrupt(digitalPinToInterrupt(2), booleanChange, CHANGE);
  pinMode(SOLENLOID, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(7, OUTPUT);
  count = 7;

  whiteLamp = false; redLamp = false;
  greenLamp = false; blueLamp = false;
  tempShow = false;  onOff = true;
  backLight = false; closetLamp = false;
  rainBowClock = false; lampClock = false;
  pallete = true;
  rSec = aC(164); gSec = aC(76); bSec = aC(61);
  rMin = aC(65); gMin = aC(91); bMin = aC(98);
  rHour = aC(95); gHour = aC(126); bHour = aC(103);
  strip.begin();
  backLED.begin();
  backLED.clear();
  strip.clear();
  closetLight.begin();
  closetLight.clear();
  Serial.begin(9600);
  Wire.begin();
  DS3231_init(DS3231_INTCN);
  /*----------------------------------------------------------------------------
    In order to synchronise your clock module, insert timetable values below !
    ----------------------------------------------------------------------------*/
  t.hour = 1;
  t.min = 00;
  t.sec = 00;
  t.mday = 11;
  t.mon = 4;
  t.year = 2020;
  //DS3231_set(t);
  DS3231_get(&t);
  hourNumbers();
  minuteNumbers();
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void loop() {
  
  randomChase();
  delay(500);
  hourNumbers();
  minuteNumbers();
  delay(500);
  backCode();
  DS3231_get(&t);
  /*
    Serial.print("Date : ");
    Serial.print(t.mday);
    Serial.print("/");
    Serial.print(t.mon);
    Serial.print("/");
    Serial.print(t.year);
    Serial.print("\t Hour : ");
    Serial.print(t.hour);
    Serial.print(":");
    Serial.print(t.min);
    Serial.print(".");
    Serial.println(t.sec);
  */
  //Normal Clock Code
  if (redLamp == false && greenLamp == false && blueLamp == false && tempShow == false) {
    //Second Code
    for (int i = 0; i <= t.sec / 2 + 2; i++) {
      if (i == 0) {
        strip.setPixelColor(0, rSec / 4, gSec / 4, bSec / 4);
      }
      strip.setPixelColor(i - 2, rSec, gSec, bSec);
      if (t.sec % 2 != 0) {
        strip.setPixelColor(i - 1, rSec / 4, gSec / 4, bSec / 4);
      }
    }
    if (lampClock == true) {
      for (int i = 0; i < 30; i++) {
        strip.setPixelColor(i, 50, 50, 50);
      }
    }

    if (t.sec == 0) {
      minuteNumbers();
      hourNumbers();
      Serial.println("CLEAR");
      for (int i = 29; i >= 1; i--) {
        strip.show();
        strip.setPixelColor(i, 0, 0, 0);
        //Minute Code
        strip.setPixelColor(tMin1, rMin, gMin, bMin);
        strip.setPixelColor(tMin2, rMin, gMin, bMin);
        //Hour Code
        strip.setPixelColor(hourNum1, rHour, gHour, bHour);
        strip.setPixelColor(hourNum2, rHour, gHour, bHour);

        //Hour=Minute Code
        if (hourNum1 == tMin1)
          strip.setPixelColor(hourNum1, 255, 255, 255);
        if (hourNum2 == tMin1)
          strip.setPixelColor(hourNum2, 255, 255, 255);
        if (hourNum1 == tMin2)
          strip.setPixelColor(hourNum1, 255, 255, 255);
        if (hourNum2 == tMin2)
          strip.setPixelColor(hourNum2, 255, 255, 255);

        //Min more than 30 Code
        if (t.min >= 30) {
          strip.setPixelColor(hourNum2 + 1, rHour, gHour, bHour);
          if (tMin1 == hourNum2 + 1)
            strip.setPixelColor(hourNum2 + 1, 255, 255, 255);
          if (tMin2 == hourNum2 + 1)
            strip.setPixelColor(hourNum2 + 1, 255, 255, 255);
        }
        strip.show();
        delay(45);
      }
    }
    if (t.min == 0 && t.sec == 0) {
      Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
      for (int j = 0; j < hourNum3; j++) {
        for (int i = 0; i < 30; i++) {
          Serial.println("BBBB");
          backLED.setPixelColor(i,255,255,255);
          backLED.setPixelColor(i-3,0,0,0);
          strip.setPixelColor(i, 255, 255, 255);
          strip.setPixelColor(i - 3, 0, 0, 0);
          if (i == 0)
            strip.setPixelColor(27, 0, 0, 0);
          if (i == 1)
            strip.setPixelColor(28, 0, 0, 0);
          if (i == 2)
            strip.setPixelColor(29, 0, 0, 0);
          strip.show();
          backLED.show();
          delay(15);
        }
      }
      strip.setPixelColor(27, 0, 0, 0);
      delay(15);
      strip.setPixelColor(28, 0, 0, 0);
      delay(15);
      strip.setPixelColor(28, 0, 0, 0);
      delay(15);
    }

    //Minute Code
    strip.setPixelColor(tMin1, rMin, gMin, bMin);
    strip.setPixelColor(tMin2, rMin, gMin, bMin);

    //Hour Code
    strip.setPixelColor(hourNum1, rHour, gHour, bHour);
    strip.setPixelColor(hourNum2, rHour, gHour, bHour);
    if (hourNum1 == tMin1)
      strip.setPixelColor(hourNum1, 255, 255, 255);
    if (hourNum2 == tMin1)
      strip.setPixelColor(hourNum2, 255, 255, 255);
    if (hourNum1 == tMin2)
      strip.setPixelColor(hourNum1, 255, 255, 255);
    if (hourNum2 == tMin2)
      strip.setPixelColor(hourNum2, 255, 255, 255);

    //Min more than 30 Code
    if (t.min >= 30) {
      strip.setPixelColor(hourNum2 + 1, rHour, gHour, bHour);
      if (tMin1 == hourNum2 + 1)
        strip.setPixelColor(hourNum2 + 1, 255, 255, 255);
      if (tMin2 == hourNum2 + 1)
        strip.setPixelColor(hourNum2 + 1, 255, 255, 255);
    }
    strip.show();
  }/*Other Unused Lamp Modes*/else if (redLamp == true) {
    for (int i = 0; i < 30; i++) {
      strip.setPixelColor(i, 255, 0, 0);
      strip.show();
    }
  } else if (greenLamp == true) {
    for (int i = 0; i < 30; i++) {
      strip.setPixelColor(i, 0, 255, 0);
      strip.show();
    }
  } else if (blueLamp == true) {
    for (int i = 0; i < 30; i++) {
      strip.setPixelColor(i, 0, 0, 255);
      strip.show();
    }
  } else if (tempShow == true) {
    strip.clear();
    int val1 = String(dat1).substring(0, 1).toInt();
    int val2 = String(dat1).substring(1, 2).toInt();
    Serial.println(val1);
    Serial.println(val2);
    for (int i = 29; i > 29 - val1; i--)
      strip.setPixelColor(i, 255, 0, 0);
    for (int b = 0; b < val2; b++)
      strip.setPixelColor(b, 0, 255, 0);
    strip.show();

  }
}
//Generates Neopixel Values for each hour
void hourNumbers() {
  if (t.hour == 1 || t.hour == 13) {
    hourNum1 = 2;
    hourNum2 = 2;
    hourNum3 = 1;
  } else if (t.hour == 2 || t.hour == 14) {
    hourNum1 = 4;
    hourNum2 = 5;
    hourNum3 = 2;
  } else if (t.hour == 3 || t.hour == 15) {
    hourNum1 = 7;
    hourNum2 = 7;
    hourNum3 = 3;
  } else if (t.hour == 4 || t.hour == 16) {
    hourNum1 = 9;
    hourNum2 = 10;
    hourNum3 = 4;
  } else if (t.hour == 5 || t.hour == 17) {
    hourNum1 = 12;
    hourNum2 = 12;
    hourNum3 = 5;
  } else if (t.hour == 6 || t.hour == 18) {
    hourNum1 = 14;
    hourNum2 = 15;
    hourNum3 = 6;
  } else if (t.hour == 7 || t.hour == 19) {
    hourNum1 = 17;
    hourNum2 = 17;
    hourNum3 = 7;
  } else if (t.hour == 8 || t.hour == 20) {
    hourNum1 = 19;
    hourNum2 = 20;
    hourNum3 = 8;
  } else if (t.hour == 9 || t.hour == 21) {
    hourNum1 = 22;
    hourNum2 = 22;
    hourNum3 = 9;
  } else if (t.hour == 10 || t.hour == 22) {
    hourNum1 = 24;
    hourNum2 = 25;
    hourNum3 = 10;
  } else if (t.hour == 11 || t.hour == 23) {
    hourNum1 = 27;
    hourNum2 = 27;
    hourNum3 = 11;
  } else if (t.hour == 12 || t.hour == 00) {
    hourNum1 = 29;
    hourNum2 = 0;
    hourNum3 = 12;
  }
}
//Generates Neopixel Values for each minute
void minuteNumbers() {
  tMin1 = t.min / 2;
  if (t.min % 2 == 0)
    tMin2 = t.min / 2 - 1;
  else
    tMin2 = t.min / 2;
  if (tMin2 == -1)
    tMin2 = 29;
}
//IR Remote Intterupt Code
void booleanChange() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    if (results.value == 16736925 || results.value == 551485695 || results.value == 2250803747) {
      Serial.println("UP");
      onOff = true;
      if (strip.getBrightness() + 25 >= 255)
        strip.setBrightness(255);
      else
        strip.setBrightness(strip.getBrightness() + 25);
    } else if (results.value == 16754775 || results.value == 551518335 || results.value == 379378719) {
      Serial.println("DOWN");
      if (strip.getBrightness() - 25 < 0)
        strip.setBrightness(0);
      else
        strip.setBrightness(strip.getBrightness() - 25);
    } else if (results.value == 16738455 || results.value == 551520375 || results.value == 719194275) {
      Serial.println("1");
      if (cCount + 1 > 6)
        cCount = 1;
      else
        cCount += 1;
      if (pallete == true) {
        if (cCount == 1) {
          rSec = aC(255); gSec = aC(0); bSec = aC(0);
          rMin = aC(0); gMin = aC(255); bMin = aC(0);
          rHour = aC(0); gHour = aC(0); bHour = aC(255);
        } else if (cCount == 2) {
          rHour = aC(255); gHour = aC(0); bHour = aC(0);
          rSec = aC(0); gSec = aC(255); bSec = aC(0);
          rMin = aC(0); gMin = aC(0); bMin = aC(255);
        } else if (cCount == 3) {
          rMin = aC(255); gMin = aC(0); bMin = aC(0);
          rHour = aC(0); gHour = aC(255); bHour = aC(0);
          rSec = aC(0); gSec = aC(0); bSec = aC(255);
        } else if (cCount == 4) {
          rSec = aC(255); gSec = aC(0); bSec = aC(0);
          rHour = aC(0); gHour = aC(255); bHour = aC(0);
          rMin = aC(0); gMin = aC(0); bMin = aC(255);
        } else if (cCount == 5) {
          rMin = aC(255); gMin = aC(0); bMin = aC(0);
          rSec = aC(0); gSec = aC(255); bSec = aC(0);
          rHour = aC(0); gHour = aC(0); bHour = aC(255);
        } else if (cCount == 6) {
          rHour = aC(255); gHour = aC(0); bHour = aC(0);
          rMin = aC(0); gMin = aC(255); bMin = aC(0);
          rSec = aC(0); gSec = aC(0); bSec = aC(255);
        }
      } else {
        if (cCount == 1) {
          rSec = aC(201); gSec = aC(150); bSec = aC(255);
          rMin = aC(152); gMin = aC(255); bMin = aC(150);
          rHour = aC(255); gHour = aC(253); bHour = aC(150);
        } else if (cCount == 2) {
          rHour = aC(201); gHour = aC(150); bHour = aC(255);
          rSec = aC(152); gSec = aC(255); bSec = aC(150);
          rMin = aC(255); gMin = aC(253); bMin = aC(150);
        } else if (cCount == 3) {
          rMin = aC(201); gMin = aC(150); bMin = aC(255);
          rHour = aC(152); gHour = aC(255); bHour = aC(150);
          rSec = aC(255); gSec = aC(253); bSec = aC(150);
        } else if (cCount == 4) {
          rSec = aC(201); gSec = aC(150); bSec = aC(255);
          rHour = aC(152); gHour = aC(255); bHour = aC(150);
          rMin = aC(255); gMin = aC(253); bMin = aC(150);
        } else if (cCount == 5) {
          rMin = aC(201); gMin = aC(150); bMin = aC(255);
          rSec = aC(152); gSec = aC(255); bSec = aC(150);
          rHour = aC(255); gHour = aC(253); bHour = aC(150);
        } else if (cCount == 6) {
          rHour = aC(201); gHour = aC(150); bHour = aC(255);
          rMin = aC(152); gMin = aC(255); bMin = aC(150);
          rSec = aC(255); gSec = aC(253); bSec = aC(150);
        }
      }

    } else if (results.value == 16750695 || results.value == 551504055 || results.value == 2137078331) {
      Serial.println("2");
      if (cCount + 1 > 6)
        cCount = 1;
      else
        cCount += 1;
      if (pallete == true) {
        if (cCount == 1) {
          rSec = aC(235); gSec = aC(22); bSec = aC(242);
          rMin = aC(29); gMin = aC(242); bMin = aC(22);
          rHour = aC(242); gHour = aC(128); bHour = aC(22);
        } else if (cCount == 2) {
          rHour = aC(235); gHour = aC(22); bHour = aC(242);
          rSec = aC(29); gSec = aC(242); bSec = aC(22);
          rMin = aC(242); gMin = aC(128); bMin = aC(22);
        } else if (cCount == 3) {
          rMin = aC(235); gMin = aC(22); bMin = aC(242);
          rHour = aC(29); gHour = aC(242); bHour = aC(22);
          rSec = aC(242); gSec = aC(128); bSec = aC(22);
        } else if (cCount == 4) {
          rSec = aC(235); gSec = aC(22); bSec = aC(242);
          rHour = aC(29); gHour = aC(242); bHour = aC(22);
          rMin = aC(242); gMin = aC(128); bMin = aC(22);
        } else if (cCount == 5) {
          rMin = aC(235); gMin = aC(22); bMin = aC(242);
          rSec = aC(29); gSec = aC(242); bSec = aC(22);
          rHour = aC(242); gHour = aC(128); bHour = aC(22);
        } else if (cCount == 6) {
          rHour = aC(235); gHour = aC(22); bHour = aC(242);
          rMin = aC(29); gMin = aC(242); bMin = aC(22);
          rSec = aC(242); gSec = aC(128); bSec = aC(22);
        }
      } else {
        if (cCount == 1) {
          rSec = aC(250); gSec = aC(170); bSec = aC(10);
          rMin = aC(250); gMin = aC(230); bMin = aC(10);
          rHour = aC(250); gHour = aC(110); bHour = aC(10);
        } else if (cCount == 2) {
          rHour = aC(250); gHour = aC(170); bHour = aC(10);
          rSec = aC(250); gSec = aC(230); bSec = aC(10);
          rMin = aC(250); gMin = aC(110); bMin = aC(10);
        } else if (cCount == 3) {
          rMin = aC(250); gMin = aC(170); bMin = aC(10);
          rHour = aC(250); gHour = aC(230); bHour = aC(10);
          rSec = aC(250); gSec = aC(110); bSec = aC(10);
        } else if (cCount == 4) {
          rSec = aC(250); gSec = aC(170); bSec = aC(10);
          rHour = aC(250); gHour = aC(230); bHour = aC(10);
          rMin = aC(250); gMin = aC(110); bMin = aC(10);
        } else if (cCount == 5) {
          rMin = aC(250); gMin = aC(170); bMin = aC(10);
          rSec = aC(250); gSec = aC(230); bSec = aC(10);
          rHour = aC(250); gHour = aC(110); bHour = aC(10);
        } else if (cCount == 6) {
          rHour = aC(250); gHour = aC(170); bHour = aC(10);
          rMin = aC(250); gMin = aC(230); bMin = aC(10);
          rSec = aC(250); gSec = aC(110); bSec = aC(10);
        }
      }

    } else if (results.value == 16756815 || results.value == 551536695 || results.value == 440841023) {
      Serial.println("3");
      if (cCount + 1 > 6)
        cCount = 1;
      else
        cCount += 1;
      if (pallete == true) {
        if (cCount == 1) {
          rSec = aC(19); gSec = aC(247); bSec = aC(186);
          rMin = aC(19); gMin = aC(80); bMin = aC(247);
          rHour = aC(50); gHour = aC(247); bHour = aC(19);
        } else if (cCount == 2) {
          rHour = aC(19); gHour = aC(247); bHour = aC(186);
          rSec = aC(19); gSec = aC(80); bSec = aC(247);
          rMin = aC(50); gMin = aC(247); bMin = aC(19);
        } else if (cCount == 3) {
          rMin = aC(19); gMin = aC(247); bMin = aC(186);
          rHour = aC(19); gHour = aC(80); bHour = aC(247);
          rSec = aC(50); gSec = aC(247); bSec = aC(19);
        } else if (cCount == 4) {
          rSec = aC(19); gSec = aC(247); bSec = aC(186);
          rHour = aC(19); gHour = aC(80); bHour = aC(247);
          rMin = aC(50); gMin = aC(247); bMin = aC(19);
        } else if (cCount == 5) {
          rMin = aC(19); gMin = aC(247); bMin = aC(186);
          rSec = aC(19); gSec = aC(80); bSec = aC(247);
          rHour = aC(50); gHour = aC(247); bHour = aC(19);
        } else if (cCount == 6) {
          rHour = aC(19); gHour = aC(247); bHour = aC(186);
          rMin = aC(19); gMin = aC(80); bMin = aC(247);
          rSec = aC(50); gSec = aC(247); bSec = aC(19);
        }
      } else {
        if (cCount == 1) {
          rSec = aC(3); gSec = aC(255); bSec = aC(171);
          rMin = aC(255); gMin = aC(108); bMin = aC(3);
          rHour = aC(255); gHour = aC(3); bHour = aC(45);
        } else if (cCount == 2) {
          rHour = aC(3); gHour = aC(255); bHour = aC(171);
          rSec = aC(255); gSec = aC(108); bSec = aC(3);
          rMin = aC(255); gMin = aC(3); bMin = aC(45);
        } else if (cCount == 3) {
          rMin = aC(3); gMin = aC(255); bMin = aC(171);
          rHour = aC(255); gHour = aC(108); bHour = aC(3);
          rSec = aC(255); gSec = aC(3); bSec = aC(45);
        }else if(cCount==4){
          rSec = aC(3); gSec = aC(255); bSec = aC(171);
          rHour = aC(255); gHour= aC(108); bHour =aC(3);
          rMin = aC(255); gMin = aC(3); bMin = aC(45);
        }else if(cCount==5){
          rMin = aC(3); gMin = aC(255); bMin = aC(171);
          rSec = aC(255); gSec = aC(108); bSec = aC(3);
          rHour = aC(255); gHour =aC(3); bHour =aC(45);
        }else if(cCount==6){
          rHour = aC(3);gHour = aC(255); bHour= aC(171);
          rMin = aC(255); gMin = aC(108); bMin = aC(3);
          rSec = aC(255); gSec = aC(3); bSec = aC(45);
        } 
      }
    } else if (results.value == 16724175 || results.value == 551495895 || results.value == 2671644771) {
      Serial.println("4");
      if (cCount + 1 > 6)
        cCount = 1;
      else
        cCount += 1;
      if (pallete == true) {
        if (cCount == 1) {
          rSec = aC(0); gSec = aC(255); bSec = aC(255);
          rMin = aC(255); gMin = aC(0); bMin = aC(255);
          rHour = aC(255); gHour = aC(255); bHour = aC(0);
        } else if (cCount == 2) {
          rHour = aC(0); gHour = aC(255); bHour = aC(255);
          rSec = aC(255); gSec = aC(0); bSec = aC(255);
          rMin = aC(255); gMin = aC(255); bMin = aC(0);
        } else if (cCount == 3) {
          rMin = aC(0); gMin = aC(255); bMin = aC(255);
          rHour = aC(255); gHour = aC(0); bHour = aC(255);
          rSec = aC(255); gSec = aC(255); bSec = aC(0);
        }else if(cCount==4){
          rSec = aC(0); gSec = aC(255); bSec = aC(255);
          rHour = aC(255); gHour= aC(0); bHour =aC(255);
          rMin = aC(255); gMin = aC(255); bMin = aC(0);
        }else if(cCount==5){
          rMin = aC(0); gMin = aC(255); bMin = aC(255);
          rSec = aC(255); gSec = aC(0); bSec = aC(255);
          rHour = aC(255); gHour =aC(255); bHour =aC(0);
        }else if(cCount==6){
          rHour = aC(0);gHour = aC(255); bHour= aC(255);
          rMin = aC(255); gMin = aC(0); bMin = aC(255);
          rSec = aC(255); gSec = aC(255); bSec = aC(0);
        } 
      } else {
        if (cCount == 1) {
          rSec = aC(254); gSec = aC(95); bSec = aC(85);
          rMin = aC(247); gMin = aC(247); bMin = aC(255);
          rHour = aC(73); gHour = aC(88); bHour = aC(200);
        } else if (cCount == 2) {
          rHour = aC(254); gHour = aC(95); bHour = aC(85);
          rSec = aC(247); gSec = aC(247); bSec = aC(255);
          rMin = aC(73); gMin = aC(88); bMin = aC(200);
        } else if (cCount == 3) {
          rMin = aC(254); gMin = aC(95); bMin = aC(85);
          rHour = aC(247); gHour = aC(247); bHour = aC(255);
          rSec = aC(73); gSec = aC(88); bSec = aC(200);
        }else if(cCount==4){
          rSec = aC(254); gSec = aC(95); bSec = aC(85);
          rHour = aC(247); gHour= aC(247); bHour =aC(255);
          rMin = aC(73); gMin = aC(88); bMin = aC(200);
        }else if(cCount==5){
          rMin = aC(254); gMin = aC(95); bMin = aC(85);
          rSec = aC(247); gSec = aC(247); bSec = aC(255);
          rHour = aC(73); gHour =aC(88); bHour =aC(200);
        }else if(cCount==6){
          rHour = aC(254);gHour = aC(95); bHour= aC(85);
          rMin = aC(247); gMin = aC(247); bMin = aC(255);
          rSec = aC(73); gSec = aC(88); bSec = aC(200);
        } 
      }
    } else if (results.value == 16718055 || results.value == 551528535 || results.value == 800219743) {
      Serial.println("5");
      //Favorite Color Pallate St. Motel Inspired
      if (cCount + 1 > 6)
        cCount = 1;
      else
        cCount += 1;
      if (pallete == true) {
        if (cCount == 1) {
          rSec = aC(164); gSec = aC(76); bSec = aC(61);
          rMin = aC(65); gMin = aC(91); bMin = aC(98);
          rHour = aC(95); gHour = aC(126); bHour = aC(103);
        } else if (cCount == 2) {
          rHour = aC(164); gHour = aC(76); bHour = aC(61);
          rSec = aC(65); gSec = aC(91); bSec = aC(98);
          rMin = aC(95); gMin = aC(126); bMin = aC(103);
        } else if (cCount == 3) {
          rMin = aC(164); gMin = aC(76); bMin = aC(61);
          rHour = aC(65); gHour = aC(91); bHour = aC(98);
          rSec = aC(95); gSec = aC(126); bSec = aC(103);
        }else if(cCount==4){
          rSec = aC(164); gSec = aC(76); bSec = aC(61);
          rHour = aC(65); gHour= aC(91); bHour =aC(98);
          rMin = aC(95); gMin = aC(126); bMin = aC(103);
        }else if(cCount==5){
          rMin = aC(164); gMin = aC(76); bMin = aC(61);
          rSec = aC(65); gSec = aC(91); bSec = aC(98);
          rHour = aC(95); gHour =aC(126); bHour =aC(103);
        }else if(cCount==6){
          rHour = aC(164);gHour = aC(76); bHour= aC(61);
          rMin = aC(65); gMin = aC(91); bMin = aC(98);
          rSec = aC(95); gSec = aC(126); bSec = aC(103);
        } 
      } else {
        if (cCount == 1) {
          rSec = aC(255); gSec = aC(3); bSec = aC(192);
          rMin = aC(3); gMin = aC(255); bMin = aC(192);
          rHour = aC(66); gHour = aC(255); bHour = aC(3);
        } else if (cCount == 2) {
          rHour = aC(255); gHour = aC(3); bHour = aC(192);
          rSec = aC(3); gSec = aC(255); bSec = aC(192);
          rMin = aC(66); gMin = aC(255); bMin = aC(3);
        } else if (cCount == 3) {
          rMin = aC(255); gMin = aC(3); bMin = aC(192);
          rHour = aC(3); gHour = aC(255); bHour = aC(192);
          rSec = aC(66); gSec = aC(255); bSec = aC(3);
        }else if(cCount==4){
          rSec = aC(255); gSec = aC(3); bSec = aC(192);
          rHour = aC(3); gHour= aC(255); bHour =aC(192);
          rMin = aC(66); gMin = aC(255); bMin = aC(3);
        }else if(cCount==5){
          rMin = aC(255); gMin = aC(3); bMin = aC(192);
          rSec = aC(3); gSec = aC(255); bSec = aC(192);
          rHour = aC(66); gHour =aC(255); bHour =aC(3);
        }else if(cCount==6){
          rHour = aC(255);gHour = aC(3); bHour= aC(192);
          rMin = aC(3); gMin = aC(255); bMin = aC(192);
          rSec = aC(66); gSec = aC(255); bSec = aC(3);
        } 
      }
    } else if (results.value == 16743045) {
      Serial.println("6");
      if (cCount + 1 > 3)
        cCount = 1;
      else
        cCount += 1;
      if (cCount == 1) {
        rHour = aC(5); gHour = aC(148); bHour = aC(33);
        rSec = aC(148); gSec = aC(34); bSec = aC(5);
        rMin = aC(34); gMin = aC(5); bMin = aC(148);
      } else if (cCount == 2) {
        rSec = 5; gSec = 148; bSec = 33;
        rMin = 148; gMin = 34; bMin = 5;
        rHour = 34; gHour = 5; bHour = 148;
      } else if (cCount == 3) {
        rMin = 5; gMin = 148; bMin = 33;
        rHour = 148; gHour = 34; bHour = 5;
        rSec = 34; gSec = 5; bSec = 148;
      }

    } else if (results.value == 16716015) {
      Serial.println("7");
      rSec = 86; gSec = 18; bSec = 204;
      rMin = 204; gMin = 192; bMin = 18;
      rHour = 204; gHour = 98; bHour = 18;

    } else if (results.value == 16726215 || results.value == 1290492415 || results.value == 551512215) {
      Serial.println("8");
      randomClock();
    } else if (results.value == 16734885) {
      Serial.println("9");
      randomColors();
      for (int i = 0; i < 24; i++)
        backLED.setPixelColor(i, ran1, ran2, ran3);
      backLED.show();
    } else if (results.value == 16712445 || results.value == 1743457375 || results.value == 551508135) {
      Serial.println("OK");
      if (onOff == true) {
        onOff = false;
        prevBrightness = strip.getBrightness();
        strip.setBrightness(0);
        strip.show();
        backLED.setBrightness(0);
        backLED.show();
      } else {
        onOff = true;
        strip.setBrightness(prevBrightness);
        strip.show();
        backLED.setBrightness(255);
        backLED.show();
      }
    } else if (results.value == 16728765 || results.value == 551491815 || results.value == 3380893819) {
      Serial.println("*");
      randomColors();
      if (backLight == true) {
        Serial.println("off");
        backLight = false;
        backLED.clear();
        backLED.show();
      } else {
        Serial.println("on");
        backLight = true;
      }
    } else if (results.value == 16732845) {
      Serial.println("#");
      if (closetLamp == true) {
        closetLamp = false;
        closetLight.clear();
        closetLight.show();
        Serial.println("false");

      } else {
        closetLamp = true;
        closetLight.setPixelColor(0, 255, 255, 255);
        closetLight.setPixelColor(1, 255, 255, 255);
        closetLight.setPixelColor(2, 255, 255, 255);
        closetLight.setPixelColor(3, 255, 255, 255);
        closetLight.setPixelColor(4, 255, 255, 255);
        closetLight.show();
        Serial.println("true");
      }

    } else if (results.value == 16730805 || results.value == 3509629087 || results.value == 551487735) {
      Serial.println("0");
      lampChange();

    } else if (results.value == 16761405 || results.value == 3889222403||results.value==551544855) {
      randomColors();
      Serial.println("R");
      if (count + 1 > 11) {
        count = 1;
      } else
        count += 1;
      backCode();

    } else if (results.value == 16720605 || results.value == 551524455 || results.value == 590459007) {
      randomColors();
      Serial.println("L");
      if (count - 1 < 1) {
        count = 11;
      } else
        count -= 1;
      backCode();

    } else if (results.value == 551550720) {

    } else if (results.value == 2900054078) {
      justLamp();
    } else if (results.value == 551540775) {
      if (pallete == true)
        pallete = false;
      else
        pallete = true;
    }

    irrecv.resume(); // Receive the next value
  }
}


//Code for TV back Light
void backCode() {
  if (backLight == true) {
    if (count == 1) {
      Serial.println("1");
      for (int i = 0; i < 24; i++)
        backLED.setPixelColor(i, 255, 0, 0);

    } else if (count == 2) {
      Serial.println("2");
      for (int i = 0; i < 24; i++)
        backLED.setPixelColor(i, 0, 255, 0);

    } else if (count == 3) {
      Serial.println("3");
      for (int i = 0; i < 24; i++)
        backLED.setPixelColor(i, 0, 0, 255);

    } else if (count == 4) {
      Serial.println("4");
      for (int i = 0; i < 24; i++)
        backLED.setPixelColor(i, 255, 255, 255);

    } else if (count == 5) {
      Serial.println("5");
      for (int i = 0; i < 7 ; i++)
        backLED.setPixelColor(i, 255, 0, 0);
      for (int j = 7; j < 15; j++)
        backLED.setPixelColor(j, 0, 255, 0);
      for (int k = 15; k < 22; k++)
        backLED.setPixelColor(k, 0, 0, 255);

    } else if (count == 6) {
      Serial.println("6");
      for (int i = 0; i < 24; i++)
        backLED.setPixelColor(i, ran1, ran2, ran3);
    } else if (count == 7) {
      Serial.println("7");
      randomColors();
      for (int i = 0; i < 24; i++)
        backLED.setPixelColor(i, ran1, ran2, ran3);
    } else if (count == 8) {
      Serial.println("8");
      for (int i = 0; i < 24; i++)
        backLED.setPixelColor(i, 255, 15, 15);
    } else if (count == 9) {
      Serial.println("9");
      for (int i = 0; i < 24; i++)
        backLED.setPixelColor(i, 95, 255, 105);
    } else if (count == 10) {
      Serial.println("10");
      for (int i = 0; i < 24; i++)
        backLED.setPixelColor(i, 75, 75, 255);
    } else if (count == 11) {
      Serial.println("11");
      stColor();
    }
  }
  backLED.show();
}

//Code for Random back Light
void randomColors() {
  ran1 = aC(random(0, 255));
  ran2 = aC(random(0, 255));
  ran3 = aC(random(0, 255));
}


uint32_t randomChase() {
  ranA = random(0, 255);
  ranB = random(0, 255);
  ranC = random(0, 255);
  return (ranA, ranB, ranC);
}

//Code for randomizing Clock Colors
void randomClock() {
  rHour = aC(random(0, 255)); gHour = aC(random(0, 255)); bHour = aC(random(0, 255));
  rMin = aC(random(0, 255)); gMin = aC(random(0, 255)); bMin = aC(random(0, 255));
  rSec = aC(random(0, 255)); gSec = aC(random(0, 255)); bSec = aC(random(0, 255));
}

//Saint Motel Back Light
void stColor() {
  if (backCount + 1 == 4)
    backCount = 1;
  else
    backCount += 1;
  if (backCount == 1)
    for (int i = 0; i < 24; i++)
      backLED.setPixelColor(i, 255, 15, 15);
  else if (backCount == 2)
    for (int i = 0; i < 24; i++)
      backLED.setPixelColor(i, 95, 255, 105);
  else if (backCount == 3)
    for (int i = 0; i < 24; i++)
      backLED.setPixelColor(i, 75, 75, 255);
}

//Code to get proper RGB number
int aC(int n) {
  return pgm_read_byte(&gamma8[n]);
}

void lampChange(){
  Serial.println(lampC);
  if(lampC==1){
    digitalWrite(8,HIGH);
    digitalWrite(7,HIGH);
  }else if(lampC==2){
    digitalWrite(8,HIGH);
    digitalWrite(7,LOW);
  }else if(lampC==3){
    digitalWrite(8,LOW);
    digitalWrite(7,HIGH);
  }else if(lampC==4){
    digitalWrite(8,LOW);
    digitalWrite(7,LOW);
  }
  
  if(lampC==4)
    lampC=1;
  else
    lampC+=1;
   
  
}

void justLamp(){
  Serial.println(lampC2);
  if(lampC2==1){
    digitalWrite(8,HIGH);
    digitalWrite(7,HIGH);
    for(int i=0; i<24; i++)
      backLED.setPixelColor(i,255,255,255);
    backLED.show();
  }else if(lampC2==2){
    digitalWrite(8,HIGH);
    digitalWrite(7,LOW);
    for(int i=0; i<24; i++)
      backLED.setPixelColor(i,0,255,255);
    backLED.show();
  }else if(lampC2==3){
    digitalWrite(8,LOW);
    digitalWrite(7,HIGH);
    for(int i=0; i<24; i++)
      backLED.setPixelColor(i,255,0,0);
    backLED.show();
  }else if(lampC2==4){
    digitalWrite(8,LOW);
    digitalWrite(7,LOW);
    backLED.clear();
    backLED.show();
  }
  
  if(lampC2==4)
    lampC2=1;
  else
    lampC2+=1;
    
}
