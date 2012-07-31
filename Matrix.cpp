/*
 *  Matrix.cpp
 *  Matrix_Calculator
 *
 *  Created by Daniel Spencer on 11/23/09.
 *  Copyright 2009 Oberlin College. All rights reserved.
 *
 */

#include "Matrix.h"

Matrix::Matrix(int _h, int _w): data(0), w(_w), h(_h) {
	data= new NumericValue[h*w];
}
Matrix::Matrix(const Matrix& r): w(r.w), h(r.h) {
	data = new NumericValue[h*w];
	(*this) = r;
}
Matrix::~Matrix() {
	delete[] data;
}


Matrix Matrix::operator+(const Matrix& r) const {
	ERROR
	Matrix rtn(h, w);
	for (int a = 0; a < h; a++ ) {
		for ( int b = 0; b < w; b++ ) {
			rtn(a,b) = (*this)(a,b) + r(a,b);
		}
	}
	return rtn;
}

Matrix Matrix::operator*(const Matrix& r) const {
	ERROR
	Matrix rtn(h, r.w);
	for (int a = 0; a < h; a++ ) {
		for ( int b = 0; b < r.w; b++ ) {
			NumericValue sum(0);
			for ( int c = 0; c < w; c++ )
				sum = sum + ((*this)(a,c) * r(c,b));
			rtn(a,b) = sum;
		}
	}
	return rtn;
}

Matrix Matrix::operator^(const NumericValue& r) const {
	Matrix current(h,w);
	for (int a = 0; a < h; a++ ) {
		current(a,a) = NumericValue(1);
	}
	float f = r.val;
	while (f > 0 ) {
		current = current * (*this);
		f--;
	}
	return current;
}

NumericValue Matrix::det() const {
	ERROR
	if (h == 1)
		//A one by one matrix
		return (*this)(0,0);
	if (h == 2)
		//A two by two matrix
		return NumericValue((*this)(0,0) * (*this)(1,1) - (*this)(1,0) * (*this)(0,1));
	//Else, devise the det based on submatrices
	
	NumericValue total(0);
	for (int a = 0; a < h; a++ ) {
		Matrix m(h-1,h-1);
		for ( int b = 0; b < h-1; b++ ) {
			for (int c = 0; c < h-1; c++ ) {
				m(b,c) = (*this)((b<a?b:b+1), c+1);
			}
		}
		if ( a%2 == 1)
			total = total - m.det();
		else 
			total = total + m.det();
	}
	return total;
}

Matrix Matrix::operator-(const Matrix& r) const {
	return (*this)+(-r);
}

Matrix Matrix::operator*(const NumericValue& r) const {
	Matrix rtn(h,w);
	for (int a = 0; a < h; a++ ) {
		for ( int b = 0; b < w; b++ ) {
			rtn(a,b) = (*this)(a,b) * r;
		}
	}
	return rtn;
}
Matrix Matrix::operator/(const NumericValue& r) const {
	ERROR

	return (*this)*(NumericValue(1.0f) / r);
}

const Matrix& Matrix::operator=(const Matrix& r) {
	ERROR
	for (int a = 0; a < h; a++ ) {
		for ( int b = 0; b < w; b++ ) {
			(*this)(a,b) = r(a,b);
		}
	}
	return *this;
}

Matrix Matrix::operator-() const {
	return (*this) * -1.0f;
}

Matrix Matrix::transpose() const {
	Matrix rtn(w, h);
	for (int a = 0; a < w; a++ ) {
		for ( int b = 0; b < h; b++ ) {
			rtn(a,b) = (*this)(b,a);
		}
	}
	return rtn;
}




NumericValue& Matrix::operator()(int _h, int _w) {
	ERROR
	return data[_w*h+_h];
}
const NumericValue& Matrix::operator()(int _h, int _w) const {
	ERROR
	return data[_w*h+_h];
}