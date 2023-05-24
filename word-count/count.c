/*
 * COMP 321 Project 2: Word Count
 *
 * This program counts the characters, words, and lines in one or more files,
 * depending on the command-line arguments.
 * 
 * Tyra Cole (tjc6)
 */

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "csapp.h"

struct counts {
	int   char_count;
	int   word_count;
	int   line_count;
};

struct node {
	int char_count;	// struct count of characters
	int word_count; // struct count of words
	int line_count; // struct count of lines
	struct node *next; // pointer to next node struct
	char *file_name; // pointer to string of file name
};

struct node *head = NULL; // head node of the linked list
struct node *current = NULL; // current node of the linked list

static void	app_error_fmt(const char *fmt, ...);
static int	do_count(char *input_files[], const int nfiles,
		    const bool char_flag, const bool word_flag,
		    const bool line_flag, const bool test_flag);
static void	print_counts(FILE *fp, struct counts *cnts, const char *name,
		    const bool char_flag, const bool word_flag,
		    const bool line_flag);

/*
 * Requires:
 *   The first argument, "fmt", is a printf-style format string, and all
 *   subsequent arguments must match the types of arguments indicated in the
 *   format string.
 *
 * Effects:
 *   Prints a formatted error message to stderr using the supplied
 *   format string and arguments.  The message is prepended with
 *   the string "ERROR: " and a newline is added at the end.
 */
static void
app_error_fmt(const char *fmt, ...)
{
	va_list ap;

	fprintf(stderr, "ERROR: ");
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
}

/*
 * Requires:
 *   The "fp" argument must be a valid FILE pointer.
 *   The "cnts" argument must not be NULL and must point to an allocated
 *   structure of type "struct counts".
 *   The "name" argument must not be NULL and must point to an allocated
 *   string.
 *
 * Effects:
 *   Prints the character, word, and line counts for a particular file
 *   as directed by the flags.
 */
static void
print_counts(FILE *fp, struct counts *cnts, const char *name,
    const bool char_flag, const bool word_flag, const bool line_flag)
{

	if (line_flag)
		fprintf(fp, "%8d", cnts->line_count);
	if (word_flag)
		fprintf(fp, "%8d", cnts->word_count);
	if (char_flag)
		fprintf(fp, "%8d", cnts->char_count);
	fprintf(fp, " %s\n", name);
}


/*
 * Requires:
 *   Nothing.
 *
 * Effects: 
 *   Helper function that creates a pointer to a count struct and a pointer to a 
 *   string to pass into the "print_counts" function. The "char/word/line_flag"
 *   arguments indicate which counts should be printed on a file-by-file basis.
 */
static void 
printLinkedList(const bool char_flag, const bool word_flag, const bool line_flag) {
   struct node *ptr = head;

   //start from the beginning
   while(ptr != NULL) {
	   struct counts count = { ptr->char_count, ptr->word_count, ptr->line_count };
	   struct counts *countPtr = &count;
	   char *namePtr = ptr->file_name;
	   print_counts(stdout, countPtr, namePtr, char_flag, word_flag, line_flag);
	   ptr = ptr->next;
   }
}

/*
 * Requires:
 *   Nothing.
 *
 * Effects: 
 *   Inserts new node at the start of the linked list.
 */
static void 
insertFirst(char *name, int num_chars, int num_lines, int num_words) {
   //create a link
   struct node *new_node = (struct node*) malloc(sizeof(struct node));
	
   
   new_node->char_count = num_chars;
   new_node->word_count = num_words;
   new_node->line_count = num_lines;
   new_node->file_name = name;

   //point it to old first node
   new_node->next = head;
	
   //point first to new first node
   head = new_node;
}

/*
 * Requires:
 *   Nothing.
 *
 * Effects: 
 *   Calculates and returns the length of the linked list.
 */
static int 
length() {
	int length = 0;
	struct node *current;
	
	for(current = head; current != NULL; current = current->next)
		length++;
   	
	return length;
}


/*
 * Requires:
 *   Nothing.
 *
 * Effects: 
 *   Sorts the nodes of the linked list in ASCIIbetical order.
 */
static void 
sort() {
   char *tempName;
   int i, j, k, temp_c_count, temp_l_count, temp_w_count;
   struct node *current, *next;
	
   int size = length();
   k = size;
	
   for (i = 0 ; i < size - 1 ; i++, k--) {
      current = head;
      next = head->next;
		
      for (j = 1 ; j < k ; j++) {   

         if (strcmp(current->file_name, next->file_name) > 0) {
            tempName = current->file_name;
            current->file_name = next->file_name;
            next->file_name = tempName;

            temp_c_count = current->char_count;
            current->char_count = next->char_count;
            next->char_count = temp_c_count;

			temp_l_count = current->line_count;
            current->line_count = next->line_count;
            next->line_count = temp_l_count;

			temp_w_count = current->word_count;
            current->word_count = next->word_count;
            next->word_count = temp_w_count;
         }
			
         current = current->next;
         next = next->next;
      }
   }   
}


