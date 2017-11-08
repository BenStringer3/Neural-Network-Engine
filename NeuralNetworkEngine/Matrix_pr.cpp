

#include "stdafx.h"

#include "Matrix_pr.h"

Matrix_pr::Matrix_pr() {
	rows = 0;
	cols = 0;
};

Matrix_pr& Matrix_pr::operator=(const Matrix& mat) {
	int i, ii;
	if (this->rows != mat.rows || this->cols != mat.cols) {
		throw std::out_of_range("dimension mismatch");
	}
	else {
		
		for (i = 0; i < this->rows; i++) {
			for (ii = 0; ii < this->cols; ii++) {
				*this->data[i*this->cols + ii] = mat.data[i*this->cols + ii];
			}
		}
	}
	return *this;
};



Matrix_pr::Matrix_pr(unsigned int r, unsigned int c) {
	int i, ii;
	rows = r;
	cols = c;
	data.resize(r*c);
	for (i = 0; i < this->rows; i++) {
		for (ii = 0; ii < this->cols; ii++) {
			this->data[i*this->cols + ii] = NULL;
		}
	}
};

Matrix_pr& Matrix_pr::operator*=(const double scalar) {
	int i;
	for (i = 0; i < this->data.size(); i++) {
		*this->data[i] *= scalar;
	}
	return *this;
}

Matrix_pr& Matrix_pr::operator/=(const double scalar) {
	int i;
	for (i = 0; i < this->data.size(); i++) {
		*this->data[i] /= scalar;
	}
	return *this;
}

Matrix_pr& Matrix_pr::operator-=(const double scalar) {
	int i;
	for (i = 0; i < this->data.size(); i++) {
		*this->data[i] -= scalar;
	}
	return *this;
}

Matrix_pr& Matrix_pr::operator-=(const Matrix mat) {
	int i, ii;
	if (this->rows != mat.rows || this->cols != mat.cols) {
		throw std::out_of_range("dimension mismatch");
	}
	else {
		for (i = 0; i < this->rows; i++) {
			for (ii = 0; ii < this->cols; ii++) {
				*this->data[i*this->cols + ii] -= mat.data[i*this->cols + ii];
			}
		}
	}
	return *this;
}

Matrix_pr& Matrix_pr::operator+=(const Matrix mat) {
	int i, ii;
	if (this->rows != mat.rows || this->cols != mat.cols) {
		throw std::out_of_range("dimension mismatch");
	}
	else {
		for (i = 0; i < this->rows; i++) {
			for (ii = 0; ii < this->cols; ii++) {
				*this->data[i*this->cols + ii] += mat.data[i*this->cols + ii];
			}
		}
	}
	return *this;
}


Matrix Matrix_pr::operator*(const double scalar) const {
	int i, ii;
	Matrix result(this->rows, this->cols);

	for (i = 0; i < this->rows; i++) {
		for (ii = 0; ii < this->cols; ii++) {
			result.data[i*result.cols + ii] = *this->data[i*this->cols + ii] * scalar;
		}
	}
	return result;
}

Matrix_pr Matrix_pr::transpose() const
{
	int i, ii;
	Matrix_pr result(this->cols, this->rows);

	for (i = 0; i < result.rows; i++) {
		for (ii = 0; ii < result.cols; ii++) {
			result.data[i*result.cols + ii] = this->data[ii*this->cols + i];
		}
	}
	return result;
}

double Matrix_pr::sum() {
	int i;
	double result = 0;
	for (i = 0; i < this->data.size(); i++) {
		result += *this->data[i];
	}
	return result;
}

double& Matrix_pr::operator()(unsigned int n) {
	if (!(this->rows == 1 || this->cols == 1)) {
		throw std::out_of_range("This is not a vector");
	}
	else if (n > this->data.size()) {
		throw std::out_of_range("Index out of bounds");
	}
	return *this->data[n];
}

double& Matrix_pr::operator()(unsigned int r, unsigned int c) {
	if (r >= rows || c >= cols) {
		throw std::out_of_range("Index out of bounds");
	}
	return *this->data[r*this->cols + c];
}

Matrix_pr Matrix_pr::diagonal() {
	Matrix_pr result;
	int i, n;
	if (this->rows > this->cols) {
		n = this->cols;
	}
	else {
		n = this->rows;
	}
	result = Matrix_pr(1, n);
	for (i = 0; i < n; i++) {
		result.data[i] = this->data[i*this->cols + i];
	}

	return result;
}

Matrix_pr& Matrix_pr::reshape(unsigned int rows, unsigned int cols)
{
	if (rows*cols != this->rows*this->cols) {
		throw std::out_of_range("Illegal reshape");
	}
	this->rows = rows;
	this->cols = cols;
	return *this;
}

double & Matrix_pr::max()
{
	unsigned int maxLocation = 0;
	for (int i = 0; i < this->data.size(); i++) {
		if (data[i] > data[maxLocation]) {
			maxLocation = i;
		}
	}
	return *data[maxLocation];
}

void Matrix_pr::maxLocation(unsigned int & row, unsigned int & col)
{
	row = 0; col = 0;
	for (int i = 0; i < this->rows; i++) {
		for (int j = 0; j < this->cols; j++) {
			if (this->data[i*cols +j] > this->data[row*cols + col]) {
				row = i;
				col = j;
			}
		}
	}
}

double  Matrix_pr::avg()
{
	int accum = 0;
	for (int i = 0; i < this->data.size(); i++) {
		accum += *data[i];
	}
	return accum / data.size();
}
