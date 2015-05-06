#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define FLOWSENSORPIN 2
#define PIN 3


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(5, PIN, NEO_GRB + NEO_KHZ800);


// count how many pulses!
volatile uint16_t pulses = 0;
// track the state of the pulse pin
volatile uint8_t lastflowpinstate;
// you can try to keep time of how long it is between pulses
volatile uint32_t lastflowratetimer = 0;
// and use that to calculate a flow rate
volatile float flowrate;
// Interrupt is called once a millisecond, looks for any pulses from the sensor!

SIGNAL(TIMER0_COMPA_vect) {
  uint8_t x = digitalRead(FLOWSENSORPIN);
  
  if (x == lastflowpinstate) {
    lastflowratetimer++;
    return; // nothing changed!
  }
  
  if (x == HIGH) {
    //low to high transition!
    pulses++;
  }
  lastflowpinstate = x;
  flowrate = 1000.0;
  flowrate /= lastflowratetimer;  // in hertz
  lastflowratetimer = 0;
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
  }
}

void setup() {
   Serial.begin(9600);
   // put your setup code here, to run once:
   pinMode(FLOWSENSORPIN, INPUT);
   digitalWrite(FLOWSENSORPIN, HIGH);
   lastflowpinstate = digitalRead(FLOWSENSORPIN);
   useInterrupt(true);
   
   strip.begin();
   strip.show();
   Serial.print("Setup Complete");
}

//Update the color of all the pixels
void changeColor(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
  }
}

void setLeds(int num) {
  Serial.print("Set LEDs");
  Serial.print(num);
  Serial.println();
  uint32_t color = strip.Color(127, 127, 127);
  for(int i=0; i<num; i++) {
      strip.setPixelColor(i, color);
      strip.show();
  }
  for(uint16_t i=num; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, 0);
      strip.show();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //I don't understand this part defining the liters
  float liters = pulses;
  liters /= 7.5;
  liters /= 60.0;
  //end of stuff I don't understand
  delay(100);
  
  float percent = 1 - (liters/2.0);

  int numLeds = (int)(percent * 5) + 1;
  if (percent < 0.025) {
    numLeds = 0;
  }
  setLeds(numLeds);
  
}


