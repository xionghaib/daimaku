#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class operate//计算器
{
public:
	operate();
	void user_define();	        //用单个符号替代单目运算符
	int PrecedenceOf(char c);   //获取算术符号优先级
	void infixtransform();	     //中缀转后缀
	void getresult();	        //计算结果
	void calculate();	        //计算方法
	double popresult();	        //获取结果

	string operators;         //运算符号
	string infix;		        //表达式缓存

private:
	vector<string> postfix; //后缀表达式向量
	stack<char> symStack;	//操作符栈
	stack<double> sumStack; //操作数栈
	string stdInfix;		//操作符
	double result;			//最终计算结果
};

operate::operate()          //构造函数，初始化成员变量
{

	operators = "！, ^, +, -, *, /, %, >=, <=, ==, &&, !=, >, <, ( )"; //可支持的操作符
	result = 0.0;
}

void operate::user_define()//用单个符号替代单目运算符
{

	stdInfix = infix;

	for (size_t k = 0; k < stdInfix.size(); k++)
	{ // string.size()返回size_type类型，避免下标运算时的类型溢出
		if (stdInfix[k] == '-' || stdInfix[k] == '+')
		{ //-x转换为0-x，+x转化为0+x
			if (k == 0)
			{ //实现正负数
				stdInfix.insert(0, 1, '0');         //在下标为0的元素前插入一个字符型0
			}
			else if (stdInfix[k - 1] == '(')
			{
				stdInfix.insert(k, 1, '0');
			}
		}
		else if (stdInfix[k] == '>' && stdInfix[k + 1] == '=')
		{                                                           //>=替换成~
			stdInfix[k] = '~';
			stdInfix[k + 1] = ' ';
		}
		else if (stdInfix[k] == '<' && stdInfix[k + 1] == '=')
		{                                                           //<=替换成$
			stdInfix[k] = '$';
			stdInfix[k + 1] = ' ';
		}
		else if (stdInfix[k] == '=' && stdInfix[k + 1] == '=')
		{                                                            //==替换成y
			stdInfix[k] = 'y';
			stdInfix[k + 1] = ' ';
		}
		else if (stdInfix[k] == '!' && stdInfix[k + 1] == '=')
		{                                                            //!=替换成x
			stdInfix[k] = 'x';
			stdInfix[k + 1] = ' ';
		}
		else if (stdInfix[k] == '&' && stdInfix[k + 1] == '&')
		{                                                            //&&替换成z
			stdInfix[k] = 'z';
			stdInfix[k + 1] = ' ';
		}
	}
}

int operate::PrecedenceOf(char c)//判断算术符号优先级
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

void operate::infixtransform()//中缀转后缀
{


	string tmp;

	for (size_t i = 0; i < stdInfix.size(); i++)
	{ // string.size()返回size_type类型，避免下标运算时的类型溢出
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
//括号匹配
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
				symStack.pop(); //将左括号出栈丢弃
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
				symStack.pop(); //将左括号出栈丢弃
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
				symStack.pop(); //将左括号出栈丢弃
			}
			break;
		default:
			if ((stdInfix[i] >= '0' && stdInfix[i] <= '9'))
			{
				tmp += stdInfix[i];
				while (i + 1 < stdInfix.length() && (stdInfix[i + 1] >= '0' && stdInfix[i + 1] <= '9' || stdInfix[i + 1] == '.'))
				{ //小数处理

					tmp += stdInfix[i + 1]; //是连续的数字，则追加
					i++;
				}
				if (tmp[tmp.length() - 1] == '.')
				{
					tmp += '0'; //将x.做x.0处理
				}
				postfix.push_back(tmp);
			}
			break;
		}
	}

	while (!symStack.empty())
	{ //将栈中剩余符号加入后缀表达式
		tmp = "";
		tmp += symStack.top();
		postfix.push_back(tmp);
		symStack.pop();
	}
}

void operate::getresult()//计算后缀表达式
{

	string tmp;
	int number = 0;
	int num1 = 0, num2 = 0;

	for (int k = 0; k < postfix.size(); k++)
	{
		tmp = postfix[k];
		if (tmp[0] >= '0' && tmp[0] <= '9')
		{
			number = tmp[0]-'0'; //把字符串转化成整数
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
				cout << "语句不合法，请重新输入！" << endl;
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

void operate::calculate()//计算结果
{

	user_define();  //表达式自定义标准格式化
	infixtransform(); //后缀表达式转换
	getresult();  //计算后缀表达式
}

double operate::popresult()//获取结果
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
		getline(cin, cal.infix);                //读取表达式
		cal.calculate();                        //计算
		cout << cal.popresult() << endl;       //输出结果
	}
	return 0;
}
