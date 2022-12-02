#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const int MOD = (1e4) + 7;

// Utilities : functions for swapping different variables, erasing matrixes;
void swap_ptrs_matrix(int ***matrix_1, int ***matrix_2) {
    int **aux = *matrix_1;
    *matrix_1 = *matrix_2;
    *matrix_2 = aux;
}

void swap_values(int *value_1, int *value_2) {
    int aux = *value_1;
    *value_1 = *value_2;
    *value_2 = aux;
}

void free_matrix(int **matrix, int size) {
    for (int i = 0; i < size; ++i) free(matrix[i]);
    free(matrix);
}

int sum_of_elements(int **matrix1, int lines, int colons) {
    int sum = 0;
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < colons; ++j) {
            sum += (((matrix1[i][j] % MOD) + MOD) % MOD);
            sum = (((sum % MOD) + MOD) % MOD);
        }
    }
    return sum % MOD;
}

int *alloc_array(int size)
{
    int *array = (int *)malloc(size * sizeof(int));
    if (!array) {
        printf("Invalid allocation.\n");
        exit(-1);
    }
    return array;
}

int **alloc_matrix(int lines, int colons)
{
    int **matrix = (int **)malloc(lines * sizeof(int *));
    if (!matrix) {
        printf("Invalid allocation.\n");
        exit(-1);
    }
    for (int i = 0; i < lines; ++i) {
        matrix[i] = (int *)malloc(colons * sizeof(int));
        if (!matrix[i]) {
            printf("Invalid allocation.\n");
            exit(-1);
        }
    }
    return matrix;
}

void load_matrix(int ****matrix_collection, int *size_collection, int **lines,
                 int **colons) {
    (*size_collection)++;
    *matrix_collection = (int ***)realloc((*matrix_collection),
                                          (*size_collection) * sizeof(int **));
    *lines = (int *)realloc((*lines), (*size_collection) * sizeof(int));
    *colons = (int *)realloc((*colons), (*size_collection) * sizeof(int));
    int m, n;
    scanf("%d%d", &m, &n);
    (*matrix_collection)[*size_collection - 1] = alloc_matrix(m, n);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j)
            scanf("%d", &((*matrix_collection)[*size_collection - 1][i][j]));
    }
    (*lines)[*size_collection - 1] = m;
    (*colons)[*size_collection - 1] = n;
}

void print_dimensions(int size_collection, int *lines, int *colons) {
    int matrix_index;
    scanf("%d", &matrix_index);
    if (matrix_index < 0 || matrix_index >= size_collection)
        printf("No matrix with the given index\n");
    else
        printf("%d %d\n", lines[matrix_index], colons[matrix_index]);
}

void print_matrix(int ****matrix_collection, int *size_collection, int **lines,
                  int **colons) {
    int matrix_index;
    scanf("%d", &matrix_index);
    if (matrix_index < 0 || matrix_index >= *size_collection) {
        printf("No matrix with the given index\n");
    } else {
        int nr_lines = (*lines)[matrix_index];
        int nr_col = (*colons)[matrix_index];
        for (int i = 0; i < nr_lines; ++i) {
            for (int j = 0; j < nr_col; ++j)
                printf("%d ", (*matrix_collection)[matrix_index][i][j]);
            printf("\n");
        }
    }
}

