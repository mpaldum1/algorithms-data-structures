#include <iostream>
#include <stdexcept>
#include <utility>
#include <string>
#include <iomanip>
#include <ctime>

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

//          BinStabloMapa

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


int main()
{

    BinStabloMapa <int, int> m;
    NizMapa<int,int> n;



    for (int i = 0; i < 1000; i++)
        m[i] = i*i, n[i] = i*i;

    /* clock_t time_1 = clock();
     m[1001];                                                                    // umetanje novog elementa u BinStabloMapa t ~ 96
     clock_t time_2 = clock();

     int total = (time_2 - time_1)/(CLOCKS_PER_SEC/100000);
     std::cout << total << std::endl; */

    /*  clock_t time_1 = clock();
      n[1001];                                                                    // umetanje novog elementa u NizMapa t ~ 626
      clock_t time_2 = clock();

      int total = (time_2 - time_1)/(CLOCKS_PER_SEC/100000);
      std::cout << total << std::endl; */

    clock_t time_1 = clock();
    n[999];                                                                    // pristup postojeceg elementa u NizMapa t ~ 125
    clock_t time_2 = clock();

    int total = (time_2 - time_1)/(CLOCKS_PER_SEC/100000);
    std::cout << total << std::endl;

    /*  clock_t time_1 = clock();
      m[999];                                                                    // pristup postojeceg elementa u BinStabloMapa t ~ 85
      clock_t time_2 = clock();

      int total = (time_2 - time_1)/(CLOCKS_PER_SEC/100000);
      std::cout << total << std::endl; */

    return 0;
}
