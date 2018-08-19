#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define led_pin 11
#define fsr_pin A0
Adafruit_NeoPixel strip = Adafruit_NeoPixel(15, led_pin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  pinMode(led_pin,OUTPUT);
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
   strip.begin();
   strip.show(); 
}

void loop() {
  
  int fsr_value = analogRead(fsr_pin);
  int led_value = map(fsr_value,0,1023,0 ,15);
  //analogWrite(led_pin,led_value);
  colorPress(strip.Color(120, 60 , 0), led_value);
  
//  colorWipe(strip.Color(0, 255, 0), 500); // Green
//  colorWipe(strip.Color(0, 0, 255), 500); // Blue
  Serial.println(fsr_value);
  Serial.println(led_value);
  delay(200);
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
