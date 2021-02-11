#include <iostream>
#include "ErrorCodes.h"
#include <stdexcept>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

const int DEFAULT_SIZE = 2;
using namespace std;

template< typename T > class Matrix 
{
public:
	Matrix<T>();                                                                     //konstruktor domyslny
	Matrix<T>(int rows, int columns, int* errorInfo);                                //konstruktor na poczatku wypelniony samymi 0
	Matrix<T>(const Matrix<T>& otherMatrix, int* errorInfo);                         //konstruktor kopiujacy
	Matrix<T>(Matrix<T>&& otherMatrix, int* errorInfo);                              //konstruktor przenoszacy
	~Matrix<T>();                                                                    //destruktor

	void createNewMatrix(int rows, int columns, int* errorInfo);                     //tworzenie macierzy o zadanych wymiarach
	Matrix<T> add(const Matrix<T>& other, int* errorInfo);                           //dodawanie macierzy
	Matrix<T> subtract(const Matrix<T>& other, int* errorInfo);                      //odejmowanie macierzy
	Matrix<T> multiply(const Matrix<T>& other, int* errorInfo);                      //mnozenie macierz * macierz
	Matrix<T> multiply(const T number, int* errorInfo);                              //mnozenie macierz * liczba
	T scalarProduct(const Matrix<T>& other, int* errorInfo);                         //iloczyn skalarny
	Matrix<T> transpose(int* errorInfo);                                             //transpozycja
	void setElement(int row, int column, const T newElem, int* errorInfo);           //przypisywanie elementowi macierzy wartosci
	T getElement(int row, int column, int* errorInfo);                               //odczyt danego elementu macierzy
	Matrix<T> createMatrixFromColumn(int column, int* errorInfo);                    //tworzy wektor z kolumny
	Matrix<T> createMatrixFromRow(int row, int* errorInfo);                          //tworzy wektor z wiersza
	void makeIdentityMatrix(int* errorInfo);                                         //zmienia macierz kwadratowa na macierz jednostkowa, dla macierzy nie kwadratowych zmienia kod bledu
	void readMatrixFromFile(string fileName, int* errorInfo);                        //wczytywanie macierzy z pliku - narazie nwm

	Matrix<T> operator+(const Matrix<T>& otherMatrix);                               //dodawanie macierzy
	Matrix<T> operator-(const Matrix<T>& otherMatrix);                               //odejmowanie macierzy
	Matrix<T> operator*(const Matrix<T>& otherMatrix);                               //mnozenie macierz * macierz
	Matrix<T> operator*(const T number);                                             //mnozenie macierz * liczba
	Matrix<T> operator~();                                                           //transpozycja
	void operator()(int row, int column, const T newElem);                           //przypisywanie elementowi macierzy wartosci
	T operator()(int row, int column);                                               //odczyt danego elementu macierzy

	void operator=(const Matrix<T>& otherMatrix);                                    //operator przyrownania
	void operator=(Matrix<T>&& otherMatrix);                                         //operator przenoszacy
	void display();                                                                  //do testow
	int getRowsAmount();
	int getColumnAmount();


private:
	T** matrix;
	int rowsAmount;
	int columnsAmount;

	void createMatrix(int rows, int columns, int* errorInfo);
	void removeMatrix();
	void copyMatrix(const Matrix<T>& otherMatrix, int* errorInfo);
	void moveMatrix(Matrix<T>& otherMatrix, int* errorInfo);
	Matrix<T>(const Matrix<T>& otherMatrix);                                         //konstruktor kopiujacy bez info o error kodzie
	Matrix<T>(Matrix<T>&& otherMatrix);                                              //konstruktor przenoszacy bez info o error kodzie
	bool commaValidator(string& number);
};


// ************************************************** implementaje **************************************************
// ****************************** konstruktory i destruktory ******************************
template <typename T>
Matrix<T>::Matrix() {
	int errorCode = NO_ERRORS;
	createMatrix(DEFAULT_SIZE, DEFAULT_SIZE, &errorCode);
	//cout << "Create " << DEFAULT_SIZE << " x " << DEFAULT_SIZE << " matrix" << endl; // napis debugowy
}


