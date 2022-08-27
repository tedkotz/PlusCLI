#include <Arduino.h>
#include "CLI.h"
#include "conio.h"
#include "mineswp.h"
#include <limits.h>


int blink (int argc, char** argv)
{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                        // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                        // wait for a second

  return SUCCESS;
}

int echo (int argc, char** argv)
{
  if( argc < 2 )
  {
    printf( "\n" );    
  }
  else
  {
    printf( "%s\n", argv[1] );
  }
  return SUCCESS;
}

int bgcolor (int argc, char** argv)
{
  if( argc < 2 )
  {
    textbackground( 0 );
    
  }
  else
  {
    textbackground( atoi(argv[1]) );
  }
  clrscr();
  return SUCCESS;
}

int fgcolor (int argc, char** argv)
{
  if( argc < 2 )
  {
    textcolor( 0 );
    
  }
  else
  {
    textcolor( atoi(argv[1]) );
  }
  return SUCCESS;
}

int seedrand (int argc, char** argv)
{
  if( argc < 2 )
  {
    randomSeed(analogRead(0));
    
  }
  else
  {
    const char* ptr = argv[1];
    unsigned long sum =0;
    while (*ptr != '\0')
    {
      sum <<= 1;
      sum += *ptr++;
    }
    printf("Checksum = %lX\n", sum);
    randomSeed(sum);
  }
  return SUCCESS;
}

const CLI_CommandEntry commandEntryTable[] =
{
  { "BLINK"   , blink           },
  { "ECHO"    , echo            },
  { "BGCOLOR" , bgcolor         },
  { "FGCOLOR" , fgcolor         },
  { "SEED"    , seedrand        },
  { "GAME"    , minesweep_main  },
};

const char* getPrompt (void) 
{
  static char prompt[32];
  static int count = 0;
  snprintf(prompt, 31, "\n%d,%d> ", count++, CLI_getLastReturnCode() );
  return prompt;
}


void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.setTimeout(LONG_MAX);
  randomSeed(analogRead(0));
  pinMode(LED_BUILTIN, OUTPUT);
  CLI_registerCommandEntryTable( commandEntryTable, sizeof(commandEntryTable)/sizeof(commandEntryTable[0]));
  CLI_getPrompt = getPrompt;
}

void loop() 
{
  CLI_loop();
}
