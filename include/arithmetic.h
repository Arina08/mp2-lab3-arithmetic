#include <string>
#include <vector>
#include <iostream>
#include "stack.h"
#include <set>

using std::string;
using std::vector;
using std::set;

class Term
{
private:
	char operation;
	double value;
	bool IsValue;
	string variableName;
public:
	Term() = default;
	Term(const double &_value) {
		value = _value;
		IsValue = true;
	}
	Term(const char &_operation) {
		operation = _operation;
		IsValue = false;
	}
	Term(const string &varName) {
		variableName = varName;
		IsValue = true;
	}
	void printTerm() {
		if (IsValue) {
			std::cout << value << " ";
		}
		else {
			std::cout << operation << " ";
		}
	}
	double getValue() { return value; }
	char getOperation() { return operation; }
	bool isValue() { return IsValue; }
	string getVariableName() { return variableName; }
	void SetValue(double Value) {
		value = Value;
	}
	int priority();
};

class Arithmetic 
{
private:
	vector<Term> terms;
	set<string> variables;
	void stringToTerm(const string &expression = "");
	void isCorrect();
	void termToPostfix();
public:
	Arithmetic(const string &expression = "");
	bool hasVariables();
	double calculate();
	void print() {
		for (size_t i = 0; i < terms.size(); i++) {
			Term lexeme(terms[i]);
			lexeme.printTerm();
		}
	}
};

