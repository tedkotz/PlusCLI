
#include <limits.h>
#include <errno.h>
#include "conio.h"

#define MAX_IO_BUFFER 4096

#define SUCCESS 0

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.setTimeout(LONG_MAX);
  pinMode(LED_BUILTIN, OUTPUT);
}

typedef int (*MainFunc) (int argc, char** argv);
//typedef int (*MainFunc) (int argc, char** argv, char* out, size_t out_size);

typedef struct CommandEntry
{
  const char* command;
  // const char* helpText;
  // const ArgParse arguments;
  MainFunc callBack;
} CommandEntry;

int returnCode = 0;
int count = 0;

void loop() 
{
  static char iobuffer[MAX_IO_BUFFER];
  // The Main REPL of the CLI

  // Print Prompt
  textcolor(count);
  printf("\n%d,%d> ", count++, returnCode);

  // Read
  size_t OutIndex = read_stdin(iobuffer, MAX_IO_BUFFER - 1);
  iobuffer[OutIndex++]='\0';
  // Evaluate
  returnCode=evaluate(&iobuffer[OutIndex],MAX_IO_BUFFER-OutIndex, iobuffer);
  // Print
  puts(&iobuffer[OutIndex]);
}


bool commandTest( const char* input, const char* testValue )
{
  while( *testValue != '\0' && *input != '\0')
  {
    if( toupper(*input++) != *testValue++ )
    {
      return false;
    }
  }

  return ( toupper(*input) == *testValue );
}

const CommandEntry commandEntryTable[] =
{
  { "BLINK"   , blink },
  { "ECHO"    , echo  },
  { "BGCOLOR" , bgcolor },
};


int evaluate ( char* out, size_t out_size, char* in )
{
  char* argv[16];
  int argc=0;
  char* token=strtok(in, " \t\n\r");

  while( token != NULL )
  {
    if (argc >= 16)
    {
      snprintf( out, out_size-1, "Syntax Error. Argument list too long.", argv[0]);
      return -E2BIG;
    }
    argv[argc++]=token;
    token=strtok(NULL, " \t\n\r");
  }

  for( int i=0; i<sizeof(commandEntryTable)/sizeof(commandEntryTable[0]); ++i )
  {
    if ( commandTest( argv[0], commandEntryTable[i].command ) )
    {
      //return commandEntryTable[i](argc, argv, out, out_size);
      snprintf( out, out_size-1, "\nDone.\n");
      return commandEntryTable[i].callBack(argc, argv);
    }
  }
  
  snprintf( out, out_size-1, "Syntax Error. Unknown command \"%s\".", argv[0]);
  return -ENOMSG;
}


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
