/*
 Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n. Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции с индексом k (0..n-1) в отсортированном массиве. Напишите нерекурсивный алгоритм.
 Требования к дополнительной памяти: O(n). Требуемое среднее время работы: O(n).
 Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
 4_4. Реализуйте стратегию выбора опорного элемента “случайный элемент”. Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.
 */
#include <iostream>
#include <vector>
using namespace std;
int partition(vector<int> &vec, int left, int right)
{
	int n = right - left;
	int main_ind = left + rand() % (n + 1);
	int main_el = vec[main_ind];
	vec[main_ind] = vec[left];
	vec[left] = main_el;
	int i = right;
	int j = i;
	while (j > left)
	{
		while (vec[j] < vec[left])
		{
			j--;
		}

		if (j != left)
		{
			int tmp = vec[i];
			vec[i] = vec[j];
			vec[j] = tmp;
			i--;
			j--;
		}
	}
	int tmp = vec[left];
	vec[left] = vec[i];
	vec[i] = tmp;
	return i;
}

int search_k_stat_index(vector<int> &vec, int k) {
 
	int start = 0;
	int end = vec.size() - 1;
	int k_stat_index = partition(vec, start, end);

	while (k_stat_index != k)
	{

		if (k_stat_index > k)
		{
			end = k_stat_index - 1;
		}

		else
		{
			start = k_stat_index + 1;
		}
		k_stat_index = partition(vec, start, end);
	}
	return k_stat_index;
}

int main()
{
	int n = 0;
	int k = 0;
	cin >> n;
	cin >> k;
	vector<int> vec = {};
	for (int i = 0; i < n; i++) {
		int value = 0;
		std::cin >> value;
		vec.push_back(value);
	}
	
	int k_stat_index = search_k_stat_index(vec, k);
	cout << vec[k_stat_index];
}
