#include "Sound.h"
#include "Input.h"
#include "Screen.h"
#include "VGA.h"
#include <stdlib.h>

unsigned const int FrameDelay = 50;
unsigned long LastFrameTime;
unsigned long LastChanceTime;
void playerJump()
{ 
  if (isJumping)
  {
      if(player_posY <= 50)
        landing = true;
        
      if(!landing)
      {
        if(isDiving)
        {
        landing = true;
        }
      }
      
      if(landing)
      {
        if(isGliding)
        {
        mana-=2;
        down_velocity = 1;
        isGliding = false; 
        }
        else if(isDiving)
        {
        mana-=1;
        down_velocity = 10;
        isDiving = false; 
        }
        else if(isFlying)
        {
        mana-=3;
        down_velocity = -1;
        isFlying = false; 
        }
        else
        {
        down_velocity = 6;  
        }
       
      player_posY+=down_velocity;
      }
      else
      player_posY-=6;
      
      
      if (player_posY >= 100)
      {
          player_posY = 100;
          isJumping = false;
          landing = false;
          down_velocity = 6;
      }
  }
  
  
}


void GameSetup()
{  
}
void ResetGame(){
    speed=3;
    score=0;
    health =50;
    mana = 50;
    player_posY = (Screen_height - player_height);
    landing = false;
    down_velocity = 4;
    isJumping = false;
    isDead = false;
    isDiving = false;
    isGliding = false;
    isFlying = false;
    healthChance = false;
    timerscreen = false;
    VGA.clear();
}
void initGame();
void processEvent( enum event_t ev )
{
	if (ev==event_1) { 
            if(startMenu){
              startMenu = false;
            }
            if (!isJumping && mana >=0 && isDead == false)
            {
            isJumping = true;
            }
            
            if(isDead == true)
              ResetGame();
	}
	if (ev==event_2) {
            if (!isDiving && mana >= 1 && isJumping)
            {
            isDiving = true;
            }
	}
	if (ev==event_3) {
            if (!isGliding && mana >= 2 && isJumping)
            {
            isGliding = true;
            }
	}
	if (ev==event_4) {
            if (!isFlying && mana >= 3 && isJumping)
            {
            isFlying = true;
            }
	}


        
}

void moveEntity(entity &Entity)
{
  Entity.updatelastpos();
  Entity.x -= speed;
  Entity.updateCords();
}


void detectCollision(entity &Entity)
{
  if(player_posX < (Entity.x + Entity.width) && (player_posX+player_width) > Entity.x && player_posY < (Entity.y + Entity.height) && (player_posY+player_height) > Entity.y)
  {
    if(Entity.type==1 && Entity.active == true)
    {
      Entity.active = false;
      score+=1;
    }
    if(Entity.type==0 && Entity.active == true)
    {
      Entity.active = false;
      drawHitScreen();
      health -=10;
    }
    
    if(Entity.type == 2 && Entity.active == true && !isDead){
      Entity.active = false;
      if(health != 50)
         health += 10;
    }
      
  }
}



void GameOver()
{
  isDead = true;
  speed = 0;
  VGA.setColor(RED);
  VGA.printtext(Screen_width/4,Screen_height/4, "GAME OVER", true);
  VGA.printtext(Screen_width/5,(Screen_height/4)+30, "PRESS BTN 1", true);
  VGA.printtext((Screen_width/4)+5,(Screen_height/4)+40, "TO RETRY", true);
}

void clearMenuArea(){
  VGA.setColor(BLACK);
  VGA.clearArea(Screen_width/2,15, Screen_width, 10);
}

void MainMenu();
void instructions(){
  VGA.clear();
  while(true){
  if (PlayingSound){
        AudioFillBuffer();
  }
  VGA.setColor(WHITE);
  VGA.printtext(Screen_width/9,5, "Instructions", true);
  VGA.printtext(Screen_width/9,25, "Jump      - BTN 1", true);
  VGA.printtext(Screen_width/9,45, "Fast drop - BTN 2", true);
  VGA.printtext(Screen_width/9,65, "Glide     - BTN 3", true);
  VGA.printtext(Screen_width/9,85, "Levitate  - BTN 4", true);
  VGA.printtext(0,105, "< Go back   - BTN 2", true);
    
  if(digitalRead(BUTTON_2)){
    Instructions = false;
    startMenu = true;
    startGame = false;
    return;
  }
  
  }
}


void MainMenu()
{
  VGA.clear();
  bool pressed = false;
  while(true){
  if (PlayingSound){
        AudioFillBuffer();
  }
  int color = colors_arr[(rand()%(6-0))+0];
  VGA.setColor(color);
  VGA.printtext(Screen_width/6,15, "Psycho Runner", true);
  VGA.setColor(WHITE);
  VGA.printtext(Screen_width/4,30, "Start Game", true);
  VGA.setColor(WHITE);
  VGA.printtext(Screen_width/5,45, "Instructions", true);
  VGA.setColor(color);
  if(digitalRead(BUTTON_2)){
    VGA.printtext((Screen_width/5)-8,45, ">", true);
    VGA.setColor(BLACK);
    VGA.clearArea((Screen_width/4)-8,30, 8, 8);
    Instructions = true;
    startGame = false;
    pressed = true;
  }else if(digitalRead(BUTTON_3)){
      VGA.printtext((Screen_width/4)-8,30, ">", true);
      VGA.setColor(BLACK);
      VGA.clearArea((Screen_width/5)-8,45, 8, 8);
      startGame = true;
      Instructions = false;
      pressed = true;
  }else if(!pressed){
    VGA.printtext((Screen_width/4)-8,30, ">", true);
    startGame = true;
  }
  if(digitalRead(BUTTON_1)){
    return;
  }
 }
    
}


void initGame(){
  VGA.clear();
  while(true){
      if (PlayingSound){
        AudioFillBuffer();
      }
      enum event_t ev = hasEvent();
      if (ev!=event_none) {
        processEvent(ev);
      }
   if((millis() - LastChanceTime) > 40000){
            healthChance = true;
            LastChanceTime = millis();
     }
  
    if((millis() - LastFrameTime) > FrameDelay)
    {
      LastFrameTime = millis();
      drawIndicators(health,mana);
      drawScore();
      if(mana < 50 && isDiving == false && isFlying == false && isGliding == false)
        mana++;
      drawPlayer();
      player_lastKnown_posX = player_posX;
      player_lastKnown_posY = player_posY;
      
      playerJump();

      
      if(healthChance){
          moveEntity(entities[6]); 
          drawEntity(entities[6]);
          detectCollision(entities[6]);
          if(entities[6].x == 0){
              healthChance = false;
              VGA.clearArea(entities[6].x, entities[6].y, entities[6].width, entities[6].height);
           }
      
      }

      for(int i=0;i<entity_count-1;i++){ 
        moveEntity(entities[i]); 
        drawEntity(entities[i]);
        detectCollision(entities[i]);
      }
      

      if(!(score%10)){
          if(score!=0){
            if(score == 20)
              speed = 4;
            else if(score == 50)
              speed = 5;
            else if(score == 100)
              speed = 6;
          }
      }
      if(!timerscreen){
        drawTimerScreen();
        timerscreen = true;
      }else
        processEvent(ev);
      
      if(health<=0)
        GameOver();
    }   
    
    
             
  }
}
void Game_Loop()
{
 if(startMenu)
   MainMenu();
 if(Instructions)
   instructions();
 if(startGame)
    initGame();
}



