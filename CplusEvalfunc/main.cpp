#include<iostream>
#include<string>

#include"StringEvaluate.h"

using namespace std;

int main()
{
	string formula;
	
	StringEvaluate streval;

	cout << "式を入力してください..." << endl;
	cout << "例) sin(PI/6)*(90+20)+acos(1/pow(2,1/2))" << endl;

	do {
		cin >> formula;
		cout << std::fixed << streval.eval(formula) << endl;

		cout << "qで終了,それ以外で続行" << endl;
		cin >> formula;

		if (formula[0] == 'q' || formula[0] == 'Q')
			break;

		cout << "--------------" << "続行" << "--------------" << endl;
	} while (true);

	return 0;
}