void crop_matrix(int ****matrix_collection, int size_collection, int **lines,
                 int **colons) {
    int matrix_index, new_m, new_n;
    scanf("%d", &matrix_index);
    scanf("%d", &new_m);
    int *new_lines = alloc_array(new_m);
    for (int i = 0; i < new_m; ++i) scanf("%d", &new_lines[i]);
    scanf("%d", &new_n);
    int *new_colons = alloc_array(new_n);
    for (int i = 0; i < new_n; ++i) scanf("%d", &new_colons[i]);
    if (matrix_index < 0 || matrix_index >= size_collection) {
        printf("No matrix with the given index\n");
        free(new_lines);
        free(new_colons);
        return;
    }
    int **aux = alloc_matrix(new_m, new_n);
    for (int i = 0; i < new_m; ++i) {
        for (int j = 0; j < new_n; ++j) {
            aux[i][j] =
                (*matrix_collection)[matrix_index][new_lines[i]][new_colons[j]];
        }
    }
    free_matrix((*matrix_collection)[matrix_index], (*lines)[matrix_index]);
    (*matrix_collection)[matrix_index] = alloc_matrix(new_m, new_n);
    for (int i = 0; i < new_m; ++i)
        for (int j = 0; j < new_n; ++j)
            (*matrix_collection)[matrix_index][i][j] = aux[i][j];
    (*lines)[matrix_index] = new_m;
    (*colons)[matrix_index] = new_n;
    free_matrix(aux, new_m);
    free(new_lines);
    free(new_colons);
}

void multiply_matrixes(int ****matrix_collection, int *size_collection,
                       int **lines, int **colons) {
    int matrix_1, matrix_2;
    scanf("%d%d", &matrix_1, &matrix_2);
    if (matrix_1 < 0 || matrix_1 >= *size_collection || matrix_2 < 0 ||
        matrix_2 >= *size_collection) {
        printf("No matrix with the given index\n");
        return;
    } else if ((*colons)[matrix_1] != (*lines)[matrix_2]) {
        printf("Cannot perform matrix multiplication\n");
        return;
    }
    (*size_collection)++;
    *matrix_collection = (int ***)realloc((*matrix_collection),
                                          (*size_collection) * sizeof(int **));
    (*matrix_collection)[*size_collection - 1] =
        (int **)malloc((*lines)[matrix_1] * sizeof(int *));
    *lines = (int *)realloc(*lines, (*size_collection) * sizeof(int));
    *colons = (int *)realloc(*colons, (*size_collection) * sizeof(int));
    for (int i = 0; i < (*lines)[matrix_1]; ++i) {
        (*matrix_collection)[*size_collection - 1][i] =
            (int *)malloc((*colons)[matrix_2] * sizeof(int));
        for (int j = 0; j < (*colons)[matrix_2]; ++j) {
            (*matrix_collection)[*size_collection - 1][i][j] = 0;
            for (int k = 0; k < (*colons)[matrix_1]; ++k) {
                (*matrix_collection)[*size_collection - 1][i][j] +=
                    (((*matrix_collection)[matrix_1][i][k] % MOD + MOD) *
                         ((*matrix_collection)[matrix_2][k][j] % MOD) +
                     MOD) %
                    MOD;
                (*matrix_collection)[*size_collection - 1][i][j] =
                    (((*matrix_collection)[*size_collection - 1][i][j] % MOD) +
                     MOD) %
                    MOD;
            }
        }
    }
    (*lines)[*size_collection - 1] = (*lines)[matrix_1];
    (*colons)[*size_collection - 1] = (*colons)[matrix_2];
}

void sort_matrixes(int ****matrix_collection, int *size_collection, int **lines,
                   int **colons) {
    for (int i = 0; i < *size_collection - 1; ++i) {
        for (int j = i + 1; j < *size_collection; ++j) {
            if (sum_of_elements((*matrix_collection)[i], (*lines)[i],
                                (*colons)[i]) >
                sum_of_elements((*matrix_collection)[j], (*lines)[j],
                                (*colons)[j])) {
                swap_ptrs_matrix(&(*matrix_collection)[i],
                                 &(*matrix_collection)[j]);
                swap_values(&((*lines)[i]), &((*lines)[j]));
                swap_values(&((*colons)[i]), &((*colons)[j]));
            }
        }
    }
}

