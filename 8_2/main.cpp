/*
 Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.
1_2. Для разрешения коллизий используйте двойное хеширование.
Формат входных данных
Каждая строка входных данных задает одну операцию над множеством. Запись операции состоит из типа операции и следующей за ним через пробел строки, над которой проводится операция.
Тип операции  – один из трех символов:
    +  означает добавление данной строки в множество;
    -  означает удаление  строки из множества;
    ?  означает проверку принадлежности данной строки множеству.
При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве. При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.
 */
#include <iostream>
#include <string>
constexpr auto nullstr = "";
constexpr auto del = "DELETE_VALUE";
using namespace std;
class HashTable {
public:
    HashTable(size_t _initial_size);
    ~HashTable();
    HashTable(const HashTable&) = delete;
    HashTable(HashTable&&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    HashTable& operator=(HashTable&&) = delete;

    bool Has(const string& key) const;
    bool Add(const string& key);
    bool Remove(const string& key);

private:
    size_t initial_size;
    string *table;
    size_t Hash(string str) const;
    size_t Hash2(string str) const;
    void ResizeTable();
    size_t size;
};

HashTable::~HashTable()
{
    delete[] table;
}

HashTable::HashTable(size_t _initial_size)
{
    initial_size = _initial_size;
    size = 0;
    table = new string [initial_size];
}

size_t HashTable::Hash(string str) const
{
    int prime_number = 559991;
    int hash = 0;
    for (char& c : str)
    {
        hash = (hash * prime_number + c)%initial_size;
    }
    return hash;
}

size_t HashTable::Hash2(string str) const
{
    int hash = 2*str.size()+1;
    return hash;
}


bool HashTable::Has(const string& key) const
{
    const size_t hash = Hash(key);
    if (table[hash] == key)
        return true;

    if (table[hash] == nullstr)
        return false;

    const size_t hash2 = Hash2(key);
    for (size_t i = 1; i < initial_size; i++)
    {
        size_t hash_i = (hash + i * hash2) % initial_size;
        if (table[hash_i] == key)
        {
            return true;
        }
        else if (table[hash_i] == nullstr)
        {
            return false;
        }
    }
    return false;
}

bool HashTable::Add(const string& key) {
    const size_t hash = Hash(key);
    if (table[hash] == key)
        return false;

    if (table[hash] == nullstr)
    {
        table[hash] = key;
        size++;
        if (size >= (3./4)*initial_size)
            ResizeTable();
        return true;
    }

    const size_t hash2 = Hash2(key);
    for (size_t i = 1; i < initial_size; i++)
    {
        size_t hash_i = (hash + i * hash2) % initial_size;
        if (table[hash_i] == nullstr)
        {
            table[hash_i] = key;
            size++;
            if (size >= (3./4)*initial_size)
                ResizeTable();
            return true;
        }
        if (table[hash_i] == key)
        {
            return false;
        }
    }
    return false;
}

bool HashTable::Remove(const string& key)
{
    const size_t hash = Hash(key);
    if (table[hash] == key)
    {
        table[hash] = del;
        return true;
    }

    if (table[hash] == nullstr)
        return false;

    const size_t hash2 = Hash2(key);
    for (size_t i = 1; i < initial_size; i++)
    {
        size_t hash_i = (hash + i * hash2) % initial_size;
        if (table[hash_i] == nullstr)
        {
            return false;
        }
        if (table[hash_i] == key)
        {
            table[hash_i] = del;
            return true;
        }
    }
    return false;
}

void HashTable::ResizeTable()
{
    size = 0;
    initial_size = initial_size * 2;
    string *new_table = new string[initial_size];
    for(int i= 0; i < initial_size/2; i++)
    {
        string key = table[i];
        if (key != nullstr && key != del)
        {
            const size_t hash = Hash(key);
            if (new_table[hash] == nullstr)
            {
                new_table[hash] = key;
                size++;
            }
            else
            {
                const size_t hash2 = Hash2(key);
                for (int j = 1; j < initial_size; j++)
                {
                    size_t hash_i = (hash + j * hash2) % initial_size;
                    if (new_table[hash_i] == nullstr)
                    {
                        new_table[hash_i] = key;
                        size++;
                        break;
                    }
                }
            }
        }
    }

    delete[] table;
    table  = new_table;
}
int main()
{
    HashTable table(8);
    char command = ' ';
    std::string value;
    while (std::cin >> command >> value)
    {
        switch (command) {
            case '?':
                std::cout << (table.Has(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '+':
                std::cout << (table.Add(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.Remove(value) ? "OK" : "FAIL") << std::endl;
                break;
        }
    }
    return 0;
}