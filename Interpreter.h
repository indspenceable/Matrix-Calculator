/*
 *  Parser.h
 *  Matrix_Calculator
 *
 *  Created by Daniel Spencer on 11/30/09.
 *  Copyright 2009 Oberlin College. All rights reserved.
 *
 */

#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <string>
#include <list>
#include <iostream>
#include <map>

#include "Value.h"
#include "Matrix.h"
#include "Expression.h"
#include "HelpTopic.h"



class Interpreter {
public:
	//sets up the help system.
	Interpreter();
	//Returns false if quit;
	bool parseLine();
private:
	
	//Parse
	C_Ptr<Expression> parseExpression();
	C_Ptr<Expression> parseMultExpression();
	C_Ptr<Expression> parseAddExpression();
	C_Ptr<Expression> parseUnaryExpression();
	C_Ptr<Expression> parseParenExpression();
	C_Ptr<Expression> parseExpExpression();
	
	C_Ptr<Expression> parseTokenExpression();
	C_Ptr<Expression> parseMatrix();
	C_Ptr<Expression> parseVariable();
	C_Ptr<Expression> parseNumber();
	
	bool accept(std::string t);
	std::string nextToken();
	
	void getLine();
	
	void tokenize(std::string str);
	std::list<std::string> tokenList;
	int currentToken;
	
	void markError(std::string e) {
		if (! hasError() ) {
			cError = e;
		}
	}
	bool hasError() { return (cError != ""); }
	void clearError() { cError = ""; }
	std::string cError;
	
	
	std::map<std::string, C_Ptr<Value> > vTable;
	C_Ptr<HelpTopic> helpTopics;
};

//ACCEPT - if the next token is ARG 2, then eat it + return true. else return false.



#endif // __INTERPRETER_H__