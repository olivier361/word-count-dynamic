#word_count.c
#Written by Olivier Gervais-Gougeon


#include "word_count.h"


int 
main(int argc, char *argv[]) {

    //Prints out a message if not enough arguments are given:
    //i.e no --infile and/or no filename.
    if (argc < 3) {
        fprintf(stderr, "program: missing '--infile <filename> [--sort]'\n");
        exit(ARGUMENT_ERROR_CODE);
    }

    //Declaring variables related to command line parameters:
    char *arg_filename;
    int arg_sort = 0;

    //Loop through given arguments to find supported parameters:
    for (int i = 0; i<argc; i++) {
        //If --infile found, set corresponding variable:
        if (strcmp(argv[i], "--infile") == 0) {
            //If there is an argument after --infile, take it as filename:
            if (i<argc-1) {
                arg_filename = (char *)emalloc(sizeof(char)*strlen(argv[i+1])+1);
                strncpy(arg_filename, argv[i+1], strlen(argv[i+1])+1);
            }
            else {
                fprintf(stderr, "program: missing '--infile <filename> [--sort]'\n");
                exit(ARGUMENT_ERROR_CODE);
            }
        }
        //If --sort found, set its variable:
        else if (strcmp(argv[i], "--sort") == 0) {
            arg_sort = 1;
        }
    } //END OF ARGUMENT PARSING.

    //DEBUG: Print state of found arguments:
    //printf("--sort: %d, filename: %s\n", arg_sort, arg_filename);

    //Declaring variables for sorting portion:
    char *file_buffer; //Dynamically allocated in read_file()
    size_t file_len = (size_t)0;
    //int word_count = 0;
    //struct word_t *words; //Dynamically allocated as nodes are added
    word_len *len_count; //Dynamically allocated as nodes are added
    
    //Open file for reading:
    file_buffer = read_file(arg_filename, &file_len);

    //DEBUG: Prints out the file buffer.
    //printf("File buffer in main():\n%s\n", file_buffer);
    //printf("file_len = %zu\n\n", file_len);    

    //If file is empty, return a prompt:
    if (file_len <= 0) {
        printf("File '%s' doesn't contain any words. Stoping program\n", arg_filename);
        exit(0);
    }

    //Tokenize buffer to words and store in the sorted linked list structure:
    len_count = tokenize_and_sort(file_buffer, file_len);    

    //Print out the results depending on given parameters:
    print_histogram(len_count, arg_sort);
    
    //TODO: Make sure the following have been freed:
    /*
    - word strings inside of each word_t node.
    - all word_t nodes.
    - all word_len nodes.
    - temp location_len nodes.
    - temp location_t nodes.
    */
    free(arg_filename);
    free(file_buffer);

    free_len_list(len_count);

    //Indicates that program has completed successfully:
    exit(SUCCESS_CODE);
}

