/* 
 * the next two lines are the header guards
 */
#ifndef _OMIT_H
#define _OMIT_H
/*
 * header file for omit
 * type definition (enum) for DFA states
 * function prototypes
 */

enum typestate {START, DOUBLE_QUOTE, SINGLE_QUOTE, BACK_SLASH_1, BACK_SLASH_2, 
                FORWARD_SLASH, SLASH_STAR, SLASH_STAR_2, SLASH_2};

enum typestate STARTst(int);
enum typestate DOUBLE_QUOTEst(int);
enum typestate SINGLE_QUOTEst(int);
enum typestate BACK_SLASH_1st(int);
enum typestate BACK_SLASH_2st(int);
enum typestate FORWARD_SLASHst(int);
enum typestate SLASH_2st(int);
enum typestate SLASH_STARst(int);
enum typestate SLASH_STAR_2st(int);

#endif
