#ifndef _ERRORS_H_
#define _ERRORS_H_

#include <stdlib.h>
#include <stdio.h>

#include "terminal.h"
#include "noted.h"

typedef enum {
    NO_TERMINAL,
    FILE_UNDEFINED,
    FILE_NOT_FOUND,
} error_code_t;

// Prints noted usage
void usage() {
    printf("%s", "\nUsage:\n");
    // TODO: Support Editor Name Change
    printf("  %s {filename}\n", EXECNAME);
}

// Prints an error message and exits
void raise_error(const char const *error_string, error_code_t error_code) {
    // Fancy Red Error Message
    printf("%s", TRED);
    printf("%s", "ERROR: ");
    printf("%s", TRES);
    printf("%s\n", error_string);

    usage();
    exit(error_code);
}


#endif /* _ERRORS_H_ */