#include "utils.h"

#include <stdio.h>

#ifdef _WIN32
    #include <conio.h>
    char getch(void) {
        return _getch();
    }
#else
    #include <unistd.h>
    #include <termios.h>
    #include <sys/select.h>
    #include <errno.h>
    char getch(void) {
        struct termios oldt, newt;
        char ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        if (read(STDIN_FILENO, &ch, 1) == -1) return EOF;
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif

int getchNB(char *key) {
    if (key == NULL) return -1;

#ifdef _WIN32
    if (!_kbhit()) return 0;
    *key = (char)_getch();
    return 1;
#else
    struct termios oldt, newt;
    fd_set readfds;
    struct timeval timeout = {0, 0};

    if (tcgetattr(STDIN_FILENO, &oldt) == -1) return -1;

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) return -1;

    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    int ready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

    int result = 0;
    if (ready > 0) {
        ssize_t bytes = read(STDIN_FILENO, key, 1);
        result = bytes == 1 ? 1 : (bytes == 0 ? 0 : -1);
    } else if (ready < 0) {
        result = -1;
    }

    if (tcsetattr(STDIN_FILENO, TCSANOW, &oldt) == -1) return -1;
    return result;
#endif
}

#if defined(_WIN32)
    #include <windows.h>
    void sleepms(unsigned int ms) {
        Sleep(ms);
    }
#else
    #include <time.h>
    void sleepms(unsigned int ms) {
        struct timespec ts;
        ts.tv_sec = ms / 1000;
        ts.tv_nsec = (ms % 1000) * 1000000L;
        while (nanosleep(&ts, &ts) == -1 && errno == EINTR);
    }
#endif