#include <iostream>
//#include "Matrix.h"
#include "RandomMatrixGenerator.h"
#include "TestMethods.h"
using namespace std;


int main()
{
    setAndGetElementTests();
    copyAndMoveConstructorsTest();
    equalsTest();
    createMatrixFromColumnAndRowTest();
    transposeTest();
    addTest();
    subtractTest();
    multiplyMatricesTest();
    multiplyMatrixByNymberTest();
    makeIndentityMatrixTest();
    createNewMatrixTest();
    readMatrixFromFileTest();
    scalarProductTest();
    
    
    usageTestOne();
    usageTestTwo();
    usageTestThree();


}

void usageTestOne() {

    int errorInfoFirstMatrix = 0;
    Matrix<int> firstMatrix(1, 1, &errorInfoFirstMatrix);
    firstMatrix.readMatrixFromFile("IntMatrix.txt", &errorInfoFirstMatrix);
    if (errorInfoFirstMatrix == NO_ERRORS || errorInfoFirstMatrix == INVALID_ARGUMENT_IN_FILE) 
        firstMatrix.display();

    cout << endl;

    int errorInfoSecondMatrix = 0;
    Matrix<int> secondMatrix(1, 1, &errorInfoSecondMatrix);
    secondMatrix.readMatrixFromFile("MatrixInt.txt", &errorInfoSecondMatrix);
    if (errorInfoSecondMatrix == NO_ERRORS || errorInfoSecondMatrix == INVALID_ARGUMENT_IN_FILE) 
        secondMatrix.display();

    if ((errorInfoFirstMatrix == NO_ERRORS || errorInfoFirstMatrix == INVALID_ARGUMENT_IN_FILE) && (errorInfoSecondMatrix == NO_ERRORS || errorInfoSecondMatrix == INVALID_ARGUMENT_IN_FILE)) {
        int resultIntError = 0;
        Matrix<int> resultMatrix;
        resultMatrix = firstMatrix.multiply(secondMatrix, &resultIntError);
        if (resultIntError == 0) {
            cout << endl;
            resultMatrix.display();
        }
        else {
            cout << "ERROR - CODE: " << resultIntError << endl;
        }
    }
}


void usageTestTwo() {
    int errorInfoFirstMatrix = 0;
    Matrix<int> firstMatrix(1, 1, &errorInfoFirstMatrix);
    firstMatrix.readMatrixFromFile("MatrixInt.txt", &errorInfoFirstMatrix);
    if (errorInfoFirstMatrix == NO_ERRORS || errorInfoFirstMatrix == INVALID_ARGUMENT_IN_FILE)
        firstMatrix.display();

    cout << endl;

    int errorInfoSecondMatrix = 0;
    Matrix<int> secondMatrix(6, 2, &errorInfoSecondMatrix);
    if (errorInfoSecondMatrix == 0) {
        try {
            secondMatrix(0, 0, 5);
            secondMatrix(0, 1, 11);
            secondMatrix(1, 0, 2);
            secondMatrix(1, 1, 0);
            secondMatrix(2, 0, 9);
            secondMatrix(2, 1, -6);
            secondMatrix(3, 0, 12);
            secondMatrix(3, 1, 3);
            secondMatrix(4, 0, -11);
            secondMatrix(4, 1, 4);
            secondMatrix(5, 0, 77);
            secondMatrix(5, 1, 2);
        }
        catch (const invalid_argument& ex) {
            cout << "ERROR - CODE: " << ex.what() << endl;
        }
        secondMatrix.display();
    }

    cout << endl;

    if ((errorInfoFirstMatrix == NO_ERRORS || errorInfoFirstMatrix == INVALID_ARGUMENT_IN_FILE) && errorInfoSecondMatrix == 0) {
        int resultIntError = 0;
        Matrix<int> resultMatrix;
        try {
            resultMatrix = firstMatrix.add(secondMatrix, &resultIntError);
        }
        catch (const invalid_argument& ex) {
            cout << ex.what();
        }

        if (resultIntError == 0) 
            resultMatrix.display();
    }
    cout << endl;
}

