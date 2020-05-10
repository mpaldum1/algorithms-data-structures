#include <iostream>
#include <stdexcept>
#include <utility>
#include <string>
#include <iomanip>
#include <ctime>
#include <typeinfo>

#define PART 1000

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

// HashMapa

template <typename Key, typename Value>
class HashMapa: public Mapa<Key,Value>
{
    std::pair<Key,Value>** container;
    int size, capacity;
    unsigned int (*functionPointer)(Key, unsigned int);


    void expand();
    void annul();
    void load(const HashMapa<Key,Value> &map);
    int seek(const Key &key) const;
    int insert (const Key &key, const Value &value);

    struct Obrisan: public std::pair<Key,Value> {};

public:

    HashMapa(): Mapa<Key, Value>(), container(new std::pair<Key,Value>*[PART]), size(0), capacity(PART), functionPointer(nullptr)
    {
        for(int i = 0; i < capacity; i++) container[i] = nullptr;
    }
    ~HashMapa() override;
    HashMapa(const HashMapa<Key,Value> &map);
    HashMapa<Key,Value> &operator= (const HashMapa<Key,Value> &map);
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
void HashMapa<Key, Value>::annul()
{
    for(int i = 0; i < capacity; i++)
        delete container[i], container[i] = nullptr;

}

template <typename Key, typename Value>
void HashMapa<Key, Value>::load(const HashMapa<Key,Value> &map)
{

    for(int i = 0; i < capacity; i++)
        if(map.container[i])
            container[i] = new std::pair<Key,Value>( map.container[i]->first, map.container[i]->second);
        else container[i] = nullptr;

}

template <typename Key, typename Value>
void HashMapa<Key, Value>::expand()                                              //redimenzionisanje
{

    std::pair<Key,Value>** temp = new std::pair<Key,Value>*[capacity + PART];

    for(int i = 0; i < capacity + PART; i++)
        if(i < capacity && container[i])
            temp[i] = new std::pair<Key,Value>(container[i]->first, container[i]->second);
        else
            temp[i] = nullptr;

    annul();
    delete [] container;

    capacity += PART;
    container = temp;
    temp = nullptr;

}

template <typename Key, typename Value>
HashMapa<Key, Value>::~HashMapa()
{
    annul();
    delete [] container;
}

template <typename Key, typename Value>
HashMapa<Key, Value>::HashMapa(const HashMapa<Key,Value> &map)
{

    container = new std::pair<Key,Value>*[map.capacity];
    size = map.size;
    capacity = map.capacity;
    functionPointer = map.functionPointer;

    load(map);
}

template <typename Key, typename Value>
HashMapa<Key,Value> &HashMapa<Key, Value>::operator= (const HashMapa<Key,Value> &map)
{

    if(this == &map) return *this;                                              // destruktivna samododjela
    annul();
    delete[] container;

    container = new std::pair<Key,Value>*[map.capacity];
    size = map.size;
    capacity = map.capacity;
    functionPointer = map.functionPointer;

    load(map);
    return *this;

}


template <typename Key, typename Value>
int HashMapa<Key, Value>::brojElemenata() const
{
    return size;
}

template <typename Key, typename Value>
void HashMapa<Key, Value>::obrisi()
{
    annul();
    size = 0;
}

template <typename Key, typename Value>
Value& HashMapa<Key,Value>::operator[] (const Key &key)
{

    auto check = seek(key);
    if(check != -1) return container[check] -> second;


    if(size >= capacity) expand();
    return container[insert(key,Value())] -> second;

}

template <typename Key, typename Value>
Value HashMapa<Key,Value>::operator[] (const Key &key) const
{

    auto check = seek(key);
    if(check != -1) return container[check] -> second;

    return Value();

}


template <typename Key, typename Value>
int HashMapa<Key,Value>::seek(const Key &key) const
{

    if(!functionPointer) throw "No function!";

    auto index = functionPointer(key,capacity);

    for(int i = index; i < capacity; i++) {

        if(!container[i]) return -1;

        if(container[i] -> first == key) return i;
    }

    for(int i = 0; i < index; i++) {
        if(!container[i]) return -1;

        if(container[i] -> first == key) return i;
    }

    return -1;

}

template <typename Key, typename Value>
int HashMapa<Key,Value>::insert (const Key &key, const Value &value)
{

    if(!functionPointer) throw "No function!";

    auto index = functionPointer(key,capacity);

    for(int i = index; i < capacity; i++)
        if(!container[i]) {
            container[i] = new std::pair<Key,Value>(key,value);
            size++;
            return i;
        }


    for(int i = 0; i < index; i++)
        if(!container[i]) {
            container[i] = new std::pair<Key,Value>(key,value);
            size++;
            return i;
        }

    return -1;

}

template <typename Key, typename Value>
void HashMapa<Key,Value>::obrisi(const Key &key)
{

    auto check = seek(key);
    if(check == -1) throw std::logic_error("Key not found!");

    delete container[check];
    container[check] = new Obrisan;
    size--;

}


template <typename Key, typename Value>
void HashMapa<Key,Value>::ispisi() const
{
    for(int i = 0; i < size; i++) {
        std::cout << std::endl << "Kljuc: " << container[i]-> first << " | Vrijednost: " << container[i]->second;
    }

    std::cout << std::endl << "-----------------------------------------------------------------------------------" << std::endl;
}

// NizMapa


template <typename Key, typename Value>
class NizMapa: public Mapa<Key, Value>
{
    std::pair<Key,Value>** container;
    int size, capacity;

