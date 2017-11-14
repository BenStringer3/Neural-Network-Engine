
#include "stdafx.h"

#include "Matrix.h"
#include <math.h>

#define SAFETY_CHECKS 1

#define RAND (-1 + ((double)rand() / RAND_MAX) * 2)

Matrix::Matrix() {
	rows = 0;
	cols = 0;
}

Matrix::Matrix(unsigned int r, unsigned int c) {
	int i, ii;
	rows = r;
	cols = c;
	data.resize(r*c);
	for (i = 0; i < this->rows; i++) {
		for (ii = 0; ii < this->cols; ii++) {
			this->data[i*this->cols + ii] = 0;
		}
	}
}

Matrix Matrix::ones(unsigned int r, unsigned int c) {
	int i, ii;
	Matrix result(r, c);
	for (i = 0; i < result.rows; i++) {
		for (ii = 0; ii < result.cols; ii++) {
			result.data[i*result.cols + ii] = 1;
		}
	}
	return result;
}

Matrix Matrix::random(unsigned int r, unsigned int c) {
	int i, ii;
	Matrix result(r, c);
	for (i = 0; i < result.rows; i++) {
		for (ii = 0; ii < result.cols; ii++) {
			result.data[i*result.cols + ii] = RAND;
		}
	}
	return result;
}

Matrix Matrix::eye(unsigned int n) {
	int i;
	Matrix result(n, n);
	for (i = 0; i < result.rows; i++) {
		result(i, i) = 1.0;
	}
	return result;
}

Matrix& Matrix::operator=(const Matrix_pr& mat) {
	int i;
	if ((this->rows != mat.rows || this->cols != mat.cols) && this->rows != 0 && this->cols != 0) {
		throw std::out_of_range("dimension mismatch");
	}
	else {
		this->rows = mat.rows;
		this->cols = mat.cols;
		this->data.resize(mat.data.size());
		for (i = 0; i < this->data.size(); i++) {
			this->data[i] = *mat.data[i];
		}
	}
}

Matrix& Matrix::operator+=(const Matrix mat) {
	unsigned int i, ii;
	if (this->rows != mat.rows || this->cols != mat.cols) {
		throw std::out_of_range("dimension mismatch");
	}
	else {
		for (i = 0; i < this->rows; i++) {
			for (ii = 0; ii < this->cols; ii++) {
				this->data[i*this->cols + ii] += mat.data[i*this->cols + ii];
			}
		}
	}
	return *this;
}

Matrix& Matrix::operator-=(const Matrix mat) {
	int i, ii;
	if (this->rows != mat.rows || this->cols != mat.cols) {
		throw std::out_of_range("dimension mismatch");
	}
	else {
		for (i = 0; i < this->rows; i++) {
			for (ii = 0; ii < this->cols; ii++) {
				this->data[i*this->cols + ii] -= mat.data[i*this->cols + ii];
			}
		}
	}
	return *this;
}

Matrix & Matrix::operator-=(const Matrix_pr & mat)
{
	int i, ii;
	if (this->rows != mat.rows || this->cols != mat.cols) {
		throw std::out_of_range("dimension mismatch");
	}
	else {
		for (i = 0; i < this->rows; i++) {
			for (ii = 0; ii < this->cols; ii++) {
				this->data[i*this->cols + ii] -= *mat.data[i*this->cols + ii];
			}
		}
	}
	return *this;
}

Matrix Matrix::operator+(const Matrix& mat) const {
	int i, ii;
	Matrix result;
	if (this->rows != mat.rows || this->cols != mat.cols) {
		throw std::out_of_range("dimension mismatch");
	}
	else {
		result = Matrix(this->rows, this->cols);
		for (i = 0; i < this->rows; i++) {
			for (ii = 0; ii < this->cols; ii++) {
				result.data[i*this->cols + ii] = this->data[i*this->cols + ii] + mat.data[i*this->cols + ii];
			}
		}
	}
	return result;
}

