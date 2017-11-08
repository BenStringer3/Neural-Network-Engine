#pragma once
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <stdexcept>
#include "Matrix_pr.h"

using namespace std;

class Matrix_pr;

class Matrix
{

public:

	unsigned int rows;

	unsigned int cols;
	
	friend class Matrix_pr;

	vector <double > data;

	Matrix(unsigned int r, unsigned int c);

	Matrix();

	static Matrix ones(unsigned int r, unsigned int c);

	static Matrix random(unsigned int r, unsigned int c);

	static Matrix eye(unsigned int n);

	Matrix& operator=(const Matrix_pr& mat);

	Matrix& operator+=(const Matrix mat);

	Matrix operator+(const Matrix & mat) const;

	Matrix& operator-=(const Matrix mat);

	Matrix operator-(const Matrix& mat) const;

	Matrix operator*(const Matrix& mat) const;

	Matrix operator*(const Matrix_pr& mat) const;

	Matrix operator*(const double scalar) const;

	Matrix operator*=(const double scalar);

	Matrix operator/(const double scalar) const;

	friend Matrix operator*(const double scalar, const Matrix& mat)
	{
		int i, ii;
		Matrix result = mat;
		for (i = 0; i < mat.rows; i++) {
			for (ii = 0; ii < mat.cols; ii++) {
				result.data[i*result.cols + ii] = mat.data[i*mat.cols + ii] * scalar;
			}
		}
		return result;
	};

	const Matrix block(unsigned int startRow, unsigned int startCol, unsigned int height, unsigned int width) const;

	Matrix_pr block(unsigned int startRow, unsigned int startCol, unsigned int height, unsigned int width);

	double& operator()(unsigned int r, unsigned int c);

	const double& operator()(unsigned int r, unsigned int c) const;

	double& operator()(unsigned int n);

	const double& operator()(unsigned int n) const;

	friend Matrix operator-(const Matrix& mat)
	{
		Matrix result = mat;
		int i;
		for (i = 0; i < mat.data.size(); i++) {
			result.data[i] = result.data[i] * -1;
		}
		return result;
	};

	friend std::ostream& operator<<(std::ostream& os, const Matrix& mat)
	{
		int i, ii;
		for (i = 0; i < mat.rows; i++) {
			for (ii = 0; ii < mat.cols; ii++) {
				os << mat(i, ii) << "  ";
			}
			os << std::endl;
		}
		return os;
	};

	void transposeInPlace();

	Matrix transpose() const;

	Matrix cwiseProduct(const Matrix& mat);

	Matrix cwiseQuotient(const Matrix& mat);

	Matrix exp() const;

	static bool LU_decomp(Matrix& lower, Matrix& upper, const Matrix& mat);

	static Matrix horzcat(const Matrix& ls, const Matrix& rs);

	static Matrix fwd_sub(const Matrix& lower);

	static Matrix bck_sub(const Matrix& upper);

	bool operator==(const Matrix& mat) const;

	Matrix inverse();

	Matrix_pr diagonal();

	Matrix& reshape(unsigned int rows, unsigned int cols);
};