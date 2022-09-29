/**
 * @file    conio.h
 * @author  Ted Kotz <ted@kotz.us>
 * @version 0.1
 *
 * A Console I/O interface declaration targeting compatibility with similar
 * functionality from older DOS Borland tools. 
 * 
 * This libabry is implemented over serial I/O relying on ANSI terminal escape sequences
 *
 */
#ifndef   CONIO_H
#define   CONIO_H

/* Includes ******************************************************************/
#include <Arduino.h>
#include <stdio.h>

/* Defines *******************************************************************/
#undef putchar
#define putchar   CONIO_putchar
#define puts      CONIO_puts
#define vprintf   CONIO_vprintf
#define printf    CONIO_printf
#undef getchar
#define getchar   CONIO_getchar
#define gets      CONIO_gets
#define scanf     CONIO_scanf

#define _cgets    CONIO_gets
#define _cprintf  CONIO_printf
#define _cputs    CONIO_puts
#define _cscanf   CONIO_scanf
#define cgets     CONIO_gets
#define cprintf   CONIO_printf
#define cputs     CONIO_puts
#define cscanf    CONIO_scanf

#define _getch    getch
#define _getche   getche
#define _kbhit    kbhit
#define _putch    CONIO_putchar
#define putch     CONIO_putchar

#define _ungetch  ungetch

#define _NOCURSOR      0
#define _NORMALCURSOR  20
#define _SOLIDCURSOR   100

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


/* Types *********************************************************************/
/* Interfaces ****************************************************************/
/* Data **********************************************************************/
/* Functions *****************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


/**
 * [Description]
 *
 * @param
 * @return
 */
int CONIO_putchar( int  c);

/**
 * [Description]
 *
 * @param
 * @return
 */
int CONIO_puts( const char * str);

/**
 * [Description]
 *
 * @param
 * @return
 */
int CONIO_vprintf( const char * format, va_list args);

/**
 * [Description]
 *
 * @param
 * @return
 */
int CONIO_printf( const char * format, ...);

/**
 * [Description]
 *
 * @param
 * @return
 */
int getch(void);

/**
 * [Description]
 *
 * @param
 * @return
 */
int getche(void);

/**
 * [Description]
 *
 * @param
 * @return
 */
int CONIO_getchar(void);

/**
 * [Description]
 *
 * @param
 * @return
 */
size_t read_until(char* str, size_t n, int until);

/**
 * [Description]
 *
 * @param
 * @return
 */
char* gets_s(char* str, size_t n);

/**
 * [Description]
 *
 * @param
 * @return
 */
char* CONIO_gets(char* str);

/**
 * [Description]
 *
 * @param
 * @return
 */
int vscanf(const char *format, va_list args);

/**
 * [Description]
 *
 * @param
 * @return
 */
int CONIO_scanf( const char * format, ...);

/**
 * [Description]
 *
 * @param
 * @return
 */
void clrscr(void);

/**
 * [Description]
 *
 * @param
 * @return
 */
void delline(void);

/**
 * [Description]
 *
 * @param
 * @return
 */
void textattr(int attr);

/**
 * [Description]
 *
 * @param
 * @return
 */
void textcolor(int color);

/**
 * [Description]
 *
 * @param
 * @return
 */
void textbackground(int color);

/**
 * [Description]
 *
 * @param
 * @return
 */
void textblink(int blink);

/**
 * [Description]
 *
 * @param
 * @return
 */
int kbhit(void);

/**
 * [Description]
 *
 * @param
 * @return
 */
void gotoxy(int x, int y);


/**
 * [Description]
 *
 * @param
 * @return
 */
void clreol (void);


/**
 * [Description]
 *
 * @param
 * @return
 */
void whereCursor( int* x, int* y);

/**
 * [Description]
 *
 * @param
 * @return
 */
int wherex(void);

/**
 * [Description]
 *
 * @param
 * @return
 */
int wherey(void);


/**
 * [Description]
 *
 * @param
 * @return
 */
int  cputsxy (int x, int y, const char* str);

/**
 * [Description]
 *
 * @param
 * @return
 */
int  putchxy (int x, int y, int ch);


/**
 * [Description]
 *
 * @param
 * @return
 */
void  _setcursortype (int type);

/**
 * [Description]
 *
 * @param
 * @return
 */
void  normvideo (void);


/**
 * [Description]
 *
 * @param
 * @return
 */
int ungetch (int);

/**
 * [Description]
 *
 * @param
 * @return
 */
void clearkeybuf (void);


#ifdef  __cplusplus
}

// C++ functions to allow use of FlashStrings

/**
 * [Description]
 *
 * @param
 * @return
 */
int CONIO_puts( const __FlashStringHelper* str);

/**
 * [Description]
 *
 * @param
 * @return
 */
int CONIO_vprintf( const __FlashStringHelper* format, va_list args);

/**
 * [Description]
 *
 * @param
 * @return
 */
int CONIO_printf( const __FlashStringHelper* format, ...);

/**
 * [Description]
 *
 * @param
 * @return
 */
int vscanf(const __FlashStringHelper* format, va_list args);

/**
 * [Description]
 *
 * @param
 * @return
 */
int vsscanf(const char *str, const __FlashStringHelper* format, va_list args);

/**
 * [Description]
 *
 * @param
 * @return
 */
int sscanf( const char *str, const __FlashStringHelper* format, ...);

/**
 * [Description]
 *
 * @param
 * @return
 */
int CONIO_scanf( const __FlashStringHelper* format, ...);

/**
 * [Description]
 *
 * @param
 * @return
 */
int cputsxy (int x, int y, const __FlashStringHelper* str);

#endif


#endif // CONIO_H