    void expand();
    void annul();
    void load(const NizMapa<Key,Value> &map);

public:
    NizMapa(): Mapa<Key, Value>(), container(new std::pair<Key,Value>*[PART]), size(0), capacity(PART) {}
    NizMapa(const NizMapa<Key,Value> &map);
    NizMapa  &operator= (const NizMapa<Key,Value> &map);
    ~NizMapa();
    Value& operator[] (const Key &key);
    Value operator[] (const Key &key) const;
    int brojElemenata () const;
    void obrisi();
    void obrisi(const Key &key);

    void ispisi() const;

};

template <typename Key, typename Value>
void NizMapa<Key, Value>::annul()
{
    for(int i = 0; i < size; i++)
        delete container[i];

}

template <typename Key, typename Value>
void NizMapa<Key, Value>::load(const NizMapa<Key,Value> &map)
{

    for(int i = 0; i < size; i++)
        container[i] = new std::pair<Key,Value>( map.container[i]->first, map.container[i]->second);
}

template <typename Key, typename Value>
int NizMapa<Key, Value>::brojElemenata() const
{
    return size;
}

template <typename Key, typename Value>
NizMapa<Key, Value>::~NizMapa()
{
    annul();
    delete [] container;
}

template <typename Key, typename Value>
NizMapa<Key, Value>::NizMapa(const NizMapa<Key,Value> &map)
{

    container = new std::pair<Key,Value>*[map.capacity];
    size = map.size;
    capacity = map.capacity;

    load(map);
}

template <typename Key, typename Value>
void NizMapa<Key, Value>::expand()                                              //redimenzionisanje
{
    capacity += PART;
    std::pair<Key,Value>** temp = new std::pair<Key,Value>*[capacity];

    for(int i = 0; i < size; i++)
        temp[i] = new std::pair<Key,Value>(container[i]->first, container[i]->second);

    annul();
    delete [] container;
    container = temp;
    temp = nullptr;


}

template <typename Key, typename Value>
NizMapa<Key,Value> &NizMapa<Key, Value>::operator= (const NizMapa<Key,Value> &map)
{

    if(this == &map) return *this;                                              // destruktivna samododjela
    annul();
    delete[] container;

    container = new std::pair<Key,Value>*[map.capacity];
    size = map.size;
    capacity = map.capacity;

    load(map);
    return *this;

}

template <typename Key, typename Value>
void NizMapa<Key, Value>::obrisi()
{
    annul();
    size = 0;
}

template <typename Key, typename Value>
Value NizMapa<Key, Value>::operator[] (const Key &key) const
{
    for(int i = 0; i < size; i++)
        if(container[i]->first == key) return container[i]->second;

    return Value();

}

template <typename Key, typename Value>
Value& NizMapa<Key, Value>::operator[] (const Key &key)
{
    for(int i = 0; i < size; i++)
        if(container[i]->first == key) return container[i]->second;

    if(size >= capacity) expand();                                              //prosirujemo niz

    container[size++] = new std::pair<Key,Value>(key,Value());
    return container[size-1]->second;

}

template <typename Key, typename Value>
void NizMapa<Key, Value>::obrisi(const Key &key)
{
    for(int i = 0; i < size; i++) {
        if(container[i]->first == key) {
            delete container[i];
            container[i] = container[--size];
            container[size] = nullptr;
            return;
        }
    }
    throw std::logic_error("Mapa ne sadrzi trazeni kljuc");

}

template <typename Key, typename Value>
void NizMapa<Key,Value>::ispisi() const
{
    for(int i = 0; i < size; i++) {
        std::cout << std::endl << "Kljuc: " << container[i]-> first << " | Vrijednost: " << container[i]->second;
    }

    std::cout << std::endl << "-----------------------------------------------------------------------------------" << std::endl;
}


// BinStabloMapa

template <typename Key, typename Value>
class BinStabloMapa: public Mapa<Key, Value>
{
    struct Node {
        Key key;
        Value value;
        Node *right, *left, *parent;
        Node(Key key, Value value, Node* parent): key(key), value(value), right(nullptr), left(nullptr), parent(parent) {}
    };