void erase_matrix(int ****matrix_collection, int *size_collection, int **lines,
                  int **colons) {
    int matrix_index;
    scanf("%d", &matrix_index);
    if (matrix_index < 0 || matrix_index >= *size_collection) {
        printf("No matrix with the given index\n");
    } else {
        for (int i = matrix_index + 1; i < *size_collection; ++i) {
            swap_ptrs_matrix(&(*matrix_collection)[i],
                             &(*matrix_collection)[i - 1]);
            swap_values(&((*lines)[i]), &((*lines)[i - 1]));
            swap_values(&((*colons)[i]), &((*colons)[i - 1]));
        }
        // sterg ultima matrice
        free_matrix((*matrix_collection)[*size_collection - 1],
                    (*lines)[*size_collection - 1]);
        --(*size_collection);
    }
}

void transpose(int ****matrix_collection, int matrix_index, int **lines,
               int **colons) {
    int **temp_matrix =
        alloc_matrix((*colons)[matrix_index], (*lines)[matrix_index]);
    for (int i = 0; i < (*colons)[matrix_index]; ++i)
        for (int j = 0; j < (*lines)[matrix_index]; ++j)
            temp_matrix[i][j] = (*matrix_collection)[matrix_index][j][i];
    free_matrix((*matrix_collection)[matrix_index], (*lines)[matrix_index]);
    (*matrix_collection)[matrix_index] =
        alloc_matrix((*colons)[matrix_index], (*lines)[matrix_index]);
    for (int i = 0; i < (*colons)[matrix_index]; ++i)
        for (int j = 0; j < (*lines)[matrix_index]; ++j)
            (*matrix_collection)[matrix_index][i][j] = temp_matrix[i][j];
    free_matrix(temp_matrix, (*colons)[matrix_index]);
    swap_values(&((*lines)[matrix_index]), &((*colons)[matrix_index]));
}

void quit(int *size_collection, int ***matrix_collection, int *lines,
          int *colons) {
    for (int i = 0; i < *size_collection; ++i) {
        for (int j = 0; j < lines[i]; ++j) free(matrix_collection[i][j]);
        free(matrix_collection[i]);
    }
    free(matrix_collection);
    free(lines);
    free(colons);
}

void matrix_multiplication(int ***matrix_1, int **matrix_2, int matrix_size) {
    // In matrix_1 we store the product between matrix_1 and matrix_2
    int **matrix_dup_1 = alloc_matrix(matrix_size, matrix_size);
    int **matrix_dup_2 = alloc_matrix(matrix_size, matrix_size);
    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            matrix_dup_1[i][j] = (*matrix_1)[i][j];
            matrix_dup_2[i][j] = matrix_2[i][j];
        }
    }
    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            (*matrix_1)[i][j] = 0;
            for (int k = 0; k < matrix_size; ++k) {
                (*matrix_1)[i][j] +=
                    ((matrix_dup_1[i][k] % MOD) * (matrix_dup_2[k][j] % MOD)) %
                    MOD;
                (*matrix_1)[i][j] = (((*matrix_1)[i][j] % MOD) + MOD) % MOD;
            }
        }
    }
    free_matrix(matrix_dup_1, matrix_size);
    free_matrix(matrix_dup_2, matrix_size);
}

void get_power(int ****matrix_collection, int matrix_index, int matrix_size,
               int power) {
    int **power_matrix = alloc_matrix(matrix_size, matrix_size);
    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            power_matrix[i][j] =
                ((*matrix_collection)[matrix_index][i][j] % MOD + MOD) % MOD;
        }
    }
    for (int i = 0; i < matrix_size; ++i)
        for (int j = 0; j < matrix_size; ++j)
            (*matrix_collection)[matrix_index][i][j] = (i == j ? 1 : 0);
    for (int i = 0; i <= 30; ++i) {
        if (i > 0)
            matrix_multiplication(&power_matrix, power_matrix, matrix_size);
        if (power & (1 << i))
            matrix_multiplication(&(*matrix_collection)[matrix_index],
                                  power_matrix, matrix_size);
    }
    free_matrix(power_matrix, matrix_size);
}