template <typename T>
Matrix<T>::Matrix(int rows, int columns, int* errorInfo) {
	*errorInfo = NO_ERRORS; // resetowanie po poprzedniej operacji - poprzednim uzyciu
	createMatrix(rows, columns, errorInfo);
	//if(*errorInfo == NO_ERRORS) cout << "Create " << rowsAmount << " x " << columnsAmount << " matrix" << endl; // napis debugowy
}


template <typename T>
Matrix<T>::Matrix(const Matrix<T>& otherMatrix, int* errorInfo) {
	*errorInfo = NO_ERRORS; // resetowanie po poprzedniej operacji - poprzednim uzyciu
	copyMatrix(otherMatrix, errorInfo);
	//if (*errorInfo == NO_ERRORS) cout << "Copy " << rowsAmount << " x " << columnsAmount << " matrix" << endl; // napis debugowy
}


template <typename T>
Matrix<T>::Matrix(Matrix<T>&& otherMatrix, int* errorInfo) {
	*errorInfo = NO_ERRORS; // resetowanie po poprzedniej operacji - poprzednim uzyciu
	moveMatrix(otherMatrix, errorInfo);
	//if (*errorInfo == NO_ERRORS) cout << "Move " << rowsAmount << " x " << columnsAmount << " matrix" << endl; // napis debugowy
}


template <typename T>
Matrix<T>::~Matrix() {
	removeMatrix();
	//cout << "Delete " << rowsAmount << " x " << columnsAmount << " matrix" << endl; // napis debugowy
}


// ****************************** metody ******************************
template <typename T>
void Matrix<T>::createNewMatrix(int rows, int columns, int* errorInfo) {
	*errorInfo = NO_ERRORS; // resetowanie po poprzedniej operacji - poprzednim uzyciu
	if (rows <= 0 || columns <= 0) {
		removeMatrix();
		*errorInfo = INCORRECT_MARTIX_SIZE_GIVEN_MATRIX_NOT_CREATED;
		rowsAmount = 0;
		columnsAmount = 0;
		matrix = nullptr;
	}
	else {
		T** newMatrix = new T * [rows];
		for (int i = 0; i < rows; i++) {
			newMatrix[i] = new T[columns];
		}

		if (rows <= rowsAmount && columns <= columnsAmount) {//taka sama lub mniejsza
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < columns; j++) {
					newMatrix[i][j] = matrix[i][j];
				}
			}
			
		}
		else if (rows <= rowsAmount && columns > columnsAmount) {//ma wiecej kolumn
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < columns; j++) {
					if (j < columnsAmount) 
						newMatrix[i][j] = matrix[i][j];
					else 
						newMatrix[i][j] = 0;
				}
			}
		}
		else if (rows > rowsAmount && columns <= columnsAmount) {//ma wiecej wierszy
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < columns; j++) {
					if (i < rowsAmount) 
						newMatrix[i][j] = matrix[i][j];
					else 
						newMatrix[i][j] = 0;
				}
			}
		}
		else {
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < columns; j++) {
					if (j < columnsAmount && i < rowsAmount) 
						newMatrix[i][j] = matrix[i][j];
					else 
						newMatrix[i][j] = 0;
				}
			}
		}

		removeMatrix();
		matrix = newMatrix;
		rowsAmount = rows;
		columnsAmount = columns;
	}
}


