#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "subs.h"
#include "misc.h"

/*
 * Instructions for PA2
 *
 * put functions in here for
 * (1) filling out the input table of tokens
 * (2) Helper function for (1) that validates a field
 * (3) Function for writing out the fields specified in
 *     the output table
 *     Plus any helper functions you like
 * Below are descriptions of the two functions called by main()
 * Feel free to change the names of the parameters
 */



 /*
  * put function prototype for any helper function here.
  * since they are only called by functions within this file
  * define them as static functions.
  *
  * static int helper(void);   // example of a private helper prototype
  */

 /*
  * function prototypes for helper functions
  */
 static int validate_normal_field(*char, char delim);
 static int validate_quoted_field(*char, char delim);


/* 
 * validate_normal_field
 *
 * usage: check if a normal field is valid
 * args:
 *  buf   one field from one record in split_input
 * returns: 0 if field is valid
 *          -1 if field is not valid (illegal char " found in field)
*/
 int 
 validate_normal_field(char *buf, char delim){
  /* loop through each char until reach delim */
  while (*buf != delim){
    if(*buf == '\"'){
      return -1;
    }
    buf++;
  }
  return 0;
 }

 /* 
 * validate_quoted_field
 *
 * usage: check if a quoted field is valid
 * args:
 *  buf   one field from one record in split_input
 * returns: int >0 if field is valid 
 *          (int will represent num of chars in the field, including quotes)
 *          -1 if last character in the field was not a quote
 *          -2 if quoted field where the closing " was missing
*/
 int 
 validate_quoted_field(char *buf, char delim){
  int num_char = 0; // to store number of chars in the field
  int stage = 1; // if prev char is quote
  int exit_success; // 
  /* simplified version of stage switches 
   * stage 1 - odd number of quotes
   *         - encounter char or delim -> stay in stage 1
   *         - encounter \n -> error -1 last char not a quote
   *         - encounter " -> switch to stage 2
   * stage 2 - even number of quotes
   *         - encounter char -> error -2 closing " missing
   *         - encounter " -> switch to stage 1
   *         - encounter delim or \n -> exit with number of char
  */

  buf++; // skip first char "
  num_char++;
  /* loop through each char until reach delim */
  while (!exit_success){
    buf++;
    num_char++;
    if(stage == 1){
      if(*buf == '\n'){
        return -1;
      }
      else if(*buf == '\"'){
        stage = 2;
      }
      else {
        stage = 1;
      }
    }
    else if(stage == 2){
      if(*buf == '\"'){
        stage = 1;
      }
      else if(*buf == '\n' || *buf == delim){
        exit_success = 1;
        return num_char;
      }
      else{
        return -2;
      }
    }
    else{
      return -1; // error
    }
  }
  return 0;
 }

/*
 * split_input
 *
 * usage:   input buffer csv format, builds table of pointers to each column
 * assumes: input buffer has at least one char in it (not counting the '\0')
 * args:
 *  buf     one record from read from standind input stream by getline()
 *  delim   the input field delimiter
 *  cnt     number of fields (in table)
 *  table   point to a table of pointers to the start of each record
 *  lineno  line number of record (for passing to dropmesg)
 *  argv    for drop mesg to print name of program
 * returns: 0 if all the columns were found,
 *          -1 if a record did not have all the columns
 *          -1 if a record had an bad field value
 */
