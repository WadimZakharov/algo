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
#include <functional>
constexpr auto nullstr = "";
constexpr auto del = "DELETE_VALUE";
using namespace std;


template<class T, class THash1, class THash2>
class HashTable {
public:
    HashTable(size_t _initial_size, THash1 hash1, THash2 hash2);
    ~HashTable();
    HashTable(const HashTable&) = delete;
    HashTable(HashTable&&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    HashTable& operator=(HashTable&&) = delete;

    bool Has(const T& key) const;
    bool Add(const T& key);
    bool Remove(const T& key);

private:
    size_t initial_size;
    T *table;
    THash1 Hash;
    THash2 Hash2;
    void ResizeTable();
    size_t size;
};

template<class T, class THash1, class THash2>
HashTable<T, THash1, THash2>::~HashTable()
{
    delete[] table;
}

template<class T, class THash1, class THash2>
HashTable<T, THash1, THash2>::HashTable(size_t _initial_size,
                                        THash1 hash1, THash2 hash2)
{
    initial_size = _initial_size;
    size = 0;
    table = new string [initial_size];
    Hash = hash1;
    Hash2 = hash2;
}


template<class T, class THash1, class THash2>
bool HashTable<T, THash1, THash2>::Has(const T& key) const
{
    const size_t hash = Hash(const_cast<T &>(key));
    if (table[hash] == key)
        return true;

    if (table[hash] == nullstr)
        return false;

    const size_t hash2 = Hash2(const_cast<T &> (key));
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


template<class T, class THash1, class THash2>
bool HashTable<T, THash1, THash2>::Add(const T& key) {
    const auto hash = Hash(const_cast<T &> (key));
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

    const size_t hash2 = Hash2(const_cast<T &> (key));
    int delete_ind = -1;
    for (size_t i = 1; i < initial_size; i++)
    {
        size_t hash_i = (hash + i * hash2) % initial_size;
        if (table[hash_i] == del && delete_ind==-1)
            delete_ind = hash_i;
        if (table[hash_i] == nullstr)
        {
            if (delete_ind != -1)
                table[delete_ind] = key;
            else
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
    if (delete_ind == -1) {
        return false;
    }
    else {
        table[delete_ind] = key;
        size++;
        if (size >= (3./4)*initial_size)
            ResizeTable();
        return true;
    }
}


template<class T, class THash1, class THash2>
bool HashTable<T, THash1, THash2>::Remove(const T& key)
{
    const size_t hash = Hash(const_cast<T &> (key));
    if (table[hash] == key)
    {
        table[hash] = del;
        size--;
        return true;
    }

    if (table[hash] == nullstr)
        return false;

    const size_t hash2 = Hash2(const_cast<T &> (key));
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
            size--;
            return true;
        }
    }
    return false;
}


template<class T, class THash1, class THash2>
void HashTable<T, THash1, THash2>::ResizeTable()
{
    size = 0;
    initial_size = initial_size * 2;
    T *new_table = new T[initial_size];
    for(int i= 0; i < initial_size/2; i++)
    {
        T key = table[i];
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

template<uint32_t prime_number>
size_t HashString(const string &str)
{
    int hash = 0;
    for (char c : str)
    {
        hash = (hash * prime_number + c);
    }
    return hash;
}

template<uint32_t length>
size_t HashString2(const string &str)
{
    int hash = length*str.size()+1;
    return hash;
}




int main()
{
    HashTable<string, function<uint32_t(const std::string&)>,
            function<uint32_t(const std::string&)>> table(8, HashString<37>, HashString2<2>);
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
