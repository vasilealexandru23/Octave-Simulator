#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "my_octave.h"

const int MOD = (1e4) + 7;

// Cateva functii utile : swap de pointeri, eliberari de memorie, etc.
void swap_ptrs_matrix(int ***matrix_1, int ***matrix_2)
{
	// In aceasta functie facem swap intre doua matrice alocate dinamic.
	int **aux = *matrix_1;
	*matrix_1 = *matrix_2;
	*matrix_2 = aux;
}

void swap_values(int *value_1, int *value_2)
{
	// In aceasta functie facem swap intre doua valori folosind pointeri.
	int aux = *value_1;
	*value_1 = *value_2;
	*value_2 = aux;
}

void free_matrix(int **matrix, int size)
{
	// In aceasta functie eliberam memoria alocata pentru o matrice.
	for (int i = 0; i < size; ++i)
		free(matrix[i]);
	free(matrix);
}

void free_matrix_collection(int ***collection, int size_collection,
							int size_matrix)
{
	// In aceasta functie eliberam memoria alocata pentru o colectie de matrice
	for (int i = 0; i < size_collection; ++i)
		free_matrix(collection[i], size_matrix);
	free(collection);
}

int sum_of_elements(int **matrix, int lines, int columns)
{
	// Aceasta functie returneaza suma elementelor unei matrice modulo MOD
	int sum = 0;
	for (int i = 0; i < lines; i++) {
		for (int j = 0; j < columns; ++j) {
			sum += (((matrix[i][j] % MOD) + MOD) % MOD);
			sum = (((sum % MOD) + MOD) % MOD);
		}
	}
	return sum % MOD;
}

void alloc_collection(int ****collection, int size, int *opperation_failed)
{
	// Prin aceasta functie alocam memorie pentru o colectie de matrice.
	// Daca alocarea nu a fost facuta corect returnam prin variabila
	// opperation_failed valoarea 1.
	(*collection) = (int ***)malloc(size * sizeof(int **));
	if (!(*collection)) {
		printf("Invalid allocation.\n");
		*opperation_failed = 1;
		return;
	}
}

void realloc_collection(int ****collection, int new_size,
						int *opperation_failed)
{
	// In aceasta functie realocam memorie pentru o colectie de matrice
	// strategic (folosind o variabila auxiliara) pentru a nu pierde
	// informatii din vechea colectie de matrice.
	// Iar prin variabila opperation_failed vedem daca alocarea a fost
	// facuta corect.
	int ***tmp;
	tmp = (int ***)realloc((*collection), (new_size) * sizeof(int **));
	if (!tmp) {
		printf("Invalid allocation.\n");
		*opperation_failed = 1;
		return;
	}
	*collection = tmp;
}

void alloc_array(int **array, int size, int *opperation_failed)
{
	// Prin aceasta functie alocam memorie pentru un array.
	(*array) = (int *)malloc(size * sizeof(int));
	if (!(*array)) {
		printf("Invalid allocation.\n");
		*opperation_failed = 1;
		return;
	}
}

void realloc_array(int **array, int new_size, int *opperation_failed)
{
	// Prin aceasta functie realocam memorie pentru un array folosind
	// un alt pointer pentru a nu pierde valorile vechi in cazul
	// in care realocarea nu a fost facuta corect.
	int *tmp = realloc((*array), new_size * sizeof(int));
	if (!tmp) {
		printf("Invalid allocation.\n");
		free(*array);
		*opperation_failed = 1;
		return;
	}
	*array = tmp;
}

void alloc_matrix(int ***matrix, int lines, int columns, int *opperation_failed)
{
	// In aceasta functie alocam memoria necesara pentru o matrice si daca
	// ceva nu a fost alocat cum trebuie marcam acest fapt prin variabila
	// opperation_failed si dealocam din memorie tot ce am folosit pana la
	// acel moment.
	(*matrix) = (int **)malloc(lines * sizeof(int *));
	if (!(*matrix)) {
		printf("Invalid allocation.\n");
		*opperation_failed = 1;
		return;
	}
	for (int i = 0; i < lines; ++i) {
		(*matrix)[i] = (int *)malloc(columns * sizeof(int));
		if (!(*matrix)[i]) {
			// Dealocam toata memoria folosita pana acum
			for (int j = 0; j < i; ++j)
				free((*matrix)[j]);
			printf("Invalid allocation.\n");
			*opperation_failed = 1;
			return;
		}
	}
}

