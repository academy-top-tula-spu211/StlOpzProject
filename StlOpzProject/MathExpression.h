#pragma once
#include <string>
#include <stack>
#include <map>
#include <algorithm>
#include <exception>

using namespace std;

class MathExpression
{
	string expression;
	string expressionOpz;
	map<string, double> variables;

	map<string, char> functions
	{
		{ "sin", 's' },
		{ "cos", 'c' },
		{ "tg", 't' },
		{ "ctg", 'g' },
	};

	const string bracketsOpen{ "([{" };
	const string bracketsClose{ ")]}" };

	const string operatorsAdditive{ "+-" };
	const string operatorsMultiplex{ "*/" };
public:
	MathExpression(string expression) 
		: expression{ expression } {};
	MathExpression(const char* expression) 
		: expression{ string(expression) } {};
	MathExpression() 
		: expression{ "" } {};

	double& operator[](string varName);
	double& operator[](const char* varName);

	string& Expression(){ return expression; }
	string ExpressionOpz() { return expressionOpz; }

	int CheckBrackets();

	void OpzCreate();
	double OpzCalculate();
};

