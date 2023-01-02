#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <stdio.h>
#include <stdlib.h>

#include <termios.h>
#include <unistd.h>

size_t TWID = 100;
size_t THEI = 15;

char * TRED = "\033[1;31m";
char * TMAG = "\033[1;35m";
char * TLOP = "\033[2;37m";
char * TRES = "\033[0m";

char * TMTL = "\033[H";
char * TCLS = "\033[2J";

typedef struct termios terminal_t;

terminal_t * TPREV;
terminal_t * TTHIS;

void terminal_move(size_t x, size_t y) {
    printf("\033[%lu;%luH", y + 1UL, x + 1UL);
}

// Clear terminal
void terminal_clear() {
    printf("%s", TCLS);
    printf("%s", TMTL);
}

// Restore previous terminal attributes
void terminal_restore() {
    terminal_clear();
    tcsetattr(STDIN_FILENO, TCSANOW, TPREV);
}

// Prepares terminal for action
void terminal_prepare() {
    terminal_clear();

    // Store terminal attributes to be restore later
    TPREV = (terminal_t *) malloc(sizeof(terminal_t));
    tcgetattr(STDIN_FILENO, TPREV);
    
    // Restore terminal when exiting
    atexit(terminal_restore);

    // Clear ICANON and ECHO
    TTHIS = (terminal_t *) malloc(sizeof(terminal_t));
    TTHIS->c_lflag &= ~(ICANON | ECHO);
    TTHIS->c_cc[VMIN] = 1;
    TTHIS->c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, TTHIS);
}

#endif /* _TERMINAL_H_ */