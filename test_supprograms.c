#include <stdio.h>
#include <stdlib.h>

const int MOD = (1e4) + 7;

void swap_ptrs_matrix(int ***a, int ***b)
{
    int **aux = *a;
    *a = *b;
    *b = aux;
}

void new_matrix(int ***matrix_collection, int *size_collection, int *lines, int *colons, int *sum)
{
    matrix_collection = (int ***)realloc(matrix_collection, ++(*size_collection));
    lines = (int *)realloc(lines, *size_collection);
    colons = (int *)realloc(colons, *size_collection);
    sum = (int *)realloc(sum, *size_collection);
    int m, n, curr_sum = 0;
    scanf("%d%d", &m, &n);
    matrix_collection[*size_collection - 1] = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; ++i) {
        matrix_collection[*size_collection - 1][i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j) {
            scanf("%d", &matrix_collection[*size_collection - 1][i][j]);
            curr_sum += matrix_collection[*size_collection - 1][i][j];
            curr_sum = ((curr_sum % MOD) + MOD) % MOD;
        }
    }
    lines[*size_collection - 1] = m;
    colons[*size_collection - 1] = n;
    sum[*size_collection - 1] = curr_sum;
}

void print_dimensions(int *size_collection, int *lines, int *colons)
{
    int matrix_index;
    scanf("%d", &matrix_index);
    if (matrix_index < 0 || matrix_index >= *size_collection) {
        printf("No matrix with the given index\n");
    } else {
        printf("%d %d\n", lines[matrix_index],
               colons[matrix_index]);
    }
}

