#include <iostream>
#include <stdexcept>
#include <utility>
#include <string>
#include <iomanip>
#include <ctime>
#include <typeinfo>
#include <deque>
#include <vector>
#include <queue>
#include <algorithm>

#define PART 100

using namespace std;

template <typename Key, typename Value>
class Mapa
{
public:

    Mapa() {}
    virtual ~Mapa() {}
    virtual Value& operator[] (const Key &key) = 0;
    virtual Value operator[] (const Key &key) const = 0;
    virtual int brojElemenata () const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const Key &key) = 0;

};

// HashMapaLan

template <typename Key, typename Value>
class HashMapaLan: public Mapa<Key,Value>
{
    std::vector<std::deque<std::pair<Key,Value>>> container;
    unsigned int (*functionPointer)(Key, unsigned int);
    unsigned int size;


    //  void expand();
    //  void annul();
    //  void load(const HashMapaLan<Key,Value> &map);
    //  int seek(const Key &key) const;
    //  int add (const Key &key, const Value &value);

    int binaryFind(unsigned int index, int l, int r, Key key, int &last) const;
    //  struct Obrisan: public std::pair<Key,Value> {};

public:

    HashMapaLan(): Mapa<Key, Value>(), container(100), functionPointer(nullptr), size(0) {}
    ~HashMapaLan() override;
    HashMapaLan(const HashMapaLan<Key,Value> &map);
    HashMapaLan<Key,Value> &operator= (const HashMapaLan<Key,Value> &map);
    Value& operator[] (const Key &key) override;
    Value operator[] (const Key &key) const override;
    int brojElemenata () const override;
    void obrisi() override;
    void obrisi(const Key &key) override;

    void ispisi() const;
    void definisiHashFunkciju(unsigned int (*functionPointer)(Key, unsigned int))
    {
        this -> functionPointer = functionPointer;
    }

};


template <typename Key, typename Value>
HashMapaLan<Key, Value>::~HashMapaLan() {}                                      //

template <typename Key, typename Value>
HashMapaLan<Key, Value>::HashMapaLan(const HashMapaLan<Key,Value> &map)
{

    container = map.container;
    functionPointer = map.functionPointer;
    size = map.size;
}

template <typename Key, typename Value>
HashMapaLan<Key,Value> &HashMapaLan<Key, Value>::operator= (const HashMapaLan<Key,Value> &map)
{

    if(this == &map) return *this;                                              // destruktivna samododjela

    container = map.container;
    functionPointer = map.functionPointer;
    size = map.size;

}


template <typename Key, typename Value>
int HashMapaLan<Key, Value>::brojElemenata() const
{
    return size;
}

template <typename Key, typename Value>
void HashMapaLan<Key, Value>::obrisi()
{
    for(auto &x: container) {
        x.clear();
    }

    size = 0;
}

template <typename Key, typename Value>
int HashMapaLan<Key,Value>::binaryFind(unsigned int index, int l, int r, Key key, int &last) const
{
    if (r >= l) {
        auto mid = l + (r - l) / 2;
        last = r;

        if (container[index][mid].first == key)
            return mid;

        if (container[index][mid].first > key)
            return binaryFind(index, l, mid - 1, key, last);

        return binaryFind(index, mid + 1, r, key, last);
    }

    return -1;
}

template <typename Key, typename Value>
Value& HashMapaLan<Key,Value>::operator[] (const Key &key)


{
    if(!functionPointer) throw "No function!";
    auto index = functionPointer(key, container.size());
    int last = 0;

    if (!container[index].empty()) {
        auto check = binaryFind(index, 0, container[index].size() - 1, key, last);
        if (check != -1) return container[index][check].second;
        last++;
    }

    size++;
    return container[index].insert(container[index].begin() + last, std::make_pair(key, Value()))->second;


}

template <typename Key, typename Value>
Value HashMapaLan<Key,Value>::operator[] (const Key &key) const
{
    if(!functionPointer) throw "No function!";
    auto index = functionPointer(key, container.size());
    int last;

    if (!container[index].empty()) {
        int check = binaryFind(index, 0, container[index].size() - 1, key, last);
        if (check != -1) return container[index][check].second;
    }

    return Value();

}

template <typename Key, typename Value>
void HashMapaLan<Key,Value>::obrisi(const Key &key)

{

    if(!functionPointer) throw "No function!";
    auto index = functionPointer(key,container.size());
    int last;

    int check = binaryFind(index, 0, container[index].size() - 1, key, last);

    if(check == -1)
        throw std::logic_error("Key not found!");

    container[index].erase(container[index].begin() + check);
    size--;

}


template <typename Key, typename Value>
void HashMapaLan<Key,Value>::ispisi() const
{
    for(int i = 0; i < size; i++) {
        std::cout << std::endl << "Kljuc: " << container[i]-> first << " | Vrijednost: " << container[i]->second;
    }

    std::cout << std::endl << "-----------------------------------------------------------------------------------" << std::endl;
}
