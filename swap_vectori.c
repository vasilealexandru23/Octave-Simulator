#include <stdio.h>
#include <stdlib.h>

void swap_vectors(int **a, int **b) {
    int *aux; aux = *a;
    *a = *b;
    *b = aux;
}

int main() {
    int *v, n = 5, m = 10, *w;
    v = (int *)malloc(n * sizeof(int));
    w = (int *)malloc(m * sizeof(int));
    for (int i = 0; i < n; ++i) scanf("%d", &v[i]);
    for (int i = 0; i < m; ++i) scanf("%d", &w[i]);
    for (int i = 0; i < n; ++i) printf("%d ", v[i]);
    printf("\n");
    for (int i = 0; i < m; ++i) printf("%d ", w[i]);
    printf("\n");
    swap_vectors(&v, &w);
    v = realloc(v, m);
    w = realloc(v, n);
    for (int i = 0; i < m; ++i) printf("%d ", v[i]);
    printf("\n");
    for (int i = 0; i < n; ++i) printf("%d ", w[i]);
    return 0;
}