#include"StringEvaluate.h"

string StringEvaluate::deletespace(string str)
{
	while (str.find(" ") != string::npos)
	{
		str.replace(str.find(" "), 1, "");
	}
	return str;
}

double StringEvaluate::eval(string s)
{
	string str = deletespace(s);//空白は短縮します
	str = replaceMathConstant(str);//PI,Eとかの置き換えをする
	isValidformula(str);//変な記号とか入ってないか確認,空文字列も判定

	string fname;//関数名
	string argstr;//関数の引数文字列

	for (int i = 0; i < str.size(); i++)//sin cos tanといった関数を計算する。
	{
		if (isalpha(str[i]))//ここでは、文字＝関数とみなす
		{
			while (isalpha(str[i]))
			{
				fname.push_back(str[i]);
				i++;
			}

			i++;//これでtan(PI)とかの「(」の次の部分に行く

			int bcounter = 1;

			while (bcounter != 0)//括弧の個数カウント。このif文に入ってる時点で既に一個
			{
				if (str[i] == '(')bcounter++;
				else if (str[i] == ')')bcounter--;
				if (bcounter != 0) argstr.push_back(str[i]);
				i++;

				if (bcounter < 0 || str.size() < i)
				{
					std::cout << "error : 括弧の数が不正です(関数eval)" << endl;
					Sleep(4 * 1000);
					exit(1);
				}
			}
			string rstr = culcFunc(fname, argstr);//置き換え先の文字列
			string bstr = fname + '(' + argstr + ')';//置き換える文字列

			str.replace(str.find(bstr), bstr.size(), rstr);
			
			if(str.find("++") != string::npos)
				str.replace(str.find("++"), 2,"+");
			if(str.find("+-") != string::npos)
				str.replace(str.find("+-"), 2,"-");
			if(str.find("-+") != string::npos)
				str.replace(str.find("-+"), 2,"-");
			if(str.find("--") != string::npos)
				str.replace(str.find("--"), 2,"+");
			
			//記号が並ばないように...

			i = -1;//基本、一回処理終わったら頭からやり直します

			fname.clear();
			argstr.clear();
		}
	}

	string blacketstr;

	for (int i = 0; i < str.size(); i++)//括弧の処理(関数呼び出し以外)をします
	{
		if (str[i] == '(')
		{
			int bcounter = 1;

			i++;

			while (bcounter != 0)
			{
				if (str[i] == '(')bcounter++;
				else if (str[i] == ')')bcounter--;
				if (bcounter != 0) blacketstr.push_back(str[i]);
				i++;
			}
			string rstr = to_string(eval(blacketstr));//置き換え先の文字列
			string bstr= '(' + blacketstr + ')';//置き換える文字列

			str.replace(str.find(bstr),bstr.size(), rstr);

			blacketstr.clear();

			i = -1;
		}
	}

	for (int i = 0; i < str.size(); i++)//掛け算/割り算 記号を見つけたらifより下の行も実行
	{
		if (str[i] != '*' && str[i] != '/')continue;

		returnstringset ans = calc(str, i);
		string replacestr = ans.replacestr;//置き換える文字列
		string calcans = ans.ansstr;//置き換え先の文字列

		str.replace(str.find(replacestr), replacestr.size(), calcans);
		i = -1;
	}

	for (int i = 0; i < str.size(); i++)//足し算/引き算 記号を見つけたらifより下の行も実行
	{	
		if (str[i] != '+' && str[i] != '-')continue;

		if (str.find_first_of('-') == str.find_last_of('-') && str[0] == '-'&& str.find('+')==string::npos)break;//唯一出てくるマイナス記号が先頭なら無視
		//マイナス記号が一つだけ、かつ、先頭についてる、かつ、プラス記号が見つからないならbreak
		if (str[0] == '-')
		{
			do
			{
				i++;
			} while (!isOperator(str[i]));
		}

		returnstringset ans = calc(str, i);
		string replacestr = ans.replacestr;//置き換える文字列
		string calcans = ans.ansstr;//置き換え先の文字列

		str.replace(str.find(replacestr), replacestr.size(), calcans);
		i = -1;
	}

	return stod(str.c_str());
}