int
split_input(char *buf, char delim, int cnt, char **table, unsigned long lineno,
            char **argv)
{
    
 /*
  * build the table of pointers to columns in a loop that walks the input
  * buffer (single record) until the '\0' is seen at the end of the buffer
  *
  * Walk the input buffer using pointers only, looking for the delimiter or
  * a '\n', and making sure to stop at '\0'. For each delimiter or the final
  * newline '\n' after the last record, replace the delimiter or '\n' with a
  * '\0' to terminate that token. Store the pointer to the next char in the
  * next input array (char **table) entry. Repeat this process until you
  * either reach the end of the input line or fill all array entries. You
  * should not need to zero out the array on each pass, you just keep track
  * of which element pointer is being processed. At the end of processing
  * the input line, you should have an array filled with pointers to the
  * start of each field (column) in the record and each field is a properly
  * '\0' terminated string.
  *
  * At the end of the loop you must check you have exactly the number of columns
  * as specified on the command line in the record (in parameter cnt)
  *
  * Error messages:
  * too many columns in record call dropmsg("too many columns", lineno, argv);
  * too few columns in record call dropmsg("too few columns", lineno, argv);
  */
  int col_cnt = 1; // number of columns in input table, used to compare w/ parameter cnt
                    // minimum one field each line, increment each delimiator reached
  int new_field = 1; // 1 if buf is at 1st char of each field, set to 0 otherwise 
  int error_mes_int = 0; // use to store error int returned from helper
  int num_char_quoted = 0; // >0 if field is quoted, 0 if is normal

  /* cannot be an empty field if col_num is 1 */
  if (*buf == '\n'){
    dropmsg("Cannot have a record with just one empty field", lineno, argv);
    return -1;
  }

  /* loop through each line */
  /* end loop when \n is present unless is in quoted field */
  while ((*buf != '\n' && (num_char_quoted=0)) && *buf !='\0') 
  {
    if (num_char_quoted >0){
      num_char_quoted--; // skip char
    }
    else {
      // start processing at start char of each field, skip rest char
      if (new_field == 1){
        /* store the pointer to the start of the field in array */
        if (col_cnt<=cnt){
          *table = buf;
          table ++;
          col_cnt++;
        }
        else (
          dropmsg("too many columns", lineno, argv);
          return -1;
        )
        new_field = 0;

        // check if the field is quoted, use corresponding helper method
        if(*buf == '\"'){
          error_mes_int = validate_quoted_field(*buf, delim);
          if(error_mes_int==-1){
            dropmsg("Quoted field not terminated properly", lineno, argv);
            return -1;
          }
          else if(error_mes_int==-2){
            dropmsg("Quoted field missing final quote", lineno, argv);
            return -1;
          }
          else{
            num_char_quoted = error_mes_int-1; // skip current char
          }
        }
        else{
          if(validate_normal_field(*buf, delim)==1){
            dropmsg("A \" is not allowed inside unquoted field", lineno, argv);
            return -1;
          }
        }
      }
      /* replace token with \0 if reached deliminator, increment num of col*/

      if(*buf == delim){
        *buf = '\0';
        new_field = 1;
      }
    }
    buf++; 
  }
  /* replace token with \0 if reached end of line */
  *buf = '\0';

  // check if number of columns matchs parameter cnt
  // Drop error message if mismatch
  if(col_cnt>cnt){
    dropmsg("too many columns", lineno, argv);
    return -1; // too many columns in record call
  }
  else(col_cnt<cnt){
    dropmsg("too few columns", lineno, argv);
    return -1; // too few columns in record call
  }

  return 0;



/*
  * delete these comments and add your own before submitting your code
  */
/*
  * How to process each field/column
  * first: check if the first char in the field is a " or not.
  * if it is a " then process it as a quoted field. o.w process it
  * as a unquoted field.
  *
  * the rules for proper fields/columns are listed in the PA writeup
  *
  * error messages for unquoted fields
  *
  * dropmsg("A \" is not allowed inside unquoted field", lineno, argv);
  *
  * error messages for quoted fields
  *
  * quoted field where the last character in the field was not a quote.
  * dropmsg("Quoted field not terminated properly", lineno, argv);
  *
  * quoted field where the closing " was missing
  * dropmsg("Quoted field missing final quote", lineno, argv);
  */


}

/*
 * wr_row
 *
 * usage:   given an array of pointers to columns and an array of columns to
 *          ouput and a count of output columns
 *          output those columns specified in the output array (contents are
 *          index numbers of the input array)
 * args:
 *  in_tab  pointer to a table of pointers to the start of each input field
 *  out_tab pointer to a table of ints containing the array index to print
 *          the order in this array is the order the cols/fields are printed
 *  delim   the output record delimiter to use
 *  lineno  line number of record for dropmsg
 *  argv    program name for dropmesg
 * return:  the number of records dropped during output
 *          0 if the record was printed
 *          1 if the record was dropped (special case 1 col and is empty)
 */
int
wr_row(char **in_tab, int *out_tab, int out_cnt, char delim,
        unsigned long lineno, char **argv)
{
    /*
     * delete these comments and add your own before submitting your code
     */
    /*
     * print all the columns execept the last one with a trailing delimiter
     *
     * Walk down the output array, (parameter int *out_tab), from the first
     * element in the array. Each element in the output array contains the
     * index number of the next field to write to the standard output stream.
     * Use this number as an index into the input array,
     * (parameter char **in_tab), to get the pointer to the input field you
     * need to output (remember array indexes start at 0 and the columns are
     * numbered from 1). The number of output record fields (also the number
     * of elements in out_tab) is in the parameter int out_cnt. Use the
     * parameter char delim as the delimiter in your output records that you
     * write to the standard output stream.
     *
     * special case!
     * if only one output column and input is empty line, skip it
     * (do not print it) and do the following
     * dropmsg("empty column", lineno, argv);
     * return 1
     */

    /*
     * special case!
     * if only one output column and input is empty line, skip it
     * (do not print it) and do the following
     * dropmsg("empty column", lineno, argv);
     * return 1
     */
    if (out_cnt == 1 && **in_tab == '\0'){
      dropmsg("empty column", lineno, argv);
      return 1;
    }
    int* otpt = out_tab; // set pointer to the start of table
    for (int i=1; i<out_cnt; i++){
      printf("%s", *otpt);
      otpt++;
      printf("%c", delim);
    }
    printf("%s", *otpt);
    printf("\n");
    return 0;
}
