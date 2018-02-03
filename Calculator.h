#ifndef CALCULATOR_H
#define CALCULATOR_H

#include<list>
#include <stack>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <algorithm>//remove white spaces in string
#include <sstream>
#include<stdio.h>
#include <tuple>

int start(int);
int menu();
std::list <std::string> getInput();
bool checkExpression(std::string);

std::list <std::string> convertInfix(std::string);
void transferStack2(std::stack <std::string> * expressionStack, std::list <std::string> * expressionList, char * token);
void transferStack2(std::stack <std::string> * expressionStack, std::list <std::string> * expressionList, char * token);
void pushToStack(std::stack <std::string> * expressionStack, char * token);

void postfixEvaluate(std::list <std::string> expressionList);
std::tuple <int, int> popVariables(std::stack <int> * expressionStack);
int convertVariable(std::stack <int> * expressionStack, std::string variable );

#endif // CALCULATOR_H
