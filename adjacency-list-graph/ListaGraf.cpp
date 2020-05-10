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



// ListaGraf

template <typename TipOznake>
class Grana;
template <typename TipOznake>
class Cvor;
template <typename TipOznake>
class GranaIterator;
template <typename TipOznake>
class ListaGraf;

template <typename TipOznake>
class UsmjereniGraf
{
public:
    UsmjereniGraf(int nodeNumber) {}
    virtual ~UsmjereniGraf() {}

    virtual unsigned int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova (int number) = 0;
    virtual void dodajGranu(int izvor, int ponor, float tezina) = 0;
    virtual void obrisiGranu(int izvor, int ponor) = 0;
    virtual void postaviTezinuGrane(int izvor, int ponor, float tezina) = 0;
    virtual float dajTezinuGrane(int izvor, int ponor) const = 0;
    virtual bool postojiGrana(int izvor, int ponor) const = 0;

    virtual  void postaviOznakuCvora(unsigned int index, TipOznake label) = 0;
    virtual  TipOznake dajOznakuCvora(unsigned int index) const = 0;
    virtual  void postaviOznakuGrane(int izvor, int ponor, TipOznake label) = 0;
    virtual  TipOznake dajOznakuGrane(int izvor, int ponor) const = 0;

    virtual Cvor<TipOznake> dajCvor(int cvor) = 0;
    virtual Grana<TipOznake> &dajGranu(int izvor, int ponor) = 0;

    virtual GranaIterator<TipOznake> dajGranePocetak()= 0;
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
    virtual GranaIterator<TipOznake> sljedecaGrana(int izvor, int ponor) = 0;

};

template <typename TipOznake>
class Grana
{
    float tezina;
    int izvor, ponor;
    TipOznake oznaka;
    UsmjereniGraf<TipOznake>* graphPointer;


public:

    Grana(float tezina, int izvor, int ponor, UsmjereniGraf<TipOznake>* graphPointer): tezina(tezina), izvor(izvor), ponor(ponor), graphPointer(graphPointer) {}
    Grana(float tezina, int izvor, int ponor): tezina(tezina), izvor(izvor), ponor(ponor), oznaka(TipOznake()), graphPointer(nullptr) {}
    Grana(float tezina, TipOznake tip): tezina(tezina), izvor(-1), ponor(-1), oznaka(tip), graphPointer(nullptr) {}
    Grana(UsmjereniGraf<TipOznake>* graph, int izvor, int ponor): tezina(0), izvor(izvor), ponor(ponor),
                                                                  oznaka(TipOznake()), graphPointer(graph) {}


    float dajTezinu();
    void postaviTezinu(float tezina);
    TipOznake dajOznaku();
    void postaviOznaku(TipOznake oznaka);
    Cvor<TipOznake> dajPolazniCvor() const
    {
        return graphPointer -> dajCvor(izvor);
    }
    Cvor<TipOznake> dajDolazniCvor() const
    {
        return graphPointer -> dajCvor(ponor);
    }

    friend class ListaGraf<TipOznake>;


};

template <typename TipOznake>
class Cvor
{

    int redniBroj;
    TipOznake oznaka;
    UsmjereniGraf<TipOznake>* graphPointer;

public:

    Cvor (UsmjereniGraf<TipOznake> *graph, int redniBroj): redniBroj(redniBroj), graphPointer(graph)
    {
        oznaka = graph -> dajOznakuCvora(redniBroj);
    }
    TipOznake dajOznaku();
    void postaviOznaku(TipOznake oznaka);
    int dajRedniBroj();

};


template <typename TipOznake>
class GranaIterator
{
    UsmjereniGraf<TipOznake> *graph;
    int izvor, ponor;

public:

    GranaIterator(UsmjereniGraf<TipOznake> *graph, int izvor, int ponor): graph(graph), izvor(izvor), ponor(ponor) {}
    Grana<TipOznake> operator*();
    bool operator==(const GranaIterator &iter) const;
    bool operator!=(const GranaIterator &iter) const;
    GranaIterator& operator++();
    GranaIterator operator++(int);


    friend class Grana<TipOznake>;
};



template <typename TipOznake>
class ListaGraf: public UsmjereniGraf<TipOznake>
{
    std::vector<std::deque<Grana<TipOznake>*>> adjList;
    std::vector<TipOznake> vertex;



public:

