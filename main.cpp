/*
 *  main.cpp
 *  Matrix_Calculator
 *
 *  Created by Daniel Spencer on 11/23/09.
 *  Copyright 2009 Oberlin College. All rights reserved.
 *
 */


#include "Matrix.h"
#include "Interpreter.h"

#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;

bool quit = false;

int main() {
	//Environment base;
	Interpreter i;
	while (i.parseLine());
	return 0;
}

