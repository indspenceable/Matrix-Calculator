/*
 *  Expression.h
 *  Matrix_Calculator
 *
 *  Created by Daniel Spencer on 12/2/09.
 *  Copyright 2009 Oberlin College. All rights reserved.
 *
 */

#include "Value.h"
#include "C_Ptr.h"
#include "Matrix.h"
#include <string>
#include <utility>

class Expression {
public:
	virtual ~Expression() {}
	bool checkSemantics() {
		return (type() != ERROR_VALUE);
	}
	virtual C_Ptr<Value> evaluate() = 0;
	virtual VALUE_TYPE type() = 0;
	virtual std::pair<int,int> getDims() = 0; //Only called on type() == MATRIX_VALUE
};


class MultExpression: public Expression {
public:
	MultExpression(C_Ptr<Expression> _l, C_Ptr<Expression> _r): l(_l), r(_r) {}
	virtual ~MultExpression() {}
	virtual VALUE_TYPE type() {
		if (l.null() || r.null() || l->type() == ERROR_VALUE || r->type() == ERROR_VALUE)
			return ERROR_VALUE;
		if (l->type() == NUMERIC_VALUE &&
			r->type() == NUMERIC_VALUE)
			return NUMERIC_VALUE;
		if (l->type() == MATRIX_VALUE &&
			r->type() == MATRIX_VALUE &&
			l->getDims().second != r->getDims().first)
				return ERROR_VALUE;
		return MATRIX_VALUE;
	}
	//Only called when no error.
	virtual C_Ptr<Value> evaluate() {
		if (l->type() == r->type()) {
			//Both matrix, or both numbers;
			if ( l->type() == MATRIX_VALUE ) {
				C_Ptr<Matrix> lv = l->evaluate().spawn<Matrix>();
				C_Ptr<Matrix> rv = r->evaluate().spawn<Matrix>();
				return C_Ptr<Value>(new Matrix(*lv * *rv));
			} else {
				C_Ptr<NumericValue> lv = l->evaluate().spawn<NumericValue>();
				C_Ptr<NumericValue> rv = r->evaluate().spawn<NumericValue>();
				return C_Ptr<Value>(new NumericValue(*lv * *rv));
			}
		} else {
			if (l->type() == MATRIX_VALUE ) {
				C_Ptr<Matrix> lv = l->evaluate().spawn<Matrix>();
				C_Ptr<NumericValue> rv = r->evaluate().spawn<NumericValue>();
				return C_Ptr<Value>(new Matrix(*lv * *rv));
			} else {
				C_Ptr<NumericValue> lv = l->evaluate().spawn<NumericValue>();
				C_Ptr<Matrix> rv = r->evaluate().spawn<Matrix>();
				return C_Ptr<Value>(new Matrix(*rv * *lv));
			}
		}
	}
	virtual std::pair<int,int> getDims() {
		if (l->type() == MATRIX_VALUE && r->type() == MATRIX_VALUE)
			return std::make_pair(l->getDims().first,r->getDims().second);
		else if (l->type() == MATRIX_VALUE)
			return l->getDims();
		else return r->getDims();
	}
private:
	C_Ptr<Expression> l;
	C_Ptr<Expression> r;
};



class AddExpression : public Expression {
public:
	AddExpression(C_Ptr<Expression> _l, C_Ptr<Expression> _r): l(_l), r(_r) {}
	virtual ~AddExpression() {}
	virtual VALUE_TYPE type() {
		if (l.null() || r.null() || l->type() == ERROR_VALUE || r->type() == ERROR_VALUE)
			return ERROR_VALUE;
		if (l->type() != r->type())
			return ERROR_VALUE;
		if (l->type() == MATRIX_VALUE) {
			if (l->getDims() != r->getDims())
				return ERROR_VALUE;
			return MATRIX_VALUE;
		}
		return NUMERIC_VALUE;
	}
	virtual std::pair<int,int> getDims() {
		return l->getDims();
	}
	virtual C_Ptr<Value> evaluate() {
		if ( l->type() == MATRIX_VALUE ) {
			C_Ptr<Matrix> lv = l->evaluate().spawn<Matrix>();
			C_Ptr<Matrix> rv = r->evaluate().spawn<Matrix>();
			return C_Ptr<Value>(new Matrix(*lv + *rv));
		} else {
			C_Ptr<NumericValue> lv = l->evaluate().spawn<NumericValue>();
			C_Ptr<NumericValue> rv = r->evaluate().spawn<NumericValue>();
			return C_Ptr<Value>(new NumericValue(*lv + *rv));
		}
	}
private:
	C_Ptr<Expression> l;
	C_Ptr<Expression> r;
};

