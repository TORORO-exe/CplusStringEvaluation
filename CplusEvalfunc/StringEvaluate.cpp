#include"StringEvaluate.h"

struct A
{
	int v1;
	int v2;
	int v3;
	int v4;
};

struct B
{
	int v1;
	int v2;
	int v3;
	int v4;
};

string StringEvaluate::AconvertString(string str)
{
	A a = { 100,200,300,400 };

	int num = 0;

	if (str == "a.v1")
		num = a.v1;
	else if (str == "a.v2")
		num = a.v2;
	else if (str == "a.v3")
		num = a.v3;
	else if (str == "a.v4")
		num = a.v4;
	else
		std::cout << "error member name (A)" << endl;

	return to_string(num);
}

string StringEvaluate::BconvertString(string str)
{
	B b = { 500,600,700,800 };

	int num = 0;

	if (str == "b.v1")
		num = b.v1;
	else if (str == "b.v2")
		num = b.v2;
	else if (str == "b.v3")
		num = b.v3;
	else if (str == "b.v4")
		num = b.v4;
	else
		std::cout << "error member name (B)" << endl;

	return to_string(num);
}

string StringEvaluate::ConvertStruct_toString(string str)
{
	string convertstr = str + '\0';

	string mname;
	bool detectmemberaccess = false;

	for (auto it = str.begin(); it != str.end(); it++)
	{
		if ((*it) == '.' && !detectmemberaccess && !isdigit((int)(*(it + 1))))//�����o�A�N�Z�X�����m
		{
			mname.push_back(*(it - 1));
			mname.push_back((*it));
			detectmemberaccess = true;
		}
		else if (detectmemberaccess)//
		{
			if (!isOperator((*it)))//���Z�q�łȂ��Ȃ�
			{
				mname.push_back((*it));
				if ((it + 1) == str.end())
				{
					if (mname[0] == 'a')
						convertstr.replace(convertstr.find(mname), mname.size(), AconvertString(mname));
					else if (mname[0] == 'b')
						convertstr.replace(convertstr.find(mname), mname.size(), BconvertString(mname));
				}
			}
			else//�����A�N���X��.�����o�������S�ɂȂ�����
			{

				if (mname[0] == 'a')
					convertstr.replace(convertstr.find(mname), mname.size(), AconvertString(mname));
				else if (mname[0] == 'b')
					convertstr.replace(convertstr.find(mname), mname.size(), BconvertString(mname));

				mname.clear();
				detectmemberaccess = false;
			}
		}
	}
	return convertstr;
}

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
	string str = deletespace(s);//�󔒂͒Z�k���܂�
	str = ConvertStruct_toString(str);//�\���̃����o�𕶎��ɕϊ�
	str = replaceMathConstant(str);
	isValidformula(str);

	string fname;
	string argstr;

	for (int i = 0; i < str.size(); i++)//sin cos tan�Ƃ������֐����v�Z����B
	{
		if (isalpha(str[i]))//�����ł́A�������֐��Ƃ݂Ȃ�
		{
			while (isalpha(str[i]))
			{
				fname.push_back(str[i]);
				i++;
			}

			i++;//�����tan(PI)�Ƃ��́u(�v�̎��̕����ɍs��

			int bcounter = 1;

			while (bcounter != 0)//���ʂ̌��J�E���g�B����if���ɓ����Ă鎞�_�Ŋ��Ɉ��
			{
				if (str[i] == '(')bcounter++;
				else if (str[i] == ')')bcounter--;
				if (bcounter != 0) argstr.push_back(str[i]);
				i++;

				if (bcounter < 0 || str.size() < i)
				{
					std::cout << "error : ���ʂ̐����s���ł�(�֐�eval)" << endl;
					exit(1);
				}
			}
			string funcans = culcFunc(fname, argstr);//�֐����ƈ����𑗂�܂�
			string funcstr = fname + '(' + argstr + ')';

			str.replace(str.find(funcstr), funcstr.size(), funcans);
			
			if(str.find("++") != string::npos)
				str.replace(str.find("++"), 2,"+");
			if(str.find("+-") != string::npos)
				str.replace(str.find("+-"), 2,"-");
			if(str.find("-+") != string::npos)
				str.replace(str.find("-+"), 2,"-");
			if(str.find("--") != string::npos)
				str.replace(str.find("--"), 2,"+");
			
			//�L�������΂Ȃ��悤��...

			i = 0;//��{�A��񏈗��I������瓪�����蒼���܂�

			fname.clear();
			argstr.clear();
		}
	}

	string blacketstr;

	for (int i = 0; i < str.size(); i++)//���ʂ̏���(�֐��Ăяo���ȊO)�����܂�
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
			string replacestr = to_string(eval(blacketstr));
			string findstr= '(' + blacketstr + ')';

			str.replace(str.find(findstr),findstr.size(), replacestr);

			blacketstr.clear();

			i = 0;
		}
	}

	for (int i = 0; i < str.size(); i++)//�|���Z/����Z �L������������if��艺�̍s�����s
	{
		if (str[i] != '*' && str[i] != '/')continue;

		returnstringset ans = calc(str, i);
		string replacestr = ans.replacestr;
		string calcans = ans.ansstr;

		str.replace(str.find(replacestr), replacestr.size(), calcans);
		i = 0;
	}

	for (int i = 0; i < str.size(); i++)//�����Z/�����Z �L������������if��艺�̍s�����s
	{	
		if (str[i] != '+' && str[i] != '-')continue;

		if (str.find_first_of('-') == str.find_last_of('-') && str[0] == '-')break;//�B��o�Ă���}�C�i�X�L�����擪�Ȃ疳��

		returnstringset ans = calc(str, i);
		string replacestr = ans.replacestr;
		string calcans = ans.ansstr;

		str.replace(str.find(replacestr), replacestr.size(), calcans);
		i = 0;
	}

	return stod(str.c_str());
}

