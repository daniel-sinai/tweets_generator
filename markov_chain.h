#ifndef _MARKOV_CHAIN_H
#define _MARKOV_CHAIN_H

#include "linked_list.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool

/**
 * This struct is representing a node in the MarkovNode's counter_list.
 * - markov_node contains the word itself.
 * - frequency is the number of occurrences of the current word after the
 * previous word in the text.
 */
typedef struct NextNodeCounter {
    struct MarkovNode *markov_node;
    long frequency;
} NextNodeCounter;

/**
 * This struct is representing a node in the MarkovChain's database.
 * - data is the content of the word.
 * - counter_list is a list of NextNodeCounters with all the words that occur
 * after the current word in the text.
 * - counter_list_size is the current size of the counter_list.
 */
typedef struct MarkovNode {
    char *data;
    NextNodeCounter *counter_list;
    int counter_list_size;
} MarkovNode;

/**
 * This struct is representing a markov chain.
 * - database is the LinkedList that holds all the data of the words in the
 * input text file.
 */
typedef struct MarkovChain {
    LinkedList *database;
} MarkovChain;

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number maximal number to return (not including)
 * @return Random number
 */
int get_random_number (int max_number);

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain A pointer to a MarkovChain struct
 * @return MarkovNode of the word that was randomized
 */
MarkovNode *get_first_random_node (MarkovChain *markov_chain);

/**
 * This function summarize all the possible frequencies of a given
 * markov_node's counter_list.
 * @param markov_node A pointer to a MarkovNode struct
 * @return Sum of all frequencies of the counter_list's Nodes
 */
long sum_all_frequency (MarkovNode *markov_node);

/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr);

/**
 * This function randomizes the first_node until the word is not ending
 * with '.'
 * @param markov_chain A pointer to a MarkovChain struct
 * @return MarkovNode of the chosen word
 */
MarkovNode *random_until_valid (MarkovChain *markov_chain);

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain A pointer to a MarkovChain struct
 * @param first_node markov_node to start with,
 *                   if NULL- choose a random markov_node
 * @param max_length maximum length of chain to generate
 */
void generate_random_sequence (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length);

/**
 * This function frees a single Node from the database
 * @param node_ptr A pointer to a pointer of Node to be freed
 */
void free_single_node (Node **node_ptr);

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_markov_chain (MarkovChain **ptr_chain);

/**
 * This function checks if second_node is in first_node's counter list, and
 * updates the frequency if it is.
 * @param first_node_ptr A pointer to first_node
 * @param second_node_ptr A pointer to second_node
 * @return true if second_node is in first_node's counter_list and update was
 * successful
 */
bool attempt_find (MarkovNode **first_node_ptr, MarkovNode **second_node_ptr);

/**
 * This function handles the case when we create the counter_list of *first_ptr
 * on the first time
 * @param first_ptr A pointer to first_node's MarkovNode
 * @return true if allocate was successful, otherwise false
 */
bool first_counter_allocate (MarkovNode **first_ptr);

/**
 * This function handles the case when counter_list of first_node is already
 * exist and we want to increase it's size by one
 * @param first_ptr A pointer to first_node's MarkovNode
 * @return true if allocate was successful, otherwise false
 */
bool re_allocate_counter (MarkovNode **first_ptr);

/**
 * Add the second markov_node to the counter list of the first markov_node.
 * If already in list, update it's counter value.
 * @param first_node A pointer to first_node's MarkovNode struct
 * @param second_node A pointer to second_node's MarkovNode struct
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool
add_node_to_counter_list (MarkovNode *first_node, MarkovNode *second_node);

/**
* Check if data_ptr is in database. If so, return the markov_node wrapping it
 * in the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node *get_node_from_database (MarkovChain *markov_chain, char *data_ptr);

/**
* If data_ptr in markov_chain, return it's markov_node. Otherwise, create new
 * markov_node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database,
 * returns NULL in case of memory allocation failure.
 */
Node *add_to_database (MarkovChain *markov_chain, char *data_ptr);

#endif /* _MARKOV_CHAIN_H */