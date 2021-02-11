#include <iostream>
#include "RandomMatrixGenerator.h"
#include <stdlib.h>
#include <random>
#include <time.h>

using namespace std;

void randomIntMatrixGenerator(Matrix<int>& matrix) {
	srand(time(NULL));
	for (int i = 0; i < matrix.getRowsAmount(); i++) {
		for (int j = 0; j < matrix.getColumnAmount(); j++) {
			try {
				matrix(i, j, rand() % 100 - 49);
			}
			catch(const invalid_argument& ex) {
				ex.what();
			}
		}
	}
}


void randomDoubleMatrixGenerator(Matrix<double>& matrix) {
	std::default_random_engine re;
	std::uniform_real_distribution<double> unif(-50, 50);
	for (int i = 0; i < matrix.getRowsAmount(); i++) {
		for (int j = 0; j < matrix.getColumnAmount(); j++) {
			try {
				matrix(i, j, unif(re));
			}
			catch (const invalid_argument& ex) {
				ex.what();
			}
		}
	}
}


void randomFloatMatrixGenerator(Matrix<float>& matrix) {
	std::default_random_engine re;
	std::uniform_real_distribution<float> unif(-50, 50);
	for (int i = 0; i < matrix.getRowsAmount(); i++) {
		for (int j = 0; j < matrix.getColumnAmount(); j++) {
			try {
				matrix(i, j, unif(re));
			}
			catch (const invalid_argument& ex) {
				ex.what();
			}
		}
	}
}

