//
// Created by vityha on 11.05.22.
//

#ifndef NNLIB_AND_TEST_EXAMPLE_MATRIXD_H
#define NNLIB_AND_TEST_EXAMPLE_MATRIXD_H

#include <vector>
#include <stdexcept>
#include <cstddef> // size_t
#include <iostream>
#include <utility> // std::move

class MatrixD {
public:
    static bool parallelExecution;
    static size_t numOfProcs;

    MatrixD(size_t nRows, size_t nCols) : nRows(nRows), nCols(nCols), data(nRows * nCols) {}
    MatrixD(size_t nRows, size_t nCols, std::vector<double> &data) : nRows(nRows), nCols(nCols), data(data) {}
    MatrixD(size_t nRows, size_t nCols, std::vector<double> &&data) : nRows(nRows), nCols(nCols), data(std::move(data)) {}

    // data is in row-major way
    double operator()(size_t i, size_t j) const {
        if (j >= nCols || i >= nRows)
            throw std::out_of_range("matrix indices out of range");
        return data[nCols * i + j];
    }

    double &operator()(size_t i, size_t j) {
        if (j >= nCols || i >= nRows)
            throw std::out_of_range("matrix indices out of range");
        return data[nCols * i + j];
    }

    MatrixD cwiseProduct(const MatrixD &other) {
        MatrixD resMat(*this);
        resMat.cwiseProductImplace(other);
        return resMat;
    }

    MatrixD &cwiseProductImplace(const MatrixD &other) {
        this->cwiseBinaryOperation([] (double el1, double el2) {return el1 * el2;}, other);
        return *this;
    }

    template<class CustomOp> // TODO: add check for non matching dimentions
    MatrixD &cwiseBinaryOperation(const CustomOp &operation, const MatrixD &other) {
        for (size_t x = 0; x < this->nCols; ++x) {
            for (size_t y = 0; y < this->nRows; ++y) {
                double &el = this->operator()(y, x);
                el = operator()(el, other(y, x));
            }
        }
        return *this;
    }

    template<class CustomOp>
    MatrixD &unaryExprInPlace(const CustomOp &operation) {
        MatrixD &thisMatrix = *this;
        for (size_t x = 0; x < this->nCols; ++x) {
            for (size_t y = 0; y < this->nRows; ++y) {
                double &val = thisMatrix(y, x);
                val = operation(val);
            }
        }
        return thisMatrix;
    }

    template<class CustomOp>
    MatrixD unaryExpr(const CustomOp &operation) const {
        MatrixD copyMatrix = *this;
        copyMatrix.unaryExprInPlace<CustomOp>(operation);
        return copyMatrix;
    }

    friend std::ostream &operator<<(std::ostream &os, const MatrixD &matrix);

    size_t rows() const {
        return nRows;
    }

    size_t cols() const {
        return nCols;
    }

    static void setParallelExecution(bool parExecution);

    static bool getParallelExecution() {
        return parallelExecution;
    }

    static void setNumberProcessors(size_t numProcessors);

    static size_t getNumberProcessors() {
        return numOfProcs;
    }
private:
    std::vector<double> data;
    size_t nRows, nCols;
};

MatrixD primitiveMultiplication(const MatrixD &left, const MatrixD &right);
void rowsMatrixMultiplication(size_t startRow, size_t endRow, const MatrixD &A, const MatrixD &B, MatrixD &C);

MatrixD operator+(const MatrixD &left, const MatrixD &right);
MatrixD operator-(const MatrixD &left, const MatrixD &right);
MatrixD operator*(double scalar, const MatrixD &mat);
MatrixD operator/(double scalar, const MatrixD &mat);

MatrixD operator*(const MatrixD &left, const MatrixD &right);

bool operator==(const MatrixD& left, const MatrixD& right);
bool operator!=(const MatrixD& left, const MatrixD& right);

template<class CustomOp> // TODO: throw error when sizes do not match
MatrixD cwiseBinaryOperation(const CustomOp &operation, const MatrixD &mat1, const MatrixD &mat2) {
    MatrixD resMat(mat1.rows(), mat1.cols());
    for (size_t x = 0; x < mat1.cols(); ++x) {
        for (size_t y = 0; y < mat1.rows(); ++y) {
            resMat(x, y) = operation(mat1(y, x), mat2(y, x));
        }
    }
    return resMat;
}

#endif //NNLIB_AND_TEST_EXAMPLE_MATRIXD_H
