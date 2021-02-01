#word_count.h
#Written by Olivier Gervais-Gougeon


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define SUCCESS_CODE 0
#define EXAMPLE_ERROR_CODE 1
#define ARGUMENT_ERROR_CODE 1
#define FILE_ERROR_CODE 2
#define MEMORY_ERROR_CODE 3

//Structures:

//Node definition for the words list:
typedef struct word_t word_t;
struct word_t {
    int len; //length of the word
    char *word; //the word string. Assumed to be already allocated

    word_t *next; //pointer to next element in word list (doubly-linked)
    word_t *prev; //pointer to previous element in word list
};

//Node definition for the list of lengths:
typedef struct word_len word_len;
struct word_len {
    int len; //length of words stored in this bucket
    int count; //count of words stored in this bucket

    word_t *w_list_head; //pointer to head of list of words of this length
    word_t *w_list_tail; //pointer to tail of list of words of this length

    word_len *next; //pointer to next word_len node in list (singly-linked)
};

//Used to store pointers to nodes before and after
//a certain location in the lengths list:
typedef struct location_len location_len;
struct location_len {
    word_len *before;
    word_len *after;
    word_len *found;
};

//Used to store pointer to nodes before and after
//a certain spot in the words list:
typedef struct location_t location_t;
struct location_t {
    word_t *before;
    word_t *after;
    word_t *found;
};

//TODO: Double check that all functions are declared.
//Function Declarations:
void *emalloc(size_t);
word_len *new_word_len(int);
word_t *new_word_t(int, char *);
//word_len *add_word_len(word_len *, word_len *); //Old version.
word_len *add_word_len(word_len *, word_len *, word_len *, word_len *);
//word_t *add_word_t(word_t *, word_t *); //Old version.
word_t *add_word_t(word_t *, word_t *, word_t *, word_t *, word_t *);
location_len *new_location_len();
location_t *new_location_t();
int contains_len(location_len *, word_len *, int); 
int contains_word(location_t *, word_t *, char *);
char* read_file(char *, size_t *);
word_len *tokenize_and_sort(char *, size_t);
void print_histogram(word_len *, int);
void print_text(word_t *, word_t*, int);
void print_text_helper(word_t*, int, int);
void print_word_len_nodes(word_len *, int);
void print_word_t_list(word_t *, int);
void free_len_list(word_len *);
void free_word_list(word_t *);

//Structure functions:

//Creates a new word_len node with given length:
word_len *new_word_len(int length) {
    word_len *newp;

    newp = (word_len *)emalloc(sizeof(word_len));

    newp->len = length;
    newp->count = 0;

    newp->w_list_head = NULL;
    newp->w_list_tail = NULL;
    
    return newp;
}

//TODO: Possibly remove this version. TBD.
/*
//Add a word_len node to the end of the given list:
word_len *add_word_len(word_len *listp, word_len *newp) {
    word_len *p;

    if (listp == NULL) {
        return newp;
    }
    for (p = listp; p->next != NULL; p = p->next)
        ;
    p->next = newp;
    return listp;
}
*/

//TODO: Determine which add method we want to keep.
/*
//Add a word_len node in the list, sorted by ascending len field:
word_len *add_word_len(word_len *listp, word_len *newp) {
    word_len *p;

    //if list is empty, add as first element:
    if (listp == NULL) {
        return newp;
    }
    //Traverse through list and insert before element of larger length:
    if (listp->next == NULL) {
        if (listp->len > newp->len) {
            newp->next = listp;
        }
    }
    for (p = listp; p->next->next != NULL; p = p->next) {
        if (p->len > newp->len) {
            
        } 
    }
    //If last element length is still smaller, add as last element:
    p->next = newp;
    return listp;
}
*/

//Adds the given new node (newp), between the given before/after node pointers:
//RETURNS: the head of the list.
word_len *add_word_len(word_len *listp, word_len *newp, word_len *before, word_len *after) {
    newp->next = after;
    
    if (before != NULL) {
        before->next = newp;
    }

    if(after == listp  || listp == NULL) {
        return newp;
    }
    return listp;
}

//Adds the given new node (newp), between the given before/after node pointers
//for a doubly-linked list:
//RETURNS: the head of the list
word_t *add_word_t(word_t *head, word_t *tail, word_t *newp, word_t *before, word_t *after) {
    newp->next = after;
    newp->prev = before;

    if (before != NULL) {
        before->next = newp;
    }
    if (after != NULL) {
        after->prev = newp;
    }
    
    if(after == head || head == NULL) {
        //if(head != NULL) {head->prev = newp;}
        return newp;
    } 
    if(before == tail) {
        //TODO: Not sure if this will update tail properly.
        tail = newp;
    }
    return head;
}

