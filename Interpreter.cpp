/*
 *  Parser.cpp
 *  Matrix_Calculator
 *
 *  Created by Daniel Spencer on 11/30/09.
 *  Copyright 2009 Oberlin College. All rights reserved.
 *
 */

#include "Interpreter.h"
#include <cstdlib>
#include "C_Ptr.h"
#include <vector>
#include <memory>

#include <string>
#include <sstream>
#include <iostream>

template <class T>
bool from_string(T& t, 
                 const std::string& s, 
                 std::ios_base& (*f)(std::ios_base&) = std::dec)
{
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}

Interpreter::Interpreter() {
	C_Ptr<HelpTopic> welcome(new HelpTopic("WELCOME", "Welcome to the matrix manipulator. Please begin calculating, or use HELP for helpfiles."));
	C_Ptr<HelpTopic> help(new HelpTopic("HELP","This program acts as a calculator that can manipulate matrices and real numbers in a variety of ways. To access subtopics, use HELP [SUBTOPIC] ... [SUBTOPIC]"));
	{
		C_Ptr<HelpTopic> commands(new HelpTopic("COMMANDS","There are a number of commands that you can use in this calculator. These commands are not a valid part of an expression, but can be used at the begining of a line of input. All commands are in all capital letters. See subtopics for more information."));
		{
			C_Ptr<HelpTopic> print_command(new HelpTopic("PRINT","To evaluate and print an expression, simply enter the expression into the calculator with no command preceding it."));
			C_Ptr<HelpTopic> set_command(new HelpTopic("SET", "To set the value of a variable, use SET <expression>. This will overwrite any previous value stored in that variable name. Variable names should be all lower case, letters only."));
			C_Ptr<HelpTopic> help_command(new HelpTopic("HELP", "To access the help files, use the HELP command. Additionally, alias exist for \"?\" and \"h\". However, it would appear you already know this."));
			C_Ptr<HelpTopic> quit_command(new HelpTopic("QUIT", "To finish using the calculator, simply type QUIT and the program will exit."));
			commands->addSubTopic(print_command);
			commands->addSubTopic(set_command);
			commands->addSubTopic(help_command);
			commands->addSubTopic(quit_command);
		}
		C_Ptr<HelpTopic> operations(new HelpTopic("OPERATIONS", "The manipulator offers a number of operations for use with both numbers and matrices: +,-,*,/,^. Additionally, there are some matrix-only operators: Transpose(~). Operator precidence is as follows: Parenthesis, Multiplication and Division, Addition and Subtraction, Unary Operations."));
		{
			C_Ptr<HelpTopic> addition_subtraction(new HelpTopic("ADDITION AND SUBTRACTION", "Any two numeric values, or two matrix values of the same size, can be added to or subtracted from one another, producing the expected result, using the '+' or '-' operators."));
			C_Ptr<HelpTopic> multiplication(new HelpTopic("MULTIPLICATION", "Any two scalar values can be multiplied together, given the expected result, and any matrix and scalar value can be multiplied together, multiplying every element of the matrix by the scalar. Two matrices can be multiplied together, following the rules of matrix multiplication, if the the width of the left matrix is equal to the height of the right matrix. To multiply in any context, use the '*' operator."));
			C_Ptr<HelpTopic> division(new HelpTopic("DIVISION", "Any scalar, or any any matrix value can be divided by any non-zero scalar value, giving the appropriate response. This is acheived with the '/' operator."));
			C_Ptr<HelpTopic> determinant(new HelpTopic("DETERMINANT", "Any matrix value preceded by a '@' will evaluate to the determinant of that matrix."));
			C_Ptr<HelpTopic> transpose(new HelpTopic("TRANSPOSE", "Any matrix value preceded by a '~' will evaluate to the transposed matrix (that is, refected over the main diagonal."));

			operations->addSubTopic(determinant);
			operations->addSubTopic(transpose);
			operations->addSubTopic(division);
			operations->addSubTopic(division,"/");
			operations->addSubTopic(multiplication);
			operations->addSubTopic(multiplication,"*");
			operations->addSubTopic(multiplication,"-");
			operations->addSubTopic(addition_subtraction,"ADDITION");
			operations->addSubTopic(addition_subtraction,"SUBTRACTION");
			operations->addSubTopic(addition_subtraction,"+");
			operations->addSubTopic(addition_subtraction,"-");
		}
		C_Ptr<HelpTopic> matrices(new HelpTopic("MATRICES", "To make a matrix, one uses the # token, followed by the correct arugments for the type of matrix one would wish to create."));
		{
		  C_Ptr<HelpTopic> filledmatrix(new HelpTopic("STANDARD", "Syntax: #[HEIGHT WIDTH][<1,1> <1,2>... <2,1> <2,2>... <HEIGHT,WIDTH].\n Example: #[1 2][0 5] -> [[0] [5]]"));
		  C_Ptr<HelpTopic> emptymatrix(new HelpTopic("EMPTY", "Syntax: #E[HEIGHT WIDTH]. Makes a matrix of the correct size, filled with zeros. Example: #E[1 2]-> [[0] [0]]"));
		  C_Ptr<HelpTopic> identity(new HelpTopic("IDENTITY", "Syntax: #I[HEIGHT]. Makes a square matrix of the correct size, with zeros except for the main diagonal, which is made up of 1s."));

		  matrices->addSubTopic(filledmatrix);
		  matrices->addSubTopic(emptymatrix);
		  matrices->addSubTopic(identity);
		}

		help->addSubTopic(matrices);
		help->addSubTopic(commands);
		help->addSubTopic(operations);
		help->addSubTopic(welcome);
	}
	helpTopics = help;
	welcome->display();
}