    ListaGraf(int nodeNumber);
    ~ListaGraf() override;
    ListaGraf(const ListaGraf<TipOznake> &graph);
    ListaGraf<TipOznake>& operator= (const ListaGraf<TipOznake> &graph);

    unsigned int dajBrojCvorova() const override;
    void postaviBrojCvorova (int number) override;
    void dodajGranu(int izvor, int ponor, float tezina) override;
    void obrisiGranu(int izvor, int ponor) override;
    void postaviTezinuGrane(int izvor, int ponor, float tezina) override;
    float dajTezinuGrane(int izvor, int ponor) const override;
    bool postojiGrana(int izvor, int ponor) const override;

    void postaviOznakuCvora(unsigned int index, TipOznake label) override;
    TipOznake dajOznakuCvora(unsigned int index) const override;
    void postaviOznakuGrane(int izvor, int ponor, TipOznake label) override;
    TipOznake dajOznakuGrane(int izvor, int ponor) const override;

    Cvor<TipOznake> dajCvor(int cvor) override;
    Grana<TipOznake> &dajGranu(int izvor, int ponor) override;

    GranaIterator<TipOznake> dajGranePocetak() override;
    GranaIterator<TipOznake> dajGraneKraj() override;
    GranaIterator<TipOznake> sljedecaGrana(int izvor, int ponor) override;

    friend class Grana<TipOznake>;
    friend class Cvor<TipOznake>;
    friend class GranaIterator<TipOznake>;


    static void dfs(UsmjereniGraf<TipOznake>* graph, std::vector<Cvor<TipOznake>> obilazak, Cvor<TipOznake> trenutni);
    static void bfs(UsmjereniGraf<TipOznake>* graph, std::vector<Cvor<TipOznake>> obilazak, Cvor<TipOznake> trenutni);
};


template<typename TipOznake>
ListaGraf<TipOznake>::~ListaGraf()
{
    for (int i = 0; i < adjList.size(); i++)
        for (int j = 0; j < adjList[i].size(); j++)
            if (adjList[i][j]) delete adjList[i][j];
}


template<typename TipOznake>
ListaGraf<TipOznake>::ListaGraf(int nodeNumber): UsmjereniGraf<TipOznake>(nodeNumber)
{

    adjList.resize(nodeNumber);
    vertex.resize(nodeNumber);
}

template<typename TipOznake>
ListaGraf<TipOznake>::ListaGraf(const ListaGraf<TipOznake> &graph)
{
    vertex = graph.vertex;
    adjList.resize(graph.adjList.size());

    for (int i = 0; i < graph.adjList.size(); i++)
        for (int j = 0; j < graph.adjList[i].size(); j++)
            adjList[i].emplace_back(new Grana<TipOznake>(graph.dajOznaku(), graph.adjList[i][j].dajTezinu));

}

template<typename TipOznake>
ListaGraf<TipOznake> &ListaGraf<TipOznake>::operator=(const ListaGraf<TipOznake> &graph)
{
    if (this == &graph) return *this;

    for (int i = 0; i < adjList.size(); i++)
        for (int j = 0; j < adjList[i].size(); j++)
            delete adjList[i][j];

    adjList = std::vector<std::vector<Grana<TipOznake> *>>(graph.adjList.size(), nullptr);

    for (int i = 0; i < adjList.size(); i++)
        for (int j = 0; j < adjList.size(); j++)
            adjList[i].emplace_back(new Grana<TipOznake>(graph.dajOznaku(), graph.adjList[i][j].dajTezinu));


    return *this;
}

template<typename TipOznake>
Grana<TipOznake> &ListaGraf<TipOznake>::dajGranu(int izvor, int ponor)
{

    for (int i = 0; i < adjList[izvor].size(); i++)
        if (adjList[izvor][i]->ponor == ponor)
            return *adjList[izvor][i];
}

template<typename TipOznake>
Cvor<TipOznake> ListaGraf<TipOznake>::dajCvor(int cvor)
{
    return Cvor<TipOznake>(this, cvor);
}


template<typename TipOznake>
unsigned int ListaGraf<TipOznake>::dajBrojCvorova() const
{
    return adjList.size();
}

template<typename TipOznake>
void ListaGraf<TipOznake>::postaviBrojCvorova(int number)
{
    if (number < adjList.size()) throw std::logic_error("Cannot resize!");

    vertex.resize(number);
    adjList.resize(number);

}

