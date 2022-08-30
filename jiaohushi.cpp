#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class operate//������
{
public:
	operate();
	void user_define();	        //�õ������������Ŀ�����
	int PrecedenceOf(char c);   //��ȡ�����������ȼ�
	void infixtransform();	     //��׺ת��׺
	void getresult();	        //������
	void calculate();	        //���㷽��
	double popresult();	        //��ȡ���

	string operators;         //�������
	string infix;		        //���ʽ����

private:
	vector<string> postfix; //��׺���ʽ����
	stack<char> symStack;	//������ջ
	stack<double> sumStack; //������ջ
	string stdInfix;		//������
	double result;			//���ռ�����
};

operate::operate()          //���캯������ʼ����Ա����
{

	operators = "��, ^, +, -, *, /, %, >=, <=, ==, &&, !=, >, <, ( )"; //��֧�ֵĲ�����
	result = 0.0;
}

void operate::user_define()//�õ������������Ŀ�����
{

	stdInfix = infix;

	for (size_t k = 0; k < stdInfix.size(); k++)
	{ // string.size()����size_type���ͣ������±�����ʱ���������
		if (stdInfix[k] == '-' || stdInfix[k] == '+')
		{ //-xת��Ϊ0-x��+xת��Ϊ0+x
			if (k == 0)
			{ //ʵ��������
				stdInfix.insert(0, 1, '0');         //���±�Ϊ0��Ԫ��ǰ����һ���ַ���0
			}
			else if (stdInfix[k - 1] == '(')
			{
				stdInfix.insert(k, 1, '0');
			}
		}
		else if (stdInfix[k] == '>' && stdInfix[k + 1] == '=')
		{                                                           //>=�滻��~
			stdInfix[k] = '~';
			stdInfix[k + 1] = ' ';
		}
		else if (stdInfix[k] == '<' && stdInfix[k + 1] == '=')
		{                                                           //<=�滻��$
			stdInfix[k] = '$';
			stdInfix[k + 1] = ' ';
		}
		else if (stdInfix[k] == '=' && stdInfix[k + 1] == '=')
		{                                                            //==�滻��y
			stdInfix[k] = 'y';
			stdInfix[k + 1] = ' ';
		}
		else if (stdInfix[k] == '!' && stdInfix[k + 1] == '=')
		{                                                            //!=�滻��x
			stdInfix[k] = 'x';
			stdInfix[k + 1] = ' ';
		}
		else if (stdInfix[k] == '&' && stdInfix[k + 1] == '&')
		{                                                            //&&�滻��z
			stdInfix[k] = 'z';
			stdInfix[k + 1] = ' ';
		}
	}
}

int operate::PrecedenceOf(char c)//�ж������������ȼ�
{
	if (c == 'z')
	{
		return 0;
	}
	else if (c == '>' || c == '<' || c == '~' || c == '$' || c == 'y' || c == 'x')
	{
		return 1;
	}
	else if (c == '+' || c == '-')
	{
		return 2;
	}
	else if (c == '*' || c == '/' || c == '%')
	{
		return 3;
	}
	else if (c == '^')
	{
		return 4;
	}
	else if (c == '!')
	{
		return 5;
	}
	else
	{
		return 0;
	}
}