    Node *root;
    int size;

    Node * search(const Key &k, Node * node) const
    {

        if(!node || k == node -> key) return node;
        if(k < node -> key) return search(k, node -> left);
        return search(k, node -> right);
    }


    Node * add (Node*& node, const Key &k, Node *parent)
    {
        if(!node) {
            node = new Node(k, Value(), parent);
            size++;
            return node;
        }

        else if (node -> key < k)
            return add(node -> right, k, node);

        return add(node -> left, k, node);

    }

    void annul(Node *root);
    void deepCopy(Node* &target, Node *from, Node *parent );


public:

    BinStabloMapa():Mapa<Key, Value>(), root(nullptr), size(0) {}
    BinStabloMapa(const BinStabloMapa<Key,Value> &map);
    BinStabloMapa &operator= (const BinStabloMapa &map);
    ~BinStabloMapa();
    Value& operator[] (const Key &key);
    Value operator[] (const Key &key) const;
    int brojElemenata () const;
    void obrisi();
    void obrisi(const Key &key);

};

template <typename Key, typename Value>
BinStabloMapa<Key,Value>::~BinStabloMapa()
{
    annul(root);
}

template <typename Key, typename Value>
void BinStabloMapa<Key,Value>::annul(Node *root)
{
    if(!root) return;
    annul(root -> left);
    annul(root -> right);
    delete root;
}

template <typename Key, typename Value>
int BinStabloMapa<Key,Value>::brojElemenata() const
{
    return size;
}


template <typename Key, typename Value>
void BinStabloMapa<Key, Value>::deepCopy(Node* &target, Node *from, Node *parent )
{

    if(!from) return;

    target = new Node(from -> key, from -> value, parent);
    deepCopy(target -> left, from -> left, target);
    deepCopy(target -> right, from -> right, target);

}



template <typename Key, typename Value>
BinStabloMapa<Key,Value>::BinStabloMapa(const BinStabloMapa<Key,Value> &map)

{

    deepCopy(root, map.root, nullptr);
    size = map.brojElemenata();

}

template <typename Key, typename Value>
BinStabloMapa<Key,Value> &BinStabloMapa<Key,Value>::operator =(const BinStabloMapa<Key,Value> &map)
{
    if(this == &map) return *this;

    annul(root);
    root = nullptr;
    size= map.size;
    deepCopy(root, map.root, nullptr);

    return *this;
}

template <typename Key, typename Value>
void BinStabloMapa<Key,Value>::obrisi()
{
    annul(root);
    root = nullptr;
    size = 0;

}

template <typename Key, typename Value>
void BinStabloMapa<Key,Value>::obrisi(const Key &key)
{
    Node *toDelete = search(key, root);
    if(!toDelete) throw std::logic_error("Ne postoji cvor sa trazenim kljucem!");

    if(!(toDelete->left) && !(toDelete -> right)) {

        if(toDelete == root)
            root = nullptr;

        else if(toDelete == toDelete->parent->left)
            toDelete->parent->left = nullptr;

        else
            toDelete -> parent -> right = nullptr;
        delete toDelete;

    } else if (toDelete-> left && toDelete -> right) {

        Node* temp = toDelete -> left;

        while(temp -> right)
            temp = temp -> right;

        temp -> right = toDelete -> right;

        if(toDelete -> right)
            toDelete -> right -> parent = temp;

        temp -> parent -> right = temp -> left;

        if(temp -> left)
            temp -> left -> parent = temp -> parent;

        if(toDelete == root) {
            root = temp;

        } else if(toDelete == toDelete -> parent -> left) {
            toDelete -> parent -> left = temp;
            temp-> parent = toDelete -> parent;
        } else {
            toDelete -> parent -> right = temp;
            temp -> parent = toDelete-> parent;
        }
        if(toDelete == root)
            temp -> left = nullptr;
        else {
            temp -> left = toDelete -> left;
            toDelete -> left-> parent = temp;
        }
        delete toDelete;
    } else {

        Node* child = toDelete -> left;

        if(!child)
            child = toDelete -> right;

        if(toDelete == root)
            root = child;

        else if(toDelete == toDelete-> parent -> left)
            toDelete -> parent -> left = child;

        else
            toDelete -> parent -> right = child;

        child -> parent = toDelete ->parent;
        delete toDelete;
    }

    size--;

}

template <typename Key, typename Value>
Value& BinStabloMapa<Key,Value>::operator[] (const Key &key)
{
    Node * result = search(key, root);
    if(result) return result -> value;
    auto temp =  add(root,key, nullptr);
    return temp -> value;

}

template <typename Key, typename Value>
Value BinStabloMapa<Key,Value>::operator[] (const Key &key) const
{
    Node * result = search(key, root);
    if(result) return result -> value;
    return Value();
}

unsigned int testHash(int ulaz, unsigned int max)
{

    return ((19 * 47) + ulaz) % max;

}

int main()
{
    BinStabloMapa <int, int> m;
    NizMapa<int,int> n;
    HashMapa<int, int> h;

    h.definisiHashFunkciju(testHash);



    for (int i = 0; i < 1000; i++)
        m[i] = i*i, n[i] = i*i, h[i] = i*i;

    /* clock_t time_1 = clock();
     m[1001];                                                                    // umetanje novog elementa u BinStabloMapa t ~ 218
     clock_t time_2 = clock();

     int total = (time_2 - time_1)/(CLOCKS_PER_SEC/100000);
     std::cout << total << std::endl; */

    /* clock_t time_1 = clock();
     n[1001];                                                                    // umetanje novog elementa u NizMapa t ~ 976
     clock_t time_2 = clock();

     int total = (time_2 - time_1)/(CLOCKS_PER_SEC/100000);

     std::cout << total << std::endl; */


    /* clock_t time_1 = clock();
      h[1001];                                                                    // umetanje novog elementa u HashMapa t ~ 512 , razlog je 'prosirivanje niza'
      clock_t time_2 = clock();                                                     // i problem kolizija

      int total = (time_2 - time_1)/(CLOCKS_PER_SEC/100000);

      std::cout << total << std::endl;*/

    /*clock_t time_1 = clock();
    n[500];                                                                       // pristup postojeceg elementa u NizMapa t ~ 77
    clock_t time_2 = clock();

    int total = (time_2 - time_1)/(CLOCKS_PER_SEC/100000);
    std::cout << total << std::endl;*/

    /* clock_t time_1 = clock();
     m[500];                                                                      // pristup postojeceg elementa u BinStabloMapa t ~ 106
     clock_t time_2 = clock();

     int total = (time_2 - time_1)/(CLOCKS_PER_SEC/100000);
     std::cout << total << std::endl; */

    /*  clock_t time_1 = clock();
      h[500];                                                                      // pristup postojeceg elementa u HashMapa t ~ 95
      clock_t time_2 = clock();

      int total = (time_2 - time_1)/(CLOCKS_PER_SEC/100000);
      std::cout << total << std::endl;*/

    /* clock_t time_1 = clock();
     n.obrisi(500);                                                                      // brisanje elementa iz NizMapa t ~ 344
     clock_t time_2 = clock();

     int total = (time_2 - time_1)/(CLOCKS_PER_SEC/100000);
     std::cout << total << std::endl;*/

    /* clock_t time_1 = clock();
     m.obrisi(500);                                                                      // brisanje elementa iz BinStabloMapa t ~ 262
     clock_t time_2 = clock();

     int total = (time_2 - time_1)/(CLOCKS_PER_SEC/100000);
     std::cout << total << std::endl;*/


    /* clock_t time_1 = clock();
     h.obrisi(500);                                                                      // brisanje elementa iz HashMapa t ~ 198
     clock_t time_2 = clock();

     int total = (time_2 - time_1)/(CLOCKS_PER_SEC/100000);
     std::cout << total << std::endl;*/



    return 0;

}