//Returns 1 if list of lengths contains node with given len.
//Otherwise gives a before and after node to the given location_len pointer:
//Parameters:
//result: pointer to a location_len node to store before/after
//pointers from given list.
//listp: pointer to list to analyse.
//length: lenght to compare nodes with.
int contains_len(location_len *result, word_len *listp, int length) {
    word_len *p;

    //if list is empty, return 0:
    if (listp == NULL) {
        result->before = NULL;
        result->after = NULL;
        result->found = NULL;
        return 0;
    }
    //printf("Made it past first condition in contains_len()\n");
    //case where list has one item:
    if (listp->next == NULL) {
        if (listp->len == length) {
            result->found = listp;
            return 1;
        }
        if (listp->len > length) {
            result->after = listp;
            result->before = NULL;
            result->found = NULL;
            return 0;
        }
        else {
            result->before = listp;
            result->after = NULL;
            result->found = NULL;
            return 0;
        }
    }
    //if list has 2+ items, find which nodes are before/after given length:
    for (p = listp; p->next != NULL; p = p->next) {
        if (p->len == length || p->next->len == length) {
            if (p->len == length) {result->found = p;}
            else {result->found = p->next;}
            return 1;
        }
        if (p->len > length && p == listp) {
            result->after = p;
            result->before = NULL;
            result->found = NULL;
            return 0;
        }
        if (p->len < length && p->next->len > length) {
            result->before = p;
            result->after = p->next;
            result->found = NULL;
            return 0;
        }
    }
    //case were given length is bigger than all nodes:
    result->before = p;
    result->after = NULL;
    result->found = NULL;
    return 0;
}

//Returns 1 if a node in the given listp has a word equal to str.
//Otherwise returns 0 with the result location_t node storing pointers
//to nodes that come before and after the given str.
int contains_word(location_t *result, word_t *listp, char *str) {
    word_t *p;
    
    //if list is empty, return 0:
    if (listp == NULL) {
        result->before = NULL;
        result->after = NULL;
        result->found = NULL;
        return 0;
    }
    //Case where list has one item:
    if (listp->next == NULL) {
        if (strcmp(listp->word, str) == 0) {
            result->found = listp;
            return 1;
        }
        if (strcmp(listp->word, str) > 0) {
            result->after = listp;
            result->before = NULL;
            result->found = NULL;
            return 0;
        }
        else {
            result->before = listp;
            result->after = NULL;
            result->found = NULL;
            return 0;
        }
    }
    //If list has 2+ items, find which nodes are before/after given length:
    for (p = listp; p->next != NULL; p = p->next) {
        if ( strcmp(p->word, str) == 0 || strcmp(p->next->word, str) == 0 ) {
            if ( strcmp(p->word, str) == 0 ) {result->found = p;}
            else {result->found = p->next;}
            return 1;
        }
        if ( strcmp(p->word, str) > 0 && p == listp ) {
            result->after = p;
            result->before = NULL;
            result->found = NULL;
            return 0;
        }
        if ( strcmp(p->word, str) < 0 && strcmp(p->next->word, str) > 0 ) {
            result->before = p;
            result->after = p->next;
            result->found = NULL;
            return 0;
        }
    }
    //Case where given length is bigger than all nodes:
    result->before = p;
    result->after = NULL;
    result->found = NULL;
    return 0;
}


//Creates a new word_t node with given length and word string:
word_t *new_word_t(int length, char *word) {
    word_t *newp;

    newp = (word_t *)emalloc(sizeof(word_t));

    newp->len = length;
    newp->word = word; //It is assumed the string has been malloc'ed already

    newp->next = NULL;
    newp->prev = NULL;
    
    return newp;
}

/*
//Add a word_t node to its sorted location in the list
//Given the head of the list and the new node:
word_t *add_word_t(word_t *head, word_t *newp) {
    //TODO Complete implementation

    return head;
}
*/

//Creates a new location_len node:
location_len *new_location_len() {
    location_len *newp;

    newp = (location_len *)emalloc(sizeof(location_len));

    newp->before = NULL;
    newp->after = NULL;
    newp->found = NULL;

    return newp;
}

//Creates a new location_t node:
location_t *new_location_t() {
    location_t *newp;

    newp = (location_t *)emalloc(sizeof(location_t));

    newp->before = NULL;
    newp->after = NULL;
    newp->found = NULL;

    return newp;
}