void alloc_entire_collection(int ****collection, int size_collection,
							 int nr_lines, int nr_colons,
							 int *opperation_failed)
{
	// Prin aceasta functie alocam memoria necesara pentru o colectie de
	// matrice impreuna cu memoria pentru fiecare matrice in parte.
	// Daca ceva nu a mers, pur si simplu dealocam memoria folosita
	// pana la acel moment si marcam variabila opperation_failed.
	alloc_collection(collection, size_collection, opperation_failed);
	if (*opperation_failed)
		return;
	for (int i = 0; i < size_collection; ++i) {
		alloc_matrix(&((*collection)[i]), nr_lines, nr_colons,
					 opperation_failed);
		if (*opperation_failed) {
			for (int j = 0; j < i; ++j)
				free_matrix((*collection)[j], nr_lines);
			free(*collection);
			return;
		}
	}
}

// Functiile necesare pentru operatiile cerute in tema
void load_matrix(int ****matrix_collection, int *size_collection, int **lines,
				 int **columns, int *opperation_failed)
{
	// Prin aceasta functie adaugam o noua matrice la colectia noastra
	// initiala, deci "marim" dimensiunea colectiei si a vectorilor
	// in care retin informatii despre dimensiunile fiecarei matrice
	// dupa care citesc matricea in sine.
	(*size_collection)++;
	realloc_collection(matrix_collection, (*size_collection),
					   opperation_failed);
	if (*opperation_failed)
		return;
	realloc_array(lines, *size_collection, opperation_failed);
	if (*opperation_failed)
		return;
	realloc_array(columns, *size_collection, opperation_failed);
	if (*opperation_failed)
		return;
	int nr_lines, nr_columns;
	scanf("%d%d", &nr_lines, &nr_columns);
	alloc_matrix(&(*matrix_collection)[*size_collection - 1], nr_lines,
				 nr_columns, opperation_failed);
	if (*opperation_failed)
		return;
	for (int i = 0; i < nr_lines; ++i) {
		for (int j = 0; j < nr_columns; ++j)
			scanf("%d", &((*matrix_collection)[*size_collection - 1][i][j]));
	}
	(*lines)[*size_collection - 1] = nr_lines;
	(*columns)[*size_collection - 1] = nr_columns;
}

void print_dimensions(int size_collection, int *lines, int *columns)
{
	// In aceasta functie afisam numarul de linii si coloane pentru
	// o matrice data (valida), in caz contrar afisam mesajul cerut.
	int matrix_index;
	scanf("%d", &matrix_index);
	if (matrix_index < 0 || matrix_index >= size_collection)
		printf("No matrix with the given index\n");
	else
		printf("%d %d\n", lines[matrix_index], columns[matrix_index]);
}

void print_matrix(int ***matrix_collection, int size_collection, int *lines,
				  int *columns)
{
	// In aceasta functie afisam o matrice data (valida) plus
	// mesajul corespunzator daca nu este dat un index valid.
	int matrix_index;
	scanf("%d", &matrix_index);
	if (matrix_index < 0 || matrix_index >= size_collection) {
		printf("No matrix with the given index\n");
	} else {
		int nr_lines = lines[matrix_index];
		int nr_col = columns[matrix_index];
		for (int i = 0; i < nr_lines; ++i) {
			for (int j = 0; j < nr_col; ++j)
				printf("%d ", matrix_collection[matrix_index][i][j]);
			printf("\n");
		}
	}
}

