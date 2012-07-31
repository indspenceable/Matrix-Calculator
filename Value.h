/*
 *  Value.h
 *  Matrix_Calculator
 *
 *  Created by Daniel Spencer on 11/30/09.
 *  Copyright 2009 Oberlin College. All rights reserved.
 *
 */
#ifndef __VALUE_H__
#define __VALUE_H__

#include <cmath>

enum VALUE_TYPE {
	NUMERIC_VALUE,
	MATRIX_VALUE,
	ERROR_VALUE
};

class Value {
public:
	virtual ~Value() {}
};
class NumericValue: public Value {
public:
	NumericValue(float f = 0): val(f) {}
	~NumericValue() {}
	

	NumericValue operator*(const NumericValue& r) const
	{
		return NumericValue(val * r.val);
	}
	NumericValue operator+(const NumericValue& r) const
	{
		return NumericValue(val + r.val);
	}
	NumericValue operator-(const NumericValue& r) const
	{
		return NumericValue(val - r.val);
	}
	NumericValue operator/(const NumericValue& r) const
	{
		return NumericValue(val / r.val);
	}
	NumericValue operator-() const {
		return NumericValue(-val);
	}
	NumericValue operator^(const NumericValue& r) const {
		return NumericValue(std::pow(val, r.val));
	}
	
	bool intval() {
		return ((int)(val) == val);
	}
	
	float val;
};

#endif