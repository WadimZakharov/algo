//Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
//Формат входных данных.
//В первой строке количество команд n. n ≤ 1000000.
//Каждая команда задаётся как 2 целых числа: a b.
//a = 1 - push front
//a = 2 - pop front
//a = 3 - push back
//a = 4 - pop back
//Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
//Если дана команда pop*, то число b - ожидаемое значение.Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
//Формат выходных данных.
//Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.
//1_3. Реализовать очередь с помощью двух стеков. Использовать стек, реализованный с помощью динамического буфера.
#include <iostream>
#include <assert.h>
class Stack_v {
public:
	~Stack_v();
	// Проверка стека на пустоту
	bool empty() const;
	// Добавление элемента
	void push(int value);
	// Извлечение
	int pop();

private:
	int size = 100;
	int *stack = new int[size];
	int index = -1;
};


class Queue {
public:
	void push_back(int value);
	int pop_front();
private:
	Stack_v input_stack;
	Stack_v output_stack;

};

bool Stack_v::empty() const
{
	return index == -1;
}

void Stack_v::push(int value)
{
	stack[index + 1] = value;
	index++;
	if (index > size / 4)
		//Выделение новой памяти
	{
		size = size * 2;
		int *new_stack = new int[size];
		memcpy(new_stack, stack, size * sizeof(int));
		delete[] stack;
		stack = new_stack;
	}
}

Stack_v::~Stack_v() {
	delete[] stack;
}

int Stack_v::pop() {
	int res = -1;
	if (!empty())
	{
		res = stack[index];
	}
	index--;
	if (index < size / 4)
		//Освобождение лишней памяти
	{
		size = size / 2;
		int *new_stack = new int[size];
		memcpy(new_stack, stack, size * sizeof(int));
		delete[] stack;
		stack = new_stack;
	}
	return res;
}




void Queue::push_back(int value) {
	input_stack.push(value);
}
int Queue::pop_front() {
	if (output_stack.empty()) { //если выходной стек пустой, заполним его элементами из входного
		while (!input_stack.empty()) {
			output_stack.push(input_stack.pop());
		}
	}
	if (output_stack.empty()) {
		return -1;
	}
	else {
		return output_stack.pop();
	}
}

int main() {
	int commands_count = 0;
	std::cin >> commands_count;
	Queue queue;
	for (int i = 0; i < commands_count; ++i) {
		int command = 0;
		int value = 0;
		std::cin >> command >> value;
		if (command == 2) {
			int q_value = queue.pop_front();
			if (q_value == value) {
				continue;
			}
			else {
				std::cout << "NO";
				return 0;
			}
		}
		else {
			queue.push_back(value);
		}
	}
	std::cout << "YES";
	return 0;
}
