#include "stdafx.h"
#include"iostream"
#include"cstring"
#include"fstream"
#include"string"
using namespace std;

ofstream file_wordlist_output("词汇表.txt");
string str_from_file_named_keyword;
int value_of_the_keyword;
extern ifstream keywordfile;
extern ifstream sourcefile;
extern string keyword;
extern int tnumber;
int getsym(char & the_single_char_readed_now,int & valueofword)
{
	keywordfile.seekg(0);
	valueofword = 0;
	int counter = 0;
	char string_readed_from_the_file_prepared_to_analyse[100] = { '\0' };
	while (the_single_char_readed_now == ' ' || the_single_char_readed_now == 10 || the_single_char_readed_now == 9 || the_single_char_readed_now == 13)
	{
		sourcefile.get(the_single_char_readed_now);
		if (sourcefile.eof())
			the_single_char_readed_now = -128;
	}
	if ((the_single_char_readed_now >= 'a'&&the_single_char_readed_now <= 'z') || (the_single_char_readed_now >= 'A'&&the_single_char_readed_now <= 'Z'))
	{
		counter = 0;
		while ((the_single_char_readed_now >= 'a'&&the_single_char_readed_now <= 'z') || (the_single_char_readed_now >= '0'&&the_single_char_readed_now <= '9') || (the_single_char_readed_now >= 'A'&&the_single_char_readed_now <= 'Z'))
		{
			string_readed_from_the_file_prepared_to_analyse[counter] = the_single_char_readed_now;
			counter++;
			sourcefile.get(the_single_char_readed_now);
			if (sourcefile.eof())
				the_single_char_readed_now = -128;
		}
		string_readed_from_the_file_prepared_to_analyse[counter] = 0;
		while (!keywordfile.eof())//judging is it keyword
		{
			keywordfile >> str_from_file_named_keyword;
			keywordfile >> value_of_the_keyword;
			if (str_from_file_named_keyword == string_readed_from_the_file_prepared_to_analyse)
			{
				valueofword = value_of_the_keyword;
				file_wordlist_output << valueofword << ' ';
				keyword = string_readed_from_the_file_prepared_to_analyse;
				break;///////very important :maybe error 如果到时可以去掉
			}
		}
		if (valueofword == 0)
		{
			valueofword = 49;//ID
			file_wordlist_output << valueofword << ' ';
			keyword = string_readed_from_the_file_prepared_to_analyse;
		}
		file_wordlist_output << string_readed_from_the_file_prepared_to_analyse << endl;
	}
	else if (the_single_char_readed_now >= '0'&&the_single_char_readed_now <= '9')
	{
		int num;
		counter = 0;
		num = 0;
		valueofword = 50;
		while (the_single_char_readed_now >= '0'&&the_single_char_readed_now <= '9')
		{
			num = 10 * num + the_single_char_readed_now - '0';
			sourcefile.get(the_single_char_readed_now);
			if (sourcefile.eof())
				the_single_char_readed_now = -128;
			counter++;
		}
		if (counter > 10) { cout << "ErrorZ1: 词法分析报错，数字过长" << endl; return 0; }//the length of number should not beyond 10
		file_wordlist_output << valueofword << ' ';
		file_wordlist_output << num << endl;
		tnumber = num;

	}
	else if (the_single_char_readed_now == '<')
	{

		sourcefile.get(the_single_char_readed_now);
		if (sourcefile.eof())
			the_single_char_readed_now = -128;

		if (the_single_char_readed_now == '=')
		{
			valueofword = 23;
			sourcefile.get(the_single_char_readed_now);
			if (sourcefile.eof())
				the_single_char_readed_now = -128;
			file_wordlist_output << valueofword << ' ';
			file_wordlist_output << "<=" << endl;
		}
		else
		{
			valueofword = 24;
			file_wordlist_output << valueofword << ' ';
			file_wordlist_output << "<" << endl;
		}
	}
	else if (the_single_char_readed_now == '>')
	{
		sourcefile.get(the_single_char_readed_now);
		if (sourcefile.eof())
			the_single_char_readed_now = -128;
		if (the_single_char_readed_now == '=')
		{
			valueofword = 25;//the value of greater equal is 25
			file_wordlist_output << valueofword << ' ';
			file_wordlist_output << ">=" << endl;
			sourcefile.get(the_single_char_readed_now);
			if (sourcefile.eof())
				the_single_char_readed_now = -128;
		}
		else
		{
			valueofword = 26;//the value of greater is 26
			file_wordlist_output << valueofword << ' ';
			file_wordlist_output << ">" << endl;
		}
	}
	else if (the_single_char_readed_now == ':')
	{
		sourcefile.get(the_single_char_readed_now);
		if (sourcefile.eof())
			the_single_char_readed_now = -128;
		if (the_single_char_readed_now == '=')
		{
			valueofword = 21;//the value of assignment is 21
			sourcefile.get(the_single_char_readed_now);
			if (sourcefile.eof())
				the_single_char_readed_now = -128;
			file_wordlist_output << valueofword << ' ';
			file_wordlist_output << ":=" << endl;
		}
		else
		{
			valueofword = 22;//the value of null is 22
			file_wordlist_output << valueofword << ' ';
			file_wordlist_output << "null" << endl;
		}
	}
	else
	{
		valueofword = int(the_single_char_readed_now);
		file_wordlist_output << valueofword << ' ';
		file_wordlist_output << the_single_char_readed_now << endl;
		sourcefile.get(the_single_char_readed_now);
		if (sourcefile.eof())
			the_single_char_readed_now = -128;
	}
	return 0;
}

//int main(int argc, char* argv[])
//{
//	sourcefile.open("file.cpp");
//	char the_single_char_readed_now;
//	sourcefile.get(the_single_char_readed_now);
//	while (!sourcefile.eof())
//		getsym(the_single_char_readed_now);//we call every independent object the word
//	sourcefile.close();
//	file_wordlist_output.close();
//	return 0;
//}
