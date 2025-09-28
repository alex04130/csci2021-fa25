#include "gradebook.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

gradebook_t *create_gradebook(const char *class_name) {
    gradebook_t *new_book = malloc(sizeof(gradebook_t));
    if (new_book == NULL) {
        return NULL;
    }

    strcpy(new_book->class_name, class_name);
    new_book->root = NULL;
    return new_book;
}

const char *get_gradebook_name(const gradebook_t *book) {
    // TODO: Not yet implemented
    return book->class_name;
}

node_t *new_node(const char *name, int score) {
    node_t *ptr = malloc(sizeof(node_t));
    ptr->left = NULL;
    ptr->right = NULL;
    ptr->score = score;
    strcpy(ptr->name, name);
    return ptr;
}

int add_score(gradebook_t *book, const char *name, int score) {
    // TODO: Not yet implemented
    if (score < 0) {
        return -1;
    }
    if (book->root == NULL) {
        book->root = new_node(name, score);
        return 0;
    }
    node_t *ptr = book->root;
    while (1) {
        if (strcmp(ptr->name, name) < 0) {
            if (ptr->left == NULL) {
                ptr->left = new_node(name, score);
                return 0;
            }
            ptr = ptr->left;
        } else {
            if (ptr->right == NULL) {
                ptr->right = new_node(name, score);
                return 0;
            }
            ptr = ptr->right;
        }
    }
}

int find_score(const gradebook_t *book, const char *name) {
    node_t *ptr = book->root;
    while (ptr != NULL) {
        if (strcmp(ptr->name, name) < 0) {
            ptr = ptr->left;
        } else if (strcmp(ptr->name, name) > 0) {
            ptr = ptr->right;
        } else {
            return ptr->score;
        }
    }
    return -1;
}

void node_traversal(int mode, const node_t *thisNode, void *parameter,
                    void *func(const node_t *, void *parameter)) {
    if (mode < 0) {
        func(thisNode, parameter);
    }
    if (thisNode->left != NULL) {
        node_traversal(mode, thisNode->left, parameter, func);
    }
    if (mode == 0) {
        func(thisNode, parameter);
    }
    if (thisNode->right != NULL) {
        node_traversal(mode, thisNode->right, parameter, func);
    }
    if (mode > 0) {
        func(thisNode, parameter);
    }
}

void print_node_text(const node_t *thisNode, void *file) {
    fprintf(file, "%s: %d\n", thisNode->name, thisNode->score);
}

void print_gradebook(const gradebook_t *book) {
    // TODO: Not yet implemented
    if (book == NULL) {
        printf("Error: You must create or load a gradebook first\n");
        return;
    }
    if (book->root) {
        node_traversal(0, book->root, stdout, print_node_text);
    }
}

void free_node(const node_t *thisNode, void *file) {
    free(thisNode);
}

void free_gradebook(gradebook_t *book) {
    // TODO: Not yet implemented
    node_traversal(1, book->root, stdout, free_node);
    free(book);
}

// Helper function to allow recursion for writing out tree
int write_gradebook_to_text_aux(const node_t *current, FILE *f) {
    if (current == NULL) {
        return 0;
    }

    // Each line is formatted as "<name> <score>"
    fprintf(f, "%s %d\n", current->name, current->score);

    if (current->left != NULL) {
        if (write_gradebook_to_text_aux(current->left, f) != 0) {
            return -1;
        }
    }

    if (current->right != NULL) {
        if (write_gradebook_to_text_aux(current->right, f) != 0) {
            return -1;
        }
    }

    return 0;
}

int write_gradebook_to_text(const gradebook_t *book) {
    char file_name[MAX_NAME_LEN + strlen(".txt")];
    strcpy(file_name, book->class_name);
    strcat(file_name, ".txt");

    FILE *f = fopen(file_name, "w");
    if (f == NULL) {
        return -1;
    }

    int result = write_gradebook_to_text_aux(book->root, f);
    fclose(f);
    return result;
}

gradebook_t *read_gradebook_from_text(const char *file_name) {
    // TODO: Not yet implemented
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        return NULL;
    }
    gradebook_t *gradebook = malloc(sizeof(gradebook_t));
    char name[MAX_NAME_LEN];
    int grade;
    while (fscanf(file, "%s %d", name, &grade) != EOF) {
        add_score(gradebook, name, grade);
    }
    fclose(file);
    return gradebook;
}

void print_node_bin(const node_t *thisNode, void *file) {
    int nameLen = strlen(thisNode->name);
    fwrite(&nameLen, sizeof(int), 1, file);
    fwrite(thisNode->name, sizeof(char), nameLen, file);
    fwrite(&thisNode->score, sizeof(int), 1, file);
}

int write_gradebook_to_binary(const gradebook_t *book) {
    // TODO: Not yet implemented
    char filename[MAX_NAME_LEN + 5];
    strcpy(filename, book->class_name);
    FILE *file = fopen(strcat(filename, ".bin"), "bw");
    if (file == NULL) {
        return -1;
    }
    node_traversal(0, book->root, file, print_node_bin);
    fclose(file);
    printf("Gradebook successfully written to %s", filename);
    return 0;
}

gradebook_t *read_gradebook_from_binary(const char *file_name) {
    // TODO: Not yet implemented
    FILE *file = fopen(file_name, "rb");
    if (file == NULL) {
        return NULL;
    }
    gradebook_t *gradebook = malloc(sizeof(gradebook_t));
    char name[MAX_NAME_LEN + 1];
    int grade;
    int nameLen;
    while (fread(&nameLen, sizeof(int), 1, file) != EOF) {
        fread(&name, sizeof(char), nameLen, file);
        name[nameLen] = 0;
        fread(&grade, sizeof(int), 1, file);
        add_score(gradebook, name, grade);
    }
    fclose(file);
    return gradebook;
}