template <typename T>
Matrix<T> Matrix<T>::add(const Matrix<T>& other, int* errorInfo) {
	*errorInfo = NO_ERRORS; // resetowanie po poprzedniej operacji - poprzednim uzyciu
	if (other.matrix == nullptr || this->matrix == nullptr) {
		Matrix<T> invalidMatrix(0, 0, errorInfo);
		*errorInfo = GIVEN_MATRIX_DOES_NOT_EXIST;
		return move(invalidMatrix);
	}
	else if (other.columnsAmount != columnsAmount || other.rowsAmount != rowsAmount) {
		Matrix<T> invalidMatrix(0, 0, errorInfo);
		*errorInfo = INVALID_MATRIX_WAS_GIVEN_MATRIX_NOT_CREATED;
		return move(invalidMatrix);
	}
	else {
		int error = NO_ERRORS;
		Matrix<T> retMatrix(rowsAmount, columnsAmount, errorInfo);
		for (int i = 0; i < rowsAmount; i++) {
			for (int j = 0; j < columnsAmount; j++) {
				T elemOne = getElement(i, j, &error);
				T elemTwo = other.matrix[i][j];
				retMatrix.setElement(i, j, elemOne + elemTwo, &error);
			}
		}
		return move(retMatrix);
	}
}


template <typename T>
Matrix<T> Matrix<T>::subtract(const Matrix<T>& other, int* errorInfo) {
	*errorInfo = NO_ERRORS; // resetowanie po poprzedniej operacji - poprzednim uzyciu
	if (other.matrix == nullptr || this->matrix == nullptr) {
		Matrix<T> invalidMatrix(0, 0, errorInfo);
		*errorInfo = GIVEN_MATRIX_DOES_NOT_EXIST;
		return move(invalidMatrix);
	}
	else if (other.columnsAmount != columnsAmount || other.rowsAmount != rowsAmount) {
		Matrix<T> invalidMatrix(0, 0, errorInfo);
		*errorInfo = INVALID_MATRIX_WAS_GIVEN_MATRIX_NOT_CREATED;
		return move(invalidMatrix);
	}
	else {
		int error = NO_ERRORS;
		Matrix<T> retMatrix(rowsAmount, columnsAmount, errorInfo);
		for (int i = 0; i < rowsAmount; i++) {
			for (int j = 0; j < columnsAmount; j++) {
				T elemOne = getElement(i, j, &error);
				T elemTwo = other.matrix[i][j];
				retMatrix.setElement(i, j, elemOne - elemTwo, &error);
			}
		}
		return move(retMatrix);
	}
}


template <typename T>
Matrix<T> Matrix<T>::multiply(const Matrix<T>& other, int* errorInfo) {
	*errorInfo = NO_ERRORS; // resetowanie po poprzedniej operacji - poprzednim uzyciu
	if (other.matrix == nullptr || this->matrix == nullptr) {
		Matrix<T> invalidMatrix(0, 0, errorInfo);
		*errorInfo = GIVEN_MATRIX_DOES_NOT_EXIST;
		return move(invalidMatrix);
	}
	else if (other.rowsAmount != columnsAmount) {
		Matrix<T> invalidMatrix(0, 0, errorInfo);
		*errorInfo = INVALID_MATRIX_WAS_GIVEN_MATRIX_NOT_CREATED;
		return move(invalidMatrix);
	}
	else {
		int error = NO_ERRORS;
		Matrix<T> retMatrix(rowsAmount, other.columnsAmount, errorInfo);
		for (int i = 0; i < rowsAmount; i++) {
			for (int j = 0; j < other.columnsAmount; j++) {
				for (int k = 0; k < columnsAmount; k++) {
					T elemOne = other.matrix[k][j];
					T elemTwo = matrix[i][k];
					T currentValue = retMatrix.getElement(i, j, &error);
					retMatrix.setElement(i, j, currentValue + (elemTwo * elemOne), &error);
				}
			}
		}
		return move(retMatrix);
	}
}


template <typename T>
Matrix<T> Matrix<T>::multiply(const T number, int* errorInfo) {
	*errorInfo = NO_ERRORS; // resetowanie po poprzedniej operacji - poprzednim uzyciu
	if (this->matrix == nullptr) {
		Matrix<T> invalidMatrix(0, 0, errorInfo);
		*errorInfo = GIVEN_MATRIX_DOES_NOT_EXIST;
		return move(invalidMatrix);
	}
	else {
		int error = NO_ERRORS;
		Matrix<T> retMatrix(rowsAmount, columnsAmount, errorInfo);
		for (int i = 0; i < rowsAmount; i++) {
			for (int j = 0; j < columnsAmount; j++) {
				T elem = matrix[i][j];
				retMatrix.setElement(i, j, elem * number, &error);
			}
		}
		return move(retMatrix);
	}
}


