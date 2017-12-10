extern void *_brk(void *addr);
extern int _open(char *path, int flags, int perm);
extern int _close(int fd);
extern int _read(int fd, void *buf, size_t n);
extern int _write(int fd, void *buf, size_t n);
extern int _lseek(int fd, long off, int whence);
extern void _Exit(int status);
extern void _access(char *path, int mode);

extern int raise(int sig);
extern void (*signal(int sig, void (*func)(int)))(int);
extern getenv(const char *var);
extern int rename(const char *from, const char *to);
extern int remove(const char *path);
