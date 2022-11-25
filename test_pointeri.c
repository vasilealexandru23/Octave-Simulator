#include <stdio.h>
#include <stdlib.h>

void swap_p(int **pa, int **pb) {
    int *aux; aux = *pa;
    *pa = *pb;
    *pb = aux;
}

void swap_ptrs_matrice(int ***a, int ***b) {
    int **aux = *a;
    *a = *b;
    *b = aux;
}

void transpunere(int ***a) {
    int **aux; aux = *a;
    *a = **a;
    **a = aux;
}

int main() {
    int **a, **b;
    int n = 2, m = 3;
    a = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i) {
        a[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j) {
            scanf("%d", &a[i][j]);
        }
    }
    b = (int **)malloc(m * sizeof(int *));
    // for (int i = 0; i < m; ++i) {
        // b[i] = (int *)malloc(m * sizeof(int));
        // for (int j = 0; j < m; ++j) {
            // scanf("%d", &b[i][j]);
        // }
    // }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    // for (int i = 0; i < m; ++i) {
        // for (int j = 0; j < m; ++j) {
            // printf("%d ", b[i][j]);
        // }
        // printf("\n");
    // }
    transpunere(&a);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    return 0;
}