template <typename T>
T Matrix<T>::scalarProduct(const Matrix<T>& other, int* errorInfo) {
	*errorInfo = NO_ERRORS; // resetowanie po poprzedniej operacji - poprzednim uzyciu
	if (matrix == nullptr || other.matrix == nullptr) {
		*errorInfo = GIVEN_MATRIX_DOES_NOT_EXIST;
		return NAN;
	}
	else if (rowsAmount != 1 || other.rowsAmount != 1 || columnsAmount != other.columnsAmount) {
		*errorInfo = INCORRECT_MARTIX_SIZE_GIVEN;
		return NAN;
	}
	else {
		T scalarProduct = 0;
		for (int i = 0; i < columnsAmount; i++) {
			scalarProduct += matrix[0][i] * other.matrix[0][i];
		}
		return scalarProduct;
	}
}


template <typename T>
Matrix<T> Matrix<T>::transpose(int* errorInfo) {
	*errorInfo = NO_ERRORS; // resetowanie po poprzedniej operacji - poprzednim uzyciu
	if (this->matrix == nullptr) {
		Matrix<T> invalidMatrix(0, 0, errorInfo);
		*errorInfo = GIVEN_MATRIX_DOES_NOT_EXIST;
		return move(invalidMatrix);
	}
	else {
		int error = NO_ERRORS;
		Matrix<T> transposed(columnsAmount, rowsAmount, errorInfo);
		for (int i = 0; i < rowsAmount; i++) {
			for (int j = 0; j < columnsAmount; j++) {
				transposed.setElement(j, i, this->getElement(i, j, &error), errorInfo);
			}
		}
		return move(transposed);
	}
}



template <typename T>
void Matrix<T>::setElement(int row, int column, const T newElem, int* errorInfo) {
	*errorInfo = NO_ERRORS; // resetowanie po poprzedniej operacji - poprzednim uzyciu
	if (this->matrix == nullptr) {
		*errorInfo = GIVEN_MATRIX_DOES_NOT_EXIST;
	}
	else if (row < 0 || row >= rowsAmount || column < 0 || column >= columnsAmount) {
		*errorInfo = INCORRECT_MARTIX_SIZE_GIVEN_MATRIX_EXISTS;
	}
	else {
		matrix[row][column] = newElem;
	}
}


template <typename T>
T Matrix<T>::getElement(int row, int column, int* errorInfo) {
	*errorInfo = NO_ERRORS; // resetowanie po poprzedniej operacji - poprzednim uzyciu
	if (this->matrix == nullptr) {
		*errorInfo = GIVEN_MATRIX_DOES_NOT_EXIST;
		return NAN;
	}
	else if (row < 0 || row >= rowsAmount || column < 0 || column >= columnsAmount) {
		*errorInfo = INCORRECT_MARTIX_SIZE_GIVEN_MATRIX_EXISTS;
		return NAN;
	}
	else {
		return matrix[row][column];
	}
}


template <typename T>
Matrix<T> Matrix<T>::createMatrixFromColumn(int column, int* errorInfo) {
	*errorInfo = NO_ERRORS; // resetowanie po poprzedniej operacji - poprzednim uzyciu
	if (this->matrix == nullptr) {
		Matrix<T> invalidMatrix(0, 0, errorInfo);
		*errorInfo = GIVEN_MATRIX_DOES_NOT_EXIST;
		return move(invalidMatrix);
	}
	else if (column < 0 || column >= columnsAmount) {
		Matrix<T> invalidMatrix(0, 0, errorInfo);
		*errorInfo = INVALID_MATRIX_WAS_GIVEN_MATRIX_NOT_CREATED;
		return move(invalidMatrix);
	}
	else {
		int innerErrorInfo = NO_ERRORS;
		Matrix<T> returnMatrix(rowsAmount, 1, &innerErrorInfo);
		for (int i = 0; i < rowsAmount; i++) {
			returnMatrix.setElement(i, 0, this->getElement(i, column, &innerErrorInfo), &innerErrorInfo);
		}
		return move(returnMatrix);
	}
}


