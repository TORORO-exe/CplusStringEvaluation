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
	string str = deletespace(s);//�󔒂͒Z�k���܂�
	str = replaceMathConstant(str);//PI,E�Ƃ��̒u������������
	isValidformula(str);//�ςȋL���Ƃ������ĂȂ����m�F,�󕶎��������

	string fname;//�֐���
	string argstr;//�֐��̈���������

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
					Sleep(4 * 1000);
					exit(1);
				}
			}
			string rstr = culcFunc(fname, argstr);//�u��������̕�����
			string bstr = fname + '(' + argstr + ')';//�u�������镶����

			str.replace(str.find(bstr), bstr.size(), rstr);
			
			if(str.find("++") != string::npos)
				str.replace(str.find("++"), 2,"+");
			if(str.find("+-") != string::npos)
				str.replace(str.find("+-"), 2,"-");
			if(str.find("-+") != string::npos)
				str.replace(str.find("-+"), 2,"-");
			if(str.find("--") != string::npos)
				str.replace(str.find("--"), 2,"+");
			
			//�L�������΂Ȃ��悤��...

			i = -1;//��{�A��񏈗��I������瓪�����蒼���܂�

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
			string rstr = to_string(eval(blacketstr));//�u��������̕�����
			string bstr= '(' + blacketstr + ')';//�u�������镶����

			str.replace(str.find(bstr),bstr.size(), rstr);

			blacketstr.clear();

			i = -1;
		}
	}

	for (int i = 0; i < str.size(); i++)//�|���Z/����Z �L������������if��艺�̍s�����s
	{
		if (str[i] != '*' && str[i] != '/')continue;

		returnstringset ans = calc(str, i);
		string replacestr = ans.replacestr;//�u�������镶����
		string calcans = ans.ansstr;//�u��������̕�����

		str.replace(str.find(replacestr), replacestr.size(), calcans);
		i = -1;
	}

	for (int i = 0; i < str.size(); i++)//�����Z/�����Z �L������������if��艺�̍s�����s
	{	
		if (str[i] != '+' && str[i] != '-')continue;

		if (str.find_first_of('-') == str.find_last_of('-') && str[0] == '-'&& str.find('+')==string::npos)break;//�B��o�Ă���}�C�i�X�L�����擪�Ȃ疳��
		//�}�C�i�X�L����������A���A�擪�ɂ��Ă�A���A�v���X�L����������Ȃ��Ȃ�break
		if (str[0] == '-')
		{
			do
			{
				i++;
			} while (!isOperator(str[i]));
		}

		returnstringset ans = calc(str, i);
		string replacestr = ans.replacestr;//�u�������镶����
		string calcans = ans.ansstr;//�u��������̕�����

		str.replace(str.find(replacestr), replacestr.size(), calcans);
		i = -1;
	}

	return stod(str.c_str());
}

returnstringset StringEvaluate::calc(string formula, int openum)
{
	string left;//����
	string right;//�E��

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
	Sleep(4 * 1000);
	exit(1);
}

string StringEvaluate::culcFunc(string tfunc, string arg)
{
	//������̏ꍇ�͂��̕ӂ�if����OK
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

	//�����łȂ��Ȃ�,,�J���}�������Ĉ����킯�܂�

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
	
	std::cout << "error : ��Ή��̊֐��ł�" << '[' + tfunc + ']' << endl;
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
		errmsg = "error : �����񂪋�ł�";
	}

	formula.push_back('\0');

	if (formula.find(")(") != string::npos)//(10+20)(90-10)���ʂ̊Ԃ͕K���v�Z�L������Ăق����̂�
	{
		errmsg = "error : �L���̓��͘R�ꂪ����܂�\n(10+20)(30+40)�̂悤�ȏ������͂ł��܂���";
	}

	for (int i = 0; i < formula.size(); i++)//(10+20)90�|���Z�L���Ȃǂ̏ȗ��͂ł��܂���
	{
		if (formula[i] == ')' && isdigit(formula[i + 1]))
		{
			errmsg = "error : �L���̓��͘R�ꂪ����܂�\n(10+20)30�̂悤�ȏ������͂ł��܂���";
		}
		if (formula[i] == '(' && i != 0)
		{
			if (isdigit(formula[i - 1]) && formula.find("log10") == string::npos)
			{
				errmsg = "error : �L���̓��͘R�ꂪ����܂�\n20(10+20)�̂悤�ȏ������͂ł��܂���";
			}
		}
		if (isalpha(formula[i]) && i != 0)
		{
			if (isdigit(formula[i - 1]))
			{
				errmsg = "error : �L���̓��͘R�ꂪ����܂�\n30sin(PI)�̂悤�ȏ������͂ł��܂���";
			}
		}
	}

	for (int i = 0; i < formula.size(); i++)
	{
		char c = formula[i];
		bool f = (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == ','|| c == '.' || c == '\0' || isalpha(c) || isdigit(c)) ? true : false;

		if (!f)
		{
			errmsg = "error : �\�����Ă��Ȃ��L�����g���Ă��܂�";
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