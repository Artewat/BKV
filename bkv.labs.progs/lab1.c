#include <stdio.h>
#include <ctype.h>

int isvowel(int c);

int main(int argc, char *argv[])
{
    FILE *F;
    char filename[256];
    int c;
    int total = 0;          
    int lines = 0;          
    int digits = 0;         
    int punct = 0;          
    int spaces = 0;         
    int letters = 0;        
    int lower = 0;          
    int upper = 0;          
    int vowels = 0;         
    int consonants = 0;     

 //   printf("Введите имя файла: ");
  //  scanf("%255s", filename);

    F = fopen(argv[1], "r");
    if (F == NULL) {
        printf("Ошибка открытия файла %s\n", filename);
        return 1;
    }

    // Чтение файла посимвольно
    while ((c = fgetc(F)) != EOF) {
        total++;

        if (c == '\n') lines++;

        if (isdigit(c)) {
            digits++;
        } else if (ispunct(c)) {
            punct++;
        } else if (isspace(c)) {
            spaces++;
        } else if (isalpha(c)) {
            letters++;
            if (isvowel(c)) {
                vowels++;
            } else {
                consonants++;
            }
            if (islower(c)) {
                lower++;
            } else if (isupper(c)) {
                upper++;
            }
        }
    }

    fclose(F);

    printf("\nРезультаты анализа файла %s:\n", filename);
    printf("Общее количество символов: %d\n", total);
    printf("Количество строк: %d\n", lines + 1); 
    printf("Количество цифр: %d\n", digits);
    printf("Количество знаков препинания: %d\n", punct);
    printf("Количество пробельных символов: %d\n", spaces);
    printf("Количество букв: %d\n", letters);
    printf("  Строчные буквы: %d\n", lower);
    printf("  Прописные буквы: %d\n", upper);
    printf("  Гласные буквы: %d\n", vowels);
    printf("  Согласные буквы: %d\n", consonants);

    return 0;
}

int isvowel(int c)
{
    switch(tolower(c))
    {
        case 'a': 
        case 'e':
        case 'i': 
        case 'o': 
        case 'u':
        case 'y': return 1;
    }
    return 0;
}