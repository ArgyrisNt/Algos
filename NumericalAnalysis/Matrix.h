#pragma once

#include <iostream>
#include <vector>

namespace NumericalAnalysis
{

template<class T>
class Matrix
{
public:
	// Constructors
	Matrix(); // Default constructor
	Matrix(int my_rows, int my_cols, T val = 0.0); // Initialize matrix by dimensions and init value
	Matrix(const Matrix&); // Copy constructor

	// Destructor
	~Matrix();

	// Overloaded operators
	Matrix& operator=(Matrix); // Equality between two matrices
	Matrix operator+(Matrix); // Addition of two metrices
	Matrix operator-(Matrix&); // Substraction of two metrices
	Matrix operator*(Matrix&); // Multiplication of two metrices
	Matrix operator*(const T&); // Multiplication of a matrix and a constant
	std::vector<T> operator*(std::vector<T> &); // Multiplication of a matrix and a vector
	T operator()(int row, int col); // Overload operator ()

	//Member functions
	void setValue(int row, int col, T val); // Set a value on a specific position
	void calcDet(); // Compute determinant
	void print(); // Print matrix
	Matrix transpose(); // Transpose matrix
	Matrix inverse(); // Inverse matrix

	std::vector<T> Jacobi_iterator(std::vector<T> b, int iters); // Jacobi iterative method to solve a linear system
	std::vector<T> GaussSeidel_iterator(std::vector<T> b, int iters); // Gauss Seidel iterative method to solve a linear system
	std::vector<T> SOR_iterator(std::vector<T> b, int iters, double omega);
	std::vector<T> gradient_iterator(std::vector<T> b, int iters);
	std::vector<T> conjugate_gradient_iter(std::vector<T> &b, int iters);

	std::vector<Matrix> LU_factor(); // LU decomposition
	std::vector<T> forward_Euler(std::vector<T> b); // forward Euler method to solve a linear system
	std::vector<T> backward_Euler(std::vector<T> b); // backward Euler method to solve a linear system
	std::vector<Matrix> QR_factor(); // with Grand Schmidt method

	// Member getter functions
	int getRows() { return rows; }
	int getCols() { return cols; }
	T getDetVal() { return detVal; }

private:
	// Member variables
	T** mat;
	int rows;
	int cols;
	T detVal;
};

} // namespace NumericaAnalysis

#include "Matrix.cpp"