class DivExpression : public Expression {
public:
	DivExpression(C_Ptr<Expression> _l, C_Ptr<Expression> _r): l(_l), r(_r) {}
	virtual ~DivExpression() {}
	virtual VALUE_TYPE type() {
		if (l.null() || r.null() || l->type() == ERROR_VALUE || r->type() == ERROR_VALUE)
			return ERROR_VALUE;
		if (NUMERIC_VALUE != r->type())
			return ERROR_VALUE;
		C_Ptr<NumericValue> nv = r->evaluate().spawn<NumericValue>();
		if ( nv->val == 0 ) {
			return ERROR_VALUE;
		}
		if (l->type() == MATRIX_VALUE) {
			return MATRIX_VALUE;
		}
		return NUMERIC_VALUE;
	}
	virtual std::pair<int,int> getDims() {
		return l->getDims();
	}
	virtual C_Ptr<Value> evaluate() {
		if ( l->type() == MATRIX_VALUE ) {
			C_Ptr<Matrix> lv = l->evaluate().spawn<Matrix>();
			C_Ptr<NumericValue> rv = r->evaluate().spawn<NumericValue>();
			//AHH CHANGE THIS.
			return C_Ptr<Value>(new Matrix(*lv / *rv));
		} else {
			C_Ptr<NumericValue> lv = l->evaluate().spawn<NumericValue>();
			C_Ptr<NumericValue> rv = r->evaluate().spawn<NumericValue>();
			return C_Ptr<Value>(new NumericValue(*lv / *rv));
		}
	}
private:
	C_Ptr<Expression> l;
	C_Ptr<Expression> r;
};


class ExpExpression : public Expression {
public:
	ExpExpression(C_Ptr<Expression> _l, C_Ptr<Expression> _r): l(_l), r(_r) {}
	virtual ~ExpExpression() {}
	virtual VALUE_TYPE type() {
		if (l.null() || r.null() || l->type() == ERROR_VALUE || r->type() == ERROR_VALUE)
			return ERROR_VALUE;
		if (NUMERIC_VALUE != r->type())
			return ERROR_VALUE;
		if (l->type() == MATRIX_VALUE) {
			if (l->getDims().first != l->getDims().second)
				return ERROR_VALUE;
			if (! r->evaluate().spawn<NumericValue>()->intval() ||
				r->evaluate().spawn<NumericValue>()->val < 0 )
				return ERROR_VALUE;
			return MATRIX_VALUE;
		}
		return NUMERIC_VALUE;
	}
	virtual std::pair<int,int> getDims() {
		return l->getDims();
	}
	virtual C_Ptr<Value> evaluate() {
		if ( l->type() == MATRIX_VALUE ) {
			C_Ptr<Matrix> lv = l->evaluate().spawn<Matrix>();
			C_Ptr<NumericValue> rv = r->evaluate().spawn<NumericValue>();

			return C_Ptr<Value>(new Matrix(*lv ^ *rv));
		} else {
			C_Ptr<NumericValue> lv = l->evaluate().spawn<NumericValue>();
			C_Ptr<NumericValue> rv = r->evaluate().spawn<NumericValue>();
			return C_Ptr<Value>(new NumericValue(*lv ^ *rv));
		}
	}
private:
	C_Ptr<Expression> l;
	C_Ptr<Expression> r;
};

class MinusExpression : public Expression {
public:
	MinusExpression(C_Ptr<Expression> _l,C_Ptr<Expression> _r): l(_l), r(_r) {}
	virtual ~MinusExpression() {
	}
	virtual VALUE_TYPE type() {
		if (l.null() || r.null() || l->type() == ERROR_VALUE || r->type() == ERROR_VALUE)
			return ERROR_VALUE;
		if (l->type() != r->type())
			return ERROR_VALUE;
		if (l->type() == MATRIX_VALUE) {
			if (l->getDims() != r->getDims())
				return ERROR_VALUE;
			return MATRIX_VALUE;
		}
		return NUMERIC_VALUE;
	}
	virtual std::pair<int,int> getDims() {
		return l->getDims();
	}
	virtual C_Ptr<Value> evaluate() {
		if ( l->type() == MATRIX_VALUE ) {
			C_Ptr<Matrix> lv = l->evaluate().spawn<Matrix>();
			C_Ptr<Matrix> rv = r->evaluate().spawn<Matrix>();
			return C_Ptr<Value>(new Matrix(*lv - *rv));
		} else {
			C_Ptr<NumericValue> lv = l->evaluate().spawn<NumericValue>();
			C_Ptr<NumericValue> rv = r->evaluate().spawn<NumericValue>();
			return C_Ptr<Value>(new NumericValue(*lv - *rv));
		}
	}
private:
	C_Ptr<Expression> l;
	C_Ptr<Expression> r;
};

