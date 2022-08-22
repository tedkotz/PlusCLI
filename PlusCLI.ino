
#include "CLI.h"
#include "conio.h"
#include <errno.h>
#include <limits.h>

#define MAX_IO_BUFFER 4096

#define SUCCESS 0

static int count = 0;


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

const CLI_CommandEntry commandEntryTable[] =
{
  { "BLINK"   , blink },
  { "ECHO"    , echo  },
  { "BGCOLOR" , bgcolor },
  { "FGCOLOR" , fgcolor },
};

const char* getPrompt (void) 
{
  static char prompt[32];
  snprintf(prompt, 31, "\n%d,%d> ", count++, CLI_getLastReturnCode() );
  return prompt;
}


void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.setTimeout(LONG_MAX);
  pinMode(LED_BUILTIN, OUTPUT);
  CLI_registerCommandEntryTable( commandEntryTable, sizeof(commandEntryTable)/sizeof(commandEntryTable[0]));
  CLI_getPrompt = getPrompt;
}

void loop() 
{
  CLI_loop();
}
