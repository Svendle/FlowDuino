#define FLOWSENSORPIN 2
// count how many pulses!
volatile uint16_t pulses = 0;
// track the state of the pulse pin
volatile uint8_t lastflowpinstate;
// you can try to keep time of how long it is between pulses
volatile uint32_t lastflowratetimer = 0;
// and use that to calculate a flow rate
volatile float flowrate;
// Interrupt is called once a millisecond, looks for any pulses from the sensor!
//integer values for the LED I/O pins that will light up while we drink
int LED1 = 13;
int LED2 = 14;
int LED3 = 15;
int LED4 = 16;
int LED5 = 17;
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
  // put your setup code here, to run once:
   pinMode(FLOWSENSORPIN, INPUT);
   digitalWrite(FLOWSENSORPIN, HIGH);
   lastflowpinstate = digitalRead(FLOWSENSORPIN);
   useInterrupt(true);
   //Initializing LEDs
   pinmode(LED1, OUTPUT);
   pinmode(LED2, OUTPUT);
   pinmode(LED3, OUTPUT);
   pinmode(LED4, OUTPUT);
   pinmode(LED5, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //I don't understand this part defining the liters
  float liters = pulses;
  liters /= 7.5;
  liters /= 60.0;
  //end of stuff I don't understand
  delay(100);
  //In theory these 5 if/else statements will 
  //run every single time and only turn the LEDs
  //on when the value of liters surpasses some value
  //How this is executed will change based on what liters does
  //since I am noob and don't know what it is
  if (liters >= 1)
    {
        digitalWrite(LED1,HIGH);
    }
  else
    {
      digitalWrite(LED1,LOW);
    }
  if (liters >= 2)
    {
        digitalWrite(LED2,HIGH);
    }
  else
    {
      digitalWrite(LED2,LOW);
    }
  if (liters >= 3)
    {
        digitalWrite(LED3,HIGH);
    }
  else
    {
      digitalWrite(LED3,LOW);
    }
  if (liters >= 4)
    {
        digitalWrite(LED4,HIGH);
    }
  else
    {
      digitalWrite(LED4,LOW);
    }
  if (liters >= 5)
    {
        digitalWrite(LED5,HIGH);
    }
  else
    {
      digitalWrite(LED5,LOW);
    }
}
