#include <Mouse.h>

/**
   Reads X/Y values from a PS/2 mouse connected to an Arduino         
   using the PS2Mouse library available from
     http://github.com/kristopher/PS2-Mouse-Arduino/
   Original by Kristopher Chambers <kristopher.chambers@gmail.com>
   Updated by Jonathan Oxer <jon@oxer.com.au>
   Updated again by Ben Makes Everything - youtube.com/channel/UCKU7oz0NZLOml3dzGOjPfXg
*/
#include <PS2Mouse.h>
#include <Mouse.h>
#define MOUSE_DATA 10
#define MOUSE_CLOCK 15

PS2Mouse mouse(MOUSE_CLOCK, MOUSE_DATA, STREAM);

//Trackball Speed multiplier
const int pointerSpeed = 4;

int buttonState;
int lastButtonState = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200;    // debounce time setting

void setup()
{
  Serial.begin(38400);
  mouse.initialize();
  //these 2 lines keep the built in LED from coming on when the trackball moves
  pinMode(LED_BUILTIN_TX,INPUT);
  pinMode(LED_BUILTIN_RX,INPUT);
}

void loop()
{
  int16_t data[3];
  mouse.report(data);
  int reading = data[0];
  Serial.println(reading);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if (reading != buttonState) {
    buttonState = reading;
    if (reading == 9) {
      Mouse.press(MOUSE_LEFT);
    }
    else{
      Mouse.release(MOUSE_LEFT);
    }
    if (reading == 10) {
      Mouse.press(MOUSE_RIGHT);
    }
    else{
      Mouse.release(MOUSE_RIGHT);
    }
  }
  if ((data[2] != 0) || (data[1] != 0)) { // reads trackball movement and converts to pixel movement
    Mouse.move(data[1]*pointerSpeed, -data[2]*pointerSpeed, 0); // (0,0,0)==(x,y,scroll)
  }
  lastButtonState = reading;
}