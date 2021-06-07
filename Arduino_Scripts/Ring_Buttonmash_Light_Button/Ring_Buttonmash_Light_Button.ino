#include <Adafruit_NeoPixel.h>

#define PIN 4   // input pin Neopixel is attached to

#define RGBSWITCH 3 // Pin for the rgb LED on the switch

#define NUMPIXELS 20 // number of neopixels

#define BRIGHTNESS 20

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, RGBSWITCH, NEO_GRB + NEO_KHZ800);

double pushCounter1 = 0;

int ButtonState1 = 0;

int LastButtonState1 = 0;

int Add;
int Other;   

int multiplyer = 4;

bool playing;

void setup()
{
  playing = false;
  pinMode(2, INPUT);
  pixels.setBrightness(BRIGHTNESS);
  delay(750);
  pixels.begin();
  pixels.show();
  Serial.begin(9600);

  strip.begin();
  strip.setPixelColor(0, 0, 255, 0);
  strip.show();
  
}

void loop()
{
  if (!playing && digitalRead(2)) {
    Serial.print("Start");
    Serial.println();
    strip.setPixelColor(0, 0, 0, 0);
    strip.show();
  }
  if(Serial.available() > 0 && !playing){
    String piCommand = Serial.readStringUntil('\n');
    if(piCommand == "Playing"){
      Serial.print("Playing");
      Serial.println();
      playing = true;

      GreenLoop(); //its green?
      delay(800);
      pixels.clear();
      GreenLoop(); //its green?
      delay(800);
      pixels.clear();
      RedLoop(); // its red?
      delay(750);
      pixels.clear();
      pixels.show();
    }else if (piCommand == "Celebrate") {
      for (int i = 0; i < 6; i++) {
        GreenLoop();
        delay(200);
        pixels.clear();
        RedLoop();
        delay(200);
        pixels.clear();
      }
    }
  } 
  if (playing) {
    ButtonState1 = digitalRead(2);
    delay(10);
    if(ButtonState1 != LastButtonState1){
      pushCounter1++;          //increment Counter 1
    }
    if (pushCounter1/multiplyer > 19){
      pixels.clear();          // clear the strip
      FlashRed(1);
      pixels.clear();// make handset one flash red
    }
    LastButtonState1 = ButtonState1;
    if ((int)pushCounter1%multiplyer == 1){
      pixels.setPixelColor(pushCounter1/multiplyer + 1,0, 0, 0,0);
      pixels.setPixelColor(pushCounter1/multiplyer,255, 20, 251,0);
      pixels.show();
    }
    if (pushCounter1 > 0){
      pushCounter1 -= 0.05;
    }
  }
}

void FlashRed(int Handset){

// The if statments here decide which handset is to flash Red and which one is to flash green. 
// i use the Add and Other Variables because the pixels, with teo ring, are numbered 0 to 23.
// It is then just a for loop to set the relevent pixels using the setPixelColor() command and show() command.
  
  if(Handset == 1){
    Add = 0;
    Other =12;
  }
    for(int j=1;j<4;j++){
        for(int i=0; i<20;i++){
          pixels.setPixelColor(i + Add,0,255,0,0);
          pixels.setPixelColor(i + Other,0,255,0,0);
          pixels.show();
        }
          delay(500);
        for(int i=0; i<12;i++){
          pixels.setPixelColor(i + Add,0,255,255,0);
          pixels.setPixelColor(i + Other,0,255,255,0);
          pixels.show();
        }
          delay(500);
      }

      playing = false;
      Serial.print("Win");
      Serial.println();
      delay(10);
      Serial.flush();
      Serial.read();
      strip.setPixelColor(0, 0, 255, 0);
      strip.show();
      
      pushCounter1  = 0;
      pixels.clear();
}

void GreenLoop(){

  for (int k=0;k<9;k++){
    pixels.setPixelColor(k,255,0,0,0);
    pixels.setPixelColor(k+1,255,0,0,0);
    pixels.setPixelColor(k+2,255,0,0,0);
    pixels.setPixelColor(k+3,255,0,0,0);
    pixels.setPixelColor(k+12,255,0,0,0);
    pixels.setPixelColor(k+13,255,0,0,0);
    pixels.setPixelColor(k+14,255,0,0,0);
    pixels.setPixelColor(k+15,255,0,0,0);
    pixels.show();
    delay(30);
  }
}

void RedLoop(){

  for (int k=0;k<9;k++){
    pixels.setPixelColor(k,0,255,0,0);
    pixels.setPixelColor(k+1,0,255,0,0);
    pixels.setPixelColor(k+2,0,255,0,0);
    pixels.setPixelColor(k+3,0,255,0,0);
    pixels.setPixelColor(k+12,0,255,0,0);
    pixels.setPixelColor(k+13,0,255,0,0);
    pixels.setPixelColor(k+14,0,255,0,0);
    pixels.setPixelColor(k+15,0,255,0,0);
    pixels.show();
    delay(30);
  }
}

void DestroyPixel(){
      
  
  }
