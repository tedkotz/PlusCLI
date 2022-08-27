/**
 * @file    mineswp.cpp
 * @author  Ted Kotz <ted@kotz.us>
 * @version 0.1
 *
 * A simple console mine sweeper type game
 *
 */

/* Includes ******************************************************************/
#include "conio.h"
#include <Arduino.h>

/* Defines *******************************************************************/
#define BOARDX 10
#define BOARDY 20
#define NUM_MINES  21

// Board Space value is stored in a single byte.
// SSxxCCCC
//
// SS = SPACE_STATE
// CCCC = 0-8 (Adjacent bombs), 15 (Bomb)

#define BOMB 15
#define HIDDEN_BOMB (HIDDEN | 15)

#define STATE_MASK 0b11000000
#define COUNT_MASK 0b00001111

/* Types *********************************************************************/
typedef enum SPACE_STATE
{
  HIDDEN    = 0b00000000,
  QUESTION  = 0b01000000,
  FLAG      = 0b10000000,
  REVEALED  = 0b11000000,
} SPACE_STATE;

/* Interfaces ****************************************************************/
static void revealArea( int startx, int stopx, int starty, int stopy );

/* Data **********************************************************************/
static byte Board[BOARDX][BOARDY];
static byte playerX=5;
static byte playerY=10;
static byte playerHidden=0;

/* Functions *****************************************************************/

/**
 * Clears screen and prints the current state of the board.
 *
 */
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
        byte S = Board[x][y] & STATE_MASK;
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
            int count = Board[x][y] & COUNT_MASK;

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

/**
 * deteremins if a space is a bomb
 *
 * @param space the value of the space to check
 * @return true is it is a bomb, false otherwise
 */
static bool isBomb( byte space )
{
    return ((space & COUNT_MASK)==BOMB);
}

/**
 * Counts bombs in all the spaces in a region of the board with range checking
 *
 * @param startx minimum X value to count
 * @param stopx maximum X value to count
 * @param starty minimum Y value to count
 * @param stopx maximum X value to count
 */
static byte bombCount( int startx, int stopx, int starty, int stopy )
{
    byte count = 0;
    if (startx < 0 ) startx = 0;
    if (stopx > (BOARDX-1)) stopx = BOARDX-1;
    if (starty < 0 ) starty = 0;
    if (stopy > (BOARDY-1)) stopy = BOARDY-1;
    for( int x = startx; x <= stopx; ++x )
    {
      for( int y = starty; y <= stopy; ++y )
      {
          if(isBomb(Board[x][y]))
          {
              ++count;
          }
      }
    }
    return count;
}

/**
 * Reavels a single space on the board
 * chains reveals if empty space found
 *
 * @param x X value of space to reveal
 * @param y Y value of space to reveal
 */
static void revealSpace( int x, int y )
{
  if( (Board[x][y] & STATE_MASK) == HIDDEN )
  {
    if (isBomb(Board[x][y]))
    {
      for( int y = 0; y< BOARDY; ++y )
      {
        for( int x = 0; x< BOARDX; ++x )
        {
          Board[x][y] &= ~STATE_MASK;
          Board[x][y] |= REVEALED;
        }
      }
    }
    else
    {
      Board[x][y] &= ~STATE_MASK;
      Board[x][y] |= REVEALED;        
      if( (Board[x][y] & COUNT_MASK) == 0 )
      {
        revealArea( x-1, x+1, y-1, y+1);
      }
    }
  }
}

/**
 * Reveals all the spaces in a region of the board with range checking
 *
 * @param startx minimum X value to reveal
 * @param stopx maximum X value to reveal
 * @param starty minimum Y value to reveal
 * @param stopx maximum X value to reveal
 */
static void revealArea( int startx, int stopx, int starty, int stopy )
{
    if (startx < 0 ) startx = 0;
    if (stopx > (BOARDX-1)) stopx = BOARDX-1;
    if (starty < 0 ) starty = 0;
    if (stopy > (BOARDY-1)) stopy = BOARDY-1;
    for( int x = startx; x <= stopx; ++x )
    {
      for( int y = starty; y <= stopy; ++y )
      {
        revealSpace( x, y);
      }
    }
}

/**
 * Initializes Game State
 *
 */
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
  for(int i=0; i<NUM_MINES; ++i)
  {
      int x = random(BOARDX);
      int y = random(BOARDY);

      while( isBomb(Board[x][y]) )
      {
        x = random(BOARDX);
        y = random(BOARDY);
      }

      Board[x][y] = HIDDEN_BOMB;

  }

  // Calculate adjacent bombs
  for( int y = 0; y< BOARDY; ++y )
  {
    for( int x = 0; x< BOARDX; ++x )
    {
      if( !isBomb(Board[x][y]) )
      {
          Board[x][y] = HIDDEN | bombCount( x-1, x+1, y-1, y+1);
      }
    }
  }

}

/**
 * process user input one character at a time
 *
 * @param c Character pressed by user
 */
static void processInput( char c )
{
  playerHidden=false;
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

    case '.':
    case '>':
      revealSpace(playerX, playerY);
      playerHidden=true;
      break;
    
    case '?':
    case '/':
      switch( Board[playerX][playerY] & STATE_MASK )
      {
        case HIDDEN:
          Board[playerX][playerY] &= ~STATE_MASK;
          Board[playerX][playerY] |= FLAG;
          playerHidden=true;
          break;

        case QUESTION:
          Board[playerX][playerY] &= ~STATE_MASK;
          Board[playerX][playerY] |= HIDDEN;
          break;

        case FLAG:
          Board[playerX][playerY] &= ~STATE_MASK;
          Board[playerX][playerY] |= QUESTION;
          playerHidden=true;
          break;
      }
      break;
  }
}

// See mineswp.h
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

/*****************************************************************************/
