/*
 3_2. Сортировка почти упорядоченной последовательности.
 Дана последовательность целых чисел a1...an и натуральное число k, такое что для любых i, j: если j >= i + k, то a[i] <= a[j]. Требуется отсортировать последовательность. Последовательность может быть очень длинной. Время работы O(n * log(k)). Память O(k). Использовать слияние.
 */
#include <vector>
#include <iostream>
class Heap {
public:
    Heap();
    void push(int value);
    int pop_min();
    int get_min() const;
    int size() const;
    bool empty() const;
    void make_heap(std::vector<int>);
    ~Heap();
private:
    std::vector<int> heap;
    void swap(int i, int j);
    void siftDown(int i);
    void siftUp(int i);
    static int parent(int i);
    static int child(int i, int side);
};

Heap::Heap()
{
    heap = {};
}


void Heap::make_heap(std::vector<int> v) {
    heap = v;
    for (int i = parent(size()-1); i >= 0; --i)
    {
        siftDown(i);
    }
}

bool Heap::empty() const
{
    return heap.size() == 0;
}
void Heap::push(int value) {
    heap.push_back(value);
    siftUp(heap.size() - 1);
}

int Heap::get_min() const{
    if (empty()) {
        return 1000000001;//нужно для работы программы решения задачи
    }
    return heap[0];
}

int Heap::pop_min() {
    if (empty()) {
        return 1000000001;
    }
    int res = heap[0];
    int new_head = heap[heap.size() - 1];
    heap.pop_back();
    if (!empty()) {
        heap[0] = new_head;
        siftDown(0);
    }
    return res;
}


void Heap::siftDown(int i) {
    int left = child(i, 1);
    int right = child(i, 2);
    int new_min = i;
    if (left < heap.size() && heap[left] < heap[new_min]) {
        new_min = left;
    }
    if (right < heap.size() && heap[right] < heap[new_min]) {
        new_min = right;
    }
    if (new_min != i) {
        swap(i, new_min);
        siftDown(new_min);
    }
}


void Heap::siftUp(int i) {
    while (heap[i] < heap[parent(i)] && i > 0) {
        swap(i, parent(i));
        i = parent(i);
    }
}

void Heap::swap(int i, int j) {
    int tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}
int Heap::parent(int i) {
    int par = (i - 1) / 2;
    return par;
}
int Heap::child(int i, int side) {
    //side = 1 для левового потомка, side = 2 для правого
    int ch = 2 * i + side;
    return ch;
}

int Heap::size() const{
    return heap.size();
}

Heap::~Heap() {
    heap.clear();
}

int max(int a, int b) {
    if (a > b)
        return a;
    else
        return b;
}
std::vector<int> meagre(std::vector<int> v1, std::vector<int> v2) {
    int i = 0;
    int j = 0;
    std::vector<int> mearge_v = {};

    for (int k = 0; k < v1.size() + v2.size(); k++) {
        if (v1[i] < v2[j])
        {
            mearge_v.push_back(v1[i]);
            i++;
        }
        else
        {
            mearge_v.push_back(v2[j]);
            j++;
        }

        if (i==v1.size()) {
            for(j = j; j<v2.size(); j++)
            {
                mearge_v.push_back(v2[j]);
            }
            return mearge_v;
        }

        if (j == v2.size()) {
            for(i = i; i<v1.size(); i++)
            {
                mearge_v.push_back(v1[i]);
            }
            return mearge_v;
        }

    }
    return mearge_v;
}

std::vector<int> HeapSort_k(std::vector<int> vec, int k){
    /*
     Принимает на вход почти упорядоченную последовательность и шаг k(из задания)
     Сортировка каждого среза длины k из последовательности производится методом HeapSort.
     Возвращает отсортированную последовательность.
     */
    int i = 0;
    Heap heap;
    for (i=0; i < k*(vec.size()/k); i=i+k)
    {
        heap.make_heap(std::vector<int>(vec.begin() + i, vec.begin() + (i+k)));
        for (int j = i; j < i + k; j++)
        {
            vec[j] = heap.pop_min();
        }
        
    }

    heap.make_heap(std::vector<int>(vec.begin() + i, vec.end()));
    for (int j = i; j < vec.size(); j++)
    {
        vec[j] = heap.pop_min();
    }
    
    for (i = 0; i < k*(vec.size()/k)-k; i=i+k){
        std::vector<int> slice1 = std::vector<int>(vec.begin() + i, vec.begin() + (i+k));
        std::vector<int> slice2 = std::vector<int>(vec.begin() + (i+k), vec.begin() + (i+2*k));
        std::vector<int> meagre_vec = meagre(slice1,slice2);
        for(int j=0; j<2*k; j++){
            vec[i+j] = meagre_vec[j];
        }
    }
    
    if (i!=vec.size()-k){
        std::vector<int> slice1 = std::vector<int>(vec.begin() + i, vec.begin() + (i+k));
        std::vector<int> slice2 = std::vector<int>(vec.begin() + (i+k), vec.end());
        std::vector<int> meagre_vec = meagre(slice1,slice2);
        for(int j=0; j<vec.size()-i; j++){
            vec[i+j] = meagre_vec[j];
        }
    }
    
    return vec;
}

int main(int argc, const char * argv[]) {
    int n = 0;
    int k = 0;
    std::cin>>n;
    std::cin>>k;
    std::vector<int> vec = {};
    for(int i = 0; i < n; i++){
        int value = 0;
        std::cin>>value;
        vec.push_back(value);
    }
    
    vec = HeapSort_k(vec,k);
    for (int i=0; i<vec.size(); i++){
           std::cout<<vec[i];
        if (i!=vec.size()-1)
           std::cout<<" ";
       }

    return 0;
}
