#include "SmallFS.h"
#include "GameLogic.h"
#include "VGA.h"

void setup()
{
  SoundSetup();
  ScreenSetup();
  Serial.begin(9600);
}

void loop()
{  
   
  Game_Loop();  
}