void crop_matrix(int ****matrix_collection, int size_collection, int **lines,
				 int **columns, int *opperation_failed)
{
	// In aceasta functie pastram doar o parte din matricea initiala,
	// anume intersectia dintre liniile si coloanele date in input.
	int matrix_index, new_m, new_n;
	scanf("%d", &matrix_index);
	scanf("%d", &new_m);
	int *new_lines;
	alloc_array(&new_lines, new_m, opperation_failed);
	if (*opperation_failed)
		return;
	for (int i = 0; i < new_m; ++i)
		scanf("%d", &new_lines[i]);
	scanf("%d", &new_n);
	int *new_columns;
	alloc_array(&new_columns, new_n, opperation_failed);
	if (*opperation_failed) {
		return;
		free(new_lines);
	}
	for (int i = 0; i < new_n; ++i)
		scanf("%d", &new_columns[i]);
	// Verificam daca matricea este valida
	if (matrix_index < 0 || matrix_index >= size_collection) {
		printf("No matrix with the given index\n");
		free(new_lines);
		free(new_columns);
		return;
	}
	// Cream o matrice auxiliara in care pastram matricea dupa crop.
	int **aux;
	alloc_matrix(&aux, new_m, new_n, opperation_failed);
	if (*opperation_failed) {
		free(new_lines);
		free(new_columns);
		return;
	}
	for (int i = 0; i < new_m; ++i) {
		for (int j = 0; j < new_n; ++j) {
			aux[i][j] = (*matrix_collection)[matrix_index][new_lines[i]]
						[new_columns[j]];
		}
	}
	// Realocam noile dimensiuni pentru matricea initiala.
	free_matrix((*matrix_collection)[matrix_index], (*lines)[matrix_index]);
	alloc_matrix(&(*matrix_collection)[matrix_index], new_m, new_n,
				 opperation_failed);
	if (*opperation_failed) {
		free_matrix(aux, new_m);
		free(new_lines);
		free(new_columns);
		return;
	}
	// Copiem matricea auxiliara in matricea noastra
	for (int i = 0; i < new_m; ++i)
		for (int j = 0; j < new_n; ++j)
			(*matrix_collection)[matrix_index][i][j] = aux[i][j];
	// Actualizam informatiile despre numarul de linii si coloane.
	(*lines)[matrix_index] = new_m;
	(*columns)[matrix_index] = new_n;
	// Eliberam memoria alocata pentru matricea auxiliara.
	free_matrix(aux, new_m);
	free(new_lines);
	free(new_columns);
}

