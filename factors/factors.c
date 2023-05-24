/*
 * COMP 321 Project 1: Factors
 *
 * This program computes the number of prime factors, and optionally the number
 * of distinct prime factors, for an unsigned integer input.
 * 
 * Tyra Cole (tjc6)
 */

#include <assert.h>
#include <limits.h>
#include <math.h>     // Included in case you use the standard math library.
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

static unsigned int	count_distinct_factors(unsigned int n);
static unsigned int	count_factors(unsigned int n);
static unsigned int	count_factors_recursive(unsigned int n);
static void		test_factors(void);


/*
 *  Requires:
 *   The inputs "n" and "i" must be greater than 1.
 *
 * Effects:
 *   Returns the smallest prime factor of "n".
 */
static unsigned int
smallest_prime_factor(unsigned int n, unsigned int i)
{
	assert(n > 1);
	assert(i > 1);
	
	// If "n" equals "i", it is prime; so, return "n".
	if (n == i)
	       	return n;
	
	/* 
	 * Otherwise, "n" is composite and we must check if "i" is 
	 * a factor of "n". If so, return "i".
	 */
	if (n % i == 0)
		return i;
	
	// We will next check if the int following "i" is a factor of "n".
	return smallest_prime_factor(n, i+1);
}
  
/* 
 * Requires:  
 *   The input "n" must be greater than 1.
 *
 * Effects: 
 *   Returns the number of factors of the input "n".
 */
static unsigned int
count_factors_recursive(unsigned int n)
{
       	unsigned int factor;
    
	assert(n > 1);
	
	factor = smallest_prime_factor(n, 2);
	
	// Base case: If n itself is prime, return a count of 1.
	if (factor == n)
		return 1;
	/*
	 * Recursive case: If "n" is composite, return the sum of counts of the
	 * prime factor of "n" and the composite factor of "n".
	 */
	else
		return count_factors_recursive(factor) + count_factors_recursive(n / factor);
}

/* 
 * Requires:  
 *   The input "n" must be greater than 1.
 *
 * Effects: 
 *   Returns the number of factors of the input "n".
 */
static unsigned int
count_factors(unsigned int n) 
{    
	assert(n > 1);
	
	 unsigned int count = 0;
	// Counts number of times 2 can be factored out of "n".
	while (n % 2 == 0){
		count++;
		n = n / 2;
	}
	
	// Counts all odd prime factors of "n".
	for (int i = 3; i < (int)(sqrt((double)n))+1; i += 2){
		while (n % i == 0){
			count++;
			n = n / i;
		}
	}
	
	// Counts the final value of "n" as a prime factor.
	if (n > 2)
		count++;
	
	return count;
}

/* 
 * Requires:  
 *   The input "n" must be greater than 1.
 *
 * Effects: 
 *   Returns the number of distinct factors of the input "n".
 */
static unsigned int
count_distinct_factors(unsigned int n)
{	
	assert(n > 1);
	
	unsigned int distinct_count = 0;
	unsigned int count = 0;
	// Factor out all 2s from "n".
	while (n % 2 == 0){
		count++;
		n = n /2;
	}
	// Count only one instance of prime factor 2.
	if (count > 0)
		distinct_count++;
	
	// Determine how many distinct odd prime factors "n" has.
	for (int i = 3; i < (int)(sqrt((double)n))+1; i += 2){
        	count = 0;
        	while (n % i == 0){
	        	count++;
	                n = n / i;
	        }
	        // Count only one instance of prime factor "i".
	        if (count > 0)
	        	distinct_count++;                                                                                      
	}
	
	// If "n" is greater than 2, it is itself a distinct prime factor.
	if (n > 2)
		distinct_count++;                                                                                                                 
	
	return distinct_count;
}

/* 
 * Requires:
 *   Nothing.
 *
 * Effects:
 *   Runs testing procedures.  Currently only prints messages.
 */
static void
test_factors(void)
{

	printf("Testing count_factors_recursive.\n");
	printf("n = 2. Expected result: 1. Actual result: %u.\n ", count_factors_recursive(2));
	printf("n = 5011. Expected result: 1. Actual result: %u.\n ", count_factors_recursive(5011));
	printf("n = 49. Expected result: 2. Actual result: %u.\n ", count_factors_recursive(49));
	printf("n = 128. Expected result: 7. Actual result: %u.\n ", count_factors_recursive(128));
	printf("n = 1155. Expected result: 4. Actual result: %u.\n ", count_factors_recursive(1155));
	printf("n = 280. Expected result: 5. Actual result: %u.\n ", count_factors_recursive(280));
		
	printf("Testing count_factors.\n");
	printf("n = 2. Expected result: 1. Actual result: %u.\n ", count_factors(2));
	printf("n = 112363. Expected result: 1. Actual result: %u.\n ", count_factors(112363));
	printf("n = 49. Expected result: 2. Actual result: %u.\n ", count_factors(49));
	printf("n = 128. Expected result: 7. Actual result: %u.\n ", count_factors(128));
	printf("n = 1155. Expected result: 4. Actual result: %u.\n ", count_factors(1155));
	printf("n = 280. Expected result: 5. Actual result: %u.\n ", count_factors(280));
	printf("n = UINT_MAX. Expected result: 5. Actual result: %u.\n ", count_factors(UINT_MAX));
	                                        
	
	printf("Testing count_distinct_factors.\n");
	printf("n = 128. Expected result: 1. Actual result: %u.\n ", count_distinct_factors(128));
	printf("n = 243. Expected result: 1. Actual result: %u.\n ", count_distinct_factors(243));
	printf("n = 280. Expected result: 3. Actual result: %u.\n ", count_distinct_factors(280));
	printf("n = UINT_MAX. Expected result: 5. Actual result: %u.\n ", count_distinct_factors(UINT_MAX));
	                                                 
}

/* 
 * Requires:
 *   Nothing.
 *
 * Effects:
 *   If the "-t" option is specified on the command line, then test code is
 *   executed and the program exits.
 *   
 *   Otherwise, requests a number from standard input.  If the "-r" option is
 *   specified on the command line, then prints the number of prime factors
 *   that the input number has, calculated using a recursive function.
 *   Otherwise, prints the number of prime factors that the input number has
 *   and the number of those factors that are distinct using iterative
 *   functions.
 *
 *   Upon completion, the program always returns 0.
 *
 *   If the number that is input is not between 2 and the largest unsigned
 *   integer, the output of the program is undefined, but it will not crash.
 */
int
main(int argc, char **argv)
{
	unsigned int n;
	int c;
	bool recursive = false;
	bool runtests = false;

	// Parse the command line.
	while ((c = getopt(argc, argv, "rt")) != -1) {
		switch (c) {
		case 'r':             // Use recursive version.
			recursive = true;
			break;
		case 't':             // Run test procedure and exit.
			runtests = true;
			break;
		default:
			break;
		}
	}

	// If "-t" is specified, run test procedure and exit program.
	if (runtests) {
		test_factors();
		return (0);
	}

	// Get input.
	printf("Enter number:\n");
	if (scanf("%u", &n) != 1) {
		fprintf(stderr, "Input error\n");
		return (1);
	}

	// Validate the input.
	if (n < 2) {
		fprintf(stderr, "Invalid input: %u\n", n);
		return (1);
	}

	// Print results.
	if (recursive) {
		// Use recursive version.
		printf("%u has %u prime factors.\n",
		    n, count_factors_recursive(n));
	} else {
		// Use iterative versions.
		printf("%u has %u prime factors, %u of them distinct.\n",
		    n, count_factors(n), count_distinct_factors(n));
	}

	// Report no errors.
	return (0);
}