void usageTestThree() {
    int errorInfoFirstMatrix = 0;
    Matrix<int> firstMatrix(1, 1, &errorInfoFirstMatrix);
    firstMatrix.readMatrixFromFile("IntMatrix.txt", &errorInfoFirstMatrix);
    if (errorInfoFirstMatrix == NO_ERRORS || errorInfoFirstMatrix == INVALID_ARGUMENT_IN_FILE)
        firstMatrix.display();

    cout << endl;

    
    if (errorInfoFirstMatrix == NO_ERRORS || errorInfoFirstMatrix == INVALID_ARGUMENT_IN_FILE) {
        int firstRowVectorErrorInfo = 0;
        Matrix<int> firstRowVector(0, 0, &firstRowVectorErrorInfo);
        firstRowVector = firstMatrix.createMatrixFromRow(0, &firstRowVectorErrorInfo);
        if (firstRowVectorErrorInfo == 0)
            firstRowVector.display();

        cout << endl;

        int secondRowVectorErrorInfo = 0;
        Matrix<int> secondRowVector(0, 0, &secondRowVectorErrorInfo);
        secondRowVector = firstMatrix.createMatrixFromRow(1, &secondRowVectorErrorInfo);
        if (secondRowVectorErrorInfo == 0)
            secondRowVector.display();

        cout << endl;

        if (firstRowVectorErrorInfo == 0 && secondRowVectorErrorInfo == 0) {
            int error = 0;
            cout << "Scalar product: " << firstRowVector.scalarProduct(secondRowVector, &error);
        }
    }
}







void setAndGetElementTests() {
    cout << "Set element method test" << endl;
    int errorOfMatrixDouble = 0;
    Matrix<double> matrixOfDouble; //domyslnie tworzy macierz 2x2
    matrixOfDouble.setElement(0, 0, 4.4, &errorOfMatrixDouble);
    matrixOfDouble.display();
    matrixOfDouble.setElement(0, 4, 5.5, &errorOfMatrixDouble);
    if (errorOfMatrixDouble == 0) {
        cout << "Matrix was chenged successfully" << endl;
    }
    else if(errorOfMatrixDouble == 2){
        cout << "Matrix was not chenged - invalid arguments" << endl;
    }
    else {
        cout << "Unknown error" << endl;
    }
    cout << endl; cout << endl;
    

    cout << "Set element operator test" << endl;
    matrixOfDouble(0, 0, 1.5);
    matrixOfDouble(0, 1, 8.625);
    matrixOfDouble(1, 0, 5.25);
    matrixOfDouble(1, 1, 4.35);
    matrixOfDouble.display();

    try {
        matrixOfDouble(0, 5, 7.7);
    }
    catch (const invalid_argument& ex) {
        cout << ex.what();
    }
    cout << endl; cout << endl;


    cout << "Get element method test" << endl;
    double elem1 = matrixOfDouble.getElement(0, 0, &errorOfMatrixDouble);
    if (errorOfMatrixDouble == 0) {
        cout << "Element at (0,0) is: " << elem1 << endl;
    }
    else if (errorOfMatrixDouble == 2) {
        cout << "Invalid arguments - matrix has no such a position" << endl;
    }
    else {
        cout << "Unknown error" << endl;
    }

    double elem2 = matrixOfDouble.getElement(-2, 0, &errorOfMatrixDouble);
    if (errorOfMatrixDouble == 0) {
        cout << "Element at (-2,0) is: " << elem2 << endl;
    }
    else if (errorOfMatrixDouble == 2) {
        cout << "Invalid arguments - matrix has no such a position" << endl;
    }
    else {
        cout << "Unknown error" << endl;
    }
    cout << endl; cout << endl;


    cout << "Get element operator test" << endl;
    double elem3 = matrixOfDouble(0, 1);
    cout << "Element at (0,1) is: " << elem3 << endl;

    try {
        matrixOfDouble(0, 5);
    }
    catch (const invalid_argument& ex) {
        cout << ex.what();
    }

    cout << endl;
    cout << "End of setAndGetElementTests()" << endl;
    cout << endl;
}


