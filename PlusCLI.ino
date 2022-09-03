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
  if( argc >= 2 )
  {
    puts(argv[1]);
  }
  putchar('\n');
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
  const __FlashStringHelper* s_dec =F("sin(%d)=%d/32768\ncos(%d)=%d/32768\n\n");
  const __FlashStringHelper* s_hex =F("sin(0x%04X)=%d/32768\ncos(0x%04X)=%d/32768\n\n");
  SINCOS16_t tmp=CORDIC16_sincos(BAM16_45_DEGREES);
  printf( s_dec, 45, tmp.sin, 45, tmp.cos);
  tmp=CORDIC16_sincos(BAM16_30_DEGREES);
  printf( s_dec, 30, tmp.sin, 30, tmp.cos);
  tmp=CORDIC16_sincos(BAM16_60_DEGREES);
  printf( s_dec, 60, tmp.sin, 60, tmp.cos);
  tmp=CORDIC16_sincos(0);
  printf( s_hex, 0, tmp.sin, 0, tmp.cos);
  tmp=CORDIC16_sincos(0x2000);
  printf( s_hex, 0x2000, tmp.sin, 0x2000, tmp.cos);
  tmp=CORDIC16_sincos(0x4000);
  printf( s_hex, 0x4000, tmp.sin, 0x4000, tmp.cos);
  tmp=CORDIC16_sincos(0x6000);
  printf( s_hex, 0x6000, tmp.sin, 0x6000, tmp.cos);
  tmp=CORDIC16_sincos(0x8000);
  printf( s_hex, 0x8000, tmp.sin, 0x8000, tmp.cos);
  tmp=CORDIC16_sincos(0xA000);
  printf( s_hex, 0xA000, tmp.sin, 0xA000, tmp.cos);
  tmp=CORDIC16_sincos(0xC000);
  printf( s_hex, 0xC000, tmp.sin, 0xC000, tmp.cos);
  tmp=CORDIC16_sincos(0xE000);
  printf( s_hex, 0xE000, tmp.sin, 0xE000, tmp.cos);

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
