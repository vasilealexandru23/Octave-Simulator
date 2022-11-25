#include <stdio.h>
#include <stdlib.h>

const int MOD = (1e4) + 7;

int ***matrix_collection; //mt[i][j][k] i-a cate matrice avem in memorie
                          //j-nr de linii, iar k = nr coloane	

void swap_ptrs_matrix(int ***a, int ***b) {
    int **aux = *a;
    *a = *b;
    *b = aux;
}

int main(void)
{
    int size_collection = 0;
    int *lines, *colons, *sum;
    while (1) {
        char c;
        scanf("%c", &c);
        if (c == 'L') {
            if (size_collection == 0) {
                matrix_collection = (int ***)malloc(++size_collection * (sizeof(int **)));
                lines = (int *)malloc(size_collection * sizeof(int));
                colons = (int *)malloc(size_collection * sizeof(int));
                sum = (int *)malloc(size_collection * sizeof(int));
            } else {
                matrix_collection = (int ***)realloc(matrix_collection, ++size_collection);
                lines = (int *)realloc(lines, size_collection);
                colons = (int *)realloc(colons, size_collection);
                sum = (int *)realloc(sum, size_collection);
            }
            int m, n, curr_sum = 0;
            scanf("%d%d", &m, &n);
            matrix_collection[size_collection - 1] = (int **)malloc(m * sizeof(int *));
            for (int i = 0; i < m; ++i) {
                matrix_collection[size_collection - 1][i] = (int *)malloc(n * sizeof(int));
                for (int j = 0; j < n; ++j) {
                    scanf("%d", &matrix_collection[size_collection - 1][i][j]);
                }
            }
            lines[size_collection - 1] = m;
            colons[size_collection - 1] = n;
        } else if (c == 'D') {
            int matrix_index;
            scanf("%d", &matrix_index);
            if (matrix_index < 0 || matrix_index >= size_collection) {
                printf("No matrix with the given index\n");
            } else {
                printf("%d %d\n", lines[matrix_index],
                       colons[matrix_index]);
            }
        } else if (c == 'P') {
            int matrix_index;
            scanf("%d", &matrix_index);
            if (matrix_index < 0 || matrix_index >= size_collection) {
                printf("No matrix with the given index\n");
            } else {
                int nr_lines = lines[matrix_index];
                int nr_col = colons[matrix_index];
                for (int i = 0; i < nr_lines; ++i) {
                    for (int j = 0; j < nr_col; ++j) {
                        printf("%d ", matrix_collection[matrix_index][i][j]);
                    }
                    printf("\n");
                }
            }
        } else if (c == 'C') {
            int matrix_index;
            scanf("%d", &matrix_index);
            int new_m, new_n;
            scanf("%d%d", &new_m, &new_n);
            if (matrix_index < 0 || matrix_index >= size_collection) {
                printf("No matrix with the given index\n");
            } else {
                matrix_collection[matrix_index] = (int **)realloc(matrix_collection[matrix_index], new_m);
                for (int i = 0; i < new_m; ++i) {

                }
            }
        } else if(c == 'M') {
            int matrix_1, matrix_2;
            scanf("%d%d", &matrix_1, &matrix_2);
            if (matrix_1 < 0 || matrix_1 >= size_collection || matrix_2 < 0 || matrix_2 >= size_collection) {
                printf("No matrix with the given index\n");
            } else if (colons[matrix_1] != lines[matrix_2]) {
                printf("Cannot perform matrix multiplication");
            } else {
                //alocare de noua matrice
                matrix_collection = (int ***)realloc(matrix_collection, ++size_collection);
                    matrix_collection[size_collection - 1] = (int **)malloc(lines[matrix_1] * sizeof(int *));
                    for (int i = 0; i < lines[matrix_1]; ++i) {
                        matrix_collection[size_collection - 1][i] = (int *)malloc(colons[matrix_2] * sizeof(int));
                        for (int j = 0; j < colons[matrix_2]; ++j) {
                            matrix_collection[size_collection - 1][i][j] = 0;
                        for (int k = 0; k < colons[matrix_1]; ++k) {
                                matrix_collection[size_collection - 1][i][j] += matrix_collection[matrix_1][i][k] * 
                                                                                matrix_collection[matrix_2][k][j];
                        }	
                        }
                    }
                lines[size_collection - 1] = lines[matrix_1];
                colons[size_collection - 1] = colons[matrix_2];
            }
        } else if (c == 'Q') {
            for (int i = 0; i < size_collection; ++i) {
                for (int j = 0; j < lines[i]; ++j) {
                    free(matrix_collection[i][j]);
                }
                free(matrix_collection[i]);
            }
            //free(matrix_collection);
            //free(lines);
            //free(colons);
            return 0;
        } else if (c == 'T') {
            int matrix_index;
            scanf("%d", &matrix_index);
            if (matrix_index < 0 || matrix_index >= size_collection) {
                printf("No matrix with the given index\n");
            } else {
                for (int i = 0; i < lines[matrix_index]; ++i) {
                    for (int j = 0; j < colons[matrix_index]; ++j) {
                        //swap(matrix_collection[matrix_index][i][j], matrix_collection[matrix_index][j][i]);
                    }
                }
            }
        } else if (c == 'R') {
            int matrix, power;
            scanf("%d%d", &matrix, &power);
            if (matrix < 0 || matrix >= size_collection) {
                printf("No matrix with the given index\n");
            } else if (power < 0) {
                printf("Power should be positive");
            } else if (lines[matrix] != colons[matrix]) {
                printf("Cannot perform matrix multiplication");
            } else {
                //get_power()
            }
        } else if (c == 'F') {
            int matrix_index;
            scanf("%d", &matrix_index);	
            if (matrix_index < 0 || matrix_index >= size_collection) {
                printf("No matrix with the given index\n");
            } else {
                // update la linii si coloane
                // trebuie sa schimb mai bine pointerii intre ei decat sa schimb valorile din ei
                for (int i = matrix_index + 1; i < size_collection; ++i) {
                    int lines_prev = lines[i - 1];
                    int lines_next = lines[i];
                    int col_prev = colons[i - 1];
                    int col_next = colons[i];
                    swap_ptrs_matrix(&matrix_collection[i - 1], &matrix_collection[i]);
                    lines[i - 1] = lines_next;
                    lines[i] = lines_prev;
                    colons[i - 1] = col_next;
                    colons[i] = col_prev;
                }
                //matrix_collection = (int ***)realloc(matrix_collection, --size_collection);
                for (int i = 0 ; i < lines[size_collection - 1]; ++i) {
                    free(matrix_collection[size_collection - 1][i]);
                }
                free(matrix_collection[size_collection - 1]);
                --size_collection;
                //sterg ultima matrice
            }
        } else if (c == 'O') {
            for (int i = 0; i < size_collection - 1; ++i) {
                for (int j = i + 1; j < size_collection; ++j) {
                    if (sum[i] > sum[j]) {
                        swap_ptrs_matrix(&matrix_collection[i], &matrix_collection[j]);
                    }
                }
            }
        }
        getchar();
    }
}