void multiply_matrixes(int ****matrix_collection, int *size_collection,
					   int **lines, int **columns, int *opperation_failed)
{
	// In aceasta functie inmultim doua matrice date.
	int matrix_1, matrix_2;
	scanf("%d%d", &matrix_1, &matrix_2);
	// Verificam daca indecsii dati sunt valizi.
	if (matrix_1 < 0 || matrix_1 >= *size_collection || matrix_2 < 0 ||
		matrix_2 >= *size_collection) {
		printf("No matrix with the given index\n");
		return;
	} else if ((*columns)[matrix_1] != (*lines)[matrix_2]) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}
	// Alocam memorie pentru matricea rezultata in colectia noastra si in
	// vectorii in care retinem informatii despre dimensiunea matricelor.
	(*size_collection)++;
	realloc_collection(matrix_collection, *size_collection, opperation_failed);
	if (*opperation_failed)
		return;
	alloc_matrix(&(*matrix_collection)[*size_collection - 1],
				 (*lines)[matrix_1], (*columns)[matrix_2],
				 opperation_failed);
	// Verificam daca alocarile au fost facute corespunzator
	if (*opperation_failed)
		return;
	realloc_array(lines, *size_collection, opperation_failed);
	if (*opperation_failed)
		return;
	realloc_array(columns, *size_collection, opperation_failed);
	if (*opperation_failed)
		return;
	for (int i = 0; i < (*lines)[matrix_1]; ++i) {
		for (int j = 0; j < (*columns)[matrix_2]; ++j) {
			(*matrix_collection)[*size_collection - 1][i][j] = 0;
			for (int k = 0; k < (*columns)[matrix_1]; ++k) {
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
	// Salvam numarul de linii si coloane pentru matricea rezultata
	(*lines)[*size_collection - 1] = (*lines)[matrix_1];
	(*columns)[*size_collection - 1] = (*columns)[matrix_2];
}

void sort_matrixes(int ****matrix_collection, int *size_collection, int **lines,
				   int **columns)
{
	// In aceasta functie sortam matricile din colectie in functie de suma
	// elementelor folosing Bubble Sort.
	for (int i = 0; i < *size_collection - 1; ++i) {
		for (int j = i + 1; j < *size_collection; ++j) {
			if (sum_of_elements((*matrix_collection)[i], (*lines)[i],
								(*columns)[i]) >
				sum_of_elements((*matrix_collection)[j], (*lines)[j],
								(*columns)[j])) {
				swap_ptrs_matrix(&(*matrix_collection)[i],
								 &(*matrix_collection)[j]);
				swap_values(&((*lines)[i]), &((*lines)[j]));
				swap_values(&((*columns)[i]), &((*columns)[j]));
			}
		}
	}
}

void erase_matrix(int ****matrix_collection, int *size_collection, int **lines,
				  int **columns)
{
	// In stergem o matrice dintr - o colectie data.
	int matrix_index;
	scanf("%d", &matrix_index);
	// Verificam daca indexul este valid.
	if (matrix_index < 0 || matrix_index >= *size_collection) {
		printf("No matrix with the given index\n");
	} else {
		// Interschibam matricile intre ele pana cand matricea de sters
		// este ultima in colectia noastra dupa care eliberam memoria alocata
		// pentru ea.
		for (int i = matrix_index + 1; i < *size_collection; ++i) {
			swap_ptrs_matrix(&(*matrix_collection)[i],
							 &(*matrix_collection)[i - 1]);
			swap_values(&((*lines)[i]), &((*lines)[i - 1]));
			swap_values(&((*columns)[i]), &((*columns)[i - 1]));
		}
		// Eliberam memoria alocata ultimelei matrice.
		free_matrix((*matrix_collection)[*size_collection - 1],
					(*lines)[*size_collection - 1]);
		--(*size_collection);
	}
}

void transpose(int ****matrix_collection, int size_collection, int **lines,
			   int **columns, int *opperation_failed)
{
	int matrix_index;
	scanf("%d", &matrix_index);
	// Verificam daca indexul exista in colectie.
	if (matrix_index < 0 || matrix_index >= size_collection) {
		printf("No matrix with the given index\n");
		return;
	}
	// In aceasta functie realizam transpusa unei matrice.
	// Folosim o matrice auxiliara in care salvam transpusa.
	int **temp_matrix;
	alloc_matrix(&temp_matrix, (*columns)[matrix_index],
				 (*lines)[matrix_index], opperation_failed);
	if (*opperation_failed)
		return;
	for (int i = 0; i < (*columns)[matrix_index]; ++i)
		for (int j = 0; j < (*lines)[matrix_index]; ++j)
			temp_matrix[i][j] = (*matrix_collection)[matrix_index][j][i];
	// Eliberam memoria alocata pentru matricea initiala si alocam
	// pentru noua matrice memoria necesara.
	free_matrix((*matrix_collection)[matrix_index], (*lines)[matrix_index]);
	alloc_matrix(&(*matrix_collection)[matrix_index],
				 (*columns)[matrix_index], (*lines)[matrix_index],
				 opperation_failed);
	if (*opperation_failed) {
		free_matrix(temp_matrix, (*columns)[matrix_index]);
		return;
	}
	for (int i = 0; i < (*columns)[matrix_index]; ++i)
		for (int j = 0; j < (*lines)[matrix_index]; ++j)
			(*matrix_collection)[matrix_index][i][j] = temp_matrix[i][j];
	// Eliberam memoria folosita pentru matricea temporara
	free_matrix(temp_matrix, (*columns)[matrix_index]);
	// Actualizam dimensiunea matricei
	swap_values(&((*lines)[matrix_index]), &((*columns)[matrix_index]));
}

void quit(int *size_collection, int ***matrix_collection, int *lines,
		  int *columns)
{
	// In aceasta functie dealocam toata memoria folosita
	// in program pentru informatiile despre matrice.
	for (int i = 0; i < *size_collection; ++i) {
		for (int j = 0; j < lines[i]; ++j)
			free(matrix_collection[i][j]);
		free(matrix_collection[i]);
	}
	free(matrix_collection);
	free(lines);
	free(columns);
}

void matrix_multiplication(int ***matrix_1, int **matrix_2, int matrix_size,
						   int *opperation_failed)
{
	// In aceasta functie stocam in matrix_1 operatia de inmultire dintre
	// matrix_1 si matrix_2.
	// Cream niste matrice auxiliare pentru a nu pierde valori.
	int **matrix_dup_1, **matrix_dup_2;
	alloc_matrix(&matrix_dup_1, matrix_size, matrix_size, opperation_failed);
	if (*opperation_failed)
		return;
	alloc_matrix(&matrix_dup_2, matrix_size, matrix_size, opperation_failed);
	if (*opperation_failed) {
		free_matrix(matrix_dup_1, matrix_size);
		return;
	}
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
	// Eliberam memoria pentru matricile auxiliare.
	free_matrix(matrix_dup_1, matrix_size);
	free_matrix(matrix_dup_2, matrix_size);
}

void get_power(int ****matrix_collection, int *lines, int *columns,
			   int size_collection, int *opperation_failed)
{
	// In aceasta functie determinam ridicarea la putere a unei matrice.
	int matrix_index, power;
	scanf("%d%d", &matrix_index, &power);
	// Verificam daca inputul este valid.
	if (matrix_index < 0 || matrix_index >= size_collection) {
		printf("No matrix with the given index\n");
		return;
	}
	if (power < 0) {
		printf("Power should be positive\n");
		return;
	} else if (lines[matrix_index] != columns[matrix_index]) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}
	// Cream o matrice auxiliara in care stocam matrice de tipul
	// (matrice)^(2^i), unde i = {0..31}.
	int **power_matrix;
	alloc_matrix(&power_matrix, lines[matrix_index], lines[matrix_index],
				 opperation_failed);
	if (*opperation_failed)
		return;
	// Punem in variabila auxiliara matricea initiala
	for (int i = 0; i < lines[matrix_index]; ++i) {
		for (int j = 0; j < lines[matrix_index]; ++j) {
			power_matrix[i][j] =
				((*matrix_collection)[matrix_index][i][j] % MOD + MOD) % MOD;
		}
	}
	// Punem in matricea originala matricea identitate care semnifica
	// matricea initiala ridicata la puterea 0.
	for (int i = 0; i < lines[matrix_index]; ++i)
		for (int j = 0; j < lines[matrix_index]; ++j)
			(*matrix_collection)[matrix_index][i][j] = (i == j ? 1 : 0);
	for (int i = 0; i <= 30; ++i) {
		// Actualizam matricea auxiliara la fiecare bit (ridicam la patrat).
		if (i > 0) {
			matrix_multiplication(&power_matrix, power_matrix,
								  lines[matrix_index], opperation_failed);
			// Daca ceva nu a mers la alocare, dealocam tot ceea ce am folosit,
			// iar memoria alocata pentru matricea noastra va fi dealocata
			// in main.
			if (*opperation_failed) {
				free_matrix(power_matrix, lines[matrix_index]);
				return;
			}
		}
		// Daca in numarul la care trebuie ridicata matricea avem un bit 1
		// actualizam matricea originala (o inmultim cu matricea auxiliara).
		if (power & (1 << i)) {
			matrix_multiplication(&(*matrix_collection)[matrix_index],
								  power_matrix, lines[matrix_index],
								  opperation_failed);
			// Daca ceva nu a mers la alocare, dealocam tot ceea ce am folosit,
			// iar memoria alocata pentru matricea noastra va fi dealocata
			// in main.
			if (*opperation_failed) {
				free_matrix(power_matrix, lines[matrix_index]);
				return;
			}
		}
	}
	// Dealocam memoria pentru matricea auxiliara.
	free_matrix(power_matrix, lines[matrix_index]);
}

void part(int ***part_matrix, int **matrix, int line_start, int colon_start,
		  int size)
{
	// In aceasta functie salvam in part_matrix o matrice de dimensiune
	// size * size care reprezinta o parte din matricea matrix(un fel de crop).
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j)
			(*part_matrix)[i][j] = matrix[line_start + i][colon_start + j];
	}
}

