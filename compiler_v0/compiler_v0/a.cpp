// compiler_v0.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "string"
#include "fstream"
#include"cstring"

#define stacksize 500
#define uMax 30000
class cCounter
{
	unsigned int cnt;
public:
	cCounter(void) { cnt = 0; }
	cCounter operator ++()
	{
		if (cnt < uMax)
			cnt++;
		return(*this);
	}
	cCounter operator ++(int dummy)
	{
		cCounter  x = *this;
		if (cnt < uMax)
			cnt++;
		return (x);
	}
	cCounter operator--()
	{//注：返回值可说明为引用&
		if (cnt > 0)
			cnt--;
		return (*this);
	}
	cCounter operator --(int dummy)
	{ //注：返回值不可说明为引用&
		cCounter x = *this;
		if (cnt > 0)
			cnt--;
		return (x);
	}
	int operator !()
	{
		return (cnt == 0);
	}
	operator unsigned int()
	{
		return(cnt);
	}
};
ifstream keywordfile;
cCounter counter;
extern int getsym(char & the_single_char_readed_now, int &);
char fname[20];
string keyword = "";
fstream sym_tab;
fstream osym_tab;
fstream iint_tab;
fstream oint_tab;
ifstream sourcefile;
int space_used;
int iaddress;
int insert(string type, string var_name);
int lookint(int integer);
int lookup(string id);
string NEWT(void);
string _add(string y, string z, string t);
string _divided(string y, string z, string t);
string _mult(string y, string z, string t);
string _sub(string y, string z, string t);
int S(char&ch, int &valueofsymbol, string &returned, int &ireturned);
int L(char&ch, int&valueofsymbol, string &returned, int &ireturned);
int LL(char&ch, int&valueofsymbol, string &returned, int &ireturned);
int B(char&ch, int&valueofsymbol, string &returned, int &ireturned);
int T(char &ch, int &valueofsymbol, string &returned, int &ireturned);
int E(char&ch, int&valueofsymbol, string &returned, int &ireturned);
int F(char& ch, int &valueofsymbol, string &returned, int &ireturned);
int TT(char &ch, int&valueofsymbol, string &returned, int &ireturned);
int FF(char &ch, int&valueofsymbol, string &returned, int &ireturned);
int F(char& ch, int &valueofsymbol, string &returned, int &ireturned)
{
	if (valueofsymbol == 40)// (
	{
		getsym(ch, valueofsymbol);
		if (!E(ch, valueofsymbol, returned, ireturned))return 0;
		if (valueofsymbol != 41)
		{
			cout << "Error17: equation的右边期待出现）";
			return 0;
		}
		getsym(ch, valueofsymbol);
		return 1;
	}
	if (valueofsymbol == 50)//NUM
	{
		getsym(ch, valueofsymbol);
		return 1;
	}
	if (valueofsymbol == 49)//ID
	{
		getsym(ch, valueofsymbol);
		return 1;
	}
	cout << "Error16: 此处期待出现（ NUM ID ";
	return 0;
}
int LL(char&ch, int&valueofsymbol, string &returned, int &ireturned)
{
	if (valueofsymbol == 59)
	{
		getsym(ch, valueofsymbol);
		if (!L(ch, valueofsymbol, returned, iret)) return 0;
		return 1;
	}
	if (valueofsymbol != 125)
	{
		cout << "Error14: 复合语句缺'}'";
		return 0;
	}
	//此处不需要超前读入｝后面的symbol因为｝并未读入，只是超前观察
	return 1;
}
int D(char &ch, int &valueofsymbol, string &returned, int &ireturned)
{
	if (valueofsymbol == 65 || valueofsymbol == 66 || valueofsymbol == 67 || valueofsymbol == 69 || valueofsymbol == 72 || valueofsymbol == 78 || valueofsymbol == 82)//int char float double long short signed
	{
		insert(keyword, NEWT());//插入到符号表
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 49)//ID
		{
			cout << "Error5: 声明语句缺少变量名";//返回0 代表程序出错
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 59)//；
		{
			cout << "Error5: 声明语句缺少‘；’";//返回0 代表程序出错
			return 0;
		}
		getsym(ch, valueofsymbol);
		while (valueofsymbol == 65 || valueofsymbol == 66 || valueofsymbol == 67 || valueofsymbol == 69 || valueofsymbol == 72 || valueofsymbol == 78 || valueofsymbol == 82)
		{
			insert(keyword, NEWT());//插入到符号表
			getsym(ch, valueofsymbol);
			if (valueofsymbol != 49)
			{
				cout << "Error6:声明语句缺少变量名";
				return 0;
			}
			getsym(ch, valueofsymbol);
			if (valueofsymbol != 59)//；
			{
				cout << "Error5: 声明语句缺少‘；’";//返回0 代表程序出错
				return 0;
			}
			getsym(ch, valueofsymbol);
		}
		cout << "声明语句编译正确！" << endl;
		return 1;
	}
	else
	{
		cout << "Error4: 声明语句未以int char float double long short signed开始";
		return 0;
	}
}
int T(char &ch, int &valueofsymbol, string &returned, int &ireturned)
{
	if (!F(ch, valueofsymbol))return 0;
	while (valueofsymbol == 42 || valueofsymbol == 47)//* /
	{//if valueo == 42 是乘。。。
		getsym(ch, valueofsymbol);
		if (!F(ch, valueofsymbol))return 0;
	}//是否考虑既不是42 又不是47但是又不是follow的问题呢
	return 1;
}
int E(char&ch, int&valueofsymbol, string &returned, int &ireturned)
{
	if (!T(ch, valueofsymbol))return 0;
	while (valueofsymbol == 43 || valueofsymbol == 45)//+ -
	{
		getsym(ch, valueofsymbol);
		if (!T(ch, valueofsymbol))return 0;
	}
	return 1;//如果没有+或者-也是可以的
}
int L(char&ch, int&valueofsymbol, string &returned, int &ireturned)
{
	if (!S(ch, valueofsymbol))return 0;
	if (!LL(ch, valueofsymbol))return 0;
	return 1;
}
int FF(char &ch, int&valueofsymbol, string &returned, int &ireturned)
{
	if (valueofsymbol != 49)//ID
	{
		cout << "Error17: 逻辑运算运算符前缺少变量";
		return 0;
	}
	getsym(ch, valueofsymbol);
	if (valueofsymbol == 23 || valueofsymbol == 24 || valueofsymbol == 25 || valueofsymbol == 26)//待实现判等号 ，判不等号
	{
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 49)//ID
		{
			cout << "Error18: 逻辑运算中运算符后缺少变量";
			return 0;
		}
		getsym(ch, valueofsymbol);
		return 1;
	}
}
int TT(char &ch, int &valueofsymbol, string &returned, int &ireturned)
{
	if (!FF(ch, valueofsymbol)) return 0;
	while (valueofsymbol == 17)//and
	{
		getsym(ch, valueofsymbol);
		if (!FF(ch, valueofsymbol))return 0;
	}
	return 1;
}
int B(char&ch, int&valueofsymbol, string &returned, int &ireturned)
{
	if (!TT(ch, valueofsymbol)) return 0;
	while (valueofsymbol == 18)//or
	{
		getsym(ch, valueofsymbol);
		if (!TT(ch, valueofsymbol))return 0;
	}
	return 1;
}
int S(char&ch, int &valueofsymbol, string &returned, int &ireturned)
{
	if (valueofsymbol == 49)
	{
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 61)
		{
			cout << "Error7: 赋值语句缺少‘=’";
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (!E(ch, valueofsymbol)) return 0;
		counter++;
		cout << "语句" << counter << "编译正确" << endl;//编写一个计数器，填入x中
		return 1;
	}
	if (valueofsymbol == 123)//{
	{
		getsym(ch, valueofsymbol);
		if (!L(ch, valueofsymbol))return 0;
		if (valueofsymbol != 125)
		{
			cout << "Error8: 复合语句缺少｝" << endl;
			return 0;
		}
		getsym(ch, valueofsymbol);
		counter++;
		string x = "语句";
		string y = "编译正确";
		cout << x + char(counter + 0x30) + y << endl;
		return 1;
	}
	if (valueofsymbol == 88)//while
	{
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 40)//(
		{
			cout << "Error9: while后面缺少‘（’";
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (!B(ch, valueofsymbol))return 0;
		if (valueofsymbol != 41)//)
		{
			cout << "Error10: while 语句判断条件后面缺少‘）’" << endl;
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (valueofsymbol == 87)//do
		{
			getsym(ch, valueofsymbol);
			if (!S(ch, valueofsymbol)) return 0;
			cout << "while语句编译成功" << endl;
			return 1;
		}
		else
		{
			cout << "Error11： while语句缺少do" << endl;
			return 0;
		}
	}
	if (valueofsymbol == 68)
	{
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 40)//(
		{
			cout << "Error19: if后面缺少‘（’";
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (!B(ch, valueofsymbol))return 0;
		if (valueofsymbol != 41)//)
		{
			cout << "Error20: if 语句判断条件后面缺少‘）’" << endl;
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (valueofsymbol == 19)//then
		{
			getsym(ch, valueofsymbol);
			if (!S(ch, valueofsymbol)) return 0;
			if (valueofsymbol == 71)//else
			{
				getsym(ch, valueofsymbol);
				if (!S(ch, valueofsymbol)) return 0;
			}
			cout << "if语句编译成功" << endl;
			return 1;
		}
	}
	{//如果上面的都没有成立
		cout << "Error12: 声明语句后面期望有｛,ID,while,if中的一种" << endl;
		return 0;
	}
}
int P(char &ch, int &valueofsymbol, string &returned, int &ireturned)//开始使用超前技术 超前读入一个symbol
{
	getsym(ch, valueofsymbol);
	if (valueofsymbol == 123)//{
	{
		getsym(ch, valueofsymbol);
		if (!D(ch, valueofsymbol)) return 0;///////ready to be changed
		if (!S(ch, valueofsymbol)) return 0;

		if (valueofsymbol != 125)//}
		{
			cout << "Error3: 程序结束缺少｝";
			return 0;
		}
		getsym(ch, valueofsymbol);
		//#读入
		return 1;
	}
	else
	{
		cout << "Error2: 程序开始缺少｛";
		return 0;
	}
}
string NEWT(void)
{
	static cCounter name_count;
	static const string tname = "t";
	string temporary_var;
	temporary_var = tname + char((name_count++) + 0x30);
	return temporary_var;
}
string _add(string y, string z, string t)
{
	return "(+," + y + "," + z + "," + t + ")";
}
string _mult(string y, string z, string t)
{
	return "(*," + y + "," + z + "," + t + ")";
}
string _sub(string y, string z, string t)
{
	return "(-," + y + "," + z + "," + t + ")";
}
string _divided(string y, string z, string t)
{
	return "(/," + y + "," + z + "," + t + ")";
}
int lookup(string id)//wait
{
	string str1;
	string str2;
	int str3;
	sym_tab.open("symbol_table.txt");
	while (!sym_tab.eof())
	{
		sym_tab >> str1;
		sym_tab >> str2;
		sym_tab >> str3;
		if (id == str2)
		{
			sym_tab.close();
			return str3;
		}
	}
	cout << "Error_x1: 语义分析出错，存在变量未定义而使用";
	sym_tab.close();
	return -1;//准备报错弹出到底，不过第一层是==-1
}
int insert(string type, string var_name)//wait
{
	osym_tab.seekp(0, ios::end);
	osym_tab << endl;//文件操作复杂，选择将space_used 变成全局变量，每次insert时加2！！！
	osym_tab << type << " " << var_name << " " << space_used;
	osym_tab.flush();//可能这里会出问题，不确定flush函数使用后的效果，根据网络上说，可以刷新缓冲区，如果不行，可以关闭后再打开一次文件
	space_used += 2;
	return 1;
}
int lookint(int integer)/////////////////////////////////////下午写，不想写了pos 好像返回是2 ，如果错了则loopup也有可能错了。
{
	int inttab;
	int postab;
	iint_tab.open("整数常量表.txt");
	oint_tab.seekp(0, ios::end);
	while (!iint_tab.eof())
	{
		iint_tab >> inttab;
		iint_tab >> postab;
		if (integer == inttab)
		{
			iint_tab.close();
			return postab;
		}
	}
	oint_tab << endl;
	oint_tab << integer << " " << iaddress;
	oint_tab.flush();
	iaddress += 2;
	iint_tab.close();
	return iaddress - 2;
}//////////////////////////////////////////这里的返回入口地址是指iaddress吗，还是在表中的定位便于以后找到这个
void closethefile()
{
	oint_tab.close();
	osym_tab.close();
	ofstream toclearthefile("symbol_table.txt");//清空文件
	toclearthefile.close();
	toclearthefile.open("整数常量表.txt");
	toclearthefile.close();
	keywordfile.close();
	sourcefile.close();
}
int main()//if 语句还没有实现
{
	string str;
#ifdef VER1
	cout << "this is version1";
#endif
	space_used = 0;///////////////////////////////////////////////////veryimportant//////////////////anothor thing is ififififififiififf you have forget to write the if statement ifififiifififififi
	iaddress = 0;
	osym_tab.open("symbol_table.txt");//'///////waitting
	oint_tab.open("整数常量表.txt");
	/////////////////////////////////////////////////////////
	keywordfile.open("keyword.txt");
	printf("输入文件名");//考虑keyword文件出错的可能性，曾经不小心更改过，不知道有没有多余的空格 tab 回车换行
	cout << endl;
	//cin >> fname;
	sourcefile.open("c.cpp");
	if (sourcefile)
	{
		char ch;
		sourcefile.get(ch);
		int valueofsymbol;
		if (!P(ch, valueofsymbol))
		{
			int x;
			cin >> x;//for testing
			closethefile();
			return 0;
		}
		if (valueofsymbol != 35) { cout << "Error17: 缺少文件结束符号#"; int x; cin >> x;  closethefile(); return 0; }
		getsym(ch, valueofsymbol);//文档已经结束，#符号后面如果不出现任何符号，则会返回-128，否则，返回那个符号
		if (valueofsymbol != -128) { cout << "Error18: #符号后面未结束！！"; int x; cin >> x; closethefile(); return 0; }
		cout << "succeed in compiling"; //relese版本是否不会暂停；
		int x;
		cout << endl;
		cin >> x;//for testing
		closethefile();
		return 1;
	}
	else
	{
		cout << "Error: Can't open the file" << endl;
		closethefile();
		return 0;
	}
}