template <typename T>
Matrix<T> Matrix<T>::createMatrixFromRow(int row, int* errorInfo) {
	*errorInfo = NO_ERRORS; // resetowanie po poprzedniej operacji - poprzednim uzyciu
	if (this->matrix == nullptr) {
		Matrix<T> invalidMatrix(0, 0, errorInfo);
		*errorInfo = GIVEN_MATRIX_DOES_NOT_EXIST;
		return move(invalidMatrix);
	}
	else if (row < 0 || row >= rowsAmount) {
		Matrix<T> invalidMatrix(0, 0, errorInfo);
		*errorInfo = INVALID_MATRIX_WAS_GIVEN_MATRIX_NOT_CREATED;
		return move(invalidMatrix);
	}
	else {
		int innerErrorInfo = NO_ERRORS;
		Matrix<T> returnMatrix(1, columnsAmount, &innerErrorInfo);
		for (int i = 0; i < columnsAmount; i++) {
			returnMatrix.setElement(0, i, this->getElement(row, i, &innerErrorInfo), &innerErrorInfo);
		}
		return move(returnMatrix);
	}
}


template <typename T>
void Matrix<T>::makeIdentityMatrix(int* errorInfo) {
	*errorInfo = NO_ERRORS; // resetowanie po poprzedniej operacji - poprzednim uzyciu
	if (this->matrix == nullptr) {
		*errorInfo = GIVEN_MATRIX_DOES_NOT_EXIST;
	}
	else if (rowsAmount != columnsAmount) {
		*errorInfo = INCORRECT_MATRIX_SIZE_GIVEN_MATRIX_NOT_CHANGED;
	}
	else {
		for (int i = 0; i < rowsAmount; i++) {
			for (int j = 0; j < columnsAmount; j++) {
				if (i == j) matrix[i][j] = 1;
				else matrix[i][j] = 0;
			}
		}
	}
}


template <typename T>
void Matrix<T>::readMatrixFromFile(string fileName, int* errorInfo) {
	*errorInfo = NO_ERRORS; // resetowanie po poprzedniej operacji - poprzednim uzyciu
	string line;
	fstream file;
	string element;
	int rowsNumber = 0;
	int maxColumnsAmount = 0;
	int elementsNumber = 0;
	vector<string> elementsFromFile;
	vector<int> amountOfElementsInRow;

	removeMatrix(); // usuwam obecna macierz

	file.open(fileName);
	if (!file) {                               
		*errorInfo = CAN_NOT_OPEN_FILE_MATRIX_NOT_CREATED;
		rowsAmount = 0;
		columnsAmount = 0;
		matrix = nullptr;
	}
	else {                                     
		while (!file.eof()) {                  
			int elementsInRowCounter = 0;
			rowsNumber++;
			getline(file, line);
			stringstream stream(line);
			while (getline(stream, element, ' ')) {
				if (element != "") { // w przypadku gdy ktos przez przypadek da np 2 spacje zamiast 1 miedzy liczbami w pliku
					elementsNumber++;
					elementsInRowCounter++;
					elementsFromFile.push_back(element);
				}
				
			}
			amountOfElementsInRow.push_back(elementsInRowCounter);
			if (elementsInRowCounter > maxColumnsAmount)
				maxColumnsAmount = elementsInRowCounter;
		}
		file.close();

		createMatrix(rowsNumber, maxColumnsAmount, errorInfo);

		if (*errorInfo == NO_ERRORS) {
			int index = 0;                                                  
			for (int i = 0; i < rowsAmount; i++) {
				int columnsNumberInRow = amountOfElementsInRow.at(i);
				for (int j = 0; j < columnsAmount; j++) {
					if (j < columnsNumberInRow) {                          
						try {
							bool ifCorrect = commaValidator(elementsFromFile.at(index));
							if (!ifCorrect) {
								//cout << "ifcorrect" << endl; - debug
								throw invalid_argument(" ");
							}
							else {
								matrix[i][j] = (T)stod(elementsFromFile.at(index));
							}
						}
						catch (const invalid_argument& ex) {               
							matrix[i][j] = 0;
							*errorInfo = INVALID_ARGUMENT_IN_FILE;
						}
						index++;
					}
					else {                                                   
						matrix[i][j] = 0;
					}
				}
			}
		}
	}
}