void add_matrix(int ***matrix_sum, int **matrix_1, int **matrix_2, int size)
{
	// In aceasta functie stocam in matrix_sum suma dintre matrix_1 si matrix_2
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			(*matrix_sum)[i][j] =
				((matrix_1[i][j] % MOD) + (matrix_2[i][j] % MOD)) % MOD;
			(*matrix_sum)[i][j] =
				((*matrix_sum)[i][j] % MOD + MOD) % MOD;
		}
	}
}

void diff_matrix(int ***matrix_diff, int **matrix_1, int **matrix_2, int size)
{
	// In aceasta functie stocam in matrix_diff diferenta dintre matrix_1
	// si matrix_2.
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			(*matrix_diff)[i][j] =
			((matrix_1[i][j] % MOD) - (matrix_2[i][j] % MOD)) % MOD;
			(*matrix_diff)[i][j] =
			((*matrix_diff)[i][j] % MOD + MOD) % MOD;
		}
	}
}

void recomposition(int ***ans, int ***R, int size_matrix)
{
	// In aceasta functie facem compunerea raspunsului de tip matrice
	// din algoritmul lui Strassen.
	for (int i = 0; i < size_matrix; ++i)
		for (int j = 0; j < size_matrix; ++j)
			if (i < size_matrix / 2)
				if (j < size_matrix / 2)
					(*ans)[i][j] = R[0][i][j];
				else
					(*ans)[i][j] = R[1][i][j - size_matrix / 2];
			else if (j < size_matrix / 2)
				(*ans)[i][j] = R[2][i - size_matrix / 2][j];
			else
				(*ans)[i][j] = R[3][i - size_matrix / 2][j - size_matrix / 2];
}