void copyAndMoveConstructorsTest() {
    cout << "Copy constructor test" << endl;
    int errorOfMatrixInt = 0;
    Matrix<int> matrixInt(2, 3, &errorOfMatrixInt);
    try {
        matrixInt(0, 0, 7);
        matrixInt(0, 1, 1);
        matrixInt(0, 2, -5);
        matrixInt(1, 0, 2);
        matrixInt(1, 1, 3);
        matrixInt(1, 2, 9);
        matrixInt.display();
    }
    catch (const invalid_argument& ex) {
        cout << ex.what();
    }

    int errorOfResultIntMatrix = 0;
    Matrix<int> resultIntMatrix(matrixInt, &errorOfResultIntMatrix);
    if(errorOfResultIntMatrix == 0) resultIntMatrix.display();
    

    int errorOfDoubleMatrix = 0;
    Matrix<double> doubleMatrix(-3, 4, &errorOfDoubleMatrix);

    int errorOfResultDoubleMatrix = 0;
    Matrix<double> resultDoubeMatrix(doubleMatrix, &errorOfResultDoubleMatrix);
    if (errorOfResultDoubleMatrix == 3) cout << "Incorrect matrix given to copy constructor. Matrix was not created" << endl;
    cout << endl; cout << endl;


    cout << "Move constructor test" << endl;
    Matrix<int> moveMatrixIntResult(std::move(matrixInt), &errorOfResultIntMatrix);
    moveMatrixIntResult.display();

    Matrix<double> moveMatrixDoubleResult(std::move(doubleMatrix), &errorOfResultDoubleMatrix);
    if(errorOfResultDoubleMatrix == 3) cout << "Incorrect matrix given to move constructor. Matrix was not created" << endl;

    cout << endl;
    cout << "End of copyAndMoveConstructorsTest()" << endl;
    cout << endl;
}


void equalsTest() {
    cout << "Copy = operator test" << endl;
    int errorOfMatrixInt = 0;
    Matrix<int> matrixInt(4, 3, &errorOfMatrixInt);
    randomIntMatrixGenerator(matrixInt);
    matrixInt.display();

    Matrix<int> destMatrixOfInt;
    try {
        destMatrixOfInt = matrixInt;
    }
    catch (const invalid_argument& ex) {
        cout << ex.what();
    }
    destMatrixOfInt.display();


    int errorOfDoubleMatrix = 0;
    Matrix<double> doubleMatrix(-3, 4, &errorOfDoubleMatrix);

    Matrix<double> resultDoubeMatrix;
    try {
        resultDoubeMatrix = doubleMatrix;
    }
    catch (const invalid_argument& ex) {
        cout << ex.what();
    }
    cout << endl; cout << endl; cout << endl;


    cout << "Move = operator test" << endl;
    int errorOfIntMatrix = 0;
    Matrix<int> intMatrix(4, 4, &errorOfIntMatrix);
    randomIntMatrixGenerator(intMatrix);
    intMatrix.display();
    if (errorOfIntMatrix == 0) {
        Matrix<int> resultIntMatrix;
        try {
            resultIntMatrix = std::move(intMatrix);
        }
        catch (const invalid_argument& ex) {
            cout << ex.what();
        }
    }

    int errorOfMatrixDouble = 0;
    Matrix<double> matrixDouble(-2, 1, &errorOfMatrixDouble);
    Matrix<double> resultDoubleMatrix;
    try {
        resultDoubleMatrix = std::move(matrixDouble);
    }
    catch (const invalid_argument& ex) {
        cout << ex.what();
    }

    cout << endl;
    cout << "End of equalsTest()" << endl;
    cout << endl;
}


void createMatrixFromColumnAndRowTest() {
    cout << "Create matrix from column method test" << endl;
    Matrix<double> matrixOfDouble; //domyslnie tworzy macierz 2x2
    matrixOfDouble(0, 0, 1.5);
    matrixOfDouble(0, 1, 8.625);
    matrixOfDouble(1, 0, 5.25);
    matrixOfDouble(1, 1, 4.35);
    matrixOfDouble.display();
    cout << endl;

    int columnVectorErrorInfo = 0;
    Matrix<double> columnVector(0, 0, &columnVectorErrorInfo);
    columnVector = matrixOfDouble.createMatrixFromColumn(0, &columnVectorErrorInfo);
    if(columnVectorErrorInfo == 0) columnVector.display();
    cout << endl;

    int rowVectorErrorInfo = 0;
    Matrix<double> rowVector(0, 0, &rowVectorErrorInfo);
    rowVector = matrixOfDouble.createMatrixFromRow(1, &rowVectorErrorInfo);
    if(rowVectorErrorInfo == 0)  rowVector.display();
 
    cout << endl;
    cout << "End of equalsTest()" << endl;
    cout << endl;
}