Matrix Matrix::operator-(const Matrix& mat) const {
	int i, ii;
	Matrix result;
	if (this->rows != mat.rows || this->cols != mat.cols) {
		throw std::out_of_range("dimension mismatch");
	}
	else {
		result = Matrix(this->rows, this->cols);
		for (i = 0; i < this->rows; i++) {
			for (ii = 0; ii < this->cols; ii++) {
				result.data[i*this->cols + ii] = this->data[i*this->cols + ii] - mat.data[i*this->cols + ii];
			}
		}
	}
	return result;
}

Matrix Matrix::operator*(const Matrix& mat) const {
	int i, ii, iii;
	Matrix result;
	if (this->cols != mat.rows) {
		throw std::out_of_range("dimension mismatch");
	}
	else {
		result = Matrix(this->rows, mat.cols);
		for (i = 0; i < mat.cols; i++) {
			for (ii = 0; ii < mat.rows; ii++) {
				for (iii = 0; iii < this->rows; iii++) {
					result.data[iii*result.cols + i] += (this->data[iii*this->cols + ii] * mat.data[ii*mat.cols + i]);
				}
			}
		}
	}
	return result;
}

Matrix Matrix::operator*(const Matrix_pr & mat) const
{
	int i, ii, iii;
	Matrix result;
	if (this->cols != mat.rows) {
		throw std::out_of_range("dimension mismatch");
	}
	else {
		result = Matrix(this->rows, mat.cols);
		for (i = 0; i < mat.cols; i++) {
			for (ii = 0; ii < mat.rows; ii++) {
				for (iii = 0; iii < this->rows; iii++) {
					result.data[iii*result.cols + i] += (this->data[iii*this->cols + ii] * (*mat.data[ii*mat.cols + i]));
				}
			}
		}
	}
	return result;
}

Matrix Matrix::operator*=(const double scalar) {
	int i;
	for (i = 0; i < this->data.size(); i++) {
		this->data[i] *= scalar;
	}
	return *this;
}

Matrix Matrix::operator*(const double scalar) const {
	int i, ii;
	Matrix result = *this;

	for (i = 0; i < this->rows; i++) {
		for (ii = 0; ii < this->cols; ii++) {
			result.data[i*result.cols + ii] = this->data[i*this->cols + ii] * scalar;
		}
	}
	return result;
}

Matrix Matrix::operator/(const double scalar) const {
	int i, ii;
	Matrix result = *this;

	for (i = 0; i < this->rows; i++) {
		for (ii = 0; ii < this->cols; ii++) {
			result.data[i*result.cols + ii] = this->data[i*this->cols + ii] / scalar;
		}
	}
	return result;
}

Matrix_pr Matrix::block(unsigned int startRow, unsigned int startCol, unsigned int height, unsigned int width) {
	Matrix_pr result(height, width);
	unsigned int i, ii;
#if SAFETY_CHECKS
	if (height == 0 || width == 0) {
		throw std::out_of_range("block must have non-zero height and width");
	}
	else if (startRow + height - 1 > rows || startCol + width - 1 > cols) {
		throw std::out_of_range("block extends beyond matrix dimensions");
	}
#endif

		//result = Matrix_pr(height, width);
		for (i = startRow; i < startRow + height; i++) {
			for (ii = startCol; ii < startCol + width; ii++) {
				result.data[(i - startRow)*result.cols + ii - startCol] = &this->data[i*this->cols + ii];
			}
		}

	return result;
}

Matrix_pr Matrix::diagonal() {
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
		result.data[i] = &this->data[i*this->cols + i];
	}
	
	return result;
}

Matrix_pr Matrix::reshape(unsigned int rows, unsigned int cols)
{
	Matrix_pr result(this->rows, this->cols);
	result.pointTo( *this);
#if SAFETY_CHECKS
		if ((rows*cols != this->rows*this->cols)  && this->rows != 0 && this->cols != 0) {
			throw std::out_of_range("Illegal reshape");
		}
#endif
		result.rows = rows;
		result.cols = cols;
		return result;
}

double& Matrix::operator()(unsigned int r, unsigned int c){
	if (r >= rows || c >= cols) {
		throw std::out_of_range("Index out of bounds");
	}
	return this->data[r*this->cols + c];
}

const double& Matrix::operator()(unsigned int r, unsigned int c) const {
	if (r >= rows || c >= cols) {
		throw std::out_of_range("Index out of bounds");
	}
	return this->data[r*this->cols + c];
}