void print_matrix(int *size_collection, int *lines, int *colons, int ***matrix_collection)
{
    int matrix_index;
    scanf("%d", &matrix_index);
    if (matrix_index < 0 || matrix_index >= *size_collection) {
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
}

void multiply_matrixes (int *size_collection, int *lines, int *colons, int *sum, int ***matrix_collection)
{
    int matrix_1, matrix_2;
    scanf("%d%d", &matrix_1, &matrix_2);
    if (matrix_1 < 0 || matrix_1 >= *size_collection || matrix_2 < 0 || matrix_2 >= *size_collection) {
        printf("No matrix with the given index\n");
    } else if (colons[matrix_1] != lines[matrix_2]) {
        printf("Cannot perform matrix multiplication\n");
    } else {
        matrix_collection = (int ***)realloc(matrix_collection, ++(*size_collection));
        matrix_collection[*size_collection - 1] = (int **)malloc(lines[matrix_1] * sizeof(int *));
    	lines = (int *)realloc(lines, *size_collection);
    	colons = (int *)realloc(colons, *size_collection);
    	sum = (int *)realloc(sum, *size_collection);
		int curr_sum = 0;
        for (int i = 0; i < lines[matrix_1]; ++i) {
            matrix_collection[*size_collection - 1][i] = (int *)malloc(colons[matrix_2] * sizeof(int));
            for (int j = 0; j < colons[matrix_2]; ++j) {
                matrix_collection[*size_collection - 1][i][j] = 0;
            	for (int k = 0; k < colons[matrix_1]; ++k) {
                    matrix_collection[*size_collection - 1][i][j] += matrix_collection[matrix_1][i][k] * 
                                                                    matrix_collection[matrix_2][k][j];
            		curr_sum += matrix_collection[*size_collection - 1][i][j];
            		curr_sum = ((curr_sum % MOD) + MOD) % MOD;
            	}	
            }
        }
        lines[*size_collection - 1] = lines[matrix_1];
        colons[*size_collection - 1] = colons[matrix_2];
		sum[*size_collection - 1] = curr_sum % MOD;
    }
}

void sort_matrixes(int *size_collection, int ***matrix_collection, int *lines, int *colons, int *sum)
{
    for (int i = 0; i < *size_collection - 1; ++i) {
        for (int j = i + 1; j < *size_collection; ++j) {
            if (sum[i] > sum[j]) {
                swap_ptrs_matrix(&matrix_collection[i], &matrix_collection[j]);
				int aux = sum[i];
				sum[i] = sum[j];
				sum[j] = aux;
                int lines_prev = lines[i];
                int lines_next = lines[j];
                int col_prev = colons[i];
                int col_next = colons[j];
                lines[i] = lines_next;
                lines[j] = lines_prev;
                colons[i] = col_next;
                colons[j] = col_prev;
            }
        }
    }
}

void erase_matrix(int *size_collection, int ***matrix_collection, int *lines, int *colons, int *sum) {
    int matrix_index;
    scanf("%d", &matrix_index);	
    if (matrix_index < 0 || matrix_index >= *size_collection) {
        printf("No matrix with the given index\n");
    } else {
        // update la linii si coloane
        // trebuie sa schimb mai bine pointerii intre ei decat sa schimb valorile din ei
        for (int i = matrix_index + 1; i < *size_collection; ++i) {
            int lines_prev = lines[i - 1];
            int lines_next = lines[i];
            int col_prev = colons[i - 1];
            int col_next = colons[i];
			int sum_prev = sum[i - 1];
			int sum_next = sum[i];
            swap_ptrs_matrix(&matrix_collection[i - 1], &matrix_collection[i]);
            lines[i - 1] = lines_next;
            lines[i] = lines_prev;
            colons[i - 1] = col_next;
            colons[i] = col_prev;
			sum[i - 1] = sum_next;
			sum[i] = sum_prev;
        }
        //matrix_collection = (int ***)realloc(matrix_collection, --size_collection);
        for (int i = 0 ; i < lines[*size_collection - 1]; ++i) {
            free(matrix_collection[*size_collection - 1][i]);
        }
        free(matrix_collection[*size_collection - 1]);
        --(*size_collection);
        //sterg ultima matrice
	}
}

void quit(int *size_collection, int ***matrix_collection, int *lines, int *colons, int *sum) {
    for (int i = 0; i < *size_collection; ++i) {
        for (int j = 0; j < lines[i]; ++j) {
            free(matrix_collection[i][j]);
        }
        free(matrix_collection[i]);
    }
    free(matrix_collection);
    free(lines);
    free(colons);
	free(sum);
}

int main(void)
{
	int ***matrix_collection, size_collection = 0, *lines, *colons, *sum;
	//INITIALIZE_MEMORY
    matrix_collection = (int ***)malloc((size_collection + 1) * (sizeof(int **)));
    lines = (int *)malloc((size_collection + 1) * sizeof(int));
    colons = (int *)malloc((size_collection + 1) * sizeof(int));
    sum = (int *)malloc((size_collection + 1) * sizeof(int));
    while (1) {
        char c;
        scanf("%c", &c);
        if (c == 'L') {
            new_matrix(matrix_collection, &size_collection, lines, colons, sum);
		} else if (c == 'D') {
			print_dimensions(&size_collection, lines, colons);
        } else if (c == 'P') {
			print_matrix(&size_collection, lines, colons, matrix_collection);
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
			multiply_matrixes(&size_collection, lines, colons, sum, matrix_collection);
        } else if (c == 'Q') {
			quit(&size_collection, matrix_collection, lines, colons, sum);
            return 0;
        } else if (c == 'T') {
            int matrix_index;
            scanf("%d", &matrix_index);
            if (matrix_index < 0 || matrix_index >= size_collection) {
                printf("No matrix with the given index\n");
            } else {
                int **temp_matrix;
                temp_matrix = (int **)malloc(colons[matrix_index] * sizeof(int *));
                for (int i = 0; i < colons[matrix_index]; ++i) {
                    temp_matrix[i] = (int *)malloc(lines[matrix_index] * sizeof(int));
                    for (int j = 0; j < lines[matrix_index]; ++j) {
                        temp_matrix[i][j] = matrix_collection[matrix_index][j][i];
                    }
                }
                for (int i = 0; i < lines[matrix_index]; ++i) {
                    free(matrix_collection[matrix_index][i]);
                }
                free(matrix_collection[matrix_index]);
                matrix_collection[matrix_index] = (int **)malloc(colons[matrix_index] * sizeof(int *));
                for (int i = 0; i < colons[matrix_index]; ++i) {
                    matrix_collection[matrix_index][i] = (int *)malloc(lines[matrix_index] * sizeof(int));
                    for (int j = 0; j < lines[matrix_index]; ++j) {
                        matrix_collection[matrix_index][i][j] = temp_matrix[i][j];
                    }
                }
                for (int i = 0; i < colons[matrix_index]; ++i) {
                    free(temp_matrix[i]);
                }
                free(temp_matrix);
                int aux = lines[matrix_index];
                lines[matrix_index] = colons[matrix_index];
                colons[matrix_index] = aux;
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
			erase_matrix(&size_collection, matrix_collection, lines, colons, sum);
        } else if (c == 'O') {
			sort_matrixes(&size_collection, matrix_collection, lines, colons, sum);
        } else {
			printf("Unrecognized command\n");
		}
        getchar();
    }
}