bool Interpreter::parseLine() {
	std::string str = "";
	std::getline(std::cin, str);
	tokenList.clear();
	tokenize(str);
	//std::string token = nextToken(str);
	clearError();
	
	if ( accept("SET") ) {
		std::string toSet = nextToken();
		if ( toSet == "" ) {
			std::cout << "You must enter a variable name and value!" << std::endl;
		} else {
			C_Ptr<Expression> e = parseExpression();
			if (hasError()) {
				std::cout << "Error: " << cError << std::endl;
				
			} else if (! e.null()) {
				if (e->checkSemantics()) {
					C_Ptr<Value> v = e->evaluate();
					vTable[toSet] = v;
					std::cout << "Set var {" << toSet << "} successfully." << std::endl;
				} else {
					std::cout << "Semantics Error." << std::endl;
				}
			} else {
				std::cout << "Parse Error: " << "(call to get_error() goes here.)" << std::endl;
			}
		}
	} else if (accept("QUIT")) {
		return false;
	} else if (accept("HELP")) {
		C_Ptr<HelpTopic> c = helpTopics;
		std::string s;
		while ((s = nextToken()) != "") {
			c = c->getSubTopic(s);
		}
		c->display();
	} else {
		C_Ptr<Expression> e = parseExpression();
		if (hasError()) {
			std::cout << "Error: " << cError << std::endl;
		} else if (!e.null()) {
			if (e->checkSemantics()) {
				C_Ptr<Value> v = e->evaluate();
				C_Ptr<NumericValue> nv = v.spawn<NumericValue>();
				if (! nv.null() ) {
					std::cout << "Value is: " << nv->val << std::endl;
				} else {
					C_Ptr<Matrix> mv = v.spawn<Matrix>();
					if ( ! mv.null() ) {
						std::cout << "[";
						for ( int a = 0; a < mv->h; a ++ ) {
							for (int b = 0; b < mv->w; b++ ) {
								std::cout << "[" << (*mv)(a,b).val << "]";
							}
							if (a < mv->h - 1)
								std::cout << std::endl << " ";
							else
								std::cout << "]" << std::endl;
						}
					}
				}
			} else {
				std::cout << "Semantics didn't clear." << std::endl;
			}
		} else {
			std::cout << "Parse Error: " << "(call to get_error() goes here.)" << std::endl;
		}
	}
	tokenList.clear();
	return true;
}

