/*
 3_2. Сортировка почти упорядоченной последовательности.
 Дана последовательность целых чисел a1...an и натуральное число k, такое что для любых i, j: если j >= i + k, то a[i] <= a[j]. Требуется отсортировать последовательность. Последовательность может быть очень длинной. Время работы O(n * log(k)). Память O(k). Использовать слияние.
 */
#include <vector>
#include <iostream>
template<typename T, typename Compare = std::less<T>()>
class Heap {
public:
	explicit Heap(Compare comp = Compare()) : comp{ comp }{}

	void push(T value);
	T pop_min();
	int size() const;
	bool empty() const;
	void make_heap(std::vector<T>);
	Heap(const Heap&) = delete;
	Heap(Heap&&) = delete;
	Heap& operator=(const Heap&) = delete;
	Heap& operator=(Heap&&) = delete;
	~Heap();
private:
	std::vector<T> heap = {};
	void swap(int i, int j);
	void siftDown(int i);
	void siftUp(int i);
	static int parent(int i);
	static int child(int i, int side);
	Compare comp;
};


template<typename T, typename Compare>
void Heap<T, Compare>::make_heap(std::vector<T> v) {
	heap = v;
	for (int i = parent(size() - 1); i >= 0; --i)
	{
		siftDown(i);
	}
}

template<typename T, typename Compare>
bool Heap<T, Compare>::empty() const
{
	return heap.size() == 0;
}

template<typename T, typename Compare>
void Heap<T, Compare>::push(T value) {
	heap.push_back(value);
	siftUp(heap.size() - 1);
}

template<typename T, typename Compare>
T Heap<T, Compare>::pop_min() {
	T res = heap[0];
	T new_head = heap[heap.size() - 1];
	heap.pop_back();
	if (!empty()) {
		heap[0] = new_head;
		siftDown(0);
	}
	return res;
}

template<typename T, typename Compare>
void Heap<T, Compare>::siftDown(int i) {
	int left = child(i, 1);
	int right = child(i, 2);
	int new_min = i;
	if (left < heap.size() && comp(heap[left], heap[new_min])) {
		new_min = left;
	}
	if (right < heap.size() && comp(heap[right], heap[new_min])) {
		new_min = right;
	}
	if (new_min != i) {
		swap(i, new_min);
		siftDown(new_min);
	}
}

template<typename T, typename Compare>
void Heap<T, Compare>::siftUp(int i) {
	while (comp(heap[i], heap[parent(i)]) && i > 0) {
		swap(i, parent(i));
		i = parent(i);
	}
}

template<typename T, typename Compare>
void Heap<T, Compare>::swap(int i, int j) {
	T tmp = heap[i];
	heap[i] = heap[j];
	heap[j] = tmp;
}

template<typename T, typename Compare>
int Heap<T, Compare>::parent(int i) {
	int par = (i - 1) / 2;
	return par;
}

template<typename T, typename Compare>
int Heap<T, Compare>::child(int i, int side) {
	//side = 1 для левового потомка, side = 2 для правого
	int ch = 2 * i + side;
	return ch;
}

template<typename T, typename Compare>
int Heap<T, Compare>::size() const {
	return heap.size();
}

template<typename T, typename Compare>
Heap<T, Compare>::~Heap() {
	heap.clear();
}

template<typename T, typename Compare = std::less<>>
std::vector<T> merge(std::vector<T> &v1, std::vector<T> &v2) {
	int i = 0;
	int j = 0;
	Compare comp = Compare();
	std::vector<T> mearge_v(v1.size() + v2.size());

	for (int k = 0; k < v1.size() + v2.size(); k++) {
		if (comp(v1[i], v2[j]))
		{
			mearge_v[k] = v1[i];
			i++;
		}
		else
		{
			mearge_v[k] = v2[j];
			j++;
		}

		if (i == v1.size()) 
		{
			mearge_v.insert(mearge_v.begin() + (i + j), v2.begin()+j, v2.end());
			return mearge_v;
		}

		if (j == v2.size()) 
		{
			mearge_v.insert(mearge_v.begin() + (i + j), v1.begin()+i, v1.end());
			return mearge_v;
		}

	}
	return mearge_v;
}

template<typename T, typename Compare = std::less<>>
std::vector<T> HeapSort_k(std::vector<T> &vec, int k) {
	/*
	 Принимает на вход почти упорядоченную последовательность и шаг k(из задания)
	 Сортировка каждого среза длины k из последовательности производится методом HeapSort.
	 Возвращает отсортированную последовательность.
	 */
	int i = 0;
	Heap<T, std::less<T>> heap;
	for (i = 0; i < k*(vec.size() / k); i = i + k)
	{
		heap.make_heap(std::vector<T>(vec.begin() + i, vec.begin() + (i + k)));
		for (int j = i; j < i + k; j++)
		{
			vec[j] = heap.pop_min();
		}

	}

	heap.make_heap(std::vector<T>(vec.begin() + i, vec.end()));
	for (int j = i; j < vec.size(); j++)
	{
		vec[j] = heap.pop_min();
	}

	for (i = 0; i < k*(vec.size() / k) - k; i = i + k) {
		std::vector<T> slice1 = std::vector<T>(vec.begin() + i, vec.begin() + (i + k));
		std::vector<T> slice2 = std::vector<T>(vec.begin() + (i + k), vec.begin() + (i + 2 * k));
		std::vector<T> meagre_vec = merge(slice1, slice2);
		for (int j = 0; j < 2 * k; j++) {
			vec[i + j] = meagre_vec[j];
		}
	}

	if (i != vec.size() - k) {
		std::vector<T> slice1 = std::vector<T>(vec.begin() + i, vec.begin() + (i + k));
		std::vector<T> slice2 = std::vector<T>(vec.begin() + (i + k), vec.end());
		std::vector<T> meagre_vec = merge(slice1, slice2);
		for (int j = 0; j < vec.size() - i; j++) {
			vec[i + j] = meagre_vec[j];
		}
	}

	return vec;
}

int main(int argc, const char * argv[]) {
	int n = 0;
	int k = 0;
	std::cin >> n;
	std::cin >> k;
	std::vector<int> vec = {};
	for (int i = 0; i < n; i++) {
		int value = 0;
		std::cin >> value;
		vec.push_back(value);
	}

	vec = HeapSort_k(vec, k);
	for (int i = 0; i < vec.size(); i++) {
		std::cout << vec[i];
		if (i != vec.size() - 1)
			std::cout << " ";
	}

	return 0;
}
