/*
 * Programming assignment 1 
 *
 * remove c comments from a source file and write to stdout
 * comments are replaced with a single space
 * handle single and double quoted sequences
 * comments and quoted sequences can span multiple lines
 *
 * error handling:
 *     prints starting line for unterminated comments or quote sequences
 */

#include <stdio.h>
#include <stdlib.h>
#include "omit.h"

int
main(void)
{
    int c; // represent a char
    int line_num = 1; // the current line number
    int open_line_num = 1; // the starting line for unterminated comments or quote sequences

    enum typestate state = START;
    
    while ((c = getchar()) != EOF) {
        // update line number when encounter '\n'
        if (c == '\n'){
            line_num++;
        }
        switch (state) {
        case START:
            state = STARTst(c);
            break;
        case DOUBLE_QUOTE:
            open_line_num = line_num; // new starting line for string
            state = DOUBLE_QUOTEst(c);
            break;
        case SINGLE_QUOTE:
            open_line_num = line_num; // new starting line for char
            state = SINGLE_QUOTEst(c); 
            break;
        case BACK_SLASH_1:
            state = BACK_SLASH_1st(c);
            break;
        case BACK_SLASH_2:
            state = BACK_SLASH_2st(c);
            break;
        case FORWARD_SLASH:
            state = FORWARD_SLASHst(c);
            break;
        case SLASH_2:
            state = SLASH_2st(c);
            break;
        case SLASH_STAR:
            open_line_num = line_num; // new starting line for comment
            state = SLASH_STARst(c);
            break;
        case SLASH_STAR_2:
            state = SLASH_STAR_2st(c);
            break;
        default:
        // should not be reached
        fprintf(stderr, "Program error, DFA state not handled\n");
        return EXIT_FAILURE;

        // will not reach
        break;
        }
    }
    if(state == FORWARD_SLASH){
        putchar('/');
    }
    if(state == DOUBLE_QUOTE){
        fprintf(stderr, "Error: line %d: unterminated quote(\")\n", open_line_num);
        return EXIT_FAILURE;
    }
    else if(state == SINGLE_QUOTE){
        fprintf(stderr, "Error: line %d: unterminated quote(\')\n", open_line_num);
        return EXIT_FAILURE;
    }
    else if (state == SLASH_STAR || state == SLASH_STAR_2){
        fprintf(stderr, "Error: line %d: unterminated comment\n", open_line_num);
        return EXIT_FAILURE;
    }
    else{
        return EXIT_SUCCESS;
    }
}