returnstringset StringEvaluate::calc(string formula, int openum)
{

	string left;
	string right;

	int i = openum - 1;

	while (isdigit(formula[i]) || formula[i] == '.')//��O�̋L�����o�Ă���܂Ŗ߂�
	{
		i--;
		if (i < 0)
		{
			i = 0;
			break;
		}
	}

	if (i == 0 && formula[0] == '-')//�����A�擪�Ƀ}�C�i�X���Ȃ�A�A�A
	{
		left.push_back('-');
		i++;
	}
	else if (i != 0) {
		i++;
	}

	while (isdigit(formula[i]) || formula[i] == '.')//���ӂ�ǂݍ���
	{
		left.push_back(formula[i]);
		i++;
		if (i < 0)
			break;
	}

	bool includeminus = false;
	i = openum + 1;
	if (formula[i] == '-')includeminus = true;//-1*-1�݂����ȏ������ɂ��Ή����邽�߂̏���

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

	std::cout << "error : ���Z�q���s���ł�" << '[' + formula[openum] + ']' << endl;
	exit(1);
}

string StringEvaluate::culcFunc(string tfunc, string arg)
{
	
	if (strcmp("abs", tfunc.c_str()) == 0) return to_string(fabs(eval(arg)));
	else if (strcmp("fabs", tfunc.c_str()) == 0) return to_string(fabs(eval(arg)));
	else if (strcmp("sqrt", tfunc.c_str()) == 0) return to_string(sqrt(eval(arg)));
	else if (strcmp("sin", tfunc.c_str()) == 0) return to_string(sin(eval(arg)));
	else if (strcmp("cos", tfunc.c_str()) == 0) return to_string(cos(eval(arg)));
	else if (strcmp("tan", tfunc.c_str()) == 0) return to_string(tan(eval(arg)));
	else if (strcmp("asin", tfunc.c_str()) == 0) return to_string(asin(eval(arg)));
	else if (strcmp("acos", tfunc.c_str()) == 0) return to_string(acos(eval(arg)));
	else if (strcmp("atan", tfunc.c_str()) == 0) return to_string(atan(eval(arg)));
	else if (strcmp("sinh", tfunc.c_str()) == 0) return to_string(sinh(eval(arg)));
	else if (strcmp("cosh", tfunc.c_str()) == 0) return to_string(cosh(eval(arg)));
	else if (strcmp("tanh", tfunc.c_str()) == 0) return to_string(tanh(eval(arg)));
	else if (strcmp("asinh", tfunc.c_str()) == 0) return to_string(asinh(eval(arg)));
	else if (strcmp("acosh", tfunc.c_str()) == 0) return to_string(acosh(eval(arg)));
	else if (strcmp("atanh", tfunc.c_str()) == 0) return to_string(atanh(eval(arg)));
	else if (strcmp("exp", tfunc.c_str()) == 0) return to_string(exp(eval(arg)));
	else if (strcmp("log", tfunc.c_str()) == 0) return to_string(log(eval(arg)));
	else if (strcmp("log10", tfunc.c_str()) == 0) return to_string(log10(eval(arg)));
	else if (strcmp("ceil", tfunc.c_str()) == 0) return to_string(ceil(eval(arg)));
	else if (strcmp("floor", tfunc.c_str()) == 0) return to_string(floor(eval(arg)));
	else if (strcmp("round", tfunc.c_str()) == 0) return to_string(round(eval(arg)));

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
	
	std::cout << "error : �֐������s���ł�" << '[' + tfunc + ']' << endl;
	exit(1);

	return "ERROR";
}

bool StringEvaluate::isOperator(const char c)
{
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') ? true : false;
}

void StringEvaluate::isValidformula(string formula)
{
	for (int i = 0; i < formula.size(); i++)
	{
		char c = formula[i];
		bool f = (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == ','|| c == '.' || c == '\0' || isalpha(c) || isdigit(c)) ? true : false;

		if (!f)
		{
			cout << "error : �\�����Ă��Ȃ��L�����g���Ă��܂�" << "[ " << c << " ]" << endl;
			exit(1);
		}
	}
}

string StringEvaluate::replaceMathConstant(string formula)
{
	while (formula.find("PI") != string::npos)
	{
		formula.replace(formula.find("PI"), 2, "3.1415926535");
	}
	return formula;
}