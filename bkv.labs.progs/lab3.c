#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <iconv.h>

#define MAX_ENTRIES 1000
#define MAX_LINE 256

typedef struct BibEntry {
    char author[100];
    char title[100];
    char publisher[100];
    struct BibEntry *left;
    struct BibEntry *right;
} BibEntry;

BibEntry *root = NULL;

int cp1251_strncmp(const char *s1, const char *s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (s1[i] != s2[i]) {
            return (unsigned char)s1[i] - (unsigned char)s2[i];
        }
        if (s1[i] == '\0') {
            return 0;
        }
    }
    return 0;
}

BibEntry* create_entry(const char *author, const char *title, const char *publisher) {
    BibEntry *entry = (BibEntry*)malloc(sizeof(BibEntry));
    strncpy(entry->author, author, sizeof(entry->author)-1);
    strncpy(entry->title, title, sizeof(entry->title)-1);
    strncpy(entry->publisher, publisher, sizeof(entry->publisher)-1);
    entry->left = entry->right = NULL;
    return entry;
}

void insert_entry(BibEntry **node, BibEntry *new_entry) {
    if (*node == NULL) {
        *node = new_entry;
        return;
    }

    int cmp = strcmp(new_entry->author, (*node)->author);
    if (cmp < 0) {
        insert_entry(&(*node)->left, new_entry);
    } else if (cmp > 0) {
        insert_entry(&(*node)->right, new_entry);
    } else {
        cmp = strcmp(new_entry->title, (*node)->title);
        if (cmp < 0) {
            insert_entry(&(*node)->left, new_entry);
        } else {
            insert_entry(&(*node)->right, new_entry);
        }
    }
}

void parse_bib_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE];
    char current_author[100] = "";
    char current_title[100] = "";
    char current_publisher[100] = "";

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "@book")) {
            current_author[0] = '\0';
            current_title[0] = '\0';
            current_publisher[0] = '\0';
        } else if (strstr(line, "author")) {
            sscanf(line, " author = {%99[^}]}", current_author);
        } else if (strstr(line, "title")) {
            sscanf(line, " title = {%99[^}]}", current_title);
        } else if (strstr(line, "publisher")) {
            sscanf(line, " publisher = {%99[^}]}", current_publisher);
        } else if (strstr(line, "}") && current_author[0] && current_title[0]) {
            BibEntry *entry = create_entry(current_author, current_title, current_publisher);
            insert_entry(&root, entry);
        }
    }

    fclose(file);
}

void inorder_traversal(BibEntry *node, FILE *output) {
    if (node == NULL) return;
    
    inorder_traversal(node->left, output);
    fprintf(output, "Author: %s\nTitle: %s\nPublisher: %s\n\n", 
            node->author, node->title, node->publisher);
    inorder_traversal(node->right, output);
}

BibEntry* search_by_prefix(BibEntry *node, const char *prefix, int is_russian) {
    if (node == NULL) return NULL;

    int cmp;
    if (is_russian) {
        cmp = cp1251_strncmp(prefix, node->author, 5);
    } else {
        cmp = strncmp(prefix, node->author, 5);
    }

    if (cmp == 0) {
        return node;
    } else if (cmp < 0) {
        return search_by_prefix(node->left, prefix, is_russian);
    } else {
        return search_by_prefix(node->right, prefix, is_russian);
    }
}

void free_tree(BibEntry *node) {
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s file1.bib [file2.bib ...]\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        parse_bib_file(argv[i]);
    }

    FILE *output = fopen("sorted_books.txt", "w");
    if (!output) {
        perror("Error creating output file");
        free_tree(root);
        return 1;
    }

    inorder_traversal(root, output);
    
    fclose(output);

    const char *search_prefix = "Push";
    BibEntry *found = search_by_prefix(root, search_prefix, 0);
    if (found) {
        printf("Found entry:\nAuthor: %s\nTitle: %s\n", found->author, found->title);
    }

    free_tree(root);
    return 0;
}