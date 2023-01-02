#ifndef _NOTED_H_
#define _NOTED_H_

#include <stdlib.h>
#include <string.h>

#include "rect.h"

typedef enum MODES {
    MOVE,
    INSERT
} editor_mode_t;

typedef struct line {
    size_t        start;
    size_t        end;
    struct line * next_line;
} line_t;

typedef struct cursor
{
    size_t x;
    size_t y;
    size_t now;
} cursor_t;

typedef struct noted {
    line_t        * lines;
    size_t          line_num;
    char          * file_content;
    size_t          file_size;
    cursor_t        cursor;
    cursor_t        scroll;
    editor_mode_t   mode;
    char            last_stroke;
} noted_t;

typedef enum DIRECTIONS {
    UP, 
    LEFT, 
    DOWN, 
    RIGHT
} dir_t;

// Executable Name
char * EXECNAME;

// Editor state
noted_t * STATE;

// Compute Lines
void compute_lines() {
        
    line_t *line = STATE->lines = malloc(sizeof(line_t));
    line->start = 0;

    for (int i = 0; i < STATE->file_size; i++) {
        if (STATE->file_content[i] == '\n' || i == STATE->file_size ) {
            line->end = i;
            line = line->next_line = malloc(sizeof(line_t));
            STATE->line_num++; 
            line->start = i + 1;
        }
    }

    line->end = STATE->file_size;
}

line_t * get_line (size_t n) {
    line_t * line = STATE->lines;
    while (n != 0) {
        line = line->next_line;
        n--;
        if (line == NULL) return line;
    }

    return line;
}

void move_cursor(dir_t direction, rect_t * rect) {
    size_t x, y;
    line_t * line;
    
    switch (direction)
    {
        case UP:
            x = STATE->cursor.x;
            y = (STATE->cursor.y) ? STATE->cursor.y - 1 : 0; 
            break;
        
        case LEFT:
            if (STATE->cursor.x) {
                x = (STATE->cursor.x) ? STATE->cursor.x - 1 : 0;
                y = STATE->cursor.y;
            } else {
                x = -1;
                y = (STATE->cursor.y) ? STATE->cursor.y - 1 : 0;
            }

            break;
        
        case DOWN:
            x = STATE->cursor.x;
            y = STATE->cursor.y + 1;
            break;
        
        case RIGHT:
            x = STATE->cursor.x + 1;
            y = STATE->cursor.y;
            break;

    }

    line = get_line(y);

    // If rect is not enough
    if (y < (STATE->scroll.y)) {
        STATE->scroll.y -= 1;
    }

    // If rect is not enough
    if (y > (STATE->scroll.y + rect->height - 1)) {
        STATE->scroll.y += 1;
    }
    
    // If line is not long enough
    if (x > (line->end - line->start)) { 
        x = 0;
        y++;
    }

    // If line does not exist
    if (y > STATE->line_num) {
        y = STATE->line_num - 1;
    }

    STATE->cursor.x = x;
    STATE->cursor.y = y;
    STATE->cursor.now = line->start + x;
}

#endif /* _NOTED_H_ */