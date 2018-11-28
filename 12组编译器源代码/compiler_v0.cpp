// compiler_v0.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "string"
#include "fstream"
#include"cstring"

#define STACKSIZE 500
#define uMax 30000
//注：flag是0表示返回的是第一个返回值，即string 
class cCounter
{
	unsigned int cnt;
public:
	cCounter(void) { cnt = 0; }
	cCounter(int x) { cnt = x; }
	void zerosetting(void) { cnt = 0; }
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
cCounter name_count;
ifstream keywordfile;
fstream func_tab;
ofstream assembler("反汇编assembler.txt");
int stack[STACKSIZE];
cCounter counter;
extern int getsym(char & the_single_char_readed_now,int &);
char fname[20];
string keyword="";
fstream sym_tab;
fstream osym_tab;
fstream iint_tab;
fstream oint_tab;
ifstream sourcefile;
int space_used;
int iaddress;
int tnumber;
int linelabel=0;
int fly_lookup;
int sp;
int insert(string type, string var_name);
int lookint(int integer);
int lookup(string id);
string NEWT(void);
string _add(string y, string z, string t);
string _divided(string y, string z, string t);
string getlabel();
string _mult(int y, string z, string t);
string _mult(string y, int z, string t);
string _mult(int y, int z, string t);
string _mult(string y, string z, string t);
string _return(string return_v);

string _WgenJP(string label, int retention_linelabel);
string _genJZ(string E, string label);
string _genJP(string label);
string _add(int y, string z, string t);
string _add(string y, int z, string t);
string _add(int y, int z, string t);
string _sub(int y, string z, string t);
string _sub(string y, int z, string t);
string _sub(int y, int z, string t);
string _equal(string y, string z);
string _equal(string y, int z);
string _divided(int y, string z, string t);
string _divided(string y, int z, string t);
string _divided(int y, int z, string t);
string _sub(string y, string z, string t);
string _relop(string y, string z, string t, string relop);
string _and(string y, string z, string t);
string _or(string y, string z, string t);
string _print(string y);
string _call(string func, string parameter);
string _par(string parameter);
string _accept(string varible);
int insert_func_tab(string func_name, string return_type, int entry_address, int num_formal_para, int entry_address_of_varible, int lens_of_varible,int end_of_varible);
int insert_func(string, string);
int int2str(int num, char *des, int des_len);
void zipper_backfilling(string label1, int linelabel)
{
	fstream backlabel("反汇编assembler.txt");
	fstream write("反汇编assembler.txt");
	char label[20];
	while (!backlabel.eof())//考虑到回填时的文件操作是否会影响原来的assembler写操作
	{
		backlabel >> label;
		if (label == label1)
		{
			write.seekp(backlabel.tellg(), ios::beg);
			int size = strlen(label);
			write.seekp(-size, ios::cur);
			int2str(linelabel, label, sizeof(label));
			int i = strlen(label);
			while (size > i)
			{
				write << " ";
				size--;
			}
			write << linelabel;
			write.flush();
			break;
		}
	}
	write.close();
	backlabel.close();
}
int S(char&ch, int &valueofsymbol,string &returned,int &ireturned,int &flag);
int L(char&ch, int&valueofsymbol, string &returned,int &ireturned,int &flag);
int LL(char&ch, int&valueofsymbol, string &returned,int &ireturned,int &flag);
int B(char&ch, int&valueofsymbol, string &returned,int &ireturned,int &flag);
int T(char &ch, int &valueofsymbol, string &returned,int &ireturned,int &flag);
int E(char&ch, int&valueofsymbol, string &returned,int &ireturned,int &flag);
int F(char& ch, int &valueofsymbol, string &returned,int &ireturned,int &flag);
int TT(char &ch, int&valueofsymbol, string &returned,int &ireturned,int &flag);
int FF(char &ch, int&valueofsymbol, string &returned,int &ireturned,int &flag);
int int2str(int num, char *des, int des_len)
{
	int count = 0;
	int tmp = num;
	// 获取数字位数
	while (tmp /= 10)
	{
		count++;
	}
	if (num < 0)	// 负数
	{
		if (des_len < count + 2)	// 目标缓冲区长度不足
			return -1;
		*(des + count + 2) = '\0';	// 字符串结束标志
		*des = '-';	// 负数符号
		num = -num;	// 转为正数
		des++;
	}
	else	// 非负数
	{
		if (des_len < count + 1)	// 目标缓冲区长度不足
			return -1;
		*(des + count + 1) = '\0';	// 字符串结束标志
	}
	// 字符串赋值
	while (count >= 0)
	{
		*(des + count) = num % 10 + '0';
		num /= 10;
		count--;
	}
	return 1;
}
int F(char& ch, int &valueofsymbol, string &returned,int &ireturned,int &flag)
{
	if (valueofsymbol == 40)// (
	{
		getsym(ch, valueofsymbol);
		if (!E(ch, valueofsymbol,returned,ireturned,flag))return 0;//如果E返回的是一个数字则F也返回数字，flag与E保持同步
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
		ireturned = tnumber;
		lookint(ireturned);
		getsym(ch, valueofsymbol);
		flag = 1;
		return 1;
	}
	if (valueofsymbol == 49)//ID
	{
		flag = 0;
		returned = keyword;
		if(lookup(keyword)==-1)return 0;
		getsym(ch,valueofsymbol);
		return 1;
	}
	cout << "Error16: 此处期待出现（ NUM ID ";
	return 0;
}
int LL(char&ch, int&valueofsymbol, string &returned,int &ireturned,int &flag)
{
	if (valueofsymbol == 59)
	{
		getsym(ch, valueofsymbol);
		if(!L(ch, valueofsymbol,returned,ireturned,flag)) return 0;
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
int D(char &ch,int &valueofsymbol, string &returned,int &ireturned,int &flag)
{
	string keyword_temp;
	if (valueofsymbol == 65|| valueofsymbol == 66|| valueofsymbol == 67|| valueofsymbol == 69|| valueofsymbol == 72|| valueofsymbol == 78||valueofsymbol==82)//int char float double long short signed
	{
		keyword_temp = keyword;
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 49)//ID
		{
			cout << "Error5: 声明语句缺少变量名";//返回0 代表程序出错
			return 0;
		}
		insert(keyword_temp, keyword);//插入到符号表
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 59)//；
		{
			cout << "Error5: 声明语句缺少‘；’";//返回0 代表程序出错
			return 0;
		}
		getsym(ch, valueofsymbol);
		while (valueofsymbol == 65 || valueofsymbol == 66 || valueofsymbol == 67 || valueofsymbol == 69 || valueofsymbol == 72 || valueofsymbol == 78 || valueofsymbol == 82)
		{
			keyword_temp = keyword;
			getsym(ch, valueofsymbol);
			if (valueofsymbol != 49)
			{
				cout << "Error6:声明语句缺少变量名";
				return 0;
			}
			insert(keyword_temp, keyword);//插入到符号表
			getsym(ch, valueofsymbol);
			if (valueofsymbol != 59)//；
			{
				cout << "Error5: 声明语句缺少‘；’";//返回0 代表程序出错
				return 0;
			}
			getsym(ch, valueofsymbol);
		}
		cout << "声明语句编译正确！"<<endl;
		return 1;
	}
	else
	{
		cout << "Error4: 声明语句未以int char float double long short signed开始";
		return 0;
	}
}
int O(char &ch, int &valueofsymbol, string &returned, int &ireturned, int &flag,int &formal_para_num)
{
	string keyword_temp;
	formal_para_num = 0;
	if (valueofsymbol == 65 || valueofsymbol == 66 || valueofsymbol == 67 || valueofsymbol == 69 || valueofsymbol == 72 || valueofsymbol == 78 || valueofsymbol == 82)//int char float double long short signed
	{
		keyword_temp = keyword;
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 49)//ID
		{
			cout << "Error22: 函数参数表缺少变量名";//返回0 代表程序出错
			return 0;
		}
		insert(keyword_temp, keyword);//插入到符号表
		formal_para_num++;
		getsym(ch, valueofsymbol);
		while (valueofsymbol==44)//,
		{
			getsym(ch, valueofsymbol);
			if (!(valueofsymbol == 65 || valueofsymbol == 66 || valueofsymbol == 67 || valueofsymbol == 69 || valueofsymbol == 72 || valueofsymbol == 78 || valueofsymbol == 82))
			{
				cout << "Error23: 函数参数表‘,’后缺少缺少变量类型";//返回0 代表程序出错
				return 0;
			}
			keyword_temp = keyword;
			getsym(ch, valueofsymbol);
			if (valueofsymbol != 49)
			{
				cout << "Error6:声明语句缺少变量名";
				return 0;
			}
			insert(keyword_temp, keyword);//插入到符号表
			formal_para_num++;
			getsym(ch, valueofsymbol);
		}
		cout << "函数参数表语句编译正确！" << endl;
		return 1;
	}
	else
	{
		cout << "Error4: 声明语句未以int char float double long short signed开始";
		return 0;
	}
}
int T(char &ch, int &valueofsymbol, string &returned,int &ireturned,int &flag)
{
	if (!F(ch, valueofsymbol,returned,ireturned,flag))return 0;
	string str = returned;
	string tstr;
	bool onlyoneF = true;
	int i = ireturned,tflag=flag;
	while (valueofsymbol == 42|| valueofsymbol == 47)//* /
	{//if valueo == 42 是乘。。。
		if (valueofsymbol == 42)//*
		{
			getsym(ch, valueofsymbol);
			if (!F(ch, valueofsymbol, returned, ireturned, flag))return 0;
			if (tflag == 0)
			{
				if (flag == 0)
				{
					tstr = NEWT();
					cout << _mult(str,returned,tstr);
					insert("int",tstr);                    //留待以后解决，根据str returned的数据类型决定插入的是int 还是。。float即解决运算时的浮点 定点变换。
				}
				else
				{
					tstr = NEWT();
					cout << _mult(str, ireturned, tstr);
					insert("int", tstr);//留待以后解决，根据str returned的数据类型决定插入的是int 还是。。float即解决运算时的浮点 定点变换。
				}
			}
			else
			{
				if (flag == 0)
				{
					tstr = NEWT();
					cout << _mult(i, returned, tstr);
					insert("int", tstr);//留待以后解决，根据str returned的数据类型决定插入的是int 还是。。float即解决运算时的浮点 定点变换。
				}
				else
				{
					tstr = NEWT();
					cout << _mult(i, ireturned, tstr);
					insert("int", tstr);//留待以后解决，根据str returned的数据类型决定插入的是int 还是。。float即解决运算时的浮点 定点变换。
				}
			}
			cout << endl;
		}
		else
		{
			getsym(ch, valueofsymbol);
			if (!F(ch, valueofsymbol, returned, ireturned, flag))return 0;
			if (tflag == 0)
			{
				if (flag == 0)
				{
					tstr = NEWT();
					cout << _divided(str, returned, tstr);
					insert("int", tstr);//留待以后解决，根据str returned的数据类型决定插入的是int 还是。。float即解决运算时的浮点 定点变换。
				}
				else
				{
					tstr = NEWT();
					cout << _divided(str, ireturned, tstr);
					insert("int", tstr);//留待以后解决，根据str returned的数据类型决定插入的是int 还是。。float即解决运算时的浮点 定点变换。
				}
			}
			else
			{
				if (flag == 0)
				{
					tstr = NEWT();
					cout << _divided(i, returned, tstr);
					insert("int", tstr);//留待以后解决，根据str returned的数据类型决定插入的是int 还是。。float即解决运算时的浮点 定点变换。
				}
				else
				{
					tstr = NEWT();
					cout << _divided(i, ireturned, tstr);
					insert("int", tstr);//留待以后解决，根据str returned的数据类型决定插入的是int 还是。。float即解决运算时的浮点 定点变换。
				}
			}
			cout << endl;
		}
		tflag = 0;
		str = tstr;
		onlyoneF = false;
	}//是否考虑既不是42 又不是47但是又不是follow的问题呢             右结合 自左向右边结合
	if(onlyoneF)	return 1;
	else { flag = 0; returned = str; return 1; }
}
int E(char&ch, int&valueofsymbol, string &returned,int &ireturned,int &flag)//E→T{+T| -T}
{
	if(!T(ch, valueofsymbol,returned,ireturned,flag))return 0;
	string str = returned;
	string tstr;
	bool onlyoneE = true;
	int i = ireturned, tflag = flag;
	while(valueofsymbol == 43|| valueofsymbol == 45)//+ -
	{
		if (valueofsymbol == 43)//+
		{
			getsym(ch, valueofsymbol);
			if (!T(ch, valueofsymbol, returned, ireturned, flag))return 0;
			if (tflag == 0)
			{
				if (flag == 0)
				{
					tstr = NEWT();
					cout << _add(str, returned, tstr);
					insert("int", tstr);//留待以后解决，根据str returned的数据类型决定插入的是int 还是。。float即解决运算时的浮点 定点变换。
				}
				else
				{
					tstr = NEWT();
					cout << _add(str, ireturned, tstr);
					insert("int", tstr);//留待以后解决，根据str returned的数据类型决定插入的是int 还是。。float即解决运算时的浮点 定点变换。
				}
			}
			else
			{
				if (flag == 0)
				{
					tstr = NEWT();
					cout << _add(i, returned, tstr);
					insert("int", tstr);//留待以后解决，根据str returned的数据类型决定插入的是int 还是。。float即解决运算时的浮点 定点变换。
				}
				else
				{
					tstr = NEWT();
					cout << _add(i, ireturned, tstr);
					insert("int", tstr);//留待以后解决，根据str returned的数据类型决定插入的是int 还是。。float即解决运算时的浮点 定点变换。
				}
			}
			cout << endl;
		}
		else
		{
			getsym(ch, valueofsymbol);
			if (!T(ch, valueofsymbol, returned, ireturned, flag))return 0;
			if (tflag == 0)
			{
				if (flag == 0)
				{
					tstr = NEWT();
					cout << _sub(str, returned, tstr);
					insert("int", tstr);//留待以后解决，根据str returned的数据类型决定插入的是int 还是。。float即解决运算时的浮点 定点变换。
				}
				else
				{
					tstr = NEWT();
					cout << _sub(str, ireturned, tstr);
					insert("int", tstr);//留待以后解决，根据str returned的数据类型决定插入的是int 还是。。float即解决运算时的浮点 定点变换。
				}
			}
			else
			{
				if (flag == 0)
				{
					tstr = NEWT();
					cout << _sub(i, returned, tstr);
					insert("int", tstr);//留待以后解决，根据str returned的数据类型决定插入的是int 还是。。float即解决运算时的浮点 定点变换。
				}
				else
				{
					tstr = NEWT();
					cout << _sub(i, ireturned, tstr);
					insert("int", tstr);//留待以后解决，根据str returned的数据类型决定插入的是int 还是。。float即解决运算时的浮点 定点变换。
				}
			}
			cout << endl;
		}
		tflag = 0;
		str = tstr;
		onlyoneE = false;
	}	
	if (onlyoneE)	return 1;
	else { flag = 0; returned = str; return 1; }//如果没有+或者-也是可以的
}
int L(char&ch, int&valueofsymbol, string &returned,int &ireturned,int &flag)
{
	if (!S(ch, valueofsymbol,returned,ireturned,flag))return 0;
	if (!LL(ch, valueofsymbol,returned,ireturned,flag))return 0;
	return 1;
}
int FF(char &ch, int&valueofsymbol, string &returned,int &ireturned,int &flag)//这里时间来不及了写的比较急躁
{
	string str;
	string tstr;
	if (valueofsymbol != 49)//ID
	{
		cout << "Error17: 逻辑运算运算符前缺少变量";
		return 0;
	}
	if(lookup(keyword)==-1)return 0;
	returned = keyword;
	str = keyword;
	getsym(ch, valueofsymbol);
	if (valueofsymbol == 23 || valueofsymbol == 24 || valueofsymbol == 25 || valueofsymbol == 26||valueofsymbol==21||valueofsymbol==22)//待实现判等号 ，判不等号
	{
		int temp = valueofsymbol;
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 49)//ID
		{
			cout << "Error18: 逻辑运算中运算符后缺少变量";
			return 0;
		}
		if (lookup(keyword) == -1)return 0;
		switch (temp) //<= 23	< 24	>= 25	> 26    := 21(判等号)      : 22
		{
			case 21://新加了21 和22 可能出错！！！
			{
				tstr = NEWT();
				cout << _relop(str, keyword, tstr, ":=");
				insert("int", tstr);
				break; 
			}
			case 22:
			{
				tstr = NEWT();
				cout << _relop(str, keyword, tstr, ":");
				insert("int", tstr);
				break;
			}
			case 23:
			{
				tstr = NEWT();
				cout<<_relop(str, keyword, tstr, "<=");
				insert("int", tstr);
				break;
			}
			case 24:
			{
				tstr = NEWT();
				cout << _relop(str, keyword, tstr, "<");
				insert("int", tstr);
				break;
			}
			case 25:
			{
				tstr = NEWT();
				cout << _relop(str, keyword, tstr, ">=");
				insert("int", tstr);
				break;
			}
			case 26:
			{
				tstr = NEWT();
				cout << _relop(str, keyword, tstr, ">");
				insert("int", tstr);
				break;
			}
		}
		cout << endl;
		getsym(ch, valueofsymbol);
		flag = 0;
		returned = tstr;
		return 1;
	}
	flag = 0;
	return 1;
}
int TT(char &ch, int &valueofsymbol, string &returned,int &ireturned,int &flag)//T’ →F’ {∧ F’ } 
{
	if (!FF(ch, valueofsymbol,returned,ireturned,flag)) return 0;
	string str = returned;
	string tstr;
	while (valueofsymbol == 17)//and
	{
		getsym(ch, valueofsymbol);
		if (!FF(ch, valueofsymbol,returned,ireturned,flag))return 0;
		tstr = NEWT();
		cout << _and(str, returned, tstr);
		insert("int", tstr);
		str = tstr;
		cout << endl;
	}
	flag = 0;
	returned = str;
	return 1;
}
int B(char&ch, int&valueofsymbol, string &returned,int &ireturned,int &flag)//B→T’{	∨T’}
{
	if (!TT(ch, valueofsymbol,returned,ireturned,flag)) return 0;
	string str = returned;
	string tstr;
	while (valueofsymbol == 18 )//or
	{
		getsym(ch, valueofsymbol);
		if (!TT(ch, valueofsymbol,returned,ireturned,flag))return 0;
		tstr = NEWT();
		cout << _or(str, returned, tstr);
		insert("int", tstr);
		str = tstr;
		cout << endl;
	}
	flag = 0;
	returned = str;
	return 1;
}
int Y(char&ch, int&valueofsymbol, string &returned, int &ireturned, int &flag)
{//Y=ID(E{,E}) 
	string func,parameter;
	int num_of_parameter=0;
	if (valueofsymbol != 49)
	{
		cout << "Error27: 函数调用缺少函数名";
		return 0;
	}
	func = keyword;//进入Y后的第一个ID是否定义不需要lookup因为在S内调用Y时已经确定过了ID不仅已定义，而且还确定是函数名
	getsym(ch, valueofsymbol);
	if (valueofsymbol != 40)//(
	{
		cout << "Error28: 函数名后面缺少‘（’";
		return 0;
	}
	getsym(ch, valueofsymbol);
	if (!E(ch, valueofsymbol, returned, ireturned, flag))return 0;
	
	if (flag == 0)//E字符串返回值
	{
		parameter = returned;
	}
	else
	{
		char b[11];
		int2str(ireturned, b, sizeof(b));
		parameter = b;
	}
	cout << _par(parameter);
	cout << endl;
	num_of_parameter++;
	while (valueofsymbol == 44)//,
	{
		getsym(ch, valueofsymbol);
		if (!E(ch, valueofsymbol, returned, ireturned, flag))return 0;
		if (flag == 0)//E字符串返回值
		{
			parameter =returned;
		}
		else
		{
			char b[11];
			int2str(ireturned, b, sizeof(b));
			parameter = b;
		}
		num_of_parameter++;
		cout << _par(parameter);
		cout << endl;
	}
	if (valueofsymbol != 41)//)
	{
		cout << "Error10: 函数调用后面缺少‘）’" << endl;
		return 0;
	}
	char b[20];
	int2str(num_of_parameter, b, sizeof(b));
	cout << _call(func, b);
	cout<< endl;
	getsym(ch, valueofsymbol);
	flag = 0;
	return 1;
}
int S(char&ch,int &valueofsymbol, string &returned,int &ireturned,int &flag)
{
	int temp1;
	if (valueofsymbol == 49)
	{
		temp1 = fly_lookup;
		fly_lookup = 0;
		if (lookup(keyword) == -2)
		{
			fly_lookup = temp1;
			if (!Y(ch, valueofsymbol, returned, ireturned, flag)) return 0;
		}
		else
		{
			fly_lookup = temp1;
			string str = keyword;
			string tstr;
			if (lookup(keyword) == -1)
				return 0;//check the lookup after ID
			getsym(ch, valueofsymbol);
			if (valueofsymbol != 61)
			{
				cout << "Error7: 赋值语句缺少‘=’";
				return 0;
			}
			getsym(ch, valueofsymbol);
			if (valueofsymbol == 49)
			{
				temp1 = fly_lookup;//fly_lookup设置的目的是为了让lookup 在全局搜索函数名和局部搜索变量名之间切换。这里是由3局部切为全局，即搜索起点的变换
				fly_lookup = 0;
				if (lookup(keyword) == -2)
				{
					fly_lookup = temp1;
					if (!Y(ch, valueofsymbol, returned, ireturned, flag)) return 0;
					cout << _accept(str);
				}
				else
				{
					fly_lookup = temp1;
					if (!E(ch, valueofsymbol, returned, ireturned, flag)) return 0;
					if (flag == 0)
						cout << _equal(str, returned);
					else
						cout << _equal(str, ireturned);
				}
			}
			else {
				if (!E(ch, valueofsymbol, returned, ireturned, flag)) return 0; 
				if (flag == 0)
					cout << _equal(str, returned);
				else
					cout << _equal(str, ireturned);
			}
			cout << endl;
			counter++;
			cout << "语句" << counter << "编译正确" << endl;//编写一个计数器，填入x中
		}
		return 1;
	}
	if (valueofsymbol == 123)//{
	{
		getsym(ch, valueofsymbol);
		if (!L(ch, valueofsymbol,returned,ireturned,flag))return 0;
		if (valueofsymbol != 125)
		{
			cout << "Error8: 复合语句缺少｝" << endl;
			return 0;
		}
		getsym(ch, valueofsymbol);
		counter++;
		cout << "语句" << counter << "编译正确" << endl;//编写一个计数器，填入x中
		return 1;
	}
	if (valueofsymbol == 86)
	{
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 49)
		{
			cout << "Error21: print 后面期望出现一个ID" << endl;
			return 0;
		}
		else {
			if (lookup(keyword) == -1)
				return 0;//check the lookup after ID
			cout << _print(keyword);
			cout << endl;
			counter++;
			cout << "语句" << counter << "编译正确" << endl;//编写一个计数器，填入x中
		}
		getsym(ch, valueofsymbol);
		return 1;
	}
	if (valueofsymbol == 88)//while
	{
		string label1 = getlabel();
		cout << label1 + ":" << endl;
		int retention_linelabel = linelabel;
		zipper_backfilling(label1, linelabel);
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 40)//(
		{
			cout << "Error9: while后面缺少‘（’";
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (!B(ch, valueofsymbol,returned,ireturned,flag))return 0;
		if (valueofsymbol != 41)//)
		{
			cout << "Error10: while 语句判断条件后面缺少‘）’" << endl;
			return 0;
		}
		string label2 = getlabel();
		cout<<_genJZ(returned, label2)<<endl;
		getsym(ch, valueofsymbol);
		if (valueofsymbol == 87)//do
		{
			getsym(ch, valueofsymbol);
			if (!S(ch, valueofsymbol,returned,ireturned,flag)) return 0;
			cout << _WgenJP(label1,retention_linelabel) << endl;
			cout << label2 + ":" << endl;
			zipper_backfilling(label2, linelabel);
			cout << "while语句编译成功" << endl;
			return 1;
		}
		else
		{
			cout << "Error11： while语句缺少do" << endl;
			return 0;
		}
	}
	if (valueofsymbol == 68)//if
	{
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 40)//(
		{
			cout << "Error19: if后面缺少‘（’";
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (!B(ch, valueofsymbol,returned,ireturned,flag))return 0;
		if (valueofsymbol != 41)//)
		{
			cout << "Error20: if 语句判断条件后面缺少‘）’" << endl;
			return 0;
		}
		string label1=getlabel();
		cout<<_genJZ(returned,label1);
		cout << endl;
		getsym(ch, valueofsymbol);
		if (valueofsymbol == 19)//then
		{
			getsym(ch, valueofsymbol);
			if (!S(ch, valueofsymbol,returned,ireturned,flag)) return 0;
			string label2 = getlabel();
			cout<<_genJP(label2);
			cout << endl;

			assembler.flush();
			cout << label1 + ":" << endl;
			//assembler << label1 + ":" << endl; 
			zipper_backfilling(label1, linelabel);
			////////////////////////////////////////////

			if (valueofsymbol == 71)//else
			{
				getsym(ch, valueofsymbol);
				if (!S(ch, valueofsymbol,returned,ireturned,flag)) return 0;
			}
			cout << label2 + ":" << endl;//////////////////////////////////////////////////
			zipper_backfilling(label2, linelabel);
			cout << "if语句编译成功" << endl;
			return 1;
		}
	}
	if (valueofsymbol == 76)//return
	{
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 49)
		{
			cout << "Error29: return 后面期望出现一个ID" << endl;
			return 0;
		}
		if (lookup(keyword) == -1)
			return 0;//check the lookup after ID
		cout << _return(keyword);
		cout << endl;
		getsym(ch, valueofsymbol);
		return 1;
	}
	{//如果上面的都没有成立
		cout << "Error12: 声明语句后面期望有｛,ID,while,if中的一种" << endl;
		return 0;
	}
}
int P(char &ch, int &valueofsymbol, string &returned,int &ireturned,int &flag)//开始使用超前技术 超前读入一个symbol
{
	//getsym(ch, valueofsymbol);
	if (valueofsymbol == 123)//{
	{
		getsym(ch, valueofsymbol);
		if (!D(ch, valueofsymbol,returned,ireturned,flag)) return 0;///////ready to be changed
		if (!S(ch, valueofsymbol,returned,ireturned,flag)) return 0;

		if (valueofsymbol != 125)//}
		{
			cout << "Error3: 程序结束缺少｝";
			return 0;
		}
		getsym(ch, valueofsymbol);
		//#读入//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		return 1;
	}
	else
	{
		cout << "Error2: 程序开始缺少｛";
		return 0;
	}
}
int X(char &ch, int &valueofsymbol, string &returned, int &ireturned, int &flag)
{
	string keyword_temp;
	getsym(ch, valueofsymbol);
	if (valueofsymbol == 65 || valueofsymbol==83||valueofsymbol == 66 || valueofsymbol == 67 || valueofsymbol == 69 || valueofsymbol == 72 || valueofsymbol == 78 || valueofsymbol == 82)//int char float double long short signed
	{//X →int ID(O) P { int ID(O) P }
		keyword_temp = keyword;
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 49)//ID
		{
			cout << "Error24: 缺少函数名";//返回0 代表程序出错
			return 0;
		}
		insert_func(keyword_temp, keyword);//插入到符号表
		int entry_of_varible = osym_tab.tellp();
		fly_lookup = entry_of_varible;
		string func_name = keyword;
		string return_type = keyword_temp;
		int entry_address = linelabel;
		int end_of_varible = 0;
		keyword_temp = keyword;
		name_count.zerosetting();
		getsym(ch, valueofsymbol);
		if (valueofsymbol != 40)//(
		{
			cout << "Error25: 函数名后面缺少‘（’";
			return 0;
		}
		getsym(ch, valueofsymbol);
		int formal_para_num=0;
		if (!O(ch, valueofsymbol, returned, ireturned, flag,formal_para_num))return 0;
		if (valueofsymbol != 41)//)
		{
			cout << "Error26: 函数参数表缺少‘）’" << endl;
			return 0;
		}
		getsym(ch, valueofsymbol);
		if (!P(ch, valueofsymbol, returned, ireturned, flag))return 0;
		end_of_varible= osym_tab.tellp();//这样的话也是在回车前的位置结束，思考光标位置是否被  结束，开始读取lookup  影响,即光标在最后一条记录输入的最后是否影响最后一个记录的lookup读取，丢失部分信息。
		insert_func_tab(func_name, return_type, entry_address,formal_para_num,entry_of_varible,space_used,end_of_varible);
		if (keyword_temp == "main") {
			assembler.width(5);
			assembler << linelabel;
			cout.width(5);
			cout << linelabel++;
			cout <<  "HALT" << endl;
			assembler <<"HALT" << endl;
		}
		while (valueofsymbol == 65 || valueofsymbol == 83 || valueofsymbol == 66 || valueofsymbol == 67 || valueofsymbol == 69 || valueofsymbol == 72 || valueofsymbol == 78 || valueofsymbol == 82)
		{
			keyword_temp = keyword;
			getsym(ch, valueofsymbol);
			if (valueofsymbol != 49)
			{
				cout << "Error24: 缺少函数名";
				return 0;
			}
			insert_func(keyword_temp, keyword);//插入到符号表
			return_type = keyword_temp;
			func_name = keyword;
			entry_of_varible = osym_tab.tellp();
			fly_lookup = entry_of_varible;
			entry_address = linelabel;
			keyword_temp = keyword;
			name_count.zerosetting();
			getsym(ch, valueofsymbol);
			if (valueofsymbol != 40)//(
			{
				cout << "Error25: 函数名后面缺少‘（’";
				return 0;
			}
			getsym(ch, valueofsymbol);
			formal_para_num = 0;
			if (!O(ch, valueofsymbol, returned, ireturned, flag,formal_para_num))return 0;
			if (valueofsymbol != 41)//)
			{
				cout << "Error26: 函数参数表缺少‘）’" << endl;
				return 0;
			}
			getsym(ch, valueofsymbol);
			if (!P(ch, valueofsymbol, returned, ireturned, flag))return 0;
			end_of_varible = osym_tab.tellp();//这样的话也是在回车前的位置结束，思考光标位置是否被  结束，开始读取lookup  影响,即光标在最后一条记录输入的最后是否影响最后一个记录的lookup读取，丢失部分信息。
			insert_func_tab(func_name, return_type, entry_address, formal_para_num, entry_of_varible, space_used,end_of_varible);
			if (keyword_temp == "main") {
				assembler.width(5);
				assembler << linelabel;
				cout.width(5);
				cout << linelabel++;
				cout << "HALT" << endl;
				assembler << "HALT" << endl;
			}
		}
		//#读入//////////////////////////////////////////////////////////////////////////////////////////
		return 1;
	}
	else
	{
		cout << "Error4: 函数缺少返回值类型——int char float double long short signed";
		return 0;
	}
}
string NEWT(void)
{
	static const string tname = "t";
	string temporary_var;
	char b[11];
	int2str(name_count++,b, sizeof(b));
	temporary_var = tname + b;
	return temporary_var;
}
string getlabel()
{
	static cCounter label_count(1);
	const string lname = "label";
	string label;
	char b[11];
	int2str(label_count++, b, sizeof(b));
	label = lname + b ;
	return label;
}
string _add(string y,string z,string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	assembler << "+ " + y + " " + z + " " + t  <<endl;;
	return "+ " + y + " " + z + " " + t  ;
}
string _relop(string y, string z, string t,string relop)//关系运算符只能被ID和ID进行
{
	assembler.width(5);
	assembler << linelabel;
	assembler <<  relop + " " + y + " " + z + " " + t  <<endl;;
	cout.width(5) ; cout << linelabel++;
	return  relop + " " + y + " " + z+" " + t  ;
}
string _and(string y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	assembler << "∧ " + y + " " + z + " " + t  <<endl;;
	return "∧ " + y + " " + z + " " + t  ;
}
string _or(string y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	assembler<<"∨ " + y + " " + z + " " + t  <<endl;;
	return "∨ " + y + " " + z + " " + t  ;
}
string _equal(string y, string z)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	assembler<< "= " + z + " " + y  <<endl;;
	return "= " + z + " " + y  ;
}
string _equal(string y, int z)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(z, b, sizeof(b));
	string bb = b;
	assembler<< "= " + bb + " " + y  <<endl;;
	return "= " + bb+ " " + y  ;
}
string _mult(string y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	assembler << "* " + y + " " + z + " " + t  <<endl;
	return "* " + y + " " + z + " " + t  ;
}
string _call(string func, string parameter)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5); 
	cout << linelabel++;
	assembler << "CALL " + func + " " + parameter<< endl;
	return "CALL " + func + " " +parameter;
}
string _par(string parameter)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5);
	cout << linelabel++;
	assembler << "PAR " + parameter << endl;
	return "PAR " + parameter;
}
string _accept(string varible)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5);
	cout << linelabel++;
	assembler << "ACC " + varible << endl;
	return "ACC " +varible;
}
string _return(string return_v)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5);
	cout << linelabel++;
	assembler << "RETURN " + return_v << endl;
	return "RETURN " + return_v;
}
string _mult(int y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(y, b, sizeof(b));
	string bb = b;
	assembler<< "* " + bb + " " + z + " " + t  <<endl;;
	return "* " +bb + " " + z + " " + t  ;
}
string _mult(string y, int z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(z, b, sizeof(b));
	string bb = b;
	assembler << "* " + y + " " + bb + " " + t  <<endl;;;
	return "* " + y + " " + bb + " " + t  ;
}
string _mult(int y, int z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	char c[11];
	int2str(y, c, sizeof(c));
	int2str(z, b, sizeof(b));
	string cc = c;
	string bb = b;
	assembler<<"* " + cc + " " + bb + " " + t  <<endl;;
	return "* " + cc + " " + bb + " " + t  ;
}
string _genJZ(string E_place, string label)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5); cout << linelabel++;
	assembler<< "JZ " + E_place + " " + label  <<endl;;
	return "JZ " + E_place + " " + label  ;
}
string _genJP(string label)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5); cout << linelabel++;
	assembler<< "JP " + label  <<endl;;
	return "JP " + label  ;
}
string _print(string y)//ERROR 可能有错 print 是新编的
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5); cout << linelabel++;
	assembler << "PRINT " + y << endl;
	return "PRINT " + y;
}
string _WgenJP(string label, int retention_linelabel)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5); cout << linelabel++;
	assembler << "JP " <<retention_linelabel << endl;;
	return "JP " + label;
}
string _sub(string y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	assembler << "- " + y + " " + z + " " + t  <<endl;;
	return "- " + y + " " + z + " " + t  ;
}
string _divided(string y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	assembler << "/ " + y + " " + z + " " + t  <<endl;;
	return "/ " + y + " " + z + " " + t  ;
}
string _divided(int y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(y, b, sizeof(b));
	string bb = b;
	assembler << "/ " + bb + " " + z + " " + t  <<endl;;
	return "/ " + bb + " " + z + " " + t  ;
}
string _divided(string y, int z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(z, b, sizeof(b));
	string bb = b;
	assembler<< "/ " + y + " " + bb + " " + t  <<endl;;
	return "/ " + y + " " + bb + " " + t  ;
}
string _divided(int y, int z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	char c[11];
	int2str(y, c, sizeof(c));
	int2str(z, b, sizeof(b));
	string cc = c;
	string bb = b;
	assembler<< "/ " + cc + " " + bb + " " + t  <<endl;;
	return "/ " + cc + " " + bb + " " + t  ;
}
string _add(int y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(y, b, sizeof(b));
	string bb = b;
	assembler<< "+ " + bb + " " + z + " " + t  <<endl;;
	return "+ " + bb + " " + z + " " + t  ;
}
string _add(string y, int z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(z, b, sizeof(b));
	string bb = b;
	assembler<< "+ " + y + " " + bb + " " + t  <<endl;;
	return "+ " + y + " " + bb + " " + t  ;
}
string _add(int y, int z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	char c[11];
	int2str(y, c, sizeof(c));
	int2str(z, b, sizeof(b));
	string cc = c;
	string bb = b;
	assembler<< "+ " + cc + " " + bb + " " + t  <<endl;;
	return "+ " + cc + " " + bb + " " + t  ;
}
string _sub(int y, string z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(y, b, sizeof(b));
	string bb = b;
	assembler << "- " + bb + " " + z + " " + t  <<endl;;
	return "- " + bb + " " + z + " " + t  ;
}
string _sub(string y, int z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	int2str(z, b, sizeof(b));
	string bb = b;
	assembler<< "- " + y + " " + bb + " " + t  <<endl;;
	return "- " + y + " " + bb + " " + t  ;
}
string _sub(int y, int z, string t)
{
	assembler.width(5);
	assembler << linelabel;
	cout.width(5) ; cout << linelabel++;
	char b[11];
	char c[11];
	int2str(y, c, sizeof(c));
	int2str(z, b, sizeof(b));
	string cc = c;
	string bb = b;
	assembler << "- " + cc + " " + bb + " " + t  <<endl;;
	return "- " + cc + " " + bb + " " + t  ;
}
int lookup(string id)//wait
{
	string str1;
	string str2;
	int str3;
	sym_tab.open("symbol_table.txt");
	sym_tab.seekg(fly_lookup, ios::beg);
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
	cout << "Error_x1: 语义分析出错，存在变量或函数未定义而使用";
	sym_tab.close();
	return -1;//准备报错弹出到底，不过第一层是==-1
}
int insert_func_tab(string func_name,string return_type,int entry_address,int num_formal_para,int entry_address_of_varible,int lens_of_varible,int end_of_varible)
//函数的名称、返回值类型、函数体的入口地址、形参个数、形参及局部变量在变量表的开始位置、数据区的长度,变量表的结束位置填入函数符号表中。
{//函数体的入口地址是行号，形参个数比形参中最大的编号大一，形参及局部变量在变量表的开始位置是tellp在隔了一个回车的距离返回的，数据区的长为形参及局部变量的个数,the last one is变量表的结束位置
	func_tab << func_name << " " << return_type << " " << entry_address << " " << num_formal_para << " " << entry_address_of_varible << " " << lens_of_varible <<" "<<end_of_varible<< endl;
	return 1;
}
class function_table_info
{//函数的名称、返回值类型、函数体的入口地址、形参个数、形参及局部变量在变量表的开始位置、数据区的长度,变量表的结束位置填入函数符号表中。
public:
	string func_name;
	string return_type;
	int entry_address;
	int num_formal_para;
	int entry_address_of_varible;
	int lens_of_varible;
	int end_of_varible;
};
function_table_info lookup_func_tab(string func_name)
{
	function_table_info func_info;
	fstream func_tabl("函数符号表.txt");
	while (!func_tabl.eof())
	{
		func_tabl >> func_info.func_name;
		func_tabl >> func_info.return_type;
		func_tabl >> func_info.entry_address;
		func_tabl >> func_info.num_formal_para;
		func_tabl >> func_info.entry_address_of_varible;
		func_tabl >> func_info.lens_of_varible;
		func_tabl >> func_info.end_of_varible;
		if (func_info.func_name == func_name)
		{
			func_tabl.close();
			return func_info;
		}
	}
	func_tabl.close();
}
int insert(string type, string var_name)//wait
{
	osym_tab.seekp(0, ios::end);
	osym_tab << endl;//文件操作复杂，选择将space_used 变成全局变量，每次insert时加1！！！
	osym_tab << type << " " << var_name << " " << space_used;
	osym_tab.flush();//可能这里会出问题，不确定flush函数使用后的效果，根据网络上说，可以刷新缓冲区，如果不行，可以关闭后再打开一次文件
	space_used += 1;
	return 1;
}
int insert_func(string type, string func_name)
{
	osym_tab.seekp(0, ios::end);
	osym_tab << endl;//文件操作复杂，选择将space_used 变成全局变量，每次insert时加1！！！
	osym_tab << type << " " << func_name << " " << -2;//-1为函数名标志
	osym_tab.flush();//可能这里会出问题，不确定flush函数使用后的效果，根据网络上说，可以刷新缓冲区，如果不行，可以关闭后再打开一次文件
	space_used =0;
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
	oint_tab<< integer <<" "<< iaddress;
	oint_tab.flush();
	iaddress += 2;
	iint_tab.close();
	return iaddress-2;
}//////////////////////////////////////////这里的返回入口地址是指iaddress吗，还是在表中的定位便于以后找到这个
void closethefile()
{
	system("pause");
	oint_tab.close();
	osym_tab.close();
	assembler.close();	
	func_tab.close();//保存文件
	ofstream toclearthefile("symbol_table.txt");//清空文件
	toclearthefile.close();
	toclearthefile.open("整数常量表.txt");
	toclearthefile.close();
	toclearthefile.open("函数符号表.txt");
	toclearthefile.close();
	keywordfile.close();
	sourcefile.close();
}
class PS
{
public:
	int run;
	int finish;
	int error;
};
void Line2op(char line[], int linesize, int &pc, string &ir_op, string &ir_op1,string &ir_op2, string &ir_op3)
{
	char t[20];
	ofstream otemp("temp.txt");
	otemp << line;
	otemp.close();
	fstream temp("temp.txt");
	temp >> pc;
	if (temp.eof())
		ir_op = "";
	temp >> ir_op;
	if (temp.eof()) 
		ir_op1 = "";
	temp >> ir_op1;
	if (temp.eof()) 
		ir_op2 = "";
	temp >> ir_op2;	
	if (temp.eof()) //如果temp结束则删除以前留下的字符串，返回空字符
		ir_op3 = "";
	temp >> ir_op3;

	temp.close();
}
int compiler()
{
	string str;
#ifdef VER5
	cout << "this is version5(带函数调用和解释处理功能的编译器)";
#endif
	string returned;
	int ireturned = 0, flag = 0;
	space_used = 0;///////////////////////////////////////////////////veryimportant//////////////////anothor thing is ififififififiififf you have forget to write the if statement ifififiifififififi
	iaddress = 0;
	osym_tab.open("symbol_table.txt");//'///////waitting
	oint_tab.open("整数常量表.txt");
	func_tab.open("函数符号表.txt");
	/////////////////////////////////////////////////////////
	keywordfile.open("keyword.txt");
	cout << endl;
	printf("输入文件名");//考虑keyword文件出错的可能性，曾经不小心更改过，不知道有没有多余的空格 tab 回车换行
	cout << endl;
	cin >> fname;
	cout << endl;
	sourcefile.open(fname);
	cout << endl;
	cout.setf(std::ios::left);
	if (!assembler) {
		cout << "打不开中间代码输出文件";
		return 0;
	}
	assembler.setf(std::ios::left);
	if (sourcefile)
	{
		char ch;
		sourcefile.get(ch);
		int valueofsymbol;
		if (!X(ch, valueofsymbol, returned, ireturned, flag))
		{
			int x;
			cin >> x;//for testing
			closethefile();
			return 0;
		}
		if (valueofsymbol != 35) { cout << "Error17: 缺少文件结束符号#"; int x; cin >> x;  closethefile(); return 0; }
		getsym(ch, valueofsymbol);//文档已经结束，#符号后面如果不出现任何符号，则会返回-128，否则，返回那个符号
		if (valueofsymbol != -128) { cout << "Error18: #符号后面未结束！！"; int x; cin >> x; closethefile(); return 0; }
		//cout << linelabel << "   HALT" << endl;
		//assembler << linelabel << "   HALT" << endl;
		cout << "succeed in compiling,now come into the phase of interpretation"; //relese版本是否不会暂停；
		//int x;
		cout << endl << endl << endl;
		//cin >> x;//for testing
		oint_tab.close();
		osym_tab.close();
		assembler.close();
		func_tab.close();
		return 1;
	}
	else
	{
		cout << "Error: Can't open the file" << endl;
		closethefile();
		return 0;
	}
}
int fly_lookup_end=0;
int lookupi(string id)                                              //wait
{
	string str1;
	string str2;
	int str3;
	sym_tab.open("symbol_table.txt");
	sym_tab.seekg(fly_lookup, ios::beg);
	while (fly_lookup_end > sym_tab.tellg())/////////////注意了可能出错，没有测试
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
	cout << "Error_x1: 语义分析出错，存在变量或函数未定义而使用";
	sym_tab.close();
	return -1;                                                              //准备报错弹出到底，不过第一层是==-1
}
void op2reg(string ir_op1, int &ax)
{
	int bx;
	if (ir_op1[0] < 60)//是数字
	{
		int b = atoi(ir_op1.c_str());
		ax = b;
	}
	else {
		bx = lookupi(ir_op1);
		ax = stack[sp+3+bx];
	}
}
int interpretation()
{
	function_table_info func_info;
	PS ps;//ps: (run, finish, error); {程序状态字}
	fstream code("反汇编assembler.txt");     //ir: order; {指令寄存器}
	string ir_op, ir_op1, ir_op2, ir_op3;//指令操作码,操作数1（x），操作数2（y）
	int ax, bx, cx, dx;
	int pc;// : integer; {指令计数器}
	int t; //integer; {栈顶寄存器}//实际上现在所占栈空间为0-11
	sp=0;// : integer; {基地址寄存器}
		  //display: ARRAY[0..7] OF integer; {地址寄存器组}
		  //code: ARRAY[0..codesize] OF order; {代码区}
	int top; //活动记录顶指针
	char line[50];
	int TimesOfTransferPar = 1;
	string func_stack[100];
	func_info = lookup_func_tab("main");
	func_stack[0] = "main";
	int func_stacki = 1;
	//函数的名称、返回值类型、函数体的入口地址、形参个数、形参及局部变量在变量表的开始位置、数据区的长度,变量表的结束位置填入函数符号表中。
	//函数体的入口地址是行号，形参个数比形参中最大的编号大一，形参及局部变量在变量表的开始位置是tellp在隔了一个回车的距离返回的，数据区的长为形参及局部变量的个数,the last one is变量表的结束位置
	//函数的名称、返回值类型、函数体的入口地址、形参个数、形参及局部变量在变量表的开始位置、数据区的长度,变量表的结束位置填入函数符号表中。
	{	
		fstream temp("反汇编assembler.txt");
		int b = func_info.entry_address;
		char a[50];
		ax = -1;
		while (ax != b - 1)
		{
			temp.getline(a, 50);
			Line2op(a, 50, ax, ir_op, ir_op1, ir_op2, ir_op3);
		}
		code.seekg(temp.tellg(), ios::beg);
		temp.close();
	}
	top = func_info.lens_of_varible+3-1;
	fly_lookup = func_info.entry_address_of_varible;
	fly_lookup_end = func_info.end_of_varible;
	cout << "程序输出结果-------------------------------------------------------" << endl;
	while (!code.eof())//如果order已读到结束，则无法移动文件指针，注意
	{
		code.getline(line, 50);
		Line2op(line, 50, pc, ir_op, ir_op1, ir_op2, ir_op3);
		if (ir_op == "=")
		{
			op2reg(ir_op1, ax);
			cx=lookupi(ir_op2);//第二个操作数只可能是变量
			stack[sp+3+cx] = ax;
		}
		else if (ir_op == "∧")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax&&cx);
		}
		else if (ir_op == "∨")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax||cx);
		}
		else if (ir_op == ">=")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax >=cx);
		}
		else if (ir_op == "<=")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax <= cx);
		}
		else if (ir_op == ">")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax > cx);
		}
		else if (ir_op == "<")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax < cx);
		}
		else if (ir_op == ":")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax != cx);
		}
		else if (ir_op == ":=")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax == cx);
		}
		else if (ir_op == "+")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax + cx);
		}
		else if (ir_op == "*")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax * cx);
		}
		else if (ir_op == "-")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax - cx);
		}
		else if (ir_op == "/")
		{
			op2reg(ir_op1, ax);
			op2reg(ir_op2, cx);
			if (cx == 0) { cout << "ErrorY2:除以0错误！"<<endl; return 0; }
			dx = lookupi(ir_op3);
			stack[sp+3+dx] = (ax / cx);
		}
		else if (ir_op == "JP")
		{
			fstream temp("反汇编assembler.txt");
			int b = atoi(ir_op1.c_str());
			char a[50];
			ax = -1;
			while (ax != b - 1)
			{
				temp.getline(a, 50);
				Line2op(a, 50, ax, ir_op, ir_op1, ir_op2, ir_op3);
			}
			code.seekg(temp.tellg(), ios::beg);
			temp.close();
		}
		else if (ir_op == "JZ")
		{
			op2reg(ir_op1, ax);
			if (ax != 0)continue;
			fstream temp("反汇编assembler.txt");
			int b = atoi(ir_op2.c_str());
			char a[50];
			ax = -1;
			while (ax != b - 1)
			{
				temp.getline(a, 50);
				Line2op(a, 50, ax, ir_op, ir_op1, ir_op2, ir_op3);
			}
			code.seekg(temp.tellg(), ios::beg);
			temp.close();
		}
		else if (ir_op == "RETURN")
		{
			op2reg(ir_op1, dx);//将返回值放在dx寄存器中，如果接受，则一定是在返回地址处，故而不会受到其他操作的影响。
			bx = stack[sp + 1];
			top = sp - 1;
			sp = stack[sp];
			{//转到返回地址
				fstream temp("反汇编assembler.txt");
				char a[50];
				ax = -1;
				while (ax != bx - 1)
				{
					temp.getline(a, 50);
					Line2op(a, 50, ax, ir_op, ir_op1, ir_op2, ir_op3);
				}
				code.seekg(temp.tellg(), ios::beg);
				temp.close();
			}
			func_stacki--;
			func_info=lookup_func_tab(func_stack[func_stacki - 1]);
			fly_lookup = func_info.entry_address_of_varible;
			fly_lookup_end = func_info.end_of_varible;//恢复lookup表的查找区间！
		}
		else if (ir_op == "ACC")
		{
			bx=lookupi(ir_op1);
			stack[sp + 3 + bx] = dx;
		}
		else if (ir_op == "CALL")
		{
			func_info = lookup_func_tab(ir_op1);
			func_stack[func_stacki++] = ir_op1;///////////////////////注意: 可能出错！如果ir_op1被放弃，则func_stack里面的字符串可能也会消失
			fly_lookup = func_info.entry_address_of_varible;
			fly_lookup_end = func_info.end_of_varible;
			TimesOfTransferPar = 1;
			stack[top + 1] = sp;
			sp = top + 1;
			top = func_info.lens_of_varible + 3 - 1+sp;
			op2reg(ir_op2, ax);
			if (ax != func_info.num_formal_para)
			{
				cout << "ErrorY3: 传入参数与函数定义时的参数个数不一样" << endl;
				cout << "-------------------------------------------------------------------" << endl;
				return 0;
			}
			stack[sp + 2] = ax;
			stack[sp + 1] = pc + 1;//返回地址是call下面一条语句的行号。
			{//转到子程序入口
				fstream temp("反汇编assembler.txt");
				int b = func_info.entry_address;
				char a[50];
				ax = -1;
				while (ax != b - 1)
				{
					temp.getline(a, 50);
					Line2op(a, 50, ax, ir_op, ir_op1, ir_op2, ir_op3);
				}
				code.seekg(temp.tellg(), ios::beg);
				temp.close();
			}
		}
		else if (ir_op == "PAR")
		{
			op2reg(ir_op1, ax);
			stack[top + 4 + TimesOfTransferPar++ - 1] = ax;
		}
		else if (ir_op == "PRINT")
		{
			op2reg(ir_op1, ax);
			cout << ax << endl;
		}
		else if (ir_op == "HALT")
		{
			cout << "-------------------------------------------------------------------" << endl;
			return 1;
		}
		else
		{
			cout << "ErrorY1: 指令无法识别" << endl;
		}
	}
}
int main()//if 语句还没有实现
{
	if (!compiler())
	{
		system("pause");
		closethefile();
		return 0;
	}
	//编译模块
	if (!interpretation())
	{
		system("pause");
		closethefile();
		return 0;
	}
	//解释模块


	system("pause");
	ofstream toclearthefile("symbol_table.txt");//清空文件
	toclearthefile.close();
	toclearthefile.open("整数常量表.txt");
	toclearthefile.close();
	toclearthefile.open("函数符号表.txt");
	toclearthefile.close();
	keywordfile.close();
	sourcefile.close();
	return 1;
}