double& Matrix::operator()(unsigned int n) {
	if (!(this->rows == 1 || this->cols == 1)) {
		throw std::out_of_range("This is not a vector");
	}
	else if (n > this->data.size()) {
		throw std::out_of_range("Index out of bounds");
	}
	return this->data[n];
}

const double& Matrix::operator()(unsigned int n) const {
	if (!(this->rows == 1 || this->cols == 1)) {
		throw std::out_of_range("This is not a vector");
	}
	else if (n > this->data.size()){
		throw std::out_of_range("Index out of bounds");
	}
	return this->data[n];
}

void Matrix::transposeInPlace()
{
	int i, ii;
	Matrix tmp = *this;

	this->cols = tmp.rows;
	this->rows = tmp.cols;

	for (i = 0; i < this->rows; i++) {
		for (ii = 0; ii < this->cols; ii++) {
			this->data[i*this->cols + ii] = tmp.data[ii*tmp.cols + i] ;
		}
	}

}

Matrix Matrix::transpose() const
{
	int i, ii;
	Matrix result(this->cols, this->rows);

	for (i = 0; i < result.rows; i++) {
		for (ii = 0; ii < result.cols; ii++) {
			result.data[i*result.cols + ii] = this->data[ii*this->cols + i];
		}
	}
	return result;
}

Matrix Matrix::cwiseProduct(const Matrix& mat) {
	
	int i, ii;
	Matrix result = *this;
	if (this->rows != mat.rows || this->cols != mat.cols) {
		throw std::out_of_range("dimension mismatch");
	} else{
	for (i = 0; i < result.rows; i++) {
		for (ii = 0; ii < result.cols; ii++) {
			result(i, ii) = this->data[i*this->cols + ii] * mat(i, ii);
		}
	}
	}
	return result;
}

Matrix Matrix::cwiseQuotient(const Matrix& mat) {

	int i, ii;
	Matrix result = *this;
	if (this->rows != mat.rows || this->cols != mat.cols) {
		throw std::out_of_range("dimension mismatch");
	}
	else {
		for (i = 0; i < result.rows; i++) {
			for (ii = 0; ii < result.cols; ii++) {
				result(i, ii) = this->data[i*this->cols + ii] / mat(i, ii);
			}
		}
	}
	return result;
}

Matrix Matrix::exp() const
{

	int i;
	Matrix result = *this;
	for (i = 0; i < result.rows*result.cols; i++) {
		result.data[i] = std::exp(this->data[i]);
	}
	
	return result;
}

const Matrix Matrix::block(unsigned int startRow, unsigned int startCol, unsigned int height, unsigned int width) const {
	Matrix result;
	int i, ii;
	if (height == 0 || width == 0) {
		throw std::out_of_range("block must have non-zero height and width");
	}
	else if (startRow + height - 1 > rows || startCol + width - 1 > cols) {
		throw std::out_of_range("block extends beyond matrix dimensions");
	}
	else {
		result = Matrix(height, width);
		for (i = startRow; i < startRow + height; i++) {
			for (ii = startCol; ii < startCol + width; ii++) {
				result(i - startRow, ii - startCol) = this->data[i*this->cols + ii];
			}
		}
	}
	return result;
};

bool Matrix::LU_decomp(Matrix& lower, Matrix & upper, const Matrix& mat) {
	int i = 0, j = 0, k = 0;
	int n = mat.rows;

	if (mat.rows != mat.cols) {
		throw std::out_of_range("This matrix is not square");
	}
	lower = mat;
	upper = mat;

	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){
			if (j < i)
				lower(j, i) = 0;
			else{
				lower(j, i) = mat(j, i);
				for (k = 0; k < i; k++){
					lower(j, i) = lower(j, i) - lower(j, k) * upper(k, i);
				}
			}
		}

		for (j = 0; j < n; j++){
			if (j < i)
				upper(i, j) = 0;
			else if (j == i)
				upper(i, j) = 1;
			else{
				upper(i, j) = mat(i, j) / lower(i, i);
				for (k = 0; k < i; k++){
					upper(i ,j) = upper(i, j) - ((lower(i, k) * upper(k, j)) / lower(i, i));
				}
			}
		}
	}
	return mat == lower*upper;
}

