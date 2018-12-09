#include "Sort.h"

int main(void) {
	vector<unsigned> num1, num2;
	vector<unsigned> res(INT_RANDN, 0);

	cout << "Sort\n" << endl;
	cout << "[Original]" << endl;
	num1 = RandGen(INT_RANDN);
	print(num1);
	qsortMain(num1, 0, num1.size() - 1);
	cout << "\n\n[Quick Sort]" << endl;
	print(num1);
	Sleep(2000);
	cout << "\n\n[Original]" << endl;
	num2 = RandGen(INT_RANDN);
	print(num2);
	msortMain(num2, 0, num2.size() - 1, res);
	cout << "\n\n[Merge Sort]" << endl;
	print(num2);
	system("pause > nul");
	return 0;
}