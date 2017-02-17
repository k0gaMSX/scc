/* See LICENSE file for copyright and license details. */
#ifndef _STDIO_H
#define _STDIO_H

#include <arch/stdio.h>

#define EOF            -1
#define _IOFBF          0
#define _IOLBF          1
#define _IONBF          2
#define SEEK_CUR        0
#define SEEK_END        1
#define SEEK_SET        2

extern int remove(const char *filename);
extern int rename(const char *old, const char *new);
extern FILE *tmpfile(void);
extern char *tmpnam(char *s);
extern int fclose(FILE *stream);
extern int fflush(FILE *stream);
extern FILE *fopen(const char * restrict filename, const char * restrict mode);
extern FILE *freopen(const char * restrict filename, const char * restrict mode,
              FILE * restrict stream);
extern void setbuf(FILE * restrict stream, char * restrict buf);
extern int setvbuf(FILE * restrict stream, char * restrict buf, int mode, size_t size);
extern int fprintf(FILE * restrict stream, const char * restrict format, ...);
extern int fscanf(FILE * restrict stream, const char * restrict format, ...);
extern int printf(const char * restrict format, ...);
extern int scanf(const char * restrict format, ...);
extern int snprintf(char * restrict s, size_t n, const char * restrict format, ...);
extern int sprintf(char * restrict s, const char * restrict format, ...);
extern int sscanf(const char * restrict s, const char * restrict format, ...);
extern int vfprintf(FILE * restrict stream, const char * restrict format, va_list arg);
extern int vfscanf(FILE * restrict stream, const char * restrict format, va_list arg);
extern int vprintf(const char * restrict format, va_list arg);
extern int vscanf(const char * restrict format, va_list arg);
extern int vsnprintf(char * restrict s, size_t n, const char * restrict format,
              va_list arg);
extern int vsprintf(char * restrict s, const char * restrict format, va_list arg);
extern int vsscanf(const char * restrict s, const char * restrict format, va_list arg);
extern int fgetc(FILE *stream);
extern char *fgets(char * restrict s, int n, FILE * restrict stream);
extern int fputc(int c, FILE *stream);
extern int fputs(const char * restrict s, FILE * restrict stream);
extern int getc(FILE *stream);
extern int getchar(void);
extern char *gets(char *s);
extern int putc(int c, FILE *stream);
extern int putchar(int c);
extern int puts(const char *s);
extern int ungetc(int c, FILE *stream);
extern size_t fread(void * restrict ptr, size_t size, size_t nmemb,
             FILE * restrict stream);
extern size_t fwrite(const void * restrict ptr, size_t size, size_t nmemb,
              FILE * restrict stream);
extern int fgetpos(FILE * restrict stream, fpos_t * restrict pos);
extern int fseek(FILE *stream, long int offset, int whence);
extern int fsetpos(FILE *stream, const fpos_t *pos);
extern long int ftell(FILE *stream);
extern void rewind(FILE *stream);
extern void clearerr(FILE *stream);
extern int feof(FILE *stream);
extern int ferror(FILE *stream);
extern void perror(const char *s);

#endif
