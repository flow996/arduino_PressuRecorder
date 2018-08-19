#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define led_pin 11
#define fsr_pin A0
Adafruit_NeoPixel strip = Adafruit_NeoPixel(50, led_pin, NEO_GRB + NEO_KHZ800);
//Above is the Pressure Sensstion and the REG LED.

const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin = 7;      // the number of the LED pin
int Rec = 11;
int Play = 13;            //the number of the Recorder pin
int buttonState = 0;         // variable for reading the pushbutton status
//Above is the Recorder and the Light Sensition

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
  pinMode(led_pin,OUTPUT);
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
   strip.begin();
   strip.show(); 
  //Above is the Pressure Sensstion and the REG LED.
  
  pinMode(ledPin, OUTPUT); 
  pinMode(buttonPin, INPUT);// initialize the pushbutton pin as an input:
  pinMode(Rec, OUTPUT);
  pinMode(Play, OUTPUT);
  //Above is the Recorder and the Light Sensition

}

void loop() {
  // put your main code here, to run repeatedly:
  int fsr_value = analogRead(fsr_pin);
  int led_value = map(fsr_value,0,1023,0 ,15);
  //analogWrite(led_pin,led_value);
  colorPress(strip.Color(120, 60 , 0), led_value);
  Serial.println(fsr_value);
  Serial.println(led_value);
  colorWipe(strip.Color(0, 0, 0), 0); // Red
  // read the state of the pushbutton value:
  
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, LOW);
    digitalWrite(Play, LOW);
  } else {
    // turn LED off:
    digitalWrite(ledPin, HIGH);
    digitalWrite(Play, HIGH);
  }
}

void Pressure(){
  int fsr_value = analogRead(fsr_pin);
  int led_value = map(fsr_value,0,1023,0 ,15);
  //analogWrite(led_pin,led_value);
  colorPress(strip.Color(120, 60 , 0), led_value);
  Serial.println(fsr_value);
  Serial.println(led_value);
  colorWipe(strip.Color(0, 0, 0), 0); // Red
}
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void colorPress(uint32_t c, uint8_t pressure) {
  for(int i=0; i<pressure; i++) {
    strip.setPixelColor(i, c);
    strip.show();   
  }
}

void Recorder(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, LOW);
    digitalWrite(Play, LOW);
  } else {
    // turn LED off:
    digitalWrite(ledPin, HIGH);
    digitalWrite(Play, HIGH);
  }
}
