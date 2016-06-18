// This sketch can genereate Plasma to the SD card if pin 2 is held low
//Otherwise it will play from the SD card. 

#include <Adafruit_NeoPixel.h>

float movement = 0;
float movementFactor = 0.093;

float brightness = 160;
float size = 10;
float worldWidth = 20;
float worldHeight = 40;
int ledFactor = 4; //add spacing to leds. higher = finer

int minShade = -112;
int maxShade = 112;
float shade = 0.1;
int circleLoc[2];
boolean circle = false;
                      
                                                         
//unsigned long generateFrames = 100000;

//LED STRIP
const int DATAPIN = 14;
//const int LEDCOUNT = 470;//vest
const int LEDCOUNT = 136;
//const int LEDCOUNT = 111;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDCOUNT, DATAPIN, NEO_GRB + NEO_KHZ800);



//Optimization Mathematics
float  sinShadePiRed, sinShadePiGreen, sinShadePiBlue = 0.0;
float cx = 20;
float cy = 10;
float dist = 0; 
int r,g,b;
uint32_t col = 0;
  
int sensors[2];
  
class LED
{
public:
      int x;
      int y;
      LED(int _x, int _y)
      {
        x=_x;
        y=_y;
      }
 };
  

LED leds[LEDCOUNT] = 
{
LED(0,0),LED(0,1),LED(0,2),LED(0,3),LED(0,4),LED(0,5),LED(0,6),LED(0,7),
LED(0,8),LED(0,9),LED(0,10),LED(0,11),LED(0,12),LED(0,13),LED(0,14),LED(0,15),
LED(0,16),LED(1,16),LED(1,15),LED(1,14),LED(1,13),LED(1,12),LED(1,11),LED(1,10),
LED(1,9),LED(1,8),LED(1,7),LED(1,6),LED(1,5),LED(1,4),LED(1,3),LED(1,2),LED(1,1),
LED(1,0),LED(2,0),LED(2,1),LED(2,2),LED(2,3),LED(2,4),LED(2,5),LED(2,6),LED(2,7),
LED(2,8),LED(2,9),LED(2,10),LED(2,11),LED(2,12),LED(2,13),LED(2,14),LED(2,15),LED(2,16),
LED(3,16),LED(3,15),LED(3,14),LED(3,13),LED(3,12),LED(3,11),LED(3,10),LED(3,9),
LED(3,8),LED(3,7),LED(3,6),LED(3,5),LED(3,4),LED(3,3),LED(3,2),LED(3,1),
LED(3,0),LED(4,0),LED(4,1),LED(4,2),LED(4,3),LED(4,4),LED(4,5),LED(4,6),
LED(4,7),LED(4,8),LED(4,9),LED(4,10),LED(4,11),LED(4,12),LED(4,13),LED(4,14),
LED(4,15),LED(4,16),LED(5,16),LED(5,15),LED(5,14),LED(5,13),LED(5,12),LED(5,11),
LED(5,10),LED(5,9),LED(5,8),LED(5,7),LED(5,6),LED(5,5),LED(5,4),LED(5,3),LED(5,2),
LED(5,1),LED(5,0),LED(6,0),LED(6,1),LED(6,2),LED(6,3),LED(6,4),LED(6,5),
LED(6,6),LED(6,7),LED(6,8),LED(6,9),LED(6,10),LED(6,11),LED(6,12),LED(6,13),
LED(6,14),LED(6,15),LED(6,16),LED(7,16),LED(7,15),LED(7,14),LED(7,13),LED(7,12),LED(7,11),LED(7,10),LED(7,9),LED(7,8),LED(7,7),LED(7,6),LED(7,5),LED(7,4),
LED(7,3),LED(7,2),LED(7,1),LED(7,0)
};

int SENSOR0 = 17;
int SENSOR1 = 23;

void setup()
{
  Serial.begin(115200);
  strip.begin();
  pinMode(13,OUTPUT);
  
  pinMode(SENSOR0,INPUT);
  pinMode(SENSOR1,INPUT);
  digitalWrite(SENSOR0, HIGH);
  digitalWrite(SENSOR1, HIGH);
  
  FlashLed(500);

}

void loop()
{
 ReadSensors();
 DecidePlasma();
 Plasma();
 FlashLed(1);
}

