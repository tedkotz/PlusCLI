
#include <limits.h>
#include "conio.h"

#define MAX_IO_BUFFER 256

static char inbuffer[MAX_IO_BUFFER];
static char outbuffer[MAX_IO_BUFFER];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.setTimeout(LONG_MAX);
  pinMode(LED_BUILTIN, OUTPUT);
}

typedef int (*MainFunc) (int argc, char** argv);

typedef struct CommandEntry
{
  const char* command;
  MainFunc callBack;
} CommandEntry;

int returnCode = 0;
int count = 0;

void loop() {
  // The Main REPL of the CLI

  // Print Prompt
  //Serial.print("\nHELLO> ");
  textcolor(count);
  printf("\n%d,%d> ", count++, returnCode);

  // Read
  gets_s(inbuffer, MAX_IO_BUFFER);
  // Evaluate
  evaluate(outbuffer, inbuffer);
  // Print
  puts(outbuffer);
  

}

char* evaluate ( char* out, const char* in )
{
  snprintf( out, MAX_IO_BUFFER, "%s - %d", in, strlen(in));
  return out;
}


int blink (int argc, char** argv)
{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for a second

  return 0;
}

int echo (int argc, char** argv)
{
  
  return 1;
}

int bgcolor (int argc, char** argv)
{
  
  return 1;
}

const CommandEntry commandEntryTable[] =
{
  { "blink"   , blink },
  { "echo"    , echo  },
  { "bgcolor" , bgcolor },
};
