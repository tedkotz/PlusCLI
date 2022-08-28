
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

#ifdef  __cplusplus
}
#endif
