# word-count-dynamic

__OVERVIEW:__
The program takes a text file and will count the frequency of words of different lengths. This program is similar to word-count however, this version makes use of dynamic memory and linked lists to process the given text data. Different parameters affect the way results are displayed. This is a modified version of an assignment.

The program must be run with command line arguments "./word_count --infile <input_file> --sort" with sort being optional.

__Parameters:__

$ ./word_count --infile <input_file>
Prints out the number of words of each registered length. This list is sorted by increasing word lenghts and displays a list for each length of found words without repeats.

$ ./word_count --infile <input_file> --sort
Adding the sort parameter will print the same results, only now the list of words is printed in reversed alphanumeric order.

__Example Output:__

input.txt:
Tomorrow, and tomorrow, and tomorrow,
To the last syllable of recorded time;

When running:
$ ./word_count --infile <input_file>

Expected Output:
Count[02]=02; (words: ”To” and ”of”)
Count[03]=03; (words: ”and” and ”the”)
Count[04]=02; (words: ”last” and ”time”)
Count[08]=05; (words: ”Tomorrow”, ”recorded” and ”syllable”)

When running:
$ ./word_count --infile <input_file> --sort

Expected Output:
Count[02]=02; (words: ”of” and ”To”)
Count[03]=03; (words: ”the” and ”and”)
Count[04]=02; (words: ”time” and ”last”)
Count[08]=05; (words: ”syllable”, ”recorded” and ”tomorrow”)