void transposeTest() {
    cout << "Transpose matrix method test" << endl;
    int errorInfo = 0;
    Matrix<int> initialIntMatrix(3, 4, &errorInfo);
    if (errorInfo == 0) {
        randomIntMatrixGenerator(initialIntMatrix);
        initialIntMatrix.display();
        cout << endl;

        Matrix<int> transMatrix;
        transMatrix = initialIntMatrix.transpose(&errorInfo);
        transMatrix.display();
    }
    cout << endl; cout << endl;

    /*int errorInfo = 0;
    Matrix<int> initialIntMatrix(0, 0, &errorInfo);

    Matrix<int> transMatrix;
    try {
        transMatrix = initialIntMatrix.transpose(&errorInfo);
        transMatrix.display();
    }
    catch (const invalid_argument& ex) {
        cout << ex.what() << endl;
        cout << errorInfo << endl;
    }
        

    cout << endl; cout << endl;*/


    cout << "Transpose matrix operator test" << endl;
    int errorInfoFloat = 0;
    Matrix<float> floatMatrix(3, 3, &errorInfoFloat);
    if (errorInfoFloat == 0) {
        randomFloatMatrixGenerator(floatMatrix);
        floatMatrix.display();
        cout << endl;

        try {
            Matrix<float> transMatrix;
            transMatrix = ~floatMatrix;
            transMatrix.display();
            cout << endl;
        }
        catch (const invalid_argument& ex) {
            cout << ex.what();
        }
    }

    cout << endl;
    cout << "End of transposeTest()" << endl;
    cout << endl;
}


void addTest() {
    cout << "Add matrices method test" << endl;
    int errorInfoFirst = 0;
    Matrix<int> firstIntMatrix(3, 3, &errorInfoFirst);
    firstIntMatrix(0, 0, 7);
    firstIntMatrix(1, 0, 1);
    firstIntMatrix(2, 0, -5);
    firstIntMatrix(0, 1, 2);
    firstIntMatrix(1, 1, 3);
    firstIntMatrix(2, 1, 9);
    firstIntMatrix(0, 2, 8);
    firstIntMatrix(1, 2, 2);
    firstIntMatrix(2, 2, 5);
    if(errorInfoFirst == 0) firstIntMatrix.display();
    cout << endl;
    
    int errorInfoSecond = 0;
    Matrix<int> secondIntMatrix(3, 3, &errorInfoSecond);
    randomIntMatrixGenerator(secondIntMatrix);
    if(errorInfoSecond == 0) secondIntMatrix.display();
    cout << endl;

    if (errorInfoFirst == 0 && errorInfoSecond == 0) {
        int resultIntError = 0;
        Matrix<int> resultMatrix;
        try {
            resultMatrix = firstIntMatrix.add(secondIntMatrix, &resultIntError);
        } catch (const invalid_argument& ex) {
            cout << ex.what();
        }
        
        if (resultIntError == 0) resultMatrix.display();
    }
    cout << endl; cout << endl;


    cout << "Add matrices operator test" << endl;
    if (errorInfoFirst == 0 && errorInfoSecond == 0) {
        int resultIntError = 0;
        Matrix<int> resultMatrix;
        try {
            resultMatrix = firstIntMatrix + secondIntMatrix;
        }
        catch (const invalid_argument& ex) {
            cout << ex.what();
        }

        if (resultIntError == 0) resultMatrix.display();
    }

    cout << endl;
    cout << "End of addTest()" << endl;
    cout << endl;
}