void operate::infixtransform()//��׺ת��׺
{


	string tmp;

	for (size_t i = 0; i < stdInfix.size(); i++)
	{ // string.size()����size_type���ͣ������±�����ʱ���������
		tmp = "";
		switch (stdInfix[i])
		{
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '^':
		case '!':
		case '>':
		case '<':
		case '~':
		case '$':
		case 'y':
		case 'x':
		case 'z':
//����ƥ��
			if (symStack.empty() || symStack.top() == '(' || symStack.top() == '[' || symStack.top() == '{' )
			{
				symStack.push(stdInfix[i]);
			}
			else
			{
				while (!symStack.empty() && (PrecedenceOf(symStack.top()) >= PrecedenceOf(stdInfix[i])))
				{
					tmp += symStack.top();
					postfix.push_back(tmp);
					symStack.pop();
					tmp = "";
				}
				symStack.push(stdInfix[i]);
			}
			break;
		case '(':
		case '[':
		case '{':
			symStack.push(stdInfix[i]);
			break;
		case ')':
			while (!symStack.empty() && symStack.top() != '(')
			{
				tmp += symStack.top();
				postfix.push_back(tmp);
				symStack.pop();
				tmp = "";
			}
			if (!symStack.empty() && symStack.top() == '(')
			{
				symStack.pop(); //�������ų�ջ����
			}
			break;
		case ']':
			while (!symStack.empty() && symStack.top() != '[')
			{
				tmp += symStack.top();
				postfix.push_back(tmp);
				symStack.pop();
				tmp = "";
			}
			if (!symStack.empty() && symStack.top() == '[')
			{
				symStack.pop(); //�������ų�ջ����
			}
			break;
		case '}':
			while (!symStack.empty() && symStack.top() != '{')
			{
				tmp += symStack.top();
				postfix.push_back(tmp);
				symStack.pop();
				tmp = "";
			}
			if (!symStack.empty() && symStack.top() == '{')
			{
				symStack.pop(); //�������ų�ջ����
			}
			break;
		default:
			if ((stdInfix[i] >= '0' && stdInfix[i] <= '9'))
			{
				tmp += stdInfix[i];
				while (i + 1 < stdInfix.length() && (stdInfix[i + 1] >= '0' && stdInfix[i + 1] <= '9' || stdInfix[i + 1] == '.'))
				{ //С������

					tmp += stdInfix[i + 1]; //�����������֣���׷��
					i++;
				}
				if (tmp[tmp.length() - 1] == '.')
				{
					tmp += '0'; //��x.��x.0����
				}
				postfix.push_back(tmp);
			}
			break;
		}
	}

	while (!symStack.empty())
	{ //��ջ��ʣ����ż����׺���ʽ
		tmp = "";
		tmp += symStack.top();
		postfix.push_back(tmp);
		symStack.pop();
	}
}