// ****************************** operatory ******************************
template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& otherMatrix) {
	int errorCode = NO_ERRORS;
	Matrix retMatrix(0, 0, &errorCode);
	retMatrix = this->add(otherMatrix, &errorCode);
	if (errorCode != NO_ERRORS) throw invalid_argument(to_string(errorCode));
	else return move(retMatrix);
}


template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& otherMatrix) {
	int errorCode = NO_ERRORS;
	Matrix retMatrix(0, 0, &errorCode);
	retMatrix = this->subtract(otherMatrix, &errorCode);
	if (errorCode != NO_ERRORS) throw invalid_argument(to_string(errorCode));
	else return move(retMatrix);
}


template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& otherMatrix) {
	int errorCode = NO_ERRORS;
	Matrix retMatrix(0, 0, &errorCode);
	retMatrix = this->multiply(otherMatrix, &errorCode);
	if (errorCode != NO_ERRORS) throw invalid_argument(to_string(errorCode));
	else return move(retMatrix);
}


template <typename T>
Matrix<T> Matrix<T>::operator*(const T number) {
	int errorCode = NO_ERRORS;
	Matrix retMatrix(0, 0, &errorCode);
	retMatrix = this->multiply(number, &errorCode);
	if (errorCode != NO_ERRORS) throw invalid_argument(to_string(errorCode));
	else return move(retMatrix);
}


template <typename T>
Matrix<T> Matrix<T>::operator~() {
	int errorCode = NO_ERRORS;
	Matrix retMatrix(0, 0, &errorCode);
	retMatrix = this->transpose(&errorCode);
	if (errorCode != NO_ERRORS) throw invalid_argument(to_string(errorCode));
	else return move(retMatrix);
}


template <typename T>
void Matrix<T>::operator()(int row, int column, const T newElem) {
	int errorCode = NO_ERRORS;
	this->setElement(row, column, newElem, &errorCode);
	if (errorCode != NO_ERRORS) throw invalid_argument(to_string(errorCode));
}


template <typename T>
T Matrix<T>::operator()(int row, int column) {
	int errorCode = NO_ERRORS;
	T retElem = this->getElement(row, column, &errorCode);
	if (errorCode != NO_ERRORS) throw invalid_argument(to_string(errorCode));
	else return retElem;
}



// ****************************** inne ******************************
template <typename T>
void Matrix<T>::operator=(const Matrix<T>& otherMatrix) {
	if (this != &otherMatrix) {
		removeMatrix();
		int errorCode = NO_ERRORS;
		copyMatrix(otherMatrix, &errorCode);
		if (errorCode != NO_ERRORS) throw invalid_argument(to_string(errorCode));
		//cout << "copy = operator" << endl; // napis debugowy
	}
}


template <typename T>
void Matrix<T>::operator=(Matrix<T>&& otherMatrix) {
	if (this != &otherMatrix) {
		removeMatrix();
		int errorCode = NO_ERRORS;
		moveMatrix(otherMatrix, &errorCode);
		if (errorCode != NO_ERRORS) throw invalid_argument(to_string(errorCode));
		//cout << "move = operator" << endl; // napis debugowy
	}
}