void subtractTest() {
    cout << "Subtract matrices method test" << endl;
    int errorInfoFirst = 0;
    Matrix<int> firstIntMatrix(3, 3, &errorInfoFirst);
    firstIntMatrix(0, 0, 7);
    firstIntMatrix(1, 0, 1);
    firstIntMatrix(2, 0, -5);
    firstIntMatrix(0, 1, 2);
    firstIntMatrix(1, 1, 3);
    firstIntMatrix(2, 1, 9);
    firstIntMatrix(0, 2, 8);
    firstIntMatrix(1, 2, 2);
    firstIntMatrix(2, 2, 5);
    if (errorInfoFirst == 0) firstIntMatrix.display();
    cout << endl;

    int errorInfoSecond = 0;
    Matrix<int> secondIntMatrix(3, 3, &errorInfoSecond);
    randomIntMatrixGenerator(secondIntMatrix);
    if (errorInfoSecond == 0) secondIntMatrix.display();
    cout << endl;

    if (errorInfoFirst == 0 && errorInfoSecond == 0) {
        int resultIntError = 0;
        Matrix<int> resultMatrix;
        try {
            resultMatrix = firstIntMatrix.subtract(secondIntMatrix, &resultIntError);
        }
        catch (const invalid_argument& ex) {
            cout << ex.what();
        }

        if (resultIntError == 0) resultMatrix.display();
    }
    cout << endl; cout << endl;


    cout << "Subtract matrices operator test" << endl;
    if (errorInfoFirst == 0 && errorInfoSecond == 0) {
        int resultIntError = 0;
        Matrix<int> resultMatrix;
        try {
            resultMatrix = firstIntMatrix - secondIntMatrix;
        }
        catch (const invalid_argument& ex) {
            cout << ex.what();
        }

        if (resultIntError == 0) resultMatrix.display();
    }

    cout << endl;
    cout << "End of subtractTest()" << endl;
    cout << endl;
}


void multiplyMatricesTest() {
    cout << "Multiply matrices method test" << endl;
    int errorInfoFirst = 0;
    Matrix<int> firstIntMatrix(3, 3, &errorInfoFirst);
    firstIntMatrix(0, 0, 7);
    firstIntMatrix(1, 0, 1);
    firstIntMatrix(2, 0, -5);
    firstIntMatrix(0, 1, 2);
    firstIntMatrix(1, 1, 3);
    firstIntMatrix(2, 1, 9);
    firstIntMatrix(0, 2, 8);
    firstIntMatrix(1, 2, 2);
    firstIntMatrix(2, 2, 5);
    if (errorInfoFirst == 0) firstIntMatrix.display();
    cout << endl;

    int errorInfoSecond = 0;
    Matrix<int> secondIntMatrix(3, 2, &errorInfoSecond);
    randomIntMatrixGenerator(secondIntMatrix);
    if (errorInfoSecond == 0) secondIntMatrix.display();
    cout << endl;

    if (errorInfoFirst == 0 && errorInfoSecond == 0) {
        int resultIntError = 0;
        Matrix<int> resultMatrix;
        try {
            resultMatrix = firstIntMatrix.multiply(secondIntMatrix, &resultIntError);
        }
        catch (const invalid_argument& ex) {
            cout << ex.what();
        }

        if (resultIntError == 0) resultMatrix.display();
    }
    cout << endl; cout << endl;


    cout << "Multiply matrices operator test" << endl;
    if (errorInfoFirst == 0 && errorInfoSecond == 0) {
        int resultIntError = 0;
        Matrix<int> resultMatrix;
        try {
            resultMatrix = firstIntMatrix * secondIntMatrix;
        }
        catch (const invalid_argument& ex) {
            cout << ex.what();
        }

        if (resultIntError == 0) resultMatrix.display();
    }

    cout << endl;
    cout << "End of multiplyMatricesTest()" << endl;
    cout << endl;
}


