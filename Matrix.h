/*
 *  Matrix.h
 *  Matrix_Calculator
 *
 *  Created by Daniel Spencer on 11/23/09.
 *  Copyright 2009 Oberlin College. All rights reserved.
 *
 */

#ifndef __MATRIX_H__
#define __MATRIX_H__

#ifdef ADD_ERROR_CHECKS
#define ERROR ADD_CHECK_HERE
#else
#define ERROR
#endif

#include "Value.h"

class Matrix: public Value {
public:
	Matrix(int _h, int _w);
	Matrix(const Matrix& r);
	~Matrix();
	
	//Binary Operators
	Matrix operator+(const Matrix& r) const;
	Matrix operator-(const Matrix& r) const;
	Matrix operator*(const Matrix& r) const;
	Matrix operator*(const NumericValue& r) const;
	Matrix operator/(const NumericValue& f) const;
	Matrix operator^(const NumericValue& f) const;
	
	//Unary operators
	Matrix operator-() const;
	const Matrix& operator=(const Matrix& r);
	Matrix transpose() const;
	
	NumericValue det() const;
	
	//Access elements
	NumericValue& operator()(int _h, int _w);
	const NumericValue& operator()(int _h, int _w) const;
	const int w, h;
private:
	//Don't access this directly! use get data
	NumericValue* data;
};


//*,/
//+,-
//unary -, transpose(~), inverse(!)
#endif