returnstringset StringEvaluate::calc(string formula, int openum)
{
	string left;//左辺
	string right;//右辺

	int i = openum - 1;

	while (isdigit(formula[i]) || formula[i] == '.')//一個前の記号が出てくるまで戻す
	{
		i--;
		if (i < 0)
		{
			i = 0;
			break;
		}
	}

	if (i == 0 && formula[0] == '-')//もし、先頭にマイナスつくなら、、、
	{
		left.push_back('-');
		i++;
	}
	else if (i != 0) {
		i++;
	}

	while (isdigit(formula[i]) || formula[i] == '.')//左辺を読み込む
	{
		left.push_back(formula[i]);
		i++;
		if (i < 0)
			break;
	}

	bool includeminus = false;
	i = openum + 1;
	if (formula[i] == '-')includeminus = true;//-1*-1みたいな書き方にも対応するための処理

	while (isdigit(formula[i]) || includeminus || formula[i] == '.')
	{
		includeminus = false;
		right.push_back(formula[i]);
		i++;
		if (formula.size() < i)
		{
			break;
		}
	}

	returnstringset retstrset;

	retstrset.replacestr = (left + formula[openum] + right);

	if (formula[openum] == '*') retstrset.ansstr=to_string(atof(left.c_str()) * atof(right.c_str()));
	else if (formula[openum] == '/')retstrset.ansstr = to_string(atof(left.c_str()) / atof(right.c_str()));
	else if (formula[openum] == '-')retstrset.ansstr = to_string(atof(left.c_str()) - atof(right.c_str()));
	else if (formula[openum] == '+')retstrset.ansstr = to_string(atof(left.c_str()) + atof(right.c_str()));
	else retstrset.ansstr = ("");

	if (retstrset.ansstr != "")
		return retstrset;

	std::cout << "error : 演算子が不正です" << '[' + formula[openum] + ']' << endl;
	Sleep(4 * 1000);
	exit(1);
}

string StringEvaluate::culcFunc(string tfunc, string arg)
{
	//引数一個の場合はこの辺のif文でOK
	if (strcmp("abs", tfunc.c_str()) == 0) return to_string(fabs(eval(arg)));
	else if (strcmp("fabs", tfunc.c_str()) == 0) return to_string(fabs(eval(arg)));
	else if (strcmp("sqrt", tfunc.c_str()) == 0) return to_string(sqrt(eval(arg)));//
	else if (strcmp("sin", tfunc.c_str()) == 0) return to_string(sin(eval(arg)));
	else if (strcmp("cos", tfunc.c_str()) == 0) return to_string(cos(eval(arg)));
	else if (strcmp("tan", tfunc.c_str()) == 0) return to_string(tan(eval(arg)));
	else if (strcmp("asin", tfunc.c_str()) == 0) return to_string(asin(eval(arg)));//
	else if (strcmp("acos", tfunc.c_str()) == 0) return to_string(acos(eval(arg)));//
	else if (strcmp("atan", tfunc.c_str()) == 0) return to_string(atan(eval(arg)));
	else if (strcmp("sinh", tfunc.c_str()) == 0) return to_string(sinh(eval(arg)));
	else if (strcmp("cosh", tfunc.c_str()) == 0) return to_string(cosh(eval(arg)));
	else if (strcmp("tanh", tfunc.c_str()) == 0) return to_string(tanh(eval(arg)));
	else if (strcmp("asinh", tfunc.c_str()) == 0) return to_string(asinh(eval(arg)));
	else if (strcmp("acosh", tfunc.c_str()) == 0) return to_string(acosh(eval(arg)));//
	else if (strcmp("atanh", tfunc.c_str()) == 0) return to_string(atanh(eval(arg)));//
	else if (strcmp("exp", tfunc.c_str()) == 0) return to_string(exp(eval(arg)));//
	else if (strcmp("log", tfunc.c_str()) == 0) return to_string(log(eval(arg)));//
	else if (strcmp("log10", tfunc.c_str()) == 0) return to_string(log10(eval(arg)));
	else if (strcmp("ceil", tfunc.c_str()) == 0) return to_string(ceil(eval(arg)));
	else if (strcmp("floor", tfunc.c_str()) == 0) return to_string(floor(eval(arg)));
	else if (strcmp("round", tfunc.c_str()) == 0) return to_string(round(eval(arg)));

	//そうでないなら,,カンマを見つけて引数わけます

	string arg1, arg2;

	int i = 0;

	for (; i < arg.size(); i++)
	{
		if (arg[i] == ',')
		{
			i++;
			break;
		}
		arg1.push_back(arg[i]);
	}
	for (; i < arg.size(); i++)
	{
		arg2.push_back(arg[i]);
	}

	if (strcmp("pow", tfunc.c_str()) == 0) return to_string(pow(eval(arg1), eval(arg2)));
	else if (strcmp("atan2", tfunc.c_str()) == 0) return to_string(atan2(eval(arg1), eval(arg2)));
	else if (strcmp("harg2pot", tfunc.c_str()) == 0) return to_string(hypot(eval(arg1), eval(arg2)));
	else if (strcmp("mod", tfunc.c_str()) == 0) return to_string(fmod(eval(arg1), eval(arg2)));
	else if (strcmp("fmod", tfunc.c_str()) == 0) return to_string(fmod(eval(arg1), eval(arg2)));
	
	std::cout << "error : 非対応の関数です" << '[' + tfunc + ']' << endl;
	Sleep(4 * 1000);
	exit(1);
}