void multiplyMatrixByNymberTest() {
    cout << "Multiply matrix by number method test" << endl;
    int errorInfoFirst = 0;
    Matrix<int> firstIntMatrix(3, 3, &errorInfoFirst);
    firstIntMatrix(0, 0, 7);
    firstIntMatrix(1, 0, 1);
    firstIntMatrix(2, 0, -5);
    firstIntMatrix(0, 1, 2);
    firstIntMatrix(1, 1, 3);
    firstIntMatrix(2, 1, 9);
    firstIntMatrix(0, 2, 8);
    firstIntMatrix(1, 2, 2);
    firstIntMatrix(2, 2, 5);
    if (errorInfoFirst == 0) firstIntMatrix.display();
    cout << endl;


    if (errorInfoFirst == 0) {
        int resultIntError = 0;
        Matrix<int> resultMatrix;
        try {
            resultMatrix = firstIntMatrix.multiply(5, &resultIntError);
        }
        catch (const invalid_argument& ex) {
            cout << ex.what();
        }

        if (resultIntError == 0) resultMatrix.display();
    }
    cout << endl; cout << endl;


    cout << "Multiply matrix by number operator test" << endl;
    if (errorInfoFirst == 0) {
        int resultIntError = 0;
        Matrix<int> resultMatrix;
        try {
            resultMatrix = firstIntMatrix * 5;
        }
        catch (const invalid_argument& ex) {
            cout << ex.what();
        }

        if (resultIntError == 0) resultMatrix.display();
    }

    cout << endl;
    cout << "End of multiplyMatrixByNymberTest()" << endl;
    cout << endl;
}


void makeIndentityMatrixTest() {
    cout << "Multiply matrices method test" << endl;
    int errorInfo = 0;
    Matrix<int> intMatrix(4, 4, &errorInfo);
    randomIntMatrixGenerator(intMatrix);
    if (errorInfo == 0) intMatrix.display();
    cout << endl;

    if (errorInfo == 0) {
        intMatrix.makeIdentityMatrix(&errorInfo);
        if (errorInfo == 0) intMatrix.display();
    }

    cout << endl;
    cout << "End of makeIndentityMatrixTest()" << endl;
    cout << endl;
}


void createNewMatrixTest() {
    cout << "Create new matrix from this method test" << endl;

    int errorInfo = 0;
    Matrix<int> intMatrix(4, 4, &errorInfo);
    randomIntMatrixGenerator(intMatrix);

    intMatrix.display();
    cout << endl;

    intMatrix.createNewMatrix(6, 7, &errorInfo);
    if (errorInfo == 0) intMatrix.display();

    //dla niepoprawnej macierzy tez zadziala
    /*int errorInfo = 0;
    Matrix<int> intMatrix(0, 0, &errorInfo);
    randomIntMatrixGenerator(intMatrix);

    intMatrix.display();
    cout << endl;

    intMatrix.createNewMatrix(6, 7, &errorInfo);
    if (errorInfo == 0) intMatrix.display();

    cout << endl;
    cout << "End of makeIndentityMatrixTest()" << endl;
    cout << endl;*/
}


void readMatrixFromFileTest(){
    cout << "Read Matrix From File method test" << endl;
    int errorInfo = 0;
    /*Matrix<int> intMatrix(0, 0, &errorInfo);
    intMatrix.readMatrixFromFile("IntMatrix.txt", &errorInfo);
    if (errorInfo == NO_ERRORS || errorInfo == INVALID_ARGUMENT_IN_FILE) intMatrix.display();*/


    Matrix<double> doubletMatrix(0, 0, &errorInfo);
    doubletMatrix.readMatrixFromFile("CorrectDoubleMatrix.txt", &errorInfo);
    if (errorInfo == NO_ERRORS || errorInfo == INVALID_ARGUMENT_IN_FILE) doubletMatrix.display();


    cout << endl;
    cout << "End of readMatrixFromFileTest()" << endl;
    cout << endl;
}


void scalarProductTest() {
    cout << "Scalar product  method test" << endl;
    int errorInfo = 0;
    Matrix<int> intMatrix(1, 5, &errorInfo);
    randomIntMatrixGenerator(intMatrix);
    intMatrix.display();

    int error = 0;
    Matrix<int> test(1, 5, &error);
    test(0, 0, 1);
    test(0, 1, 2);
    test(0, 2, 1);
    test(0, 3, 2);
    test(0, 4, 1);
    test.display();


    if (errorInfo == NO_ERRORS && error == NO_ERRORS) {
        int prod = intMatrix.scalarProduct(test, &error);
        cout << "Error: " << error << endl;
        cout << "Prod: " << prod << endl;
    }




    cout << endl;
    cout << "End of scalarProductTest()" << endl;
    cout << endl;
}