class UnaryMinusExpression: public Expression {
public:
	UnaryMinusExpression(C_Ptr<Expression> _e): e(_e) {}
	virtual ~UnaryMinusExpression() {}
	virtual VALUE_TYPE type() {
		if (e.null()) {
			return ERROR_VALUE;
		}
		return e->type();
	}
	virtual std::pair<int,int> getDims()
	{
		return e->getDims();
	}
	virtual C_Ptr<Value> evaluate() {
		if ( e->type() == MATRIX_VALUE ) {
			C_Ptr<Matrix> ev = e->evaluate().spawn<Matrix>();
			return C_Ptr<Value>(new Matrix(- *ev));
		} else {
			C_Ptr<NumericValue> ev = e->evaluate().spawn<NumericValue>();
			return C_Ptr<Value>(new NumericValue(- *ev));
		}
	}
private:
	C_Ptr<Expression> e;
};

class TransposeExpression: public Expression {
public:
	TransposeExpression(C_Ptr<Expression> _e): e(_e) {}
	virtual ~TransposeExpression() {}
	virtual VALUE_TYPE type() {
		if (e.null() || e->type() != MATRIX_VALUE)
			return ERROR_VALUE;
		return MATRIX_VALUE;
	};
	virtual std::pair<int,int> getDims() {
		return std::make_pair(e->getDims().second, e->getDims().first);
	}
	virtual C_Ptr<Value> evaluate() {
		C_Ptr<Matrix> ev = e->evaluate().spawn<Matrix>();
		return C_Ptr<Value>(new Matrix(ev->transpose()));
	}
private:
	C_Ptr<Expression> e;
};

class InverseExpression: public Expression {
public:
	InverseExpression(C_Ptr<Expression> _e): e(_e) {}
private:
	C_Ptr<Expression> e;
};

class NumericExpression: public Expression {
public:
	NumericExpression(float f): val(f) {}
	
	virtual C_Ptr<Value> evaluate() {
		return C_Ptr<Value>(new NumericValue(val));
	}
	
	virtual VALUE_TYPE type() {
		return NUMERIC_VALUE;
	}
	virtual std::pair<int,int> getDims() {
		return std::make_pair(0, 0);
	}
private:
	NumericValue val;
};

class MatrixExpression: public Expression {
public:
	MatrixExpression(int a, int b): mat(a,b) {}
	virtual C_Ptr<Value> evaluate() {
		return C_Ptr<Value>(new Matrix(mat));
	}
	
	virtual VALUE_TYPE type() {
		return MATRIX_VALUE;
	}
	virtual std::pair<int,int> getDims() {
		return std::make_pair(mat.h, mat.w);
	}
	
public:
	Matrix mat;
};

class DetExpression: public Expression {
public:
	DetExpression(C_Ptr<Expression> m): myMat(m) {}
	virtual C_Ptr<Value> evaluate() {
		return C_Ptr<Value>(new NumericValue(myMat.spawn<MatrixExpression>()->evaluate().spawn<Matrix>()->det()));
	}
	
	virtual VALUE_TYPE type() {
		if (myMat.null() || myMat.spawn<MatrixExpression>().null() )
			return ERROR_VALUE;
		return NUMERIC_VALUE;
	}
	virtual std::pair<int,int> getDims() {
		return std::make_pair(0,0);
	}
	
public:
	C_Ptr<Expression> myMat;
};

class VariableExpression: public Expression {
public:
	VariableExpression(C_Ptr<Value> val): var(val) {}
	virtual C_Ptr<Value> evaluate() {
		return var;
	}
	virtual VALUE_TYPE type() {
		if (! var.spawn<NumericValue>().null()) return NUMERIC_VALUE;
		if (! var.spawn<Matrix>().null()) return MATRIX_VALUE;
		//shouldn't happen!
		return ERROR_VALUE;
	}
	virtual std::pair<int,int> getDims() {
		C_Ptr<Matrix> mat = var.spawn<Matrix>();
		if (mat.null()) return std::make_pair<int,int>(0, 0);
		return std::make_pair(mat->h, mat->w);
	}
private:
	C_Ptr<Value> var;
};