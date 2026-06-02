#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100000
#define MAX_FILES 256

typedef struct word {
    int count;
    char *w;
} WORD;

WORD *words[MAX_WORDS] = {NULL};
int word_count = 0;

void to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int is_word_char(char c) {
    return isalnum(c);
}

int compare_alpha(const void *a, const void *b) {
    const WORD *wa = *(const WORD **)a;
    const WORD *wb = *(const WORD **)b;
    return strcmp(wa->w, wb->w);
}

int compare_freq(const void *a, const void *b) {
    const WORD *wa = *(const WORD **)a;
    const WORD *wb = *(const WORD **)b;
    if (wa->count == wb->count) {
        return strcmp(wa->w, wb->w);
    }
    return wb->count - wa->count;
}

void add_word(const char *word) {
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i]->w, word) == 0) {
            words[i]->count++;
            return;
        }
    }

    if (word_count < MAX_WORDS) {
        WORD *new_word = (WORD *)malloc(sizeof(WORD));
        new_word->w = strdup(word);
        new_word->count = 1;
        words[word_count++] = new_word;
    }
}

void process_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return;
    }

    char buffer[1024];
    char *token;
    const char *delimiters = " \t\n\r\f\v.,;:!?\"'()[]{}<>\\/|-_+=*&^%$#@~`";

    while (fgets(buffer, sizeof(buffer), file)) {
        token = strtok(buffer, delimiters);
        while (token != NULL) {
            if (isalpha(token[0])) {
                to_lower(token);
                add_word(token);
            }
            token = strtok(NULL, delimiters);
        }
    }

    fclose(file);
}

void write_results() {
    WORD *sorted_words[MAX_WORDS];
    memcpy(sorted_words, words, sizeof(words));

    qsort(sorted_words, word_count, sizeof(WORD *), compare_alpha);
    FILE *alpha_file = fopen("alpha.txt", "w");
    if (!alpha_file) {
        perror("Ошибка создания файла alpha.txt");
        return;
    }
    for (int i = 0; i < word_count; i++) {
        fprintf(alpha_file, "%s %d\n", sorted_words[i]->w, sorted_words[i]->count);
    }
    fclose(alpha_file);

    qsort(sorted_words, word_count, sizeof(WORD *), compare_freq);
    FILE *freq_file = fopen("frequency.txt", "w");
    if (!freq_file) {
        perror("Ошибка создания файла frequency.txt");
        return;
    }
    for (int i = 0; i < word_count; i++) {
        fprintf(freq_file, "%s %d\n", sorted_words[i]->w, sorted_words[i]->count);
    }
    fclose(freq_file);
}

void free_memory() {
    for (int i = 0; i < word_count; i++) {
        free(words[i]->w);
        free(words[i]);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s файл1 [файл2 ... файлN]\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc && i <= MAX_FILES; i++) {
        process_file(argv[i]);
    }

    write_results();
    free_memory();

    return 0;
}