int **part(int **a, int line_start, int colon_start, int size) {
    int **ans = alloc_matrix(size, size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j)
            ans[i][j] = a[line_start + i][colon_start + j];
    }
    return ans;
}

int **add_matrix(int **a, int **b, int size) {
    int **ans = alloc_matrix(size, size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            ans[i][j] = ((a[i][j] % MOD) + (b[i][j] % MOD)) % MOD;
            ans[i][j] = (ans[i][j] % MOD + MOD) % MOD;
        }
    }
    return ans;
}

int **diff_matrix(int **a, int **b, int size) {
    int **ans = alloc_matrix(size, size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            ans[i][j] = ((a[i][j] % MOD) - (b[i][j] % MOD)) % MOD;
            ans[i][j] = (ans[i][j] % MOD + MOD) % MOD;
        }
    }
    return ans;
}

int **fast_multiplication(int **a, int **b, int n) {
    int **ans = alloc_matrix(n, n);
    if (n == 1) {
        ans[0][0] = a[0][0] * b[0][0];
        return ans;
    }
    int ***A = (int ***)malloc(4 * sizeof(int **));
    int ***B = (int ***)malloc(4 * sizeof(int **));
    int ***R = (int ***)malloc(4 * sizeof(int **));
    int ***M = (int ***)malloc(7 * sizeof(int **));
    A[0] = part(a, 0, 0, n / 2);
    A[1] = part(a, 0, n / 2, n / 2);
    A[2] = part(a, n / 2, 0, n / 2);
    A[3] = part(a, n / 2, n / 2, n / 2);
    B[0] = part(b, 0, 0, n / 2);
    B[1] = part(b, 0, n / 2, n / 2);
    B[2] = part(b, n / 2, 0, n / 2);
    B[3] = part(b, n / 2, n / 2, n / 2);
    int **aux1,
        **aux2;  // matrice in care retin diferite operatii de adunare / scadere

    aux1 = add_matrix(A[0], A[3], n / 2);
    aux2 = add_matrix(B[0], B[3], n / 2);
    M[0] = fast_multiplication(aux1, aux2, n / 2);
    free_matrix(aux1, n / 2);
    free_matrix(aux2, n / 2);

    aux1 = add_matrix(A[2], A[3], n / 2);
    M[1] = fast_multiplication(aux1, B[0], n / 2);
    free_matrix(aux1, n / 2);

    aux1 = diff_matrix(B[1], B[3], n / 2);
    M[2] = fast_multiplication(A[0], aux1, n / 2);
    free_matrix(aux1, n / 2);

    aux1 = diff_matrix(B[2], B[0], n / 2);
    M[3] = fast_multiplication(A[3], aux1, n / 2);
    free_matrix(aux1, n / 2);

    aux1 = add_matrix(A[0], A[1], n / 2);
    M[4] = fast_multiplication(aux1, B[3], n / 2);
    free_matrix(aux1, n / 2);

    aux1 = diff_matrix(A[2], A[0], n / 2);
    aux2 = add_matrix(B[0], B[1], n / 2);
    M[5] = fast_multiplication(aux1, aux2, n / 2);
    free_matrix(aux1, n / 2);
    free_matrix(aux2, n / 2);

    aux1 = diff_matrix(A[1], A[3], n / 2);
    aux2 = add_matrix(B[2], B[3], n / 2);
    M[6] = fast_multiplication(aux1, aux2, n / 2);
    free_matrix(aux1, n / 2);
    free_matrix(aux2, n / 2);

    aux1 = add_matrix(M[0], M[3], n / 2);
    aux2 = diff_matrix(aux1, M[4], n / 2);
    R[0] = add_matrix(aux2, M[6], n / 2);
    free_matrix(aux1, n / 2);
    free_matrix(aux2, n / 2);

    R[1] = add_matrix(M[2], M[4], n / 2);
    R[2] = add_matrix(M[1], M[3], n / 2);

    aux1 = add_matrix(M[0], M[2], n / 2);
    aux2 = diff_matrix(aux1, M[1], n / 2);
    R[3] = add_matrix(aux2, M[5], n / 2);
    free_matrix(aux1, n / 2);
    free_matrix(aux2, n / 2);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (i < n / 2)
                if (j < n / 2)
                    ans[i][j] = R[0][i][j];
                else
                    ans[i][j] = R[1][i][j - n / 2];
            else if (j < n / 2)
                ans[i][j] = R[2][i - n / 2][j];
            else
                ans[i][j] = R[3][i - n / 2][j - n / 2];
    for (int i = 0; i < 7; ++i) {
        if (i < 4) {
            free_matrix(A[i], n / 2);
            free_matrix(B[i], n / 2);
            free_matrix(R[i], n / 2);
        }
        free_matrix(M[i], n / 2);
    }
    free(A);
    free(B);
    free(M);
    free(R);
    return ans;
}

