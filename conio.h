
#include <Arduino.h>
#include <stdio.h>

#undef putchar
#define putchar   CONIO_putchar
#define puts      CONIO_puts
#define vprintf   CONIO_vprintf
#define printf    CONIO_printf
#undef getchar
#define getchar   CONIO_getchar
#define gets      CONIO_gets
#define scanf     CONIO_scanf

#ifdef __cplusplus
extern "C" {
#endif

int CONIO_putchar( int  c);
int CONIO_puts( const char * str);
int CONIO_vprintf( const char * format, va_list args);
int CONIO_printf( const char * format, ...);
int getch(void);
int getche(void);
int CONIO_getchar(void);
size_t read_stdin(char* str, size_t n);
char* gets_s(char* str, size_t n);
char* CONIO_gets(char* str);
int vscanf(const char *format, va_list args);
int CONIO_scanf( const char * format, ...);
void clrscr(void);
void delline(void);
void textcolor(int color);
void textbackground(int color);
int kbhit(void);
void gotoxy(int x, int y);



// void whereCursor( int* x, int* y)
// int wherex(vodi);
// int wherey(void);

// COLORS

// Old CONIO mapping
// BLACK         0
// BLUE          1   X
// GREEN         2
// CYAN          3   X
// RED           4   X
// MAGENTA       5
// BROWN         6   X
// LIGHTGRAY     7
// DARKGRAY      8
// LIGHTBLUE     9   X
// LIGHTGREEN    10
// LIGHTCYAN     11  X
// LIGHTRED      12  X
// LIGHTMAGENTA  13
// YELLOW        14  X
// WHITE         15 

// Numbers reassigned to match ANSI colors
#define BLACK            0
#define RED              1
#define GREEN            2
#define BROWN            3
#define BLUE             4
#define MAGENTA          5
#define CYAN             6
#define LIGHTGRAY        7
#define DARKGRAY         8
#define LIGHTRED         9
#define LIGHTGREEN       10
#define YELLOW           11
#define LIGHTBLUE        12
#define LIGHTMAGENTA     13
#define LIGHTCYAN        14
#define WHITE            15


#ifdef  __cplusplus
}

// C++ functions to allow use of FlashStrings
int CONIO_puts( const __FlashStringHelper* str);
int CONIO_vprintf( const __FlashStringHelper* format, va_list args);
int CONIO_printf( const __FlashStringHelper* format, ...);
int vscanf(const __FlashStringHelper* format, va_list args);
int CONIO_scanf( const __FlashStringHelper* format, ...);

#endif
