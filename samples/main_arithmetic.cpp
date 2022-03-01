// реализация пользовательского приложения
#include <arithmetic.h>

using namespace std;

int main()
{
	string s;
	cout << "Calculator" << endl;
	for (int i = 0; i < 100; i++) {
		cout << "Enter the expression: ";
		try {
			getline(cin, s);
			Arithmetic res(s);
			bool same = true;
			do {
				bool ok = false;
				double result = res.calculate();
				cout << "Result : " << result << endl;
				if (res.hasVariables() == false)
					same = false;
				else {
					do {
						string choose;
						cout << "Continue with this expression? (Yes/No): ";
						getline(cin,choose);
						if (choose == "Yes") {
							same = true;
							ok = true;
						}
						else if (choose == "No") {
							same = false;
							ok = true;
						}
						else {
							cout << "Incorrect answer, enter again" << endl;
						}
					} while (!ok);
				}
			} while (same);
		}
		catch (const char* smth) {
			cout << smth << endl;
		}
		cout << endl;
	}
	return 0;
}
