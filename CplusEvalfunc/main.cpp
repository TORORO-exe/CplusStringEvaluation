#include<iostream>
#include<string>

#include"StringEvaluate.h"

using namespace std;

int main()
{
	string formula;

	cout << "式を入力してください..." << endl;
	cout << "例) sin(PI/6)*(90+20)+acos(1/pow(2,1/2))" << endl;

	do {
		cin >> formula;
		cout <<"ans : "<< std::fixed << StringEvaluate::getInst()->eval(formula) << "\n\n";
		cout << "[q]で続行、それ以外で終了" << endl;
		cin >> formula;

		if (formula[0] == 'Q' || formula[0] == 'q')break;

		cout << "---------" << "続行" << "---------" << endl;

	} while (true);

	cout << "終了します" << endl;
	Sleep(3 * 1000);

	return 0;
}