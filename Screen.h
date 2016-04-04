#include "VGA.h"
#include "Structures.h"


int block_size;

unsigned char* current_structure;

void ScreenSetup()
{
  VGA.begin(VGAWISHBONESLOT(9),CHARMAPWISHBONESLOT(10));
  VGA.clear();
  VGA.setBackgroundColor(BLACK);
}

void drawPlayer()
{
    if(isDead)
      player_layer_count = 7;
    else if(isJumping)
      player_layer_count = 6;
    else
    {
      player_layer_count++;
      if(player_layer_count>=7)
        player_layer_count = 0;
    }
    VGA.setColor(BLACK);
    VGA.clearArea(player_lastKnown_posX,player_lastKnown_posY,player_width,player_height);  
    VGA.writeArea(player_posX,player_posY,player_width,player_height,player_layers[player_layer_count]);
    
}

void drawEntity(entity Entity)
{
  VGA.setColor(BLACK);
  VGA.clearArea(Entity.last_posx,Entity.last_posy,Entity.width,Entity.height);
  if(Entity.type==1 && Entity.active == true)
  VGA.writeArea(Entity.x,Entity.y,Entity.width,Entity.height,coin_layer);
  if(Entity.type==0 && Entity.active == true)
  {
  color_count++;
  VGA.setColor(colors_arr[color_count]);
  if(color_count>=6)
    color_count = 0;
  //VGA.setColor(RED);
  VGA.drawRect(Entity.x,Entity.y,Entity.width,Entity.height);
  }else if(Entity.type == 2 && Entity.active == true){
    VGA.writeArea(Entity.x, Entity.y, Entity.width, Entity.height, heartcontainer);
  }
  
}

void drawScore()
{

  VGA.setColor(YELLOW);
  VGA.printtext(0, 2, "Score:", true);
  char* scorePtr = "";
  itoa(score,scorePtr,10);
  VGA.setColor(BLACK);
  VGA.clearArea(50,2,25,10);
  VGA.setColor(YELLOW);
  VGA.printtext(50,2,scorePtr, true);
  
}

void drawHitScreen()
{
  VGA.setColor(RED);
  VGA.drawRect(player_posX,player_posY, player_width, player_height);
  
}


void drawTimerScreen(){
  int count = 0;
  while(true){
    if (PlayingSound){
        AudioFillBuffer();
   }
    VGA.setColor(CYAN);
    VGA.printtext(Screen_width/4,Screen_height/4, "GET READY", true);
    if(count == 1000){
        VGA.printtext((Screen_width/3)+12,(Screen_height/4)+20, "GO!", true);
    }
    if(count == 1500){
          VGA.clear();
          return;
    }
    count++;
  }
}

void drawIndicators(int h,int g)
{
  if(h<0)
    h=0;
  if(g<0)
    g=0;
  VGA.setColor(WHITE);
  VGA.drawRect(79,2,52,5);
  VGA.drawRect(79,8,52,5);
  VGA.setColor(RED);
  VGA.drawRect(80,3,h,3);
  VGA.setColor(BLUE);
  VGA.drawRect(80,9,g,3);
}