void Interpreter::tokenize(std::string s) {
	while (s.size() > 0) {
		while (s.size() > 0 && s[0] == ' ') s.erase(0,1);
		if ( s.size() == 0 ) return;
		if ( s[0] >= '0' && s[0] <= '9' ) {
			std::string t = "";
			while ( s[0] >= '0' && s[0] <= '9' || s[0] == '.' ) {
				t += s[0];
				s.erase(0,1);
			}
			tokenList.push_back(t);
		} else if ((s[0] >= 'a' && s[0] <= 'z') ||
				   (s[0] >= 'A' && s[0] <= 'Z')) {
			std::string t = "";
			while ((s[0] >= 'a' && s[0] <= 'z') ||
				   (s[0] >= 'A' && s[0] <= 'Z')) {
				t += s[0];
				s.erase(0,1);
			}
			tokenList.push_back(t);
		} else {
			tokenList.push_back(std::string("") + s[0]);
			s.erase(0,1);
		}
	}
}

bool Interpreter::accept(std::string t) {
	if ( tokenList.size() > 0 && *tokenList.begin() == t ) {
		tokenList.pop_front();
		return true;
	}
	return false;
	/*
	if ( s.size() < t.size() ) return false;
	for (int a = 0; a < t.size(); a++ ) {
		if ( s[a] != t[a] ) return false;
	}
//	if (s.size() > t.size() &&
//		s[t.size()] != ' ' )return false;
	s = s.substr(t.size()+1);
	return true;
	 */
}

std::string Interpreter::nextToken() {
	if (tokenList.size() == 0)
		return "";
	std::string rtn = *tokenList.begin();
	tokenList.pop_front();
	return rtn;
}

C_Ptr<Expression> Interpreter::parseExpression() {
	return parseAddExpression();
}

C_Ptr<Expression> Interpreter::parseMultExpression() {
	C_Ptr<Expression> expr1 = parseExpExpression();
	while (1) {
		if (accept("*"))
			expr1 = C_Ptr<Expression>(new MultExpression(expr1,parseExpExpression()));
		else if ( accept("/"))
			expr1 = C_Ptr<Expression>(new DivExpression(expr1, parseExpExpression()));
		else break;
	}
	return expr1;
}
C_Ptr<Expression> Interpreter::parseAddExpression() {
	C_Ptr<Expression> expr1 = parseMultExpression();
	while (1) {
		if (accept("+")) 
			expr1 = C_Ptr<Expression>(new AddExpression(expr1,parseMultExpression()));
		else if ( accept("-"))
			expr1 = C_Ptr<Expression>(new MinusExpression(expr1, parseMultExpression()));
		else
			break;
	}
	//if (accept(s,"-"))
	//	return new MinusExpression(expr1,parseUnaryExpression(s));
	return expr1;
}
C_Ptr<Expression> Interpreter::parseUnaryExpression() {
	if (accept("-"))
		return C_Ptr<Expression>(new UnaryMinusExpression(parseUnaryExpression()));
	if (accept("~"))
		return C_Ptr<Expression>(new TransposeExpression(parseUnaryExpression()));
	if (accept("@"))
		return C_Ptr<Expression>(new DetExpression(parseUnaryExpression()));
	//if (accept(s,"%"))
	//	return new InverseExpression(parseParenExpression(s));
	return parseParenExpression();
}

C_Ptr<Expression> Interpreter::parseExpExpression() {
	C_Ptr<Expression> expr = parseUnaryExpression();
	while (accept("^")) 
		expr = C_Ptr<Expression>(new ExpExpression(expr,parseUnaryExpression()));
	return expr;
}

