#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

// Dummy heap implementation for malloc
extern char _end; // Provided by the linker
static char *heap_end;
caddr_t _sbrk(int incr) {
    if (heap_end == 0) {
        heap_end = &_end;
    }
    char *prev_heap_end = heap_end;
    heap_end += incr;
    return (caddr_t)prev_heap_end;
}

int _write(int file, char *ptr, int len) {
    // Optionally send chars to UART here
    return len;
}

int _close(int file) {
    return -1;
}

int _fstat(int file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file) {
    return 1;
}

int _lseek(int file, int ptr, int dir) {
    return 0;
}

int _read(int file, char *ptr, int len) {
    return 0;
}

void _exit(int status) {
    while (1) {} // Loop forever
}

int _kill(int pid, int sig) {
    errno = EINVAL;
    return -1;
}

int _getpid(void) {
    return 1;
}