void make_partisions(int ****collection_1, int ****collection_2, int **matrix_1,
					 int **matrix_2, int size_matrix)
{
	// In aceasta functie facem partitiile necesare pentru
	// algoritmul lui Strassen.
	part(&(*collection_1)[0], matrix_1, 0, 0, size_matrix / 2);
	part(&(*collection_1)[1], matrix_1, 0, size_matrix / 2, size_matrix / 2);
	part(&(*collection_1)[2], matrix_1, size_matrix / 2, 0, size_matrix / 2);
	part(&(*collection_1)[3], matrix_1, size_matrix / 2, size_matrix / 2,
		 size_matrix / 2);
	part(&(*collection_2)[0], matrix_2, 0, 0, size_matrix / 2);
	part(&(*collection_2)[1], matrix_2, 0, size_matrix / 2, size_matrix / 2);
	part(&(*collection_2)[2], matrix_2, size_matrix / 2, 0, size_matrix / 2);
	part(&(*collection_2)[3], matrix_2, size_matrix / 2, size_matrix / 2,
		 size_matrix / 2);
}

void fast_multiplication(int ***product, int **matrix_1, int **matrix_2,
						 int size_matrix, int *opperation_failed)
{
	// In aceasta functie realizam inmultirea dintre doua matrice cu Strassen.
	int ***A, ***B, ***R, ***M, **aux1, **aux2;
	if (size_matrix == 1) {
		(*product)[0][0] = matrix_1[0][0] * matrix_2[0][0];
		return;
	}
	alloc_matrix(&aux1, size_matrix / 2, size_matrix / 2, opperation_failed);
	if (*opperation_failed)
		return;
	alloc_matrix(&aux2, size_matrix / 2, size_matrix / 2, opperation_failed);
	if (*opperation_failed) {
		free_matrix(aux1, size_matrix / 2);
		return;
	}
	alloc_entire_collection(&A, 4, size_matrix / 2, size_matrix / 2,
							opperation_failed);
	if (*opperation_failed) {
		free_matrix(aux1, size_matrix / 2);
		free_matrix(aux2, size_matrix / 2);
	}
	alloc_entire_collection(&B, 4, size_matrix / 2, size_matrix / 2,
							opperation_failed);
	if (*opperation_failed) {
		free_matrix(aux1, size_matrix / 2);
		free_matrix(aux2, size_matrix / 2);
		free_matrix_collection(A, 4, size_matrix / 2);
	}
	alloc_entire_collection(&R, 4, size_matrix / 2, size_matrix / 2,
							opperation_failed);
	if (*opperation_failed) {
		free_matrix(aux1, size_matrix / 2);
		free_matrix(aux2, size_matrix / 2);
		free_matrix_collection(A, 4, size_matrix / 2);
		free_matrix_collection(B, 4, size_matrix / 2);
	}
	alloc_entire_collection(&M, 7, size_matrix / 2, size_matrix / 2,
							opperation_failed);
	if (*opperation_failed) {
		free_matrix(aux1, size_matrix / 2);
		free_matrix(aux2, size_matrix / 2);
		free_matrix_collection(A, 4, size_matrix / 2);
		free_matrix_collection(B, 4, size_matrix / 2);
		free_matrix_collection(R, 4, size_matrix / 2);
	}
	make_partisions(&A, &B, matrix_1, matrix_2, size_matrix);
	add_matrix(&aux1, A[0], A[3], size_matrix / 2);
	add_matrix(&aux2, B[0], B[3], size_matrix / 2);
	fast_multiplication(&M[0], aux1, aux2, size_matrix / 2, opperation_failed);
	add_matrix(&aux1, A[2], A[3], size_matrix / 2);
	fast_multiplication(&M[1], aux1, B[0], size_matrix / 2, opperation_failed);
	diff_matrix(&aux1, B[1], B[3], size_matrix / 2);
	fast_multiplication(&M[2], A[0], aux1, size_matrix / 2, opperation_failed);
	diff_matrix(&aux1, B[2], B[0], size_matrix / 2);
	fast_multiplication(&M[3], A[3], aux1, size_matrix / 2, opperation_failed);
	add_matrix(&aux1, A[0], A[1], size_matrix / 2);
	fast_multiplication(&M[4], aux1, B[3], size_matrix / 2, opperation_failed);
	diff_matrix(&aux1, A[2], A[0], size_matrix / 2);
	add_matrix(&aux2, B[0], B[1], size_matrix / 2);
	fast_multiplication(&M[5], aux1, aux2, size_matrix / 2, opperation_failed);
	diff_matrix(&aux1, A[1], A[3], size_matrix / 2);
	add_matrix(&aux2, B[2], B[3], size_matrix / 2);
	fast_multiplication(&M[6], aux1, aux2, size_matrix / 2, opperation_failed);
	add_matrix(&aux1, M[0], M[3], size_matrix / 2);
	diff_matrix(&aux2, aux1, M[4], size_matrix / 2);
	add_matrix(&R[0], aux2, M[6], size_matrix / 2);
	add_matrix(&R[1], M[2], M[4], size_matrix / 2);
	add_matrix(&R[2], M[1], M[3], size_matrix / 2);
	add_matrix(&aux1, M[0], M[2], size_matrix / 2);
	diff_matrix(&aux2, aux1, M[1], size_matrix / 2);
	add_matrix(&R[3], aux2, M[5], size_matrix / 2);
	recomposition(product, R, size_matrix);
	free_matrix(aux1, size_matrix / 2);
	free_matrix(aux2, size_matrix / 2);
	free_matrix_collection(A, 4, size_matrix / 2);
	free_matrix_collection(B, 4, size_matrix / 2);
	free_matrix_collection(R, 4, size_matrix / 2);
	free_matrix_collection(M, 7, size_matrix / 2);
}

