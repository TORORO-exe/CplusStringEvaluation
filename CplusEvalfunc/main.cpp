#include<iostream>
#include<string>

#include"StringEvaluate.h"

using namespace std;

int main()
{
	string formula;
	
	StringEvaluate streval;

	cout << "������͂��Ă�������..." << endl;
	cout << "��) sin(PI/6)*(90+20)+acos(1/pow(2,1/2))" << endl;

	do {
		cin >> formula;
		cout << std::fixed << streval.eval(formula) << endl;

		cout << "q�ŏI��,����ȊO�ő��s" << endl;
		cin >> formula;

		if (formula[0] == 'q' || formula[0] == 'Q')
			break;

		cout << "--------------" << "���s" << "--------------" << endl;
	} while (true);

	return 0;
}