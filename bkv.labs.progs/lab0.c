#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int* sieveOfEratosthenes(int n, int* primeCount) {
    if (n < 2) {
        *primeCount = 0;
        return NULL;
    }

    bool* is_prime = (bool*)malloc((n + 1) * sizeof(bool));
    if (!is_prime) {
        perror("Ошибка выделения памяти");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i <= n; i++) {
        is_prime[i] = true;
    }
    is_prime[0] = is_prime[1] = false;

    for (int p = 2; p * p <= n; p++) {
        if (is_prime[p]) {
            for (int i = p * p; i <= n; i += p) {
                is_prime[i] = false;
            }
        }
    }

    *primeCount = 0;
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            (*primeCount)++;
        }
    }

    int* primes = (int*)malloc(*primeCount * sizeof(int));
    if (!primes) {
        perror("Ошибка выделения памяти");
        free(is_prime);
        exit(EXIT_FAILURE);
    }

    int index = 0;
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            primes[index++] = i;
        }
    }

    free(is_prime);
    return primes;
}

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int primeCount;
    int* primes = sieveOfEratosthenes(n, &primeCount);

    printf("Простые числа до %d:\n", n);
    for (int i = 0; i < primeCount; i++) {
        printf("%d ", primes[i]);
    }
    printf("\n");

    free(primes);
    return 0;
}