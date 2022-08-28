
#include "CLI.h"

#include <Arduino.h>
#include "conio.h"

#define MAX_IO_BUFFER 128


static int returnCode = 0;
static const CLI_CommandEntry* commandEntryTable=NULL;
static size_t commandEntryTableSize=0;

static const char* DEFAULTgetPrompt (void)
{
  static const PROGMEM char defaultPrompt[] = "\n> ";
  return defaultPrompt;
}

const char* (*CLI_getPrompt) (void) = DEFAULTgetPrompt;

extern "C" void CLI_registerCommandEntryTable ( const CLI_CommandEntry* table, size_t size )
{
  commandEntryTable = table;
  commandEntryTableSize = size;
}

extern "C" int CLI_getLastReturnCode (void)
{
  return returnCode;
}


static bool commandTest( const char* input, const char* testValue )
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

static int evaluate ( char* out, size_t out_size, char* in )
{
  static const PROGMEM char DELIM[] = " \t\n\r";
  char* argv[16];
  int argc=0;
  char* token=strtok(in, DELIM);

  while( token != NULL )
  {
    if (argc >= 16)
    {
      static const PROGMEM char E2BIG_MSG[] = "Syntax Error. Argument list(%d) too long.\n";
      snprintf( out, out_size-1, E2BIG_MSG, argc);
      return -E2BIG;
    }
    argv[argc++]=token;
    token=strtok(NULL, DELIM);
  }

  for( size_t i=0; i<commandEntryTableSize; ++i )
  {
    if ( commandTest( argv[0], commandEntryTable[i].command ) )
    {
      //return commandEntryTable[i].callBack(argc, argv, out, out_size);
      static const PROGMEM char DONE_MSG[] = "\nDone.\n";
      snprintf( out, out_size-1, DONE_MSG);
      return commandEntryTable[i].callBack(argc, argv);
    }
  }

  if ( commandTest( argv[0], "HELP" ) )
  {
    for( size_t i=0; out_size > 1 && i<commandEntryTableSize; ++i )
    {
      int j=0;
      while( out_size > 2 && commandEntryTable[i].command[j] != '\0' )
      {
        *out++ = commandEntryTable[i].command[j++];
        --out_size;
      }
      *out++ = '\n';
      --out_size;
    }
    *out++ = '\0';
    --out_size;
    return CLI_getLastReturnCode();
  }
  
  static const PROGMEM char ENOMSG_MSG[] = "Syntax Error. Unknown command \"%s\".\n";
  snprintf( out, out_size-1, ENOMSG_MSG, argv[0]);
  return -ENOMSG;
}

extern "C" void CLI_loop(void) 
{
  static char iobuffer[MAX_IO_BUFFER];
  // The Main REPL of the CLI

  // Read

  // Print Prompt
  puts( CLI_getPrompt() );
  size_t OutIndex = read_stdin(iobuffer, MAX_IO_BUFFER - 1);
  while( OutIndex == 0 )
  {
    puts( CLI_getPrompt() );
    OutIndex = read_stdin(iobuffer, MAX_IO_BUFFER - 1);
  }
  iobuffer[OutIndex++]='\0';

  // Evaluate
  returnCode=evaluate(&iobuffer[OutIndex],MAX_IO_BUFFER-OutIndex, iobuffer);

  // Print
  puts(&iobuffer[OutIndex]);
}