void operate::getresult()//�����׺���ʽ
{

	string tmp;
	int number = 0;
	int num1 = 0, num2 = 0;

	for (int k = 0; k < postfix.size(); k++)
	{
		tmp = postfix[k];
		if (tmp[0] >= '0' && tmp[0] <= '9')
		{
			number = tmp[0]-'0'; //���ַ���ת��������
			sumStack.push(number);
		}
		else if (postfix[k] == "+")
		{
			if (!sumStack.empty())
			{
				num2 = sumStack.top();
				sumStack.pop();
			}
			if (!sumStack.empty())
			{
				num1 = sumStack.top();
				sumStack.pop();
			}
			sumStack.push(num1 + num2);
		}
		else if (postfix[k] == "-")
		{
			if (!sumStack.empty())
			{
				num2 = sumStack.top();
				sumStack.pop();
			}
			else
				num2 = 0;
			if (!sumStack.empty())
			{
				num1 = sumStack.top();
				sumStack.pop();
			}
			else
				num1 = 0;
			sumStack.push(num1 - num2);
		}
		else if (postfix[k] == "*")
		{
			if (!sumStack.empty())
			{
				num2 = sumStack.top();
				sumStack.pop();
			}
			if (!sumStack.empty())
			{
				num1 = sumStack.top();
				sumStack.pop();
			}
			sumStack.push(num1 * num2);
		}
		else if (postfix[k] == "/")
		{
			if (!sumStack.empty())
			{
				num2 = sumStack.top();
				sumStack.pop();
			}
			if (!sumStack.empty())
			{
				num1 = sumStack.top();
				sumStack.pop();
			}
			if (num2 != 0)
			{
				sumStack.push(num1 / num2);
			}
			else
				cout << "��䲻�Ϸ������������룡" << endl;
		}
		else if (postfix[k] == "%")
		{
			if (!sumStack.empty())
			{
				num2 = sumStack.top();
				sumStack.pop();
			}
			if (!sumStack.empty())
			{
				num1 = sumStack.top();
				sumStack.pop();
			}
			sumStack.push(num1 % num2);
		}
		else if (postfix[k] == "^")
		{
			if (!sumStack.empty())
			{
				num2 = sumStack.top();
				sumStack.pop();
			}
			if (!sumStack.empty())
			{
				num1 = sumStack.top();
				sumStack.pop();
			}
			sumStack.push(pow(num1, num2));
		}
		else if (postfix[k] == "!")
		{
			if (!sumStack.empty())
			{
				num1 = sumStack.top();
				sumStack.pop();
			}
			if (num1 > 0)
				sumStack.push(0);
			if (num1 <= 0)
				sumStack.push(1);
		}
		else if (postfix[k] == ">")
		{
			if (!sumStack.empty())
			{
				num2 = sumStack.top();
				sumStack.pop();
			}
			if (!sumStack.empty())
			{
				num1 = sumStack.top();
				sumStack.pop();
			}
			if (num1 > num2)
				sumStack.push(1);
			else
				sumStack.push(0);
		}
		else if (postfix[k] == "<")
		{
			if (!sumStack.empty())
			{
				num1 = sumStack.top();
				sumStack.pop();
			}
			if (!sumStack.empty())
			{
				num1 = sumStack.top();
				sumStack.pop();
			}
			if (num1 < num2)
				sumStack.push(1);
			else
				sumStack.push(0);
		}
		else if (postfix[k] == "~")
		{
			if (!sumStack.empty())
			{
				num2 = sumStack.top();
				sumStack.pop();
			}
			if (!sumStack.empty())
			{
				num1 = sumStack.top();
				sumStack.pop();
			}
			if (num1 >= num2)
				sumStack.push(1);
			else
				sumStack.push(0);
		}
		else if (postfix[k] == "$")
		{
			if (!sumStack.empty())
			{
				num2 = sumStack.top();
				sumStack.pop();
			}
			if (!sumStack.empty())
			{
				num1 = sumStack.top();
				sumStack.pop();
			}
			if (num1 <= num2)
				sumStack.push(1);
			else
				sumStack.push(0);
		}
		else if (postfix[k] == "y")
		{
			if (!sumStack.empty())
			{
				num2 = sumStack.top();
				sumStack.pop();
			}
			if (!sumStack.empty())
			{
				num1 = sumStack.top();
				sumStack.pop();
			}
			if (num1 == num2)
				sumStack.push(1);
			else
				sumStack.push(0);
		}
		else if (postfix[k] == "x")
		{
			if (!sumStack.empty())
			{
				num2 = sumStack.top();
				sumStack.pop();
			}
			if (!sumStack.empty())
			{
				num1 = sumStack.top();
				sumStack.pop();
			}
			if (num1 != num2)
				sumStack.push(1);
			else
				sumStack.push(0);
		}
		else if (postfix[k] == "z")
		{
			if (!sumStack.empty())
			{
				num2 = sumStack.top();
				sumStack.pop();
			}
			if (!sumStack.empty())
			{
				num1 = sumStack.top();
				sumStack.pop();
			}
			sumStack.push(num1 && num2);
		}
	} // end for
	if (!sumStack.empty())
	{
		result = sumStack.top();
	}
}

void operate::calculate()//������
{

	user_define();  //���ʽ�Զ����׼��ʽ��
	infixtransform(); //��׺���ʽת��
	getresult();  //�����׺���ʽ
}

double operate::popresult()//��ȡ���
{
	return result;
}

int main()
{
	operate cal;
	cout << cal.operators << endl;
	cout << "\n\n" << endl;

	while (true)
	{
		getline(cin, cal.infix);                //��ȡ���ʽ
		cal.calculate();                        //����
		cout << cal.popresult() << endl;       //������
	}
	return 0;
}