bool Matrix::operator==(const Matrix& mat) const{
	bool result = true;
	int i;
	if (this->rows != mat.rows || this->cols != mat.cols) {
		return false;
	}
	else {
		for (i = 0; i < this->data.size(); i++) {
			result = result && this->data[i] == mat.data[i];
		}
		return result;
	}
}

Matrix Matrix::fwd_sub(const Matrix& lower) {
	Matrix result, mat;
	int i, j, ii, w, h;
	result = lower;
	mat = horzcat(lower, eye(lower.rows));
	h = mat.rows;
	w = mat.cols;

	for (i = 0; i < h; i++) {
		for (ii = 0; ii < i; ii++) {
			mat.block(i, 0, 1, w) -= mat.block(ii, 0, 1, w)*mat(i, ii);
			//cout << "mat.block(" << i << ", 0, 1, w) -= mat.block(" << ii << ", 0, 1, w)*mat(" << i << ", " << ii << ");\n";
			//cout << mat << endl;
		}
		mat.block(i, 0, 1, w) /= mat(i, i);
		//cout << "mat.block(" << i << ", 0, 1, w) /= mat(" << i << ", " << i << "); \n";
		//cout << mat << endl;
	}
	result = mat.block(0, lower.rows, h, h);

	return result;
}

Matrix Matrix::bck_sub(const Matrix& upper) {
	Matrix result, mat;
	int i, j, ii, w, h;
	result = upper;
	mat = horzcat(upper, eye(upper.rows));
	h = mat.rows;
	w = mat.cols;

	for (i = h - 1; i >= 0; i--) {
		for (ii = h - 1; ii > i; ii--) {
			mat.block(i, 0, 1, w) -= mat.block(ii, 0, 1, w)*mat(i, ii);
			//cout << "mat.block(" << i << ", 0, 1, w) -= mat.block(" << ii << ", 0, 1, w)*mat(" << i << ", " << ii << ");\n";
			//cout << mat << endl;
		}

		mat.block(i, 0, 1, w) /= mat(i, i);
		//cout << "mat.block(" << i << ", 0, 1, w) /= mat(" << i << ", " << i << "); \n";
		//cout << mat << endl;
	}
	result = mat.block(0, upper.rows, h, h);

	return result;
}

Matrix Matrix::inverse()
{
	Matrix lower, upper, lower_inv, upper_inv;
	LU_decomp(lower, upper, *this);
	//cout << "LU_decomp returned: " << LU_decomp(lower, upper, *this) << endl;
	//cout << "Lower: \n" << lower << endl;
	//cout << "Upper: \n" << upper << endl;
	lower_inv = fwd_sub(lower);
	upper_inv = bck_sub(upper);
	//cout << "Lower inverse: \n" << lower_inv << endl;
	//cout << "Upper inverse: \n" << upper_inv << endl;
	return   upper_inv * lower_inv;
}

Matrix Matrix::horzcat(const Matrix& ls, const Matrix& rs) {
	Matrix result(ls.rows, ls.cols + rs.cols);
	int i, ii;
	if (ls.rows != rs.rows) {
		throw std::out_of_range("These matrices' row sizes don't match");
	}
	else {
		for (i = 0; i < result.rows; i++) {
			for (ii = 0; ii < result.cols; ii++) {
				if (ii < ls.cols) {
					result(i, ii) = ls(i, ii);
				}
				else {
					result(i, ii) = rs(i, ii - ls.cols);
				}
			}
		}
	}
	return result;
}

Matrix Matrix::vertcat(const Matrix& top, const Matrix& bot) {
	Matrix result(top.rows + bot.rows, top.cols);
	int i, ii;
	if (top.cols != bot.cols) {
		throw std::out_of_range("These matrices' col sizes don't match");
	}
	else {
		for (i = 0; i < result.rows; i++) {
			for (ii = 0; ii < result.cols; ii++) {
				if (i < top.rows) {
					result(i, ii) = top(i, ii);
				}
				else {
					result(i, ii) = bot(i - top.rows, ii);
				}
			}
		}
	}
	return result;
}