int main(void) {
    int size_collection = 0;
    char c;
    // INITIALIZE_MEMORY
    int ***matrix_collection =
        (int ***)malloc((size_collection + 1) * sizeof(int **));
    int *lines = alloc_array(size_collection + 1);
    int *colons = alloc_array(size_collection + 1);
    scanf(" %c", &c);
    while (1) {
        if (c == 'L') {
            load_matrix(&matrix_collection, &size_collection, &lines, &colons);
        } else if (c == 'D')
            print_dimensions(size_collection, lines, colons);
        else if (c == 'P')
            print_matrix(&matrix_collection, &size_collection, &lines, &colons);
        else if (c == 'C')
            crop_matrix(&matrix_collection, size_collection, &lines, &colons);
        else if (c == 'M')
            multiply_matrixes(&matrix_collection, &size_collection, &lines,
                              &colons);
        else if (c == 'Q') {
            quit(&size_collection, matrix_collection, lines, colons);
            return 0;
        } else if (c == 'T') {
            int matrix_index;
            scanf("%d", &matrix_index);
            if (matrix_index < 0 || matrix_index >= size_collection)
                printf("No matrix with the given index\n");
            else
                transpose(&matrix_collection, matrix_index, &lines, &colons);
        } else if (c == 'R') {
            int matrix_index, power;
            scanf("%d%d", &matrix_index, &power);
            if (matrix_index < 0 || matrix_index >= size_collection)
                printf("No matrix with the given index\n");
            else if (power < 0)
                printf("Power should be positive\n");
            else if (lines[matrix_index] != colons[matrix_index])
                printf("Cannot perform matrix multiplication\n");
            else
                get_power(&matrix_collection, matrix_index, lines[matrix_index],
                          power);
        } else if (c == 'F')
            erase_matrix(&matrix_collection, &size_collection, &lines, &colons);
        else if (c == 'O')
            sort_matrixes(&matrix_collection, &size_collection, &lines,
                          &colons);
        else if (c == 'S') {
            int matrix_1, matrix_2;
            scanf("%d%d", &matrix_1, &matrix_2);
            if (matrix_1 < 0 || matrix_1 >= size_collection || matrix_2 < 0 ||
                matrix_2 >= size_collection) {
                printf("No matrix with the given index\n");
            } else if (lines[matrix_1] != lines[matrix_2])
                printf("Cannot perform matrix multiplication\n");
            else {
                size_collection++;
                matrix_collection = (int ***)realloc(
                    matrix_collection, size_collection * sizeof(int **));
                matrix_collection[size_collection - 1] = fast_multiplication(
                    matrix_collection[matrix_1], matrix_collection[matrix_2],
                    lines[matrix_1]);
                lines = (int *)realloc(lines, size_collection * sizeof(int));
                colons = (int *)realloc(colons, size_collection * sizeof(int));
                lines[size_collection - 1] = lines[matrix_1];
                colons[size_collection - 1] = lines[matrix_1];
            }
        } else
            printf("Unrecognized command\n");
        scanf(" %c", &c);
    }
}
