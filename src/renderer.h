#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <stdio.h>
#include <stdlib.h>

#include "rect.h"
#include "noted.h"
#include "terminal.h"

const char CTRLD = '\004';

// Render name Editor
void render_banner(rect_t * rect) {
    terminal_move(rect->offset_x + rect->width / 2 - 3, rect->offset_y);
    printf("%snoted%s", TMAG, TRES);
}

// Render command bar
void render_command(noted_t * state, rect_t * rect) {
    // Position the cursor
    terminal_move(rect->offset_x, rect->offset_y);
    
    // Color the Bar
    printf("%s", "\033[37;45;1m");
    for (size_t i = 0; i < rect->width; i++)
        printf(" ");
    
    // Write Mode
    terminal_move(rect->offset_x + 2, rect->offset_y);
    switch (state->mode) {
        case INSERT:
            printf("%s", "I");
            break;
        case MOVE:
            printf("%s", "M");
            break;
    }

    // Last Stroke
    terminal_move(4UL, rect->offset_y);
    printf("%c", state->last_stroke);

    // Cursor
    terminal_move(rect->width - 14, rect->offset_y);
    printf("%5lu [%2lu, %2lu]",state->cursor.now , state->cursor.x, state->cursor.y);

    printf("%s", TRES);
}

// Render line numbers and reduce opacity
void render_line(noted_t * state, rect_t * rect) {
    printf("%s", TLOP);
    for (int i = 0; i < rect->height; i++) {
        terminal_move(rect->offset_x, rect->offset_y + i);
        printf("%4lu", state->scroll.y + i + 1);
    }
    printf("%s", TRES);
}

// Render text area and truncate long lines
void render_text(noted_t * state, rect_t * rect) {
    line_t * line = state->lines;
    size_t ln = 0;

    for (size_t x = 0; x < rect->width; x++) {
        for (size_t y = 0; y < rect->height; y++) {
            terminal_move(x + rect->offset_x, y + rect->offset_y);
            putc(' ', stdout);
        }
    }

    terminal_move(rect->offset_x, rect->offset_y);
    while (line != NULL && ln < rect->height + state->scroll.y) {

        if (ln >= state->scroll.y) {
            for (size_t i = line->start; i <= line->end && i <= (line->start + rect->width); i++) {
                putc(state->file_content[i], stdout);

                if (i == line->start + rect->width || i == line->end)
                    terminal_move(rect->offset_x, rect->offset_y + ln + 1 - state->scroll.y);
            }
        }

        line = line->next_line;
        ln += 1;
    }
}

// Log state in stderr for debub porposes
void log_state(noted_t *state) {
    fprintf(stderr, "%s\n ", "State");
    fprintf(stderr, " lines = %p\n",  state->lines);
    fprintf(stderr, "  line_num = %lu\n", state->line_num);
    fprintf(stderr, "  file_content = %p\n",  state->file_content);
    fprintf(stderr, "  file_size = %lu\n", state->file_size);
    fprintf(stderr, "  cursor = { x: %lu, y: %lu }\n", state->cursor.x, state->cursor.y);
    fprintf(stderr, "  scroll = { x: %lu, y: %lu }\n", state->scroll.x, state->scroll.y);
    fprintf(stderr, "  mode = %u\n",  state->mode);
    fprintf(stderr, "  last_stroke = %c\n",  state->last_stroke);
    fprintf(stderr, "%s\n\n", "---------------------------------------");
}

// Start Render Loop
void render(noted_t * state) {

    // Prepare Rects
    // rect_t banner_rect = {0UL, 0UL,  55UL, 1UL}; 
    rect_t line_rect = {0UL,  0UL,  5UL, 15UL};
    rect_t text_rect = {5UL,  0UL, 90UL, 15UL};
    rect_t comm_rect = {0UL, 15UL, 95UL,  1UL};

    // render_banner(&banner_rect);
    render_line(state, &line_rect);
    render_text(state, &text_rect);
    render_command(state, &comm_rect);

    // Move cursor in the text editor 
    terminal_move(
        text_rect.offset_x + STATE->cursor.x,
        text_rect.offset_y + STATE->cursor.y
    );

    // Wait for user input
    while ((state->last_stroke = fgetc(stdin)) != CTRLD) {

        if (state->mode == MOVE) {
            switch (state->last_stroke) {
                case 'w':
                    move_cursor(UP, &text_rect);
                    break;            
                case 'a':
                    move_cursor(LEFT, &text_rect);
                    break;
                case 's':
                    move_cursor(DOWN, &text_rect);
                    break;
                case 'd':
                    move_cursor(RIGHT, &text_rect);
                    break;

                case 'i':
                    state->mode = INSERT;
                    break;
                

                default:
                    break;
            }
        }

        // Render Command
        render_command(state, &comm_rect);
        render_text(state, &text_rect);
        render_line(state, &line_rect);

        // Move cursor in the text editor 
        terminal_move(
            text_rect.offset_x + STATE->cursor.x - STATE->scroll.x,
            text_rect.offset_y + STATE->cursor.y - STATE->scroll.y
        );
        
        // Log Terminal State at the end of the loop
        log_state(state);
    }
}

#endif /* _RENDERER_H_ */