void strassen(int ****matrix_collection, int *size_collection, int **lines,
			  int **columns, int *opperation_failed)
{
	int matrix_1, matrix_2;
	scanf("%d%d", &matrix_1, &matrix_2);
	// Verificam daca indecsii matricilor sunt valizi.
	if (matrix_1 < 0 || matrix_1 >= *size_collection || matrix_2 < 0 ||
		matrix_2 >= *size_collection) {
		printf("No matrix with the given index\n");
	} else if ((*lines)[matrix_1] != (*lines)[matrix_2]) {
		printf("Cannot perform matrix multiplication\n");
	} else {
		++(*size_collection);
		// Realocam/Alocam cele necesare pentru o noua matrice, care va retine
		// rezultatul inmultirii celor doua matrice, iar in cazul in care ceva
		// nu a fost corect, iesim din functie (vom dealoca in main).
		realloc_collection(matrix_collection, *size_collection,
						   opperation_failed);
		if (*opperation_failed)
			return;
		alloc_matrix(&(*matrix_collection)[(*size_collection) - 1],
					 (*lines)[matrix_1], (*lines)[matrix_2],
					 opperation_failed);
		if (*opperation_failed)
			return;
		fast_multiplication(&(*matrix_collection)[(*size_collection) - 1],
							(*matrix_collection)[matrix_1],
							(*matrix_collection)[matrix_2],
							(*lines)[matrix_1], opperation_failed);
		if (*opperation_failed)
			return;
		realloc_array(lines, *size_collection, opperation_failed);
		if (*opperation_failed)
			return;
		realloc_array(columns, *size_collection, opperation_failed);
		if (*opperation_failed)
			return;
		(*lines)[*size_collection - 1] = (*lines)[matrix_1];
		(*columns)[*size_collection - 1] = (*lines)[matrix_1];
	}
}

