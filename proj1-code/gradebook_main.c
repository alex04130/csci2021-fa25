#include <stdio.h>
#include <string.h>

#include "gradebook.h"

#define MAX_CMD_LEN 128

// This is in general similar to the list_main file seen in lab
// One big difference is the notion of switching between gradebooks in one
// run of the program.
// You have to create or load a gradebook from a file before you can do things
// like add, lookup, or write.
// Also, the user must explicitly clear the current gradebook before
// they can create or load in a new one.
int main(int argc, char **argv) {
    gradebook_t *book = NULL;

    // TODO Support a file name as a possible command line argument
    if (argc > 1) {
        if (strlen(argv[1]) > 4) {
            if (strcmp(".bin", argv[1] + strlen(argv[1]) - 4) == 0) {
                book = read_gradebook_from_binary(argv[1]);
                if (book != NULL) {
                    printf("Gradebook loaded from binary file\n");
                } else {
                    printf("Failed to read gradebook from binary file\n");
                }
            } else if (strcmp(".txt", argv[1] + strlen(argv[1]) - 4) == 0) {
                book = read_gradebook_from_text(argv[1]);
                if (book != NULL) {
                    printf("Gradebook loaded from text file\n");
                } else {
                    printf("Failed to read gradebook from text file\n");
                }
            } else {
                printf("Error: Unknown gradebook file extension\n");
            }
        } else {
            printf("Error: Unknown gradebook file extension\n");
        }
    }

    printf("CSCI 2021 Gradebook System\n");
    printf("Commands:\n");
    printf("  create <name>:          creates a new class with specified name\n");
    printf("  class:                  shows the name of the class\n");
    printf("  add <name> <score>:     adds a new score\n");
    printf("  lookup <name>:          searches for a score by student name\n");
    printf("  clear:                  resets current gradebook\n");
    printf("  print:                  shows all scores, sorted by student name\n");
    printf("  write_text:             saves all scores to text file\n");
    printf("  read_text <file_name>:  loads scores from text file\n");
    printf("  write_bin:              saves all scores to binary file\n");
    printf("  read_bin <file_name>:   loads scores from binary file\n");
    printf("  exit:                   exits the program\n");

    char cmd[MAX_CMD_LEN];
    while (1) {
        printf("gradebook> ");
        if (scanf("%s", cmd) == EOF) {
            printf("\n");
            if (book != NULL) {
                free_gradebook(book);
                book = NULL;
            }
            break;
        }

        if (strcmp("exit", cmd) == 0) {
            if (book != NULL) {
                free_gradebook(book);
                book = NULL;
            }
            break;
        }

        else if (strcmp("create", cmd) == 0) {
            scanf("%s", cmd);

            if (book != NULL) {
                printf("Error: You already have a gradebook.\n");
                printf("You can remove it with the \'clear\' command\n");
            } else {
                book = create_gradebook(cmd);
                if (book == NULL) {
                    printf("Gradebook creation failed\n");
                }
            }
        }

        // TODO: Add cases for remaining commands

        else if (strcmp("class", cmd) == 0) {
            if (book != NULL) {
                printf("%s\n", book->class_name);
            } else {
                printf("Error: You must create or load a gradebook first\n");
            }
        }

        else if (strcmp("add", cmd) == 0) {
            int score;
            scanf("%s %d", cmd, &score);
            if (book != NULL) {
                if (score > 0) {
                    if (add_score(book, cmd, score)) {
                        printf("Error: failed to add score to gradebook\n");
                    }
                } else {
                    printf("Error: You must enter a score in the valid range (0 <= score)\n");
                }
            } else {
                printf("Error: You must create or load a gradebook first\n");
            }
        } else if (strcmp("lookup", cmd) == 0) {
            scanf("%s", cmd);
            if (book != NULL) {
                int score = find_score(book, cmd);
                if (score == -1) {
                    printf("No score for '%s' found\n", cmd);
                } else {
                    printf("%s: %d\n", cmd, score);
                }
            } else {
                printf("Error: You must create or load a gradebook first\n");
            }
        } else if (strcmp("clear", cmd) == 0) {
            if (book != NULL) {
                free_gradebook(book);
                book = NULL;
            } else {
                printf("Error: No gradebook to clear\n");
            }
        } else if (strcmp("print", cmd) == 0) {
            if (book != NULL) {
                print_gradebook(book);
            } else {
                printf("Error: You must create or load a gradebook first\n");
            }
        } else if (strcmp("write_text", cmd) == 0) {
            if (book != NULL) {
                if (write_gradebook_to_text(book)) {
                    printf("Failed to write gradebook to text file\n");
                } else {
                    char file_name[MAX_NAME_LEN + strlen(".txt")];
                    strcpy(file_name, book->class_name);
                    strcat(file_name, ".txt");
                    printf("Gradebook successfully written to %s\n", file_name);
                }
            } else {
                printf("Error: You must create or load a gradebook first\n");
            }
        } else if (strcmp("read_text", cmd) == 0) {
            scanf("%s", cmd);
            if (book != NULL) {
                printf("Error: You must clear current gradebook first\n");
            } else {
                book = read_gradebook_from_text(cmd);
                if (book != NULL) {
                    printf("Gradebook loaded from text file\n");
                } else {
                    printf("Failed to read gradebook from text file\n");
                }
            }
        } else if (strcmp("write_bin", cmd) == 0) {
            if (book != NULL) {
                if (write_gradebook_to_binary(book)) {
                    printf("Failed to write gradebook to binary file\n");
                } else {
                    char file_name[MAX_NAME_LEN + strlen(".txt")];
                    strcpy(file_name, book->class_name);
                    strcat(file_name, ".bin");
                    printf("Gradebook successfully written to %s\n", file_name);
                }
            } else {
                printf("Error: You must create or load a gradebook first\n");
            }
        } else if (strcmp("read_bin", cmd) == 0) {
            scanf("%s", cmd);
            if (book != NULL) {
                printf("Error: You must clear current gradebook first\n");
            } else {
                book = read_gradebook_from_binary(cmd);
                if (book != NULL) {
                    printf("Gradebook loaded from binary file\n");
                } else {
                    printf("Failed to read gradebook from binary file\n");
                }
            }
        }

        else {
            printf("Unknown command %s\n", cmd);
        }
    }

    free_gradebook(book);
    return 0;
}