void DecidePlasma()
{

   if(sensors[0] == LOW && sensors[1] == HIGH)
  {
    Serial.println("1");
    circleLoc[0] = worldWidth;
    circleLoc[1] = worldHeight;
    circle = true;
    
  } 
  else  if(sensors[0] == HIGH && sensors[1] == LOW)
  {
    Serial.println("2");
    circleLoc[0] = worldWidth*2;
    circleLoc[1] = worldHeight*2;
    circle = true;
  } 
  else if(sensors[0] == LOW && sensors[1] == LOW)
  {
    Serial.println("3");
    circleLoc[0] = worldWidth/2;
    circleLoc[1] = worldHeight/2;
    circle = true;
  } 
  else
  {
    Serial.println("4");
    circle = false;
  }
  
}

void Plasma()
{

  for(int i = 0; i<LEDCOUNT; i++)
  {
    //get the xy co-ordinates and spread out by factor.
    LED c = leds[i];
    c.x*=ledFactor;
    c.y*=ledFactor;

    if(circle)
    {
      shade = SinCircleHover(c.x,c.y, size);
    }
    else
    {
    shade = (SinVerticle(c.x,c.y,size) //42ms
             + SinRotating(c.x,c.y,size)  //91ms
             + SinCircle(c.x,c.y, size))/3; //120ms
             //250ms
    }
    
    //Optimization Mathematics
    sinShadePiRed = sin(shade*PI)*100; //21ms
    //sinShadePiGreen = sin(shade*PI+2)*100; //36ms
   // sinShadePiBlue = sin(shade*PI+4)*100; //42ms
 
 
 if(circle)
 {
   
    //g = map( sinShadePiGreen, minShade, maxShade, 0, brightness);//2ms
    r = map( sinShadePiRed, minShade, maxShade, 0, brightness);//2ms
     g = 0;
     b=0;
 }
 else
 {
    //r = map( sinShadePiRed, minShade, maxShade, 0, brightness);//2ms
  r=0;
    //r = map( sin(shade*PI)*100, minShade, maxShade, 0, brightness);
    g = map( sin(shade*PI+2*PI*sin(movement/23))*100, minShade, maxShade, 0, brightness);
    b = map( sin(shade*PI+4*PI*sin(movement/20))*100, minShade, maxShade, 0, brightness);
  }
  
    strip.setPixelColor(i,Color(r,g,b));//0.5ms
     
   
    }
  
    movement+=movementFactor;
    strip.show();
   
    //self correct mapping values to use maximum led resolution
    //if(sinShadePiRed < minShade) minShade = sinShadePiRed;
    //if(sinShadePiRed > maxShade) maxShade = sinShadePiRed;
  
    //Serial.println(minShade);
    //show the last frame/pixel
    /*
    Serial.print("Shade="); Serial.println(shade);
    Serial.print("r="); Serial.print(r); Serial.print("From="); Serial.println(sinShadePiRed);
    Serial.print("g="); Serial.print(g); Serial.print("From="); Serial.println(sinShadePiGreen);
    Serial.print("b="); Serial.print(b); Serial.print("From="); Serial.println(sinShadePiBlue);
    Serial.println();
    */
    //Serial.println(millis());
}

float SinVerticle(float x, float y, float s)
{
  return sin(x / s + movement);
}
 
float SinRotating(float x, float y, float s)
{
  return sin(  (x * sin(movement/33 ) + y * cos(movement/22) )  /size) ;
}
 
float SinCircle(float x, float y, float s)
{
  cx = worldWidth * sin(movement/10)*ledFactor;
  cy = worldHeight * cos(movement/10)*ledFactor;
  
  dist = sqrt(sq(cy-y) + sq(cx-x));
  return sin((dist/s ) + movement );
}

float SinCircleHover(float x, float y, float s)
{
  cx = circleLoc[0];
  cy = circleLoc[1];
  
  dist = sqrt(sq(cy-y) + sq(cx-x));
  return sin((dist/s ) + movement );
}

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

void ReadSensors()
{
  sensors[0] = digitalRead(17);
  sensors[1] = digitalRead(18);
  
    Serial.print("s0="); Serial.print(sensors[0]);
  Serial.print(",s1="); Serial.println(sensors[1]);
  
}

void FlashLed(int wait)
{
      digitalWrite(13, HIGH);
      delay(wait);
      digitalWrite(13, LOW);
}