//MAIN FUNCTIONS:

/* Dynamically allocate memory of size_t n and return the 
 * pointer to the memory address
 *
 * exit with return code on allocation error 
 */
void *emalloc(size_t n) {
    void *p;

    p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "malloc of %zu bytes failed", n);
        exit(MEMORY_ERROR_CODE);
    }

    return p;
}

//Tries to read to file from the given filepath and allocates and
//writes it to a buffer which is returned. You must also provide
//a char_count parameter which will be updated to the file's length.
char* read_file(char *filepath, size_t *char_count) {
    FILE *file_ptr = fopen(filepath, "r");
    //size_t char_count = 0;
    long int size;
    char *buffer;

    if (file_ptr == NULL) {
        fprintf(stderr, "unable to open '%s' for reading\n", filepath);
        exit(FILE_ERROR_CODE);
    }
    else {
        //Determine file size:
        //Source: https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
        fseek(file_ptr, 0L, SEEK_END);
        size = ftell(file_ptr);
        rewind(file_ptr);
        
        //Allocate buffer & read file:
        buffer = (char *)emalloc(sizeof(char)*size+1);
        *char_count = fread(buffer, 1, size+1, file_ptr);

        //DEBUG: Prints the string read from file and num of char found:
        //printf("String from file:\n%s\n", buffer);
        //printf("char_count = %zu\n\n", *char_count);

        fclose(file_ptr);
        return buffer;
    }
}

//Tokenizes words from the given file buffer and allocates a string
//for each word. Then words are added to the linked list structure.
//RETURNS: The pointer to the start of the word_len list.
word_len *tokenize_and_sort(char *buffer, size_t file_len) {
    char *token;
    location_len *position = new_location_len();
    location_t *position_t = new_location_t();
    word_len *cur_len;
    int i = 0;


    //Declare our linked list of word_len nodes:
    word_len *len_list = NULL;

    //Get first word from buffer:
    token = strtok(buffer, " \t\r\n\v\f.,;()");

    //DEBUG:
    //printf("Found words:\n");

    while (token != NULL) {

        //DEBUG: Print contains:
        //printf("contains_len=%d\n", contains_len(position, len_list, strlen(token)));
        //Determine if word_len of that length is in the word_len list:
        //If Not, add a node of that length and set count to 1.
        //Else just bump up the count by 1.
        if( contains_len(position, len_list, strlen(token)) ) {
            //printf("List contains item!\n");
            position->found->count += 1;
            cur_len = position->found;            
        }
        else {
            //printf("item not in list\n");
            word_len *len_node = new_word_len(strlen(token));
            len_node->count = 1;
            //printf("len_node->count=%d\n", len_node->count);
            //printf("position->after=%p\n", position->after);
            len_list = add_word_len(len_list, len_node, position->before, position->after);
            cur_len = len_node;
        }

        //Determine if word is in the list of word_t under that word_len node:
        
        //If the same word is already there, do nothing more.
        //If it's not there, add it to the list in ascending sorted order:
        if ( !contains_word(position_t, cur_len->w_list_head, token) ) {
            //Create a word_t and add it
            //printf("Must add word %s to list\n", token);
            word_t *word_node = new_word_t(strlen(token), token);
            cur_len->w_list_head = add_word_t(cur_len->w_list_head, cur_len->w_list_tail, word_node, position_t->before, position_t->after);
        }

        //DEBUG:
        //printf("In Loop: token=%s, strlen(token)=%ld\n", token, strlen(token));        

        //Change token to next word and repeat:
        token = strtok(NULL, " \t\r\n\v\f.,;()");
        i++;
    }    

    //DEBUG: Print the word_len list nodes:
    //No sort:
    //print_word_len_nodes(len_list, 0);
    //Sorted:
    //print_word_len_nodes(len_list, 1); 

    free(position);
    free(position_t);

    //Return pointer to head of word_len linked list:
    if (i==0) {
        return NULL;
    }
    else {
        return len_list;
    }
} 

//Prints out the formated histogram of the given word_len list
//which contains word_t list inside:
//Parameteres:
//count: word_len list.
//sort: argument for sorting.
void print_histogram(word_len *count, int sort) {
    word_len *listp = count;
    if (listp != NULL) {
        printf("Count[%02d]=%02d;", listp->len, listp->count);
        print_text(listp->w_list_head, listp->w_list_tail, sort);
        print_histogram(listp->next, sort);
    }
    return;
}

