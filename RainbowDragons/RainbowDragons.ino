// Script by Matt, June 2015
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 53
const int maxLEDs = 1056;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(maxLEDs, PIN, NEO_GRB + NEO_KHZ800);

int brightness[maxLEDs];
int LEDsArray[] = {24,25,26,27,38,40,40,40,40,37,36,35,35,34,34,34,34,34,35,35,36,37,40,40,40,40,38,27,26,25,24};
// int Rgb[1056][3];

int stripNum; // This is which strip the LED is on
int rowNum; //This is how far up the LED is
int LEDNum; // This is the LED ID
bool shineAgain = false;

class BirdObject {
  public:
  float posX;
  float posY;
  float speeds;
  int r,g,b;
  void Move();
  BirdObject(float inX, float inY, int _r, int _g, int _b, float inSPEED, float inDirAngle);
 
  private:
  float dirAngle=0;            // direction of birds
  float randomSpeedChange=random(1)-0.5;       // magnitude of random speed changes
  int unrest=10;                                // % chance of random changes
  
  float minSpeed=1;                            // lower speed limit
  float maxSpeed=4;                            // upper speed limit
  int timer;
  float endTime = 1000;
  float moveStepX = 0;
  float moveStepY = 1;
  float dir;
  float ranWave = random(1, 7);
};
 
BirdObject::BirdObject(float inX, float inY, int _r, int _b, int _g, float inSPEED, float inDirAngle){
    posX = inX;
    posY = inY; 
    r = _r;
    g = _g;
    b = _b;
    speeds = inSPEED;
    dirAngle=inDirAngle;     
    timer = millis();
  }

void BirdObject::Move() {
    if (endTime <=  millis() - timer) {
      dir = cos(radians(random(-100, 100)));
      timer = millis();
      endTime = random(500, 3000);
    } 
   // if(posX< 160){
     // dir += 0.1;}
     // else if(posX>160){
     // dir -=0.1; }
    float wave = sin(radians(millis()/ranWave));
    moveStepX = dir + wave;
    moveStepY = 1;
    moveStepX = moveStepX * speeds;
    moveStepY = moveStepY * speeds;
    posX += moveStepX;
    posY += moveStepY;
    // check border condition
    if (posX<0) posX+=320;
    if (posX>=320) posX-=320;
    if (posY<0) posY+=410;
    if (posY>=410) posY-=410;

    // RandomChange the movement
    if (random(10)<unrest) {          
      speeds += randomSpeedChange;
      randomSpeedChange += 0.05-random(0.01, 0.5);
      randomSpeedChange = constrain(randomSpeedChange, -0.3, 0.3);
      speeds = constrain(speeds, minSpeed, maxSpeed);
    }
  }
BirdObject Birdy(random(0, 310), 0, int(random(255)), int(random(255)), int(random(255)), random(4)+0.2, random(TWO_PI)); // This is for the sending the signal away
BirdObject BirdyReborn(random(0, 310), 0, 200, 0, int(random(255)), random(4)+0.2, random(TWO_PI)); // This is for the sending the signal away

void setup()
{
  Serial.begin(9600);
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));

 // size(320, 410);
 
   // Birds = new ArrayList();
  // for (int i=0; i<birdNum; i++) {
  //  Birdy = new BirdObject(random(0, 310), 0, random(5,8)+0.2);
   // Birds.add(Birdy);
 // }
 
 for(int i = 0; i < maxLEDs; i++){
   brightness[i] = 0;
 }
 
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


// int birdNum = 1;

void loop()
{
  if(millis() > 500 && !shineAgain){
   shineAgain = true;
  }
 // for (int i=0; i<birdNum; i++) {
 //   Birdy = ( BirdObject ) Birds.get(i);
     Birdy.Move();
     initiateShine(Birdy.posX, Birdy.posY);
     strip.setPixelColor(LEDNum, Birdy.r, Birdy.g, Birdy.b);
   
 if(shineAgain){
      BirdyReborn.Move(); 
      initiateShine(BirdyReborn.posX, BirdyReborn.posY);
      strip.setPixelColor(LEDNum, BirdyReborn.r, BirdyReborn.g, BirdyReborn.b);
      //////////// This updates the LEDs ////////////
     }
  
//}

  //////////// This updates the LEDs ////////////
 for (int i=0; i< maxLEDs; i++) {
  if(brightness[i] != 0){
  // Serial.println("Again the Number is " + i);
   ShineOn(i, Birdy.speeds); //Get the right ID, brightness and speed to update LED
    }
  }
  

  
  
  strip.show();
}

void ShineOn(int ID, int thisSpeed){
 
  ///// Make sure to get LED brightness before we go any further!
  brightness[ID] -= 1*thisSpeed;
   
  if(brightness[ID] <= 0){
    brightness[ID] = 0;
  }
  uint32_t color = strip.getPixelColor(ID);
  color = color*brightness[ID]/255;
   //int arr = color.r*brightness[ID]/255;
   //int gee =color.g*brightness[ID]/255;
   //int bee = color.b*brightness[ID]/255;
 
  // int arr = 40*brightness[ID]/255;
  // int gee =200*brightness[ID]/255;
  // int bee = 50*brightness[ID]/255;
   
   ///// For really cool effect just use this //////
    // int bee = 255*brightness[ID];
  strip.setPixelColor(ID, color);
  }
  
void initiateShine(float pX, float pY){
     stripNum = (int)(map(pX, 0, 320, 0, 31));
     rowNum = (int)(map(pY, 0, 410, 0, 40));
     LEDNum = 0;
 for(int n = 0; n < stripNum; n++){
    LEDNum += LEDsArray[n];
  }
    LEDNum += rowNum;
   if(rowNum < LEDsArray[stripNum]){
    brightness[LEDNum] = 255;
    //LEDNum = 0;
   }
}
