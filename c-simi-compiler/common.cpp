#include"common.h"
using namespace std;

bool isNum(string num)
{
	bool flag = 1;
	int point_cnt = 0;
	for (auto s : num)
	{
		if (!(s >= '0' && s <= '9' || s == '.'))
		{
			flag = 0;
			break;
		}
		if (s == '.')
			point_cnt++;
	}
	if (!flag || point_cnt > 1)
		return false;
	else
		return true;
}
string charToString(char ch)
{
	char str[2];
	str[0] = ch;
	str[1] = '\0';
	return str;
}
void errorPrint(const char* info, Token kind)
{
	cout << info << " ";
	switch (kind)
	{
	case NUM:
		cout << "出错,不是常量" << endl;
		break;
	case ID:
		cout << "出错,不是标识符" << endl;
	default:
		break;
	}
}