//Prints out the word list portion of the histogram for the given
//list of words:
//Parameters:
//head: pointer to head of the word list.
//tail: pointer to tail of the word list.
//sort: argument for sorting.
void print_text(word_t *head, word_t *tail, int sort) {
    word_t *list;
    word_t *cursor = head;
    int size = 0;
    
    if (sort == 0) {
        list = head;
    }
    else {
        list = tail;
    }

    //Find the length/size of the word list:
    while (cursor != NULL) {
        size++;

        if (sort == 1) {
            list = cursor;
        }
        cursor = cursor->next;
    }

    printf(" (words:");

    print_text_helper(list, sort, size);
    /*
    if (size >= 2) {
        for(int i=0; i<size-2; i++) {
            printf(" \"%s\",", print_text_helper(list, sort));
        }
        printf(" \"%s\" and \"%s\")\n",list[size-2], list[size-1]);
    }
    else if (size == 1) {
        printf(" \"%s\")\n", list[0]);
    } 
    */  
}

void print_text_helper(word_t *cur, int sort, int size) {
    if (size == 0 || cur == NULL) {
        return;
    }
    if (size == 1) {
        printf(" \"%s\")\n", cur->word);    
    }
    else if (size == 2) {
        if (sort == 0) {
            printf(" \"%s\" and \"%s\")\n", cur->word, cur->next->word);
        }
        else {
            printf(" \"%s\" and \"%s\")\n", cur->word, cur->prev->word);
        }
    }
    else {
        if (sort == 0) {
            printf(" \"%s\",", cur->word);
            print_text_helper(cur->next, sort, size-1);
        }
        else {
            printf(" \"%s\",", cur->word);
            print_text_helper(cur->prev, sort, size-1);
        }
    }
}

//Prints out the fields of the nodes in the given word_len list:
//Mainly used for debug.
void print_word_len_nodes(word_len *listp, int sort) {
    if (listp == NULL) {
        printf("List is empty.\n");
    }
    else {
        printf("Word_len Node:\n");
        printf("len: %d\n", listp->len);
        printf("count: %d\n", listp->count);
        printf("words: ");
        print_word_t_list(listp->w_list_head, sort);
        print_word_len_nodes(listp->next, sort);
    }
}

//Prints out the fields of the nodes in the given word_t list:
//Mainly used for debug.
void print_word_t_list(word_t *listp, int sort) {
    if (listp == NULL) {
        printf("'END'\n");
    }
    else {
        printf("%s, ", listp->word);
        if (sort == 0) {
            print_word_t_list(listp->next, sort);
        }
        else {
            printf("I am here!");
            print_word_t_list(listp->prev, sort);
        }
    }
}

//Free the list of lengths:
void free_len_list(word_len *listp) {
    if (listp == NULL) {
        return;
    }
    free_len_list(listp->next);
    //TODO: Eventually free the words list here.
    free_word_list(listp->w_list_head);
    free(listp);
}

//Free the list of words:
void free_word_list(word_t *listp) {
    if (listp == NULL) {
        return;
    }
    free_word_list(listp->next);
    free(listp);
}

// COMMENTING OUT EXAMPLES BY AMANDA DASH
/*

 // *
 // * linkedlist.h
 // *
 // * This header file contains examples of how to:
 // *
 // * a) Dynamically allocate memory in a function
 // * b) Free memory used by a linked list
 // * c) Print the contents of a linked list 
 // * d) Apply a function to a linked list
 // *
 // * All the linked lists are set to void*.  You may replace them
 // * with your own struct typedefs.
 
 
// Example of how to free memory used by a double-linked list
void cleanup_list(void *list) {
    if(list == NULL)
       return;
    cleanup_list(list->left);
    free(list->word);
    cleanup_list(list->right);
    free(list);
}

// Example of how to print a linked list
void print_list(void* list)
{    
    if (list == NULL) {
        return;
    }
    
    print_list(list->left);
    fprintf(stderr, "%s ", list->word); 
    print_list(list->right);
}

// Example of how to apply a function to a each node in a linked list.  This is made so that different functions/arguments can be used.
//
// void *list : head of linked list to apply function
// void (*fn)(void *list, void*) : function that takes in two arguments.
// void *arg : second argument to pass to argument
void apply(void *list,
           void (*fn)(void *list, void *),
           void *arg)
{
    for ( ; list != NULL; list = list->next) {
        (*fn)(list, arg);
    }
}

*/
//END OF EXAMPLES
