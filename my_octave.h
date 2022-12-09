#ifndef __MY_OCTAVE_H__
#define __MY_OCTAVE_H__

void swap_ptrs_matrix(int ***matrix_1, int ***matrix_2);

void swap_values(int *value_1, int *value_2);

void free_matrix(int **matrix, int size);

void free_matrix_collection(int ***collection, int size_collection,
							int size_matrix);

int sum_of_elements(int **matrix, int lines, int columns);

void alloc_collection(int ****collection, int size, int *opperation_failed);

void realloc_collection(int ****collection, int new_size,
						int *opperation_failed);

void alloc_array(int **array, int size, int *opperation_failed);

void realloc_array(int **array, int new_size, int *opperation_failed);

void alloc_matrix(int ***matrix, int lines, int columns,
				  int *opperation_failed);

void alloc_entire_collection(int ****collection, int size_collection,
							 int nr_lines, int nr_colons,
							 int *opperation_failed);

void load_matrix(int ****matrix_collection, int *size_collection, int **lines,
				 int **columns, int *opperation_failed);

void print_dimensions(int size_collection, int *lines, int *columns);

void print_matrix(int ***matrix_collection, int size_collection, int *lines,
				  int *columns);

void crop_matrix(int ****matrix_collection, int size_collection, int **lines,
				 int **columns, int *opperation_failed);

void multiply_matrixes(int ****matrix_collection, int *size_collection,
					   int **lines, int **columns, int *opperation_failed);

void sort_matrixes(int ****matrix_collection, int *size_collection, int **lines,
				   int **columns);

void erase_matrix(int ****matrix_collection, int *size_collection, int **lines,
				  int **columns);

void transpose(int ****matrix_collection, int size_collection, int **lines,
			   int **columns, int *opperation_failed);

void quit(int *size_collection, int ***matrix_collection, int *lines,
		  int *columns);

void matrix_multiplication(int ***matrix_1, int **matrix_2, int matrix_size,
						   int *opperation_failed);

void get_power(int ****matrix_collection, int *lines, int *columns,
			   int size_collection, int *opperation_failed);

void part(int ***part_matrix, int **matrix, int line_start, int colon_start,
		  int size);

void add_matrix(int ***matrix_sum, int **matrix_1, int **matrix_2, int size);

void diff_matrix(int ***matrix_diff, int **matrix_1, int **matrix_2, int size);

void recomposition(int ***ans, int ***R, int size_matrix);

void make_partisions(int ****collection_1, int ****collection_2, int **matrix_1,
					 int **matrix_2, int size_matrix);

void fast_multiplication(int ***product, int **matrix_1, int **matrix_2,
						 int size_matrix, int *opperation_failed);

void strassen(int ****matrix_collection, int *size_collection, int **lines,
			  int **columns, int *opperation_failed);

#endif	// __MY_OCTAVE_H__

