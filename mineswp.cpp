#include "conio.h"
#include <Arduino.h>

typedef enum SPACE_STATE
{
  HIDDEN    = 0b00000000,
  QUESTION  = 0b01000000,
  FLAG      = 0b10000000,
  REVEALED  = 0b11000000,
} SPACE_STATE;

#define BOMB 15

// SSxxCCCC
//
// SS = SPACE_STATE
// CCCC = 0-8 (Adjacent bombs), 15 (Bomb)

#define BOARDX 10
#define BOARDY 20

byte Board[BOARDX][BOARDY];
byte playerX=5;
byte playerY=10;
byte playerHidden=0;



static void printBoard( )
{
  clrscr();
  for( int y = 0; y< BOARDY; ++y )
  {
    putchar('|');
    for( int x = 0; x< BOARDX; ++x )
    {
      char c=' ';
      if( (x == playerX) && (y == playerY) && (!playerHidden))
      {
         c='X';
      }
      else
      {
        byte S = Board[x][y] & 0b11000000;
        switch(S)
        {
          case HIDDEN:
            c='#';
            break;
            
          case QUESTION:
            c= '?';
            break;

          case FLAG:
            c='F';
            break;

          case REVEALED:
          {
            int count = Board[x][y] & 0b00001111;

            switch (count)
            {
              case BOMB:
                c='*';
                break;

              case 0:
                c=' ';
                break;

              default:
                c='0'+count;
                break;
            }
          }
            break;

          default:
            c='@';
            break;
        }
      }
      putchar(c);
    }
    putchar('|');
    putchar('\n');
  }
  putchar('|');
  for( int x = 0; x< BOARDX; ++x )
  {
    putchar('-');
  }
  putchar('|');
  putchar('\n');
}

static void initGame()
{
  playerX=5;
  playerY=10;
  playerHidden=0;
  
  for( int y = 0; y< BOARDY; ++y )
  {
    for( int x = 0; x< BOARDX; ++x )
    {
      Board[x][y]=HIDDEN;
    }
  }

  // Place Random bombs

  // Calcualate adjacent bombs
}

static void processInput( char c )
{
  switch (toupper(c))
  {
    case 'W':
      if(playerY > 0)
      {
        --playerY;      
      }
      break;
      
    case 'S':
      if(playerY <  (BOARDY-1))
      {
        ++playerY;      
      }
      break;
      
    case 'A':
      if(playerX > 0)
      {
        --playerX;      
      }
      break;
      
    case 'D':
      if(playerX <  (BOARDX-1))
      {
        ++playerX;
      }
      break;
      
  }
}

int minesweep_main (int argc, char** argv)
{
  initGame();
  printBoard();
  char input = getch();
  while( toupper(input) != 'Q')
  {
    processInput( input );
    printBoard();
    input = getch();    
  }
  return 0;
}