template <typename T>
void Matrix<T>::display() {
	for (int i = 0; i < rowsAmount; i++) {
		for (int j = 0; j < columnsAmount; j++) {
			cout << matrix[i][j] << "  ";
		}
		cout << endl;
	}
}


template <typename T>
int Matrix<T>::getRowsAmount() { return rowsAmount; }

template <typename T>
int Matrix<T>::getColumnAmount() { return columnsAmount; }



// ****************************** prywatne metody ******************************
template <typename T>
void Matrix<T>::createMatrix(int rows, int columns, int* errorInfo) {
	if (rows <= 0 || columns <= 0) {
		*errorInfo = INCORRECT_MARTIX_SIZE_GIVEN_MATRIX_NOT_CREATED;
		rowsAmount = 0;
		columnsAmount = 0;
		matrix = nullptr;
	}
	else {
		rowsAmount = rows;
		columnsAmount = columns;
		matrix = new T * [rows];
		for (int i = 0; i < rows; i++) {
			matrix[i] = new T[columns];
		}
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				matrix[i][j] = 0;
			}
		}
	}
}


template <typename T>
void Matrix<T>::removeMatrix() {
	if (matrix != nullptr) {
		for (int i = 0; i < rowsAmount; i++) {
			delete[] matrix[i];
		}
		delete[] matrix;
		rowsAmount = 0;
		columnsAmount = 0;
	}
}


template <typename T>
void Matrix<T>::copyMatrix(const Matrix<T>& otherMatrix, int* errorInfo) {
	if (otherMatrix.matrix == nullptr) {
		*errorInfo = INVALID_MATRIX_WAS_GIVEN_MATRIX_NOT_CREATED;
		rowsAmount = 0;
		columnsAmount = 0;
		matrix = nullptr;
	}
	else {
		rowsAmount = otherMatrix.rowsAmount;
		columnsAmount = otherMatrix.columnsAmount;
		matrix = new T * [otherMatrix.rowsAmount];
		for (int i = 0; i < rowsAmount; i++) {
			matrix[i] = new T[columnsAmount];
		}
		for (int i = 0; i < rowsAmount; i++) {
			for (int j = 0; j < columnsAmount; j++) {
				matrix[i][j] = otherMatrix.matrix[i][j];
			}
		}
	}
}


template <typename T>
void Matrix<T>::moveMatrix(Matrix<T>& otherMatrix, int* errorInfo) {
	if (otherMatrix.matrix == nullptr) {
		*errorInfo = INVALID_MATRIX_WAS_GIVEN_MATRIX_NOT_CREATED;
		rowsAmount = 0;
		columnsAmount = 0;
		matrix = nullptr;
	}
	else {
		matrix = otherMatrix.matrix;
		rowsAmount = otherMatrix.rowsAmount;
		columnsAmount = otherMatrix.columnsAmount;
		otherMatrix.matrix = nullptr;
		otherMatrix.rowsAmount = 0;
		otherMatrix.columnsAmount = 0;
	}
}


template <typename T>
Matrix<T>::Matrix(const Matrix<T>& otherMatrix) {
	int err = NO_ERRORS;
	copyMatrix(otherMatrix, &err);
	//if(err == 0) cout << "Copy " << rowsAmount << " x " << columnsAmount << " matrix" << endl; // napis debugowy
}


template <typename T>
Matrix<T>::Matrix(Matrix<T>&& otherMatrix) {
	int err = NO_ERRORS;
	moveMatrix(otherMatrix, &err);
	//if(err == 0) cout << "Move " << rowsAmount << " x " << columnsAmount << " matrix" << endl; // napis debugowy
}


template <typename T>
bool Matrix<T>::commaValidator(string& number) {
	for (int i = 0; i < number.length(); i++) {
		if (number[i] == ',') {
			number[i] = '.';
		}
		char c = number[i];
		if (!(std::isspace(c) || std::isdigit(c) || c == '.'))
			return false;
	}
	return true;
}


