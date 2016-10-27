// SquareMatrixMultiplication.cpp : Defines the entry point for the console application.
//
#include <stdlib.h>
#include <omp.h>
#include "stdafx.h"
#include "timers.h"

#define matrixSize 320
#define numThreads 8

void sequential_matrix_mult(int **, int **, int **);
void openmp_matrix_mult(int **, int **, int **);
void timing_wrapper(void(*func)(int **, int **, int **), int **, int **, int **, const char *);
void reset_matrix(int **, int);

int main()
{
	int **A = (int **)malloc(matrixSize*sizeof(int *));
	int **B = (int **)malloc(matrixSize*sizeof(int *));
	int **C = (int **)malloc(matrixSize*sizeof(int *));

	for (int i = 0; i < matrixSize; i++) {
		A[i] = (int *)malloc(matrixSize*sizeof(int));
		B[i] = (int *)malloc(matrixSize*sizeof(int));
		C[i] = (int *)malloc(matrixSize*sizeof(int));
		for (int j = 0; j < matrixSize; j++) {
			A[i][j] = 1;
			B[i][j] = 1;
			C[i][j] = 0;
		}
	}

	//do work here
	timing_wrapper(sequential_matrix_mult, A, B, C, "Sequential");
	reset_matrix(C, 0);
	timing_wrapper(openmp_matrix_mult, A, B, C, "OpenMP");

	for (int i = 0; i < matrixSize; i++) {
		free(A[i]);
		free(B[i]);
		free(C[i]);
	}
	free(A);
	free(B);
	free(C);

	system("PAUSE");
    return 0;
}

void openmp_matrix_mult(int **A, int **B, int **C) {
	omp_set_dynamic(0);

#pragma omp parallel for num_threads(numThreads)
	for (int rowA = 0; rowA < matrixSize; rowA++) {
		for (int colB = 0; colB < matrixSize; colB++) {
			C[rowA][colB] = 0;
			for (int elem = 0; elem < matrixSize; elem++) {
				C[rowA][colB] += A[rowA][elem] * B[elem][colB];
			}
		}
	}
}

void sequential_matrix_mult(int **A, int **B, int **C) {
	for (int rowA = 0; rowA < matrixSize; rowA++) {
		for (int colB = 0; colB < matrixSize; colB++) {
			C[rowA][colB] = 0;
			for (int elem = 0; elem < matrixSize; elem++) {
				C[rowA][colB] += A[rowA][elem] * B[elem][colB];
			}
		}
	}
}

void timing_wrapper(void(*func)(int **, int **, int **), int **A, int **B, int **C, const char *desc) {
	StartCounter();
	(*func)(A, B, C);
	printf("%s time: %f\n", desc, GetCounter());
}

void reset_matrix(int **matrix, int val) {
	for (int row = 0; row < matrixSize; row++) {
		for (int col = 0; col < matrixSize; col++) {
			matrix[row][col] = val;
		}
	}
}