#include <Adafruit_NeoPixel.h>
#include <IRremote.h>

const int RECV_PIN = A0;
const int LEDS_PIN = 11;
const int LEDS_TOTAL = 12;
const int LIGHT_STEP = 20;
const int COLOR_START = 42;
boolean is_on = true;
IRrecv irrecv(RECV_PIN);
decode_results results;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS_TOTAL, LEDS_PIN);

enum CONTROLS {
  // arrows
  RIGHT = 0xFFC23D,
  UP = 0xFF629D,
  OK = 0xFF02FD,
  DOWN = 0xFFA857,
  LEFT = 0xFF22DD,
  // numbers
  ONE = 0xFF6897,
  TWO = 0x9867,
  THREE = 0xFFB04F,
  FOUR = 0xFF30CF,
  FIVE = 0xFF18E7,
  SIX = 0xFF7A85,
  SEVEN = 0xFF10EF,
  EIGHT = 0xFF38C7,
  NINE = 0xFF5AAA5,
  ZERO = 0xFF4AB5,
  ASTERISK = 0xFF42BD,
  HASH = 0xFF52AD
};


void debug_print_color(int r, int g, int b){
  char str[16];
  sprintf(str, "%d,%d,%d", r, g, b);
  Serial.println(str);
}

void parse_color(unsigned long color, byte* res){
  res[0] = (color & 0xFF0000) >> 16;
  res[1] = (color & 0x00FF00) >> 8;
  res[2] = color & 0x0000FF;
  debug_print_color(res[0], res[1], res[2]);
}


void setColor(unsigned int r, unsigned int g, unsigned int b){
  for (int i=0;i<LEDS_TOTAL;i++){
    strip.setPixelColor(i, r, g, b);
    debug_print_color(r, g, b);
  }
  strip.show();
}


void dim(int step){
  byte r, g, b = 0;
  for (int i=0;i<LEDS_TOTAL;i++){
    unsigned long color = strip.getPixelColor(i);
    r = (((color & 0xFF0000) >> 16) - step) & 0xFF;
    g = (((color & 0x00FF00) >> 8) - step) & 0xFF;
    b = ((color & 0x0000FF) - step) & 0xFF;
    debug_print_color(r, g, b);
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
}


void brighten(int step){
  byte r, g, b = 0;
  for (int i=0;i<LEDS_TOTAL;i++){
    unsigned long color = strip.getPixelColor(i);
    r = (step + ((color & 0xFF0000) >> 16)) & 0xFF;
    g = (step + ((color & 0x00FF00) >> 8)) & 0xFF;
    b = (step + (color & 0x0000FF)) & 0xFF;
    debug_print_color(r, g, b);
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
}


void adjust(int step_red, int step_green, int step_blue){
  unsigned long color = 0;
  byte rgb[3] = {0};
  for (int i=0;i<LEDS_TOTAL;i++){
    color = strip.getPixelColor(i);
    parse_color(color, rgb);
    strip.setPixelColor(i, rgb[0] + step_red, rgb[1] + step_green, rgb[2] + step_blue);
  }
  strip.show();
}


void on(){
  setColor(COLOR_START, COLOR_START, COLOR_START);
}

void off(){
  setColor(0, 0, 0);
}

void setup(){
  Serial.begin(9600);
  pinMode(RECV_PIN, INPUT);
  irrecv.enableIRIn();
  // set up led strip
  strip.begin();
  if (is_on){
    on();
  }
}


void loop(){
  if (irrecv.decode(&results)){
    switch(results.value){
    case UP:
      brighten(LIGHT_STEP);
      break;
    case DOWN:
      dim(LIGHT_STEP);
      break;
    case LEFT:
      adjust(0, LIGHT_STEP, 0);
      break;
    case RIGHT:
      adjust(LIGHT_STEP, 0, 0);
      break;
    case OK:
      (is_on) ? off() : on();
      is_on = !is_on;
      break;
    default:
      Serial.println(results.value, HEX);
    }
    irrecv.resume();}
}
