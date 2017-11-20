#pragma once
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <stdexcept>
#include "Matrix.h"


using namespace std;

class Matrix;

class Matrix_pr
{
	
public:
	friend class Matrix;
		unsigned int rows;
		unsigned int cols;
		vector <double *> data;
	
		Matrix_pr();

		~Matrix_pr();

		Matrix_pr& operator/=(const double scalar);
	
		Matrix_pr(unsigned int r, unsigned int c);

		double& operator()(unsigned int n);

		double & operator()(unsigned int r, unsigned int c);
	
		Matrix_pr& operator=(const Matrix& mat);

		Matrix_pr & pointTo(Matrix & mat);

		Matrix_pr& operator*=(const double scalar);

		Matrix_pr& operator-=(const double scalar);

		Matrix_pr& operator-=(const Matrix mat);

		Matrix_pr& operator+=(const Matrix mat);

		Matrix operator*(const double scalar) const;

		Matrix operator*(const Matrix_pr & mat) const;

		friend std::ostream& operator<<(std::ostream& os, const Matrix_pr& mat)
		{
			unsigned int i, ii;
			for (i = 0; i < mat.rows; i++) {
				for (ii = 0; ii < mat.cols; ii++) {
					os << *mat.data[i*mat.cols + ii] << "  ";
				}
				os << std::endl;
			}
			return os;
		};

		Matrix_pr transpose() const;

		double sum();

		Matrix_pr diagonal();

		Matrix_pr& reshape(unsigned int rows, unsigned int cols);
		
		double max();

		void maxLocation(unsigned int & row, unsigned int & col);

		double avg();

		void assign(const Matrix_pr & mat);

		Matrix operator+(const Matrix & mat) const;

		static Matrix_pr vertcat(const Matrix_pr & top, const Matrix_pr & bot);

		static Matrix_pr horzcat(const Matrix_pr & ls, const Matrix_pr & rs);

		Matrix_pr block(unsigned int startRow, unsigned int startCol, unsigned int height, unsigned int width);

		Matrix_pr cwiseProduct(const Matrix_pr& mat);
};