template<typename TipOznake>
void ListaGraf<TipOznake>::obrisiGranu(int izvor, int ponor)
{
    for (int i = 0; i < adjList[izvor].size(); i++)
        if (adjList[izvor][i]->ponor == ponor) {
            delete adjList[izvor][i];
            adjList[izvor].erase(adjList[izvor].begin() + i);
        }
}

template<typename TipOznake>
void ListaGraf<TipOznake>::dodajGranu(int izvor, int ponor, float tezina)
{
    adjList[izvor].emplace_back(new Grana<TipOznake>(tezina, izvor, ponor, this));
}

template<typename TipOznake>
void ListaGraf<TipOznake>::postaviTezinuGrane(int izvor, int ponor, float tezina)
{
    for (int i = 0; i < adjList[izvor].size(); i++)
        if (adjList[izvor][i]->ponor == ponor)
            adjList[izvor][i]->tezina = tezina;
}

template<typename TipOznake>
float ListaGraf<TipOznake>::dajTezinuGrane(int izvor, int ponor) const
{

    for (int i = 0; i < adjList[izvor].size(); i++)
        if (adjList[izvor][i]->ponor == ponor)
            return adjList[izvor][i]->tezina;;

    return -1;
}


template<typename TipOznake>
bool ListaGraf<TipOznake>::postojiGrana(int izvor, int ponor) const
{
    for (int i = 0; i < adjList[izvor].size(); i++)
        if (adjList[izvor][i]->ponor == ponor)
            return true;

    return false;
}

template<typename TipOznake>
void ListaGraf<TipOznake>::postaviOznakuCvora(unsigned int index, TipOznake label)
{
    vertex[index] = label;
}

template<typename TipOznake>
TipOznake ListaGraf<TipOznake>::dajOznakuCvora(unsigned int index) const
{
    return vertex[index];
}

template<typename TipOznake>
void ListaGraf<TipOznake>::postaviOznakuGrane(int izvor, int ponor, TipOznake label)
{
    for (int i = 0; i < adjList[izvor].size(); i++)
        if (adjList[izvor][i]->ponor == ponor)
            adjList[izvor][i]->oznaka = label;
}

template<typename TipOznake>
TipOznake ListaGraf<TipOznake>::dajOznakuGrane(int izvor, int ponor) const
{
    for (int i = 0; i < adjList[izvor].size(); i++)
        if (adjList[izvor][i]->ponor == ponor)
            return adjList[izvor][i]->oznaka;
}

template<typename TipOznake>
GranaIterator<TipOznake> ListaGraf<TipOznake>::sljedecaGrana(int izvor, int ponor)
{

    for (int i = 0; i < adjList[izvor].size(); i++)
        if (adjList[izvor][i]->ponor == ponor)
            for (int j = i + 1; j < adjList[izvor].size(); j++)
                return GranaIterator<TipOznake>(this, izvor, adjList[izvor][j]->ponor);


    for (int i = izvor + 1; i < adjList.size(); i++)
        if (adjList[i].size() != 0) return GranaIterator<TipOznake>(this, i, adjList[i][0]->ponor);


    return GranaIterator<TipOznake>(this, -1, -1);
}

template<typename TipOznake>
GranaIterator<TipOznake> ListaGraf<TipOznake>::dajGranePocetak()
{
    for (int i = 0; i < adjList.size(); i++)
        if (adjList[i].size() != 0)
            return GranaIterator<TipOznake>(this, i, adjList[i][0] -> ponor);

}

template<typename TipOznake>
GranaIterator<TipOznake> ListaGraf<TipOznake>::dajGraneKraj()
{
    return GranaIterator<TipOznake>(this, -1, -1);
}


template<typename TipOznake>
float Grana<TipOznake>::dajTezinu()
{
    return tezina;
}

template<typename TipOznake>
void Grana<TipOznake>::postaviTezinu(float tezina)
{
    graphPointer->dajGranu(izvor, ponor).tezina = tezina;
    this->tezina = tezina;
}

template<typename TipOznake>
TipOznake Grana<TipOznake>::dajOznaku()
{
    return oznaka;
}

template<typename TipOznake>
void Grana<TipOznake>::postaviOznaku(TipOznake oznaka)
{

    this->oznaka = oznaka;
    graphPointer->postaviOznakuGrane(izvor, ponor, oznaka);
}


