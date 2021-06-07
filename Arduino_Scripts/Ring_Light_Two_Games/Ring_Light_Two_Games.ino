#include <Adafruit_NeoPixel.h>

#define PIN 4   // input pin Neopixel is attached to

#define RGBSWITCH 3 // Pin for the rgb LED on the switch

#define NUMPIXELS 20 // number of neopixels

#define BRIGHTNESS 20

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, RGBSWITCH, NEO_GRB + NEO_KHZ800);

// Game 1 - Button mashing
double pushCounter1 = 0;
int ButtonState1 = 0;
int LastButtonState1 = 0;
int Add;
int Other;
int multiplyer = 4;

// Game 2 - Ring around the rosies
int playerPos;
int foodPos;
int poisonPos;
int progress = 0;
int increment = 2;


bool finish1 = false;
bool finish2 = false;

bool playing;

void setup()
{
  playing = true;
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
    if (!finish1) {
      ButtonState1 = digitalRead(2);
      delay(10);
      if(ButtonState1 != LastButtonState1){
        pushCounter1++;          //increment Counter 1
      }
      if (pushCounter1/multiplyer > 19){
        pixels.clear();          // clear the strip
        FlashRed(1);
        pixels.clear();// make handset one flash red
        finish1 = true;
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
    if (finish1 && !finish2) {
      if (progress < 20) {
        if (digitalRead(2)) {
          playerPos++;
          if (playerPos == 21) playerPos = 0;
          delay(100);
        } else {
          playerPos--;
          if (playerPos < 0) playerPos = 20;
          delay(100);
        }
        if (playerPos == foodPos) {
          progress += increment;
          foodPos = random(NUMPIXELS);
          poisonPos = random(NUMPIXELS);
        }
        if (playerPos == poisonPos) {
          progress -= 2 * increment;
          if (progress < 0) progress = 0;
          if (playerPos < 0) playerPos = 0;
          poisonPos = random(NUMPIXELS);
          foodPos = random(NUMPIXELS);
        }
        pixels.clear();
        setProgress();
        pixels.setPixelColor(playerPos, 255, 255, 0);
        pixels.setPixelColor(foodPos, 0, 255, 0);
        pixels.setPixelColor(poisonPos, 255, 0, 0);
        pixels.show();
      } else {
        finish2 = true;
      }
    }
    if (finish1 && finish2) {
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
  }
}

void setProgress() {
  for (int i = 0; i < progress; i++) {
    pixels.setPixelColor(i, 0, 255, 255);
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
