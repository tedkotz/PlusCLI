
#include "conio.h"

#include <stdarg.h>

#define SCAN_BUFFER_SIZE 128


extern "C" int CONIO_putchar( int  c)
{
  if( c == '\n' )
  {
    Serial.write('\r');  
  }
  Serial.write(c);
  return c;
}

extern "C" int CONIO_puts( const char * str)
{
  const char * ptr = str;
  while( *ptr != '\0' )
  {
    putchar(*ptr++);
  }
  return (ptr-str);
}

extern "C" int CONIO_vprintf( const char * format, va_list args)
{
  const unsigned trySize = 128;
  char tryBuf[trySize];
  unsigned realSize = vsnprintf(tryBuf, trySize, format, args) + 1;

  if( realSize > trySize)
  {
    char realBuf[realSize];
    vsnprintf(realBuf, realSize, format, args);
    return puts(realBuf);
  }
  
  return puts(tryBuf);
}

extern "C" int CONIO_printf( const char * format, ...)
{
  va_list arg_ptr;

  va_start(arg_ptr, format);
  int returnVal = vprintf(format, arg_ptr);
  va_end(arg_ptr);
  return returnVal;
}

extern "C" int getch(void)
{
  int c = Serial.read();
  while (c==-1)
  {
    delay(100);
    c = Serial.read();
  } 

  switch( c )
  {
    // Check for enter
    case '\n':
    case '\r':
      return '\n';

    default:
      return c;
  } 
}

extern "C" int getche(void)
{
  int c = getch();

  switch( c )
  {
    // Check for backspace or delete
    case '\b':
    case 127:
      putchar((char)c);
      putchar(' ');
      putchar((char)c);
      break;

    default:
      putchar((char)c);
  } 
  
  return c;
}

extern "C" int CONIO_getchar(void)
{
  return getche();
}

extern "C" size_t read_stdin(char* str, size_t n)
{
  int numRead=0;
  for( numRead=0; numRead <= n; numRead++)
  {
    int c=getche();
    
    switch( c )
    {
      // Check for enter
      case '\n':
        return numRead;

      // Check for backspace or delete
      case '\b':
      case 127:
        numRead -= 2;
        break;

      default:
        str[numRead]=c;
        break;
    } 
  }

  return numRead;
}

extern "C" char* gets_s(char* str, size_t n)
{
  int numRead=read_stdin(str, n-1);
  str[numRead]='\0';
  return str;
}

extern "C" char* CONIO_gets(char* str)
{
  return gets_s( str, SIZE_MAX);
}

#ifdef __AVR__
// AVR compiler doesn't define vssscanf so we will make a very naive version that only supports 3 args
static int vsscanf(const char *str, const char *format, va_list args)
{
  return sscanf(str, format, va_arg(args, void*), va_arg(args, void*), va_arg(args, void*));
}
#endif

extern "C" int vscanf(const char *format, va_list args)
{
  char temp[SCAN_BUFFER_SIZE];
  gets_s(temp, SCAN_BUFFER_SIZE);
  return vsscanf(temp, format, args);
}


extern "C" int CONIO_scanf( const char * format, ...)
{
  va_list arg_ptr;

  va_start(arg_ptr, format);
  int returnVal = vscanf(format, arg_ptr);
  va_end(arg_ptr);
  return returnVal;
}

extern "C" void clrscr(void)
{
  Serial.write("\033[2J\033[H");
}

extern "C" void delline(void)
{
  Serial.write("\033[2K");
}

extern "C" void textcolor(int color)
{
  char temp[128];
  snprintf(temp,128,"\033[38;5;%dm", color);
  Serial.write(temp);
}

extern "C" void textbackground(int color)
{
  char temp[128];
  snprintf(temp,128,"\033[48;5;%dm", color);
  Serial.write(temp);
}

extern "C" int kbhit(void)
{
  return (-1 != Serial.peek());
}

extern "C" void gotoxy(int x, int y)
{
  char temp[128];
  snprintf(temp,128,"\033[%d;%dH", y, x);
  Serial.write(temp);
}

//extern "C" void whereCursor( int* x, int* y)
//{
//  //char temp[128];
//  Serial.write("\033[6n");  
//  //Serial.read()
//  //scanf("\033[%d;%dR", y, x)
//}

// extern "C" int wherex(void);
// extern "C" int wherey(void);
