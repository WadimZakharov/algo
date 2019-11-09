//2_3. Тупики.
//На вокзале есть некоторое количество тупиков, куда прибывают электрички. Этот вокзал является их конечной станцией. Дано расписание движения электричек, в котором для каждой электрички указано время ее прибытия, а также время отправления в следующий рейс. Электрички в расписании упорядочены по времени прибытия. Когда электричка прибывает, ее ставят в свободный тупик с минимальным номером. При этом если электричка из какого-то тупика отправилась в момент времени X, то электричку, которая прибывает в момент времени X, в этот тупик ставить нельзя, а электричку, прибывающую в момент X+1 — можно.
//В данный момент на вокзале достаточное количество тупиков для работы по расписанию.
//Напишите программу, которая по данному расписанию определяет, какое минимальное количество тупиков требуется для работы вокзала.
//Формат входных данных. Вначале вводится n - количество электричек в расписании. Затем вводится n строк для каждой электрички, в строке - время прибытия и время отправления. Время - натуральное число от 0 до 10^9. Строки в расписании упорядочены по времени прибытия.
//Формат выходных данных. Натуральное число - минимальное количеством тупиков.
//Максимальное время: 50мс, память: 5Мб.
#include <vector>
#include <iostream>
class Heap{
public:
    Heap();
    void push(int value);
    int pop_min();
    int get_min() const;
    int size() const;
    bool empty() const;
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

bool Heap::empty() const
{
  return heap.size() == 0;
}
void Heap::push(int value){
    heap.push_back(value);
    siftUp(heap.size()-1);
}

int Heap::get_min() const{
    if (empty()){
        return 1000000001;//нужно для работы программы решения задачи
    }
    return heap[0];
}

int Heap::pop_min(){
    if (empty()){
        return 1000000001;
    }
    int res = heap[0];
    int new_head = heap[heap.size()-1];
    heap.pop_back();
    if (!empty()){
        heap[0] = new_head;
        siftDown(0);
    }
    return res;
}


void Heap::siftDown(int i){
  int left = child(i, 1);
  int right = child(i, 2);
  int new_min = i;
    if (left < heap.size() && heap[left] < heap[new_min]){
    new_min = left;
    }
    if (right < heap.size() && heap[right] < heap[new_min]){
    new_min = right;
    }
  if (new_min != i) {
    swap(i,new_min);
    siftDown(new_min);
  }
}


void Heap::siftUp(int i){
    while (heap[i] < heap[parent(i)] && i > 0){
        swap(i, parent(i));
        i = parent(i);
        }
}

void Heap::swap(int i, int j){
    int tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}
int Heap::parent(int i){
    int par = (i - 1)/2;
    return par;
}
int Heap::child(int i, int side){
    //side = 1 для левового потомка, side = 2 для правого
    int ch = 2*i + side;
    return ch;
}

int Heap::size() const{
    return heap.size();
}

Heap::~Heap() {
    heap.clear();
}


int count_tupiks(Heap &heap, int actual_max_count_tupik, int arrival_time, int departure_time){
        int count_tupik = actual_max_count_tupik;
        while (arrival_time >  heap.get_min()){
            int value = heap.pop_min();
         }
        heap.push(departure_time);
        if (heap.size() > actual_max_count_tupik){
            count_tupik = heap.size();
        }
    return count_tupik;
}
int main(int argc, const char * argv[]) {
    int n = 0;
    int count_tupik = 0;
    Heap* heap = new Heap();
    std::cin >> n;
    for (int i = 0; i < n; ++i){
        int arrival_time = 0;
        int departure_time = 0;
        std::cin >> arrival_time >> departure_time;
        count_tupik = count_tupiks(*heap, count_tupik, arrival_time, departure_time);
    }
    std::cout<<count_tupik;
    delete heap;
    return 0;
}
