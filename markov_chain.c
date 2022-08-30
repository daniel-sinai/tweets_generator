#include "markov_chain.h"
#include <stdlib.h>
#include <string.h>

#define SUCCESS 0
#define FAILURE 1
#define EMPTY 0
#define DOT '.'
#define NEW_LINE "\n"
#define FIRST_SIZE 1

// See full documentation in header file
int get_random_number (int max_number)
{
  return rand () % max_number;
}

// See full documentation in header file
MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  int first_node = get_random_number (markov_chain->database->size);
  int i = 0;
  Node *first = markov_chain->database->first;
  while (i < first_node)
    {
      first = first->next;
      i++;
    }
  return first->data;
}

// See full documentation in header file
long sum_all_frequency (MarkovNode *markov_node)
{
  long result = 0;
  for (int i = 0; i < markov_node->counter_list_size; i++)
    {
      result += markov_node->counter_list[i].frequency;
    }
  return result;
}

// See full documentation in header file
MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)
{
  long cur_sum = sum_all_frequency (state_struct_ptr);
  long i = get_random_number (cur_sum);
  int index = 0;
  while (i >= 0)
    {
      if (i >= 0)
        {
          i -= state_struct_ptr->counter_list[index].frequency;
          index++;
        }
    }
  return state_struct_ptr->counter_list[index - 1].markov_node;
}

// See full documentation in header file
MarkovNode *random_until_valid (MarkovChain *markov_chain)
{
  MarkovNode *random = get_first_random_node (markov_chain);
  char *word = random->data;
  while (word[strlen (word) - 1] == DOT)
    {
      random = get_first_random_node (markov_chain);
      word = random->data;
    }
  return random;
}

// See full documentation in header file
void generate_random_sequence (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
  if (first_node == NULL)
    {
      first_node = random_until_valid (markov_chain);
    }
  char *word = first_node->data;
  printf ("%s", word);
  MarkovNode *current = first_node;
  int counter = 1;
  while (counter < max_length && word[strlen (word) - 1] != DOT)
    {
      counter++;
      current = get_next_random_node (current);
      word = current->data;
      printf (" %s", word);
    }
  printf (NEW_LINE);
}

// See full documentation in header file
void free_single_node (Node **node_ptr)
{
  Node *node = *node_ptr;
  free (node->data->data);
  node->data->data = NULL;
  free (node->data->counter_list);
  node->data->counter_list = NULL;
  free (node->data);
  node->data = NULL;
  free (node);
  node = NULL;
}

// See full documentation in header file
void free_markov_chain (MarkovChain **ptr_chain)
{
  MarkovChain *chain = *ptr_chain;
  Node *cur = chain->database->first, *next;
  while (cur != NULL)
    {
      next = cur->next;
      free_single_node (&cur);
      cur = next;
    }
  free (chain->database);
  chain->database = NULL;
  free (chain);
  chain = NULL;
}

// See full documentation in header file
bool attempt_find (MarkovNode **first_node_ptr, MarkovNode **second_node_ptr)
{
  MarkovNode *first_node = *first_node_ptr;
  MarkovNode *second_node = *second_node_ptr;
  int size = first_node->counter_list_size;
  for (int i = 0; i < size; i++)
    {
      char *first_word = first_node->counter_list[i].markov_node->data;
      char *second_word = second_node->data;
      if (strcmp (first_word, second_word) == SUCCESS)
        {
          first_node->counter_list[i].frequency++;
          return true;
        }
    }
  return false;
}

// See full documentation in header file
bool first_counter_allocate (MarkovNode **first_ptr)
{
  MarkovNode *first = *first_ptr;
  first->counter_list = malloc (sizeof (NextNodeCounter));
  if (first->counter_list == NULL)
    {
      return false;
    }
  first->counter_list_size = 1;
  return true;
}

// See full documentation in header file
bool re_allocate_counter (MarkovNode **first_ptr)
{
  MarkovNode *first = *first_ptr;
  first->counter_list_size++;
  NextNodeCounter *temp = realloc (first->counter_list,
                                   sizeof (NextNodeCounter)
                                   * first->counter_list_size);
  if (temp == NULL)
    {
      first->counter_list_size--;
      return false;
    }
  first->counter_list = temp;
  return true;
}

// See full documentation in header file
bool add_node_to_counter_list (MarkovNode *first_node, MarkovNode *second_node)
{
  char *word = first_node->data;
  if (word[strlen (word) - 1] == DOT)
    {
      first_node->counter_list = NULL;
      return true;
    }
  if (attempt_find (&first_node, &second_node) == true)
    {
      return true;
    }
  if (first_node->counter_list_size == EMPTY)
    {
      if (first_counter_allocate (&first_node) == false)
        {
          return false;
        }
    }
  else if (re_allocate_counter (&first_node) == false)
    {
      return false;
    }
  first_node->counter_list[first_node->counter_list_size
                           - 1].markov_node = second_node;
  first_node->counter_list[first_node->counter_list_size - 1].frequency = 1;
  return true;
}

// See full documentation in header file
Node *get_node_from_database (MarkovChain *markov_chain, char *data_ptr)
{
  Node *first = markov_chain->database->first;
  while (first != NULL)
    {
      char *word = first->data->data;
      if (word != NULL && strcmp (word, data_ptr) == SUCCESS)
        {
          return first;
        }
      first = first->next;
    }
  return NULL;
}

// See full documentation in header file
Node *add_to_database (MarkovChain *markov_chain, char *data_ptr)
{
  Node *new = get_node_from_database (markov_chain, data_ptr);
  if (new == NULL)
    {
      MarkovNode *new_markov = calloc (FIRST_SIZE, sizeof (MarkovNode));
      if (new_markov == NULL)
        {
          return NULL;
        }
      new_markov->data = malloc (sizeof (char) * strlen (data_ptr) + 1);
      if (new_markov->data == NULL)
        {
          free (new_markov);
          return NULL;
        }
      strcpy (new_markov->data, data_ptr);
      if (add (markov_chain->database, new_markov) == FAILURE)
        {
          free (new_markov->data);
          free (new_markov);
          return NULL;
        }
      return markov_chain->database->last;
    }
  return new;
}