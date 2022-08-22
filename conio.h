
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif  

int putchar( int  c);
int puts( const char * str);
int vprintf( const char * format, va_list args);
int printf( const char * format, ...);
int getch(void);
int getche(void);
int getchar(void);
extern "C" size_t read_stdin(char* str, size_t n);
char* gets_s(char* str, size_t n);
char* gets(char* str);
int vscanf(const char *format, va_list args);
int scanf( const char * format, ...);
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
