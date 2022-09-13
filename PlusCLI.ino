#include <Arduino.h>
#include "CLI.h"
#include "DSP.h"
#include "conio.h"
#include "mineswp.h"
#include <limits.h>

int monitor_main (int argc, char** argv);

int blink (int argc, char** argv)
{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                        // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                        // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                        // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW

  return SUCCESS;
}

int echo (int argc, char** argv)
{
  int x, y;
  if( argc >= 2 )
  {
    puts(argv[1]);
  }
  whereCursor( &x, &y);
  printf(F("\nx=%d y=%d\n"), x, y);
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
    printf(F("Checksum = %lX\n"), sum);
    randomSeed(sum);
  }
  return SUCCESS;
}


int DSP_main (int argc, char** argv)
{
  SINCOS16_t tmp;
  Q_15 cosine;
  Q_15 sine;
  BAM16 deg = 0;
  for( int i=0; i<COSINE_TABLE_SIZE; ++i)
  {
    tmp = CORDIC16_sincos(deg);
    cosine = cosine_table(deg>>8);
    sine = cosine_table((deg+BAM16_270_DEGREES)>>8);
    printf( F("Angle(%04hX) sin:% 6d - % 6d=% 6d\tcos:% 6d - % 6d=% 6d\n"), deg, tmp.sin, sine, tmp.sin-sine, tmp.cos, cosine, tmp.cos-cosine);
    deg += 0x0100;
  }

  if (argc > 1)
  {
    printf( F("%d\n"), cosine_table(atoi(argv[1])) );
  }
  
  return 2;
}

const CLI_CommandEntry commandEntryTable[] =
{
  { "BLINK"   , blink           },
  { "ECHO"    , echo            },
  { "BGCOLOR" , bgcolor         },
  { "FGCOLOR" , fgcolor         },
  { "SEED"    , seedrand        },
  { "GAME"    , minesweep_main  },
  { "DSP"     , DSP_main        },
  { "MONITOR" , monitor_main    },
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
  Serial.begin(115200);
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
