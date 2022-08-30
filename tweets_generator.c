#include <stdlib.h>
#include <stdio.h>
#include "markov_chain.h"
#include <stdbool.h>
#include <string.h>
#include "linked_list.h"

#define FAILED_OPEN_FILE "Error: Failed to open file, make sure you use"\
" absolute path.\n"
#define WRONG_ARGS "Usage: The program require the following parameters\n"\
" 1) Seed number.\n "\
"2) Number of lines to generate.\n "\
"3) Absolute path to file.\n "\
"4) Optional - Number of lines to read from file. If no value is given"\
" read the entire file.\n"
#define ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate"\
            "new memory\n"
#define TWEET_FORMAT "Tweet %d: "
#define READ "r"
#define SPACE_BAR " "
#define END_LINE "\r\n"
#define VALID_ARGS_4 4
#define VALID_ARGS_5 5
#define FULL_READ -1
#define MAX_TWEET_SIZE 1001
#define SUCCESS 0
#define FAILURE 1
#define DECIMAL 10
#define MAX_WORDS_SENTENCE 20
#define DATABASE_SIZE 1

/**
 * This function manages the main operations
 * @param num_of_tweets Integer representing the desired number of tweets
 * to be printed
 * @param in_file A pointer to the input file
 * @param words_to_read Integer representing the number of words to read from
 * the input file
 * @return 0 if all operations and allocations was successful, otherwise 1
 */
int
run_main (long num_of_tweets, FILE *in_file, long words_to_read);

/**
 * This function generates all the tweets, according to num_of_tweets
 * @param markov_chain A pointer to the MarkovChain chain
 * @param num_of_tweets Integer representing the number of tweets to print
 */
void generate_all_sequences (MarkovChain *markov_chain, long num_of_tweets);

/**
 * This function try to open a file from a given file path
 * @param path String representing a file path
 * @return Pointer to the file, NULL upon open failed
 */
FILE *try_open_file (char path[]);

/**
 * This function reads data from the file fp and insert it to the database
 * @param fp Input file
 * @param words_to_read Number of words to read from the input file
 * @param markov_chain A pointer to MarkovChain struct
 * @return 0 upon success, otherwise 1
 */
int fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain);

int main (int argc, char *argv[])
{
  if (argc > VALID_ARGS_5 || argc < VALID_ARGS_4)
    {
      printf (WRONG_ARGS);
      return EXIT_FAILURE;
    }
  long words_to_read = FULL_READ;
  if (argc == VALID_ARGS_5)
    {
      words_to_read = strtol (argv[4], NULL, DECIMAL);
    }
  long seed = strtol (argv[1], NULL, DECIMAL);
  long num_of_tweets = strtol (argv[2], NULL, DECIMAL);
  srand (seed);
  FILE *in_file = try_open_file (argv[3]);
  if (in_file == NULL)
    {
      return EXIT_FAILURE;
    }
  if (run_main (num_of_tweets, in_file, words_to_read) == FAILURE)
    {
      printf (ALLOCATION_ERROR_MASSAGE);
      fclose(in_file);
      return EXIT_FAILURE;
    }
  fclose (in_file);
  return EXIT_SUCCESS;
}

// See full documentation above the main function
int run_main (long num_of_tweets, FILE* in_file, long words_to_read)
{
  MarkovChain *markov_chain = malloc (sizeof (MarkovChain));
  if (markov_chain == NULL)
    {
      return FAILURE;
    }
  markov_chain->database = calloc (DATABASE_SIZE, sizeof (LinkedList));
  if (markov_chain->database == NULL)
    {
      free (markov_chain);
      return FAILURE;
    }
  markov_chain->database->size = 0;
  if (fill_database (in_file, words_to_read, markov_chain) == FAILURE)
    {
      free_markov_chain (&markov_chain);
      return FAILURE;
    }
  generate_all_sequences (markov_chain, num_of_tweets);
  free_markov_chain (&markov_chain);
  return SUCCESS;
}

// See full documentation above the main function
void generate_all_sequences (MarkovChain *markov_chain, long num_of_tweets)
{
  int counter = 0;
  while (counter < num_of_tweets)
    {
      printf (TWEET_FORMAT, counter + 1);
      generate_random_sequence (markov_chain, NULL, MAX_WORDS_SENTENCE);
      counter++;
    }
}

// See full documentation above the main function
FILE *try_open_file (char path[])
{
  FILE *in_file = fopen (path, READ);
  if (in_file == NULL)
    {
      printf (FAILED_OPEN_FILE);
    }
  return in_file;
}

// See full documentation above the main function
int fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
  char buffer[MAX_TWEET_SIZE];
  MarkovNode *prev = NULL, *cur = NULL;
  int counter = 0;
  while (fgets (buffer, MAX_TWEET_SIZE, fp) != NULL)
    {
      char *word = strtok (buffer, SPACE_BAR);
      while (word != NULL)
        {
          counter++;
          word[strcspn (word, END_LINE)] = 0;
          cur = add_to_database (markov_chain, word)->data;
          if (cur == NULL)
            {
              return FAILURE;
            }
          if (prev != NULL)
            {
              if (add_node_to_counter_list (prev, cur) == false)
                {
                  return FAILURE;
                }
            }
          prev = cur;
          if (words_to_read != FULL_READ && counter == words_to_read)
            {
              return SUCCESS;
            }
          word = strtok (NULL, SPACE_BAR);
        }
    }
  return SUCCESS;
}