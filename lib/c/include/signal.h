#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <arch/signal.h>

void ( *signal(int signum, void (*handler)(int)) ) (int);
int raise(int sig);

#endif
