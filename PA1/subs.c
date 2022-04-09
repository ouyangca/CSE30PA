/*
 * subroutines for omit program
 * one subroutine for each state of the DFA
 */
#include <stdio.h>
#include "omit.h"

enum typestate STARTst(int c){
    if (c == '\''){
        putchar(c);
        return SINGLE_QUOTE;
    }
    else if (c == '\"'){
        putchar(c);
        return DOUBLE_QUOTE;
    }
    else if (c == '/'){
        return FORWARD_SLASH;
    }
    else{
        putchar(c);
        return START;
    }
}

enum typestate DOUBLE_QUOTEst(int c){
    if (c == '\\'){
        putchar(c);
        return BACK_SLASH_2;
    }
    else if (c == '\"'){
        putchar(c);
        return START;
    }
    else{
        putchar(c);
        return DOUBLE_QUOTE;
    }
}

enum typestate SINGLE_QUOTEst(int c){
    if (c == '\\'){
        putchar(c);
        return BACK_SLASH_1;
    }
    else if (c == '\''){
        putchar(c);
        return START;
    }
    else{
        putchar(c);
        return SINGLE_QUOTE;
    }
}

enum typestate BACK_SLASH_1st(int c){
    putchar(c);
    return SINGLE_QUOTE;
}

enum typestate BACK_SLASH_2st(int c){
    putchar(c);
    return DOUBLE_QUOTE;
}

enum typestate FORWARD_SLASHst(int c){
    if (c == '/'){
        return SLASH_2;
    }
    else if (c == '*'){
        return SLASH_STAR;
    }
    else{
        putchar('/');
        putchar(c);
        return START;
    }
}

enum typestate SLASH_STARst(int c){
    if (c == '\n'){
        putchar('\n');
        return SLASH_STAR;
    }
    else if (c == '*'){
        return SLASH_STAR_2;
    }
    else{
        return SLASH_STAR;
    }
}

enum typestate SLASH_STAR_2st(int c){
    if (c == '/'){
        return START;
    }
    else{
        putchar('*');
        putchar(c);
        return SLASH_STAR;
    }
}

enum typestate SLASH_2st(int c){
    if (c == '\n'){
        putchar(c);
        return START;
    }
    else{
        return SLASH_2;
    }
}