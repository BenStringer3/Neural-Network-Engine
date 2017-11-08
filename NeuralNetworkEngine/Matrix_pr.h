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

		Matrix_pr& operator/=(const double scalar);
	
		Matrix_pr(unsigned int r, unsigned int c);

		double& operator()(unsigned int n);

		double & operator()(unsigned int r, unsigned int c);
	
		Matrix_pr& operator=(const Matrix& mat);


		Matrix_pr& operator*=(const double scalar);

		Matrix_pr& operator-=(const double scalar);

		Matrix_pr& operator-=(const Matrix mat);

		Matrix_pr& operator+=(const Matrix mat);

		Matrix operator*(const double scalar) const;

		friend std::ostream& operator<<(std::ostream& os, const Matrix_pr& mat)
		{
			int i, ii;
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
		
		double& max();

		void maxLocation(unsigned int & row, unsigned int & col);

		double avg();
};


