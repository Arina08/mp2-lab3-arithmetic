#include <string>
#include <vector>
#include <iostream>
#include "stack.h"

using std::string;
using std::vector;

const string allowedTerms = "0123456789.()+-/* ";

class Term
{
private:
	char operation;
	double isValue;
	bool isType;
public:
	Term() = default;
	Term(const double &_isValue) {
		isValue = _isValue;
		isType = true;
	}
	Term(const char &_operation) {
		operation = _operation;
		isType = false;
	}
	void printTerm() {
		if (isType) {
			std::cout << isValue << " ";
		}
		else {
			std::cout << operation << " ";
		}
	}
	double getValue() { return isValue; }
	char getOperation() { return operation; }
	bool getType() { return isType; }

	int priority();
};

class Arithmetic 
{
private:
	vector<Term> terms;
public:
	Arithmetic() = default;

	void stringToTerm(string &expression);
	void termToPostfix();
	double calculate();
	void print() {
		for (size_t i = 0; i < terms.size(); i++) {
			Term lexeme(terms[i]);
			lexeme.printTerm();
		}
	}
};

bool checkBrackets(const string &s);
bool checkSymbols(const string &s);
bool isCorrect(const string &s);
