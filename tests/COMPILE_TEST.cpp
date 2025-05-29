
/**
 * need environment:
 * c++: c++98
 * os: Linux (Unix)
 */

// depend headers
#include <unistd.h>
#include <sys/wait.h>

// definition test
#include <signal.h>

#ifndef SIGALRM
# error Need SIGALRM
#endif
