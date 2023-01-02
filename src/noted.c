#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <termios.h>
#include <unistd.h>

#include "errors.h"
#include "renderer.h"

int main (int argc, char **argv) {

    EXECNAME = argv[0];
    STATE = calloc(1UL, sizeof(noted_t));

    // Make sure stdin is a terminal 
    if (!isatty(STDIN_FILENO)) {
        raise_error("noted only works in a terminal.", NO_TERMINAL);
    }

    // Make sure a file is specified
    if (argc != 2) {
        raise_error("please specify a file name.", FILE_UNDEFINED);
    }

    // Make sure the file exists
    FILE *fp = fopen(argv[1], "r+"); 
    if (fp == NULL) {
        raise_error("the file does not exist.", FILE_NOT_FOUND);
    }

    // Get file size
    fseek(fp, 0L, SEEK_END);
    STATE->file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    // Read File in Memory
    STATE->file_content = malloc(sizeof(char) * STATE->file_size);
    fread(STATE->file_content, sizeof(char), STATE->file_size, fp);

    // File lines
    compute_lines(STATE);

    // Prepare Terminal
    terminal_prepare();

    // Render Loop
    render(STATE);

    // Close File
    fclose(fp);

    return 0;
}