bool StringEvaluate::isOperator(const char c)
{
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '\0') ? true : false;
}

void StringEvaluate::isValidformula(string formula)
{
	string errmsg = "";

	if (formula == "")
	{
		errmsg = "error : 文字列が空です";
	}

	formula.push_back('\0');

	if (formula.find(")(") != string::npos)//(10+20)(90-10)括弧の間は必ず計算記号入れてほしいので
	{
		errmsg = "error : 記号の入力漏れがあります\n(10+20)(30+40)のような書き方はできません";
	}

	for (int i = 0; i < formula.size(); i++)//(10+20)90掛け算記号などの省略はできません
	{
		if (formula[i] == ')' && isdigit(formula[i + 1]))
		{
			errmsg = "error : 記号の入力漏れがあります\n(10+20)30のような書き方はできません";
		}
		if (formula[i] == '(' && i != 0)
		{
			if (isdigit(formula[i - 1]) && formula.find("log10") == string::npos)
			{
				errmsg = "error : 記号の入力漏れがあります\n20(10+20)のような書き方はできません";
			}
		}
		if (isalpha(formula[i]) && i != 0)
		{
			if (isdigit(formula[i - 1]))
			{
				errmsg = "error : 記号の入力漏れがあります\n30sin(PI)のような書き方はできません";
			}
		}
	}

	for (int i = 0; i < formula.size(); i++)
	{
		char c = formula[i];
		bool f = (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == ','|| c == '.' || c == '\0' || isalpha(c) || isdigit(c)) ? true : false;

		if (!f)
		{
			errmsg = "error : 予期していない記号が使われています";
			string tmp = "[ x ]";
			tmp[2] = c;
			errmsg += tmp;
		}
	}

	if (errmsg != "")
	{
		cout << errmsg << endl;
		Sleep(4 * 1000);
		exit(1);
	}
}

string StringEvaluate::replaceMathConstant(string formula)
{
	while (formula.find("PI") != string::npos)
	{
		formula.replace(formula.find("PI"), 2, "3.1415926535");
	}
	while (formula.find("E") != string::npos)
	{
		formula.replace(formula.find("E"), 1, "2.71828182845");
	}
	return formula;
}