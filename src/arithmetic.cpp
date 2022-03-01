#include "../include/arithmetic.h"

int Term::priority()
{
	if (operation == '(' || operation == ')') { return 0; }
	else if (operation == '+' || operation == '-') { return 1; }
	else if (operation == '*' || operation == '/') { return 2; }
	else if (operation == '_') { return 3; }
	else throw "Priority problem";
}

bool Arithmetic::hasVariables() {
	return !variables.empty();
}

Arithmetic::Arithmetic(const string &expression) {
	stringToTerm(expression);
	if (terms.empty())
		throw "Expression is empty";
	isCorrect();
	termToPostfix();
}

void Arithmetic::stringToTerm(const string &expression)
{
	unsigned length = expression.length();
	string stringForNumber;
	string stringForVariable;
	double doubleNumber;
	unsigned i = 0;
	while (i < length) {
		while (expression[i] == ' ') i++; // пропускаем пробелы
		if (expression[i] == '+' || expression[i] == '*' || expression[i] == '/') { // обработка плюс, умножение, деление
			Term operation(expression[i]);
			terms.push_back(operation);
			i++;
		}
		else if (expression[i] == '-') {
			if (terms.empty() || terms.back().isValue() == false) {
				terms.push_back(Term('_'));
			}
			else
				terms.push_back(Term('-'));
			i++;
			while (i < expression.size() && expression[i] == '-') {
				i++;
				terms.push_back(Term('_'));
			}
		}
		else if (expression[i] >= '0' && expression[i] <= '9' || expression[i] == '.') { // обработка чисел
			int pointCounter = 0;
			while (i < expression.size() && (expression[i] >= '0' && expression[i] <= '9' || expression[i] == '.')) {
				stringForNumber += expression[i];
				i++;
				if (expression[i] == '.') {
					pointCounter++;
				}
				if (pointCounter > 1) {
					throw "You have more than one point in the number";
				}
			}
			doubleNumber = stod(stringForNumber);
			Term number(doubleNumber);
			terms.push_back(number);
			stringForNumber.clear(); // очищаем строку для будущих значений
		}
		else if (expression[i] >= 'a' && expression[i] <= 'z' || expression[i] >= 'A' && expression[i] <= 'Z') { // обработка переменных
			while (i < expression.size() && (expression[i] >= 'a' && expression[i] <= 'z' || expression[i] >= 'A' && expression[i] <= 'Z')) {
				stringForVariable += expression[i];
				i++;
			}
			Term variable(stringForVariable);
			terms.push_back(variable);
			variables.insert(stringForVariable);
			stringForVariable.clear();
		}
		else if (expression[i] == ')' || expression[i] == '(') {
			Term bracket(expression[i]);
			terms.push_back(bracket);
			i++;
		}
		else
			throw "String has unknown symbol";
	}
}

void Arithmetic::isCorrect()
{
	Term last_term = Term('(');
	Term current_term;
	Stack<char> brackets;
	for (int i = 0; i < terms.size(); i++) {
		current_term = terms[i];
		char operation = current_term.getOperation();
		if (last_term.isValue() == true) {
			if (current_term.isValue() == true || operation == '(')
				throw "Number or opening bracket can't be after number";
		}
		else if (last_term.getOperation() == '(') {
			if (current_term.isValue() == false && (operation == ')' || operation != '_' && operation != '('))
				throw "Binary operation or closing bracket can't be after opening bracket";
		}
		else if (last_term.getOperation() == ')') {
			if (last_term.isValue() == true || operation == '(')
				throw "Number or opening bracket can't be after closing bracket";
		}
		else {
			if (current_term.isValue() == false && operation != '(' && operation != '_')
				throw "Closing bracket or binary operation can't be after operation";
		}
		if (current_term.getOperation() == '(')
			brackets.push('(');
		else if (current_term.getOperation() == ')') {
			if (brackets.isEmpty())
				throw "Expression has unpaired brackets";
			brackets.pop();
		}
		last_term = current_term;
	}
	if (last_term.isValue() == false && last_term.getOperation() != ')')
		throw "Expression must end with a number or closing bracket";
	if (!brackets.isEmpty())
		throw "Expression has unpaired brackets";
}

void Arithmetic::termToPostfix()
{
	unsigned size = terms.size();
	vector <Term> postfix;
	Stack <Term> operations;
	for (unsigned i = 0; i < size; i++) {
		if (terms[i].isValue() == true) { // если нам встретилось число
			postfix.push_back(terms[i]); // помещаем число в выходную строку в виде Term
		}
		// если встретилась операция
		else {
			if (terms[i].getOperation() == '(') {
				operations.push(terms[i]);
				continue;
			}
			else if (terms[i].getOperation() == ')') {
				while (operations.getTop().getOperation() != '(') {
					postfix.push_back(operations.pop());
				}
				operations.pop(); // pop '('
				continue;
			}
			
			if (i > 0 && terms[i].getOperation() == '_') {
				operations.push(terms[i]);
			}
			else {
				while (!operations.isEmpty() && operations.getTop().priority() >= terms[i].priority()) {
					postfix.push_back(operations.pop());
				}
				operations.push(terms[i]);
			}
		}
	}
	while (!operations.isEmpty()) {
		postfix.push_back(operations.pop());
	}
	terms = postfix;
}
double Arithmetic::calculate()
{	
	auto termsCopy = terms;
	// сначала нужно ввести значения переменных
	if (!variables.empty()) {
		std::cout << "Enter values of variables:" << std::endl;
		for (auto it = variables.begin(); it != variables.end(); it++) {
			string variableName = *it;
			string variableValue;
			double value;
			bool success = false;

			do {
				std::cout << variableName << " = ";
				getline(std::cin, variableValue);
				try {
					value = std::stod(variableValue);
					success = true;
				}
				catch (...) {
					std::cout << "Incorrect input, enter value again" << std::endl;
				}
			} while (!success);
			for (int i = 0; i < termsCopy.size(); i++) {
				if (termsCopy[i].getVariableName() == variableName)
					termsCopy[i].SetValue(value);
			}
		}
	}

	Stack <Term> stack;
	Term termOperand1, termOperand2;
	double operand1, operand2, res;
	for (unsigned i = 0; i < termsCopy.size(); i++) {
		if (termsCopy[i].isValue() == true) {
			stack.push(termsCopy[i].getValue());
		}
		else if (termsCopy[i].getOperation() == '_') {
			res = (-1)*stack.pop().getValue();
			stack.push(res);
			continue;
		}
		else {
			termOperand2 = stack.pop();
			termOperand1 = stack.pop();
			operand1 = termOperand1.getValue();
			operand2 = termOperand2.getValue();
			if (termsCopy[i].getOperation() == '+')
				res = operand1 + operand2;
			if (termsCopy[i].getOperation() == '-')
				res = operand1 - operand2;
			if (termsCopy[i].getOperation() == '*')
				res = operand1 * operand2;
			if (termsCopy[i].getOperation() == '/') {
				if (abs(operand2) < 1e-9) throw "Division by the zero";
				res = operand1 / operand2;
			}

			stack.push(res);
		}
	}
	Term result = stack.pop();
	return result.getValue();
}
