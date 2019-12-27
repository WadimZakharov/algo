/*
 Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки ­– n. Время O(n + p), доп. память – O(p).
Вариант 2. С помощью z-функции.
Формат входного файла
Шаблон, символ перевода строки, строка.
Формат выходного файла
Позиции вхождения шаблона в строке.
 */
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

class KMP {
public:
	KMP(string &_pattern);
	~KMP() {}
	vector<int> KMPalgoritm(string &text);
	KMP(const KMP&) = delete;
	KMP(KMP&&) = delete;
	KMP& operator=(const KMP&) = delete;
	KMP& operator=(KMP&&) = delete;
private:
	string pattern;
	int pattern_size;
	int ZFunctionFromPattern(int i, const string &pattern, int &Left, int &Right) const;
	int ZFunctionFromText(int i, const string &text, int &Left, int &Right) const;
	vector<int> z_function_pattern;
};


KMP::KMP(string &_pattern)
{
    int Left = 0;
    int Right = 0;
	pattern = _pattern;
	pattern_size = pattern.size();
	z_function_pattern = vector<int>(pattern_size);
    z_function_pattern[0] = pattern_size;
	for (int i = 1; i < pattern_size; i++)
		z_function_pattern[i] = ZFunctionFromPattern(i, pattern, Left, Right);
}


int KMP::ZFunctionFromPattern(int i, const string &pattern, int &Left, int &Right) const
{
	int resault = 0;
	if (i <= Right)
		resault = min(Right - i, z_function_pattern[i - Left]);
	while (i + resault < pattern_size && pattern[resault] == pattern[i + resault])
		resault++;
	if (i + resault >= Right)
    {
        Left = i;
        Right = i + resault;
    }
	return resault;
}


int KMP::ZFunctionFromText(int i, const string &text, int &Left, int &Right) const
{
	int resault = 0;
	if (i <= Right)
		resault = min(Right - i, z_function_pattern[i - Left]);
	while (i + resault < text.size() && resault < pattern_size &&  pattern[resault] == text[i + resault])
		resault++;
	if (i + resault >= Right)
    {
        Left = i;
        Right = i + resault;
    }
	return resault;
}

vector<int> KMP::KMPalgoritm(string &text)
{
	int Left = -10;
	int Right = -10;
	vector<int> resault = {};
	for (int i = 0; i < text.size(); i++)
	{
		if (ZFunctionFromText(i, text, Left, Right) == pattern_size)
			resault.push_back(i);
	}
	return resault;
}

int main() {
	string pattern;
	string text;
	cin >> pattern;
	cin >> text;
	KMP kmp(pattern);
	auto resault = kmp.KMPalgoritm(text);
	for (int res : resault)
		cout << res << " ";
	return 0;
}