/*
 * Requires:
 *   The "input_files" argument is an array of strings with "nfiles" valid
 *   strings.
 *
 * Effects: 
 *   Prints to stdout the counts for each file in input_files in
 *   alphabetical order followed by the total counts.  The
 *   "char/word/line_flag" arguments indicate which counts should be
 *   printed on a file-by-file basis.  The total count will include all
 *   three counts regardless of the flags.  An error message is printed
 *   to stderr for each file that cannot be opened.
 *
 *   Returns 0 if every file in input_files was successfully opened and
 *   processed.  If, however, an error occurred opening or processing any
 *   of the files, a small positive integer is returned.
 *
 *   The behavior is undefined when "test_flag" is true.
 */
static int
do_count(char *input_files[], const int nfiles, const bool char_flag,
    const bool word_flag, const bool line_flag, const bool test_flag)
{
	int c, error = 0;
	struct counts total_count = { 0, 0, 0 };

	// looping through the input_files array
	for (int f = 0; f < nfiles; f++) {
  		struct counts count = { 0, 0, 0 };
		
		 // Open source files in 'r' mode 
    	FILE *input_file = fopen(input_files[f], "r");


    	// Check if file opened successfully
    	if (input_file == NULL)
    	{
        	app_error_fmt("cannot open file \'%s\'", input_files[f]);
            error = 1;  // non-zero for error
		// continue to next file 
    	}

    	/*
     	 * Logic to count characters, words and lines.
     	 */
    	while ((c = fgetc(input_file)) != EOF)
    	{
        	if(char_flag){
				count.char_count++;
				total_count.char_count++;
			}

        	// Checks new line
			if(line_flag){
				if (c == '\n'){
					count.line_count++;
					total_count.line_count++;
				}
			}

        	// Checks words 
			if(word_flag){
				if (isspace(c) > 0){
					count.word_count++;
					total_count.word_count++;
				}
			}
    	}
    	// Increments words and lines for last word
    	if (count.char_count > 0)
    	{
        	count.line_count++;
			total_count.line_count++;
			count.word_count++;
			total_count.word_count++;
    	}

		// Adds new node to linked list
		insertFirst(input_files[f], count.char_count, count.line_count, count.word_count);

		if (!feof(input_file)) {
        	/*
             * If feof() returns FALSE, then the above while loop
             * didn't reach the end of file.  The EOF returned by
             * fgetc() instead meant that an error occurred while
             * reading from the input file. 
             */
            app_error_fmt("cannot read file \'%s\'", input_files[f]);
            error = 1;  /* non-zero for error */
		}
		fclose(input_file);
	}

	/* This is where the total counts are printed in ASCIIbetical order. */
	sort();
	printLinkedList(char_flag, word_flag, line_flag);
	struct counts *countPtr = &total_count;
	char *namePtr = "total";
	print_counts(stdout, countPtr, namePtr, true, true, true);
	
	if(test_flag)
		return (0);

	return (error);
}

/*
 * Requires:
 *   Nothing.
 *
 * Effects:
 *   Parses command line arguments and invokes do_count with the appropriate
 *   arguments based on the command line.
 */
int 
main(int argc, char **argv)
{
	int c;			// Option character
	extern int optind;	// Option index

	// Abort flag: Was there an error on the command line? 
	bool abort_flag = false;

	// Option flags: Were these options on the command line?
	bool char_flag = false;
	bool line_flag = false;
	bool test_flag = false;
	bool word_flag = false;

	// Process the command line arguments.
	while ((c = getopt(argc, argv, "cltw")) != -1) {
		switch (c) {
		case 'c':
			// Count characters.
			if (char_flag) {
				// A flag can only appear once.
				abort_flag = true;
			} else {
				char_flag = true;
			}
			break;
		case 'l':
			// Count lines.
			if (line_flag) {
				// A flag can only appear once.
				abort_flag = true;
			} else {
				line_flag = true;
			}
			break;
		case 't':
			// Enable test flag.
			if (test_flag) {
				// A flag can only appear once.
				abort_flag = true;
			} else {
				test_flag = true;
			}
			break;
		case 'w':
			// Count words.
			if (word_flag) {
				// A flag can only appear once.
				abort_flag = true;
			} else {
				word_flag = true;
			}
			break;
		case '?':
			// An error character was returned by getopt().
			abort_flag = true;
			break;
		}
	}
	if (abort_flag || optind == argc) {
		/*
		 * In this case, use fprintf instead of app_error_fmt because
		 * error messages describing how to run a program typically
		 * begin with "usage: " not "ERROR: ".
		 */
		fprintf(stderr,
		    "usage: %s [-c] [-l] [-t] [-w] <input filenames>\n",
		    argv[0]);
		return (1);	// Indicate an error.
	}

	// Process the input files, and return the result.
	return (do_count(&argv[optind], argc - optind, char_flag, word_flag,
	    line_flag, test_flag));
}
