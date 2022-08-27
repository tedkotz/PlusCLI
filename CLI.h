#include <Arduino.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif  

#define SUCCESS 0

typedef int (*CLI_CommandFunction) (int argc, char** argv);
//typedef int (*CLI_CommandFunction) (int argc, char** argv, char* out, size_t out_size);

typedef struct CLI_CommandEntry
{
  const char* command;
  // const char* helpText;
  // const ArgParse arguments;
  CLI_CommandFunction callBack;
} CLI_CommandEntry;

extern const char* (*CLI_getPrompt) (void);

void CLI_registerCommandEntryTable ( const CLI_CommandEntry* table, size_t size );
void CLI_loop(void);
int CLI_getLastReturnCode (void);


#ifdef  __cplusplus
}
#endif
