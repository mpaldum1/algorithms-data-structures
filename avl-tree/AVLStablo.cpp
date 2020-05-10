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
class AVLStabloMapa: public Mapa<Key, Value>
{
    struct Node {
        Key key;
        Value value;
        int balance;
        Node *right, *left, *parent;
        Node(Key key, Value value, Node* parent): key(key), value(value), balance(0),right(nullptr), left(nullptr), parent(parent) {}
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
            auto temp = node;
            size++;
            azurirajBalans(node);
            return temp;
        }

        else if (node -> key < k)
            return add(node -> right, k, node);

        return add(node -> left, k, node);

    }

    void annul(Node *root);
    void deepCopy(Node* &target, Node *from, Node *parent );
    void azurirajBalans(Node* node);
    void rotateRight(Node *node);
    void rotateLeft(Node *node);


public:

    AVLStabloMapa():Mapa<Key, Value>(), root(nullptr), size(0) {}
    AVLStabloMapa(const AVLStabloMapa<Key,Value> &map);
    AVLStabloMapa &operator= (const AVLStabloMapa &map);
    ~AVLStabloMapa();
    Value& operator[] (const Key &key);
    Value operator[] (const Key &key) const;
    int brojElemenata () const;
    void obrisi();
    void obrisi(const Key &key);

};

template <typename Key, typename Value>
AVLStabloMapa<Key,Value>::~AVLStabloMapa()
{
    annul(root);
}

template <typename Key, typename Value>
void AVLStabloMapa<Key,Value>::annul(Node *root)
{
    if(!root) return;
    annul(root -> left);
    annul(root -> right);
    delete root;
}

template <typename Key, typename Value>
int AVLStabloMapa<Key,Value>::brojElemenata() const
{
    return size;
}


template <typename Key, typename Value>
void AVLStabloMapa<Key, Value>::deepCopy(Node* &target, Node *from, Node *parent )
{

    if(!from) return;

    target = new Node(from -> key, from -> value, parent);
    deepCopy(target -> left, from -> left, target);
    deepCopy(target -> right, from -> right, target);

}



template <typename Key, typename Value>
AVLStabloMapa<Key,Value>::AVLStabloMapa(const AVLStabloMapa<Key,Value> &map)

{

    deepCopy(root, map.root, nullptr);
    size = map.brojElemenata();

}

template <typename Key, typename Value>
AVLStabloMapa<Key,Value> &AVLStabloMapa<Key,Value>::operator =(const AVLStabloMapa<Key,Value> &map)
{
    if(this == &map) return *this;

    annul(root);
    root = nullptr;
    size= map.size;
    deepCopy(root, map.root, nullptr);

    return *this;
}

template <typename Key, typename Value>
void AVLStabloMapa<Key,Value>::obrisi()
{
    annul(root);
    root = nullptr;
    size = 0;

}

template <typename Key, typename Value>
void AVLStabloMapa<Key,Value>::obrisi(const Key &key)
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

        if(toDelete -> parent && toDelete -> parent -> left == toDelete) toDelete -> parent -> balance--;
        else if(toDelete -> parent) toDelete -> parent -> balance++;
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
        if(toDelete -> parent && toDelete -> parent -> left == toDelete) toDelete -> parent -> balance--;
        else if(toDelete -> parent) toDelete -> parent -> balance++;
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
        if(toDelete -> parent && toDelete -> parent -> left == toDelete) toDelete -> parent -> balance--;
        else if(toDelete -> parent) toDelete -> parent -> balance++;
        delete toDelete;
    }

    size--;

}

template <typename Key, typename Value>
Value& AVLStabloMapa<Key,Value>::operator[] (const Key &key)
{
    auto result = search(key, root);
    if(result) return result -> value;
    auto temp =  add(root,key, nullptr);
    return temp -> value;

}

template <typename Key, typename Value>
Value AVLStabloMapa<Key,Value>::operator[] (const Key &key) const
{
    Node * result = search(key, root);
    if(result) return result -> value;
    return Value();
}


template <typename Key, typename Value>
void AVLStabloMapa<Key,Value>::azurirajBalans(Node* node)
{
    if(!node || !(node -> parent)) return;

    if(!(node -> left) || !(node ->right) || node -> balance != 0)
        if(node == node -> parent -> left) node -> parent -> balance++;
        else node -> parent -> balance--;

    auto upper = node -> parent;

    if(upper -> balance < -1) {
        if(node -> balance > 0) rotateRight(node), rotateLeft(upper);

        if(node -> balance < 0) rotateLeft(node);
    }

    if(upper -> balance > 1) {
        if(node -> balance < 0) rotateLeft(node), rotateRight(upper);

        if(node -> balance > 0) rotateRight(node);
    }

    azurirajBalans(node -> parent);
}

//rotacije

template <typename Key,typename Value>
void AVLStabloMapa<Key,Value>::rotateLeft(Node *node)
{
    // Helper pointers

    Node *tmp1 = node -> right;
    if(!tmp1) return;
    Node *tmp2 = tmp1 -> right;

    auto upper = node -> parent;

    // Changing parent nodes

    tmp1 -> parent = node -> parent;
    node -> parent = tmp1;


    // Perform rotation

    tmp1 -> left = node;
    node -> right = tmp2;

}

template <typename Key,typename Value>
void AVLStabloMapa<Key,Value>::rotateRight(Node *node)
{

    // Helper pointers

    Node *tmp1 = node -> left;
    if(!tmp1) return;
    Node *tmp2 = tmp1 -> right;
    auto upper = node -> parent;

    // Changing parent nodes

    tmp1 -> parent = node -> parent;
    node -> parent = tmp1;


    // Perform rotation

    tmp1 -> right = node;
    node -> left = tmp2;
}


int main()
{
    return 0;
}