int main(void)
{
	int size_collection = 0, opperation_failed = 0;
	// Initializam memoria necesara
	int *lines, *columns, ***matrix_collection;
	alloc_array(&lines, size_collection + 1, &opperation_failed);
	// Daca ceva a mers prost, dealocam memoria folosita la fiecare alocare.
	if (opperation_failed) {
		printf("Invalid allocation.\n");
		return 0;
	}
	alloc_array(&columns, size_collection + 1, &opperation_failed);
	if (opperation_failed) {
		free(lines);
		printf("Invalid allocation.\n");
		return 0;
	}
	alloc_collection(&matrix_collection, size_collection + 1,
					 &opperation_failed);
	if (opperation_failed) {
		printf("Invalid allocation.\n");
		free(lines);
		free(columns);
		return 0;
	}
	while (1) {
		char op;
		scanf(" %c", &op);
		// Daca vreo operatie nu a mers, dealocam ce am folosit
		// si iesim din program.
		if (opperation_failed) {
			quit(&size_collection, matrix_collection, lines, columns);
			break;
		}
		if (op == 'L') {
			load_matrix(&matrix_collection, &size_collection, &lines,
						&columns, &opperation_failed);
		} else if (op == 'D') {
			print_dimensions(size_collection, lines, columns);
		} else if (op == 'P') {
			print_matrix(matrix_collection, size_collection, lines, columns);
		} else if (op == 'C') {
			crop_matrix(&matrix_collection, size_collection, &lines,
						&columns, &opperation_failed);
		} else if (op == 'M') {
			multiply_matrixes(&matrix_collection, &size_collection, &lines,
							  &columns, &opperation_failed);
		} else if (op == 'Q') {
			quit(&size_collection, matrix_collection, lines, columns);
			break;
		} else if (op == 'T') {
			transpose(&matrix_collection, size_collection, &lines,
					  &columns, &opperation_failed);
		} else if (op == 'R') {
			get_power(&matrix_collection, lines, columns, size_collection,
					  &opperation_failed);
		} else if (op == 'F') {
			erase_matrix(&matrix_collection, &size_collection, &lines,
						 &columns);
		} else if (op == 'O') {
			sort_matrixes(&matrix_collection, &size_collection, &lines,
						  &columns);
		} else if (op == 'S') {
			strassen(&matrix_collection, &size_collection, &lines,
					 &columns, &opperation_failed);
		} else {
			printf("Unrecognized command\n");
		}
	}
	return 0;
}

