#include<iostream>
#include<string>

#include"StringEvaluate.h"

using namespace std;

int main()
{
	string formula;

	cout << "������͂��Ă�������..." << endl;
	cout << "��) sin(PI/6)*(90+20)+acos(1/pow(2,1/2))" << endl;

	do {
		cin >> formula;
		cout <<"ans : "<< std::fixed << StringEvaluate::getInst()->eval(formula) << "\n\n";
		cout << "[q]�ő��s�A����ȊO�ŏI��" << endl;
		cin >> formula;

		if (formula[0] == 'Q' || formula[0] == 'q')break;

		cout << "---------" << "���s" << "---------" << endl;

	} while (true);

	cout << "�I�����܂�" << endl;
	Sleep(3 * 1000);

	return 0;
}