C_Ptr<Expression> Interpreter::parseParenExpression() {
	if (accept("(")) {
		C_Ptr<Expression> e = parseExpression();
		if (accept(")"))
			return e;
		markError("Expected \")\" after paren expression");
		return C_Ptr<Expression>(0);
	}
	return parseTokenExpression();
}

C_Ptr<Expression> Interpreter::parseTokenExpression() {
	if (accept("#") )
		return parseMatrix();
	if (accept("$") )
		return parseVariable();
	return parseNumber();
}

C_Ptr<Expression> Interpreter::parseVariable() {
	//Next token should be a variable name
	std::string vName = nextToken();
	if (vName == "" || vTable.find(vName) == vTable.end()) {
		markError("Reference to non-existant variable: " + vName);
		return C_Ptr<Expression>(NULL);
	}
	return C_Ptr<Expression>(new VariableExpression(vTable[vName]));
}
C_Ptr<Expression> Interpreter::parseMatrix() {
	if ( accept("[") ) {
		float h, w;
		if (!from_string<float>(h, nextToken()) || !from_string<float>(w, nextToken())) {
			markError("Expected height and width as first elements of a matrix.");
			return C_Ptr<Expression>(NULL);
		}
		if (!accept("]") || !accept("[") ) {
			markError("expected ] [ between dimensions and elements of a matrix.");
			return C_Ptr<Expression>(NULL);
		}
		
		C_Ptr<MatrixExpression> mex(new MatrixExpression(h,w));
		std::vector<Expression*> numbers;
		for ( int a = 0; a < h; a++ ) {
			for (int b = 0; b < w; b++) {
				C_Ptr<Expression> nex(parseExpression());
				//is this value ok?
				if ( nex.null() || nex->type() != NUMERIC_VALUE) {
					markError("Expected numbers for the elements of a matrix.");
					return C_Ptr<Expression>(NULL);
				}
				mex->mat(a,b) = nex->evaluate().spawn<NumericValue>()->val;
			}
		}
		if (accept("]"))
			return mex.spawn<Expression>();
		markError("Expected \"]\" after matrix");
		return C_Ptr<Expression>(NULL);
	} else if ( accept("I") ) {
		if ( !accept("["))
			markError("Expected \"[\" after Identity matrix tokens.");
			return C_Ptr<Expression>(NULL);
		int h;
		if (!from_string<int>(h, nextToken())) {
			markError("Expected integer for identity matrix dimension.");
			return C_Ptr<Expression>(NULL);
		}
		C_Ptr<MatrixExpression> mex(new MatrixExpression(h,h));
		for ( int a = 0; a < h; a++ ) {
			mex->mat(a,a) = 1;
		}
		if (accept("]"))
			return mex.spawn<Expression>();
		markError("Expected \"]\" after identity matrix");
		return C_Ptr<Expression>(NULL);
	} else if ( accept("E")) {
		if ( !accept("["))
			markError("Expected \"[\" after Zero matrix tokens.");
			return C_Ptr<Expression>(NULL);
		int h,w;
		if (!from_string<int>(h, nextToken()) || !from_string<int>(w,nextToken())) {
			markError("Expected integers for zero matrix dimension.");
			return C_Ptr<Expression>(NULL);
		}
		C_Ptr<MatrixExpression> mex(new MatrixExpression(h,h));
		if (accept("]")) 
			return mex.spawn<Expression>();
		markError("Expected \"]\" after zero matrix");
		return C_Ptr<Expression>(NULL);
	}
	markError("Unknown matrix identifier (should be I, E, or [)");
	return C_Ptr<Expression>(NULL);
}



C_Ptr<Expression> Interpreter::parseNumber() {
	float f;
	if (from_string<float>(f, nextToken()))
		return C_Ptr<Expression>(new NumericExpression(f));
	markError("Expected a number but got something else...");
	return C_Ptr<Expression>(NULL);
}
