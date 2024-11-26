#pragma once

#include<string>
#include<math.h>
#include<iostream>
#include<windows.h>
#include"Singleton.h"

using namespace std;

struct returnstringset
{
	string replacestr;
	string ansstr;
};

class StringEvaluate final :public Singleton<StringEvaluate>
{
public:
	double eval(string str);

private:

	string culcFunc(string tfunc,string arg);
	returnstringset calc(string formula,int openum);
	string deletespace(string str);

	bool isOperator(const char c);
	void isValidformula(string formula);
	string replaceMathConstant(string formula);
};