template<typename TipOznake>
TipOznake Cvor<TipOznake>::dajOznaku()
{
    return dynamic_cast<ListaGraf<TipOznake> *>(graphPointer)->vertex[redniBroj];
}

template<typename TipOznake>
void Cvor<TipOznake>::postaviOznaku(TipOznake oznaka)
{
    dynamic_cast<ListaGraf<TipOznake> *>(graphPointer)->vertex[redniBroj] = oznaka;
}

template<typename TipOznake>
int Cvor<TipOznake>::dajRedniBroj()
{
    return redniBroj;
}

template<typename TipOznake>
Grana<TipOznake> GranaIterator<TipOznake>::operator*()
{
    return dynamic_cast<ListaGraf<TipOznake> *>(graph)->dajGranu(izvor, ponor);
}

template<typename TipOznake>
bool GranaIterator<TipOznake>::operator==(const GranaIterator &iter) const
{
    return graph == iter.graph && izvor == iter.izvor && ponor == iter.ponor;
}

template<typename TipOznake>
bool GranaIterator<TipOznake>::operator!=(const GranaIterator &iter) const
{
    return !(*this == iter);
}

template<typename TipOznake>
GranaIterator<TipOznake> &GranaIterator<TipOznake>::operator++()
{


    if (*this == graph->dajGraneKraj()) throw std::logic_error("Posljednja grana u grafu!");

    auto temp = graph->sljedecaGrana(izvor, ponor);
    izvor = temp.izvor;
    ponor = temp.ponor;

    return *this;
}

template<typename TipOznake>
GranaIterator<TipOznake> GranaIterator<TipOznake>::operator++(int)
{
    auto kopija(*this);
    if (*this == graph->dajGraneKraj()) throw std::logic_error("Posljednja grana u grafu!");
    ++(*this);

    return kopija;
}

template<typename TipOznake>
void traziPoDubini(UsmjereniGraf<TipOznake> *graf, std::vector<Cvor<TipOznake>> &obilazak, Cvor<TipOznake> pocetni,
                   std::vector<bool> &posjecen)
{

    posjecen[pocetni.dajRedniBroj()] = true;
    obilazak.emplace_back(pocetni);

    for (int i = 0; i < posjecen.size(); i++)
        if (graf->postojiGrana(pocetni.dajRedniBroj(), i) && !posjecen[i])
            traziPoDubini(graf, obilazak, graf->dajCvor(i), posjecen);

}

template<typename TipOznake>
static void dfs(UsmjereniGraf<TipOznake> *graf, std::vector<Cvor<TipOznake>> &obilazak, Cvor<TipOznake> pocetni)
{
    std::vector<bool> posjecen(graf->dajBrojCvorova(), false);
    for (int i = 0; i < posjecen.size(); i++)
        if (!posjecen[i]) traziPoDubini(graf, obilazak, pocetni, posjecen);
}

template<typename TipOznake>
void traziPoSirini(UsmjereniGraf<TipOznake> *graf, std::vector<Cvor<TipOznake>> &obilazak, Cvor<TipOznake> pocetni,
                   std::vector<bool> &posjecen,
                   std::queue<Cvor<TipOznake>> &red)
{

    posjecen[pocetni.dajRedniBroj()] = true;
    obilazak.emplace_back(pocetni);
    red.push(pocetni);

    while (!red.empty()) {
        auto v = red.front();
        red.pop();
        for (int i = 0; i < posjecen.size(); i++)
            if (graf->postojiGrana(v.dajRedniBroj(), i) && !posjecen[i]) {
                posjecen[i] = true;
                red.push(graf->dajCvor(i));
                obilazak.emplace_back(graf->dajCvor(i));


            }
    }

}

template<typename TipOznake>
static void bfs(UsmjereniGraf<TipOznake> *graf, std::vector<Cvor<TipOznake>> &obilazak, Cvor<TipOznake> pocetni)
{
    std::vector<bool> posjecen(graf->dajBrojCvorova(), false);
    std::queue<Cvor<TipOznake>> red;
    for (int i = 0; i < posjecen.size(); i++)
        if (!posjecen[i]) traziPoSirini(graf, obilazak, pocetni, posjecen, red);
}



int main()
{


    return 0;
}
