#include <stdio.h>
#include <stdlib.h>

void citire(int n, int *v) {
    v = (int *)realloc(v, 5);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &v[i]);
    }
    // for (int i = 0; i < n; ++i) {
        // printf("%d ", v[i]);
    // }
    
}

int main() {
    int *v;
    v = (int *)malloc(1 * sizeof(int));
    citire(5, v);
    for (int i = 0; i < 5; ++i) {
        printf("%d ", v[i]);
    }
}