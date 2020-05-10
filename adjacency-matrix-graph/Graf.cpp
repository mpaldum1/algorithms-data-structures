#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

using namespace std;

template <typename TipOznake>
class Grana;
template <typename TipOznake>
class Cvor;
template <typename TipOznake>
class GranaIterator;
template <typename TipOznake>
class MatricaGraf;

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

    friend class MatricaGraf<TipOznake>;


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
class MatricaGraf: public UsmjereniGraf<TipOznake>
{
    std::vector<std::vector<Grana<TipOznake>*>> matrix;
    std::vector<TipOznake> vertex;



public:

    MatricaGraf(int nodeNumber);
    ~MatricaGraf() override;
    MatricaGraf(const MatricaGraf<TipOznake> &graph);
    MatricaGraf<TipOznake>& operator= (const MatricaGraf<TipOznake> &graph);

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



template <typename TipOznake>
MatricaGraf<TipOznake>::~MatricaGraf()
{
    for(int i = 0; i < matrix.size(); i++)
        for(int j = 0; j < matrix[i].size(); j++)
            if(matrix[i][j]) delete matrix[i][j];
}


template <typename TipOznake>
MatricaGraf<TipOznake>::MatricaGraf(int nodeNumber): UsmjereniGraf<TipOznake>(nodeNumber)
{

    matrix.resize(nodeNumber);
    for(int i = 0; i < nodeNumber; i++)
        matrix[i].resize(nodeNumber, nullptr);

    vertex.resize(nodeNumber);
}

template <typename TipOznake>
MatricaGraf<TipOznake>::MatricaGraf(const MatricaGraf<TipOznake> &graph)
{
    vertex = graph.vertex;
    matrix.resize(graph.matrix.size());

    for(int i = 0; i < matrix.size(); i++)
        matrix[i].resize(graph.matrix.size(),nullptr);                          //kvadratna

    for(int i = 0; i < graph.matrix.size(); i++)
        for(int j = 0; j < graph.matrix.size(); j++) {                          //kvadratna
            if(graph.matrix[i][j]) matrix[i][j] = new Grana<TipOznake>(graph.dajOznaku, graph.dajTezinu);
            else matrix[i][j] = nullptr;
        }

}

template <typename TipOznake>
MatricaGraf<TipOznake> &MatricaGraf<TipOznake>::operator= (const MatricaGraf<TipOznake> &graph)
{
    if(this == &graph) return*this;

    for(int i = 0; i < matrix.size(); i++)
        for(int j = 0; j < matrix.size(); j++)
            delete matrix[i][j];

    matrix = std::vector<std::vector<Grana<TipOznake>*>>(graph.matrix.size(), nullptr);

    for(int i = 0; i < matrix.size(); i++)
        for(int j = 0; j < matrix.size(); j++)
            if(graph.matrix[i][j]) matrix[i][j] = new Grana<TipOznake>(graph.dajOznaku, graph.dajTezinu);


    return *this;
}

template <typename TipOznake>
Grana<TipOznake> &MatricaGraf<TipOznake>::dajGranu(int izvor, int ponor)
{
    return *matrix[izvor][ponor];
}
template <typename TipOznake>
Cvor<TipOznake> MatricaGraf<TipOznake>::dajCvor(int cvor)
{
    return Cvor<TipOznake>(this, cvor);
}


template <typename TipOznake>
unsigned int MatricaGraf<TipOznake>::dajBrojCvorova() const
{
    return matrix.size();
}

template <typename TipOznake>
void MatricaGraf<TipOznake>::postaviBrojCvorova(int number)
{
    if(number < matrix.size()) throw std::logic_error("Cannot resize!");

    for(int i = 0; i < matrix.size(); i++)
        matrix[i].resize(number, nullptr);

    std::vector<Grana<TipOznake>*> empty(number, nullptr);
    matrix.resize(number, empty);
}

template <typename TipOznake>
void MatricaGraf<TipOznake>::obrisiGranu(int izvor, int ponor)
{
    delete matrix[izvor][ponor];
    matrix[izvor][ponor] = nullptr;
}

template <typename TipOznake>
void MatricaGraf<TipOznake>::dodajGranu(int izvor, int ponor, float tezina)
{
    matrix[izvor][ponor] = new Grana<TipOznake>(tezina, izvor, ponor, this);
}

template <typename TipOznake>
void MatricaGraf<TipOznake>::postaviTezinuGrane (int izvor, int ponor, float tezina)
{
    matrix[izvor][ponor] -> tezina = tezina;
}

template <typename TipOznake>
float MatricaGraf<TipOznake>::dajTezinuGrane (int izvor, int ponor) const
{
    return matrix[izvor][ponor] -> tezina;
}


template <typename TipOznake>
bool MatricaGraf<TipOznake>::postojiGrana (int izvor, int ponor) const
{
    return matrix[izvor][ponor];
}

template <typename TipOznake>
void MatricaGraf<TipOznake>::postaviOznakuCvora(unsigned int index, TipOznake label)
{
    vertex[index] = label;
}

template <typename TipOznake>
TipOznake MatricaGraf<TipOznake>::dajOznakuCvora(unsigned int index) const
{
    return vertex[index];
}

template <typename TipOznake>
void MatricaGraf<TipOznake>::postaviOznakuGrane(int izvor, int ponor, TipOznake label)
{
    matrix[izvor][ponor] -> oznaka = label;
}

template <typename TipOznake>
TipOznake MatricaGraf<TipOznake>::dajOznakuGrane(int izvor, int ponor) const
{
    return matrix[izvor][ponor] -> oznaka;
}

template <typename TipOznake>
GranaIterator<TipOznake> MatricaGraf<TipOznake>::sljedecaGrana(int izvor, int ponor)
{
    for(int i = izvor; i < matrix.size(); i++)
        for(int j = 0; j < matrix.size(); j++) {                                //kvadratna matrica je u pitanju
            if(i == izvor && j <= ponor) continue;

            if(matrix[i][j]) return GranaIterator<TipOznake>(this, i, j);
        }

    return GranaIterator<TipOznake>(this,-1,-1);
}

template <typename TipOznake>
GranaIterator<TipOznake> MatricaGraf<TipOznake>::dajGranePocetak()
{
    GranaIterator<TipOznake> iter(this,0,-1);
    return ++iter;
}

template <typename TipOznake>
GranaIterator<TipOznake> MatricaGraf<TipOznake>::dajGraneKraj()
{
    return GranaIterator<TipOznake> (this,-1,-1);;
}



template<typename TipOznake>
float Grana<TipOznake>::dajTezinu()
{
    return tezina;
}

template<typename TipOznake>
void Grana<TipOznake>::postaviTezinu(float tezina)
{
    graphPointer -> dajGranu(izvor, ponor).tezina = tezina;
    this -> tezina = tezina;
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
    graphPointer -> postaviOznakuGrane(izvor,ponor,oznaka);
}


template<typename TipOznake>
TipOznake Cvor<TipOznake>::dajOznaku()
{
    return dynamic_cast<MatricaGraf<TipOznake>*>(graphPointer) -> vertex[redniBroj];
}

template<typename TipOznake>
void Cvor<TipOznake>::postaviOznaku(TipOznake oznaka)
{
    dynamic_cast<MatricaGraf<TipOznake>*>(graphPointer) -> vertex[redniBroj] = oznaka;
}

template <typename TipOznake>
int Cvor<TipOznake>::dajRedniBroj()
{
    return redniBroj;
}

template <typename TipOznake>
Grana<TipOznake> GranaIterator<TipOznake>::operator*()
{
    return Grana<TipOznake>(dynamic_cast<MatricaGraf<TipOznake>*>(graph) -> matrix[izvor][ponor] -> dajTezinu(),izvor, ponor, graph);
}

template <typename TipOznake>
bool GranaIterator<TipOznake>::operator==(const GranaIterator &iter) const
{
    return graph == iter.graph && izvor == iter.izvor && ponor == iter.ponor;
}

template <typename TipOznake>
bool GranaIterator<TipOznake>::operator!=(const GranaIterator &iter) const
{
    return !(*this == iter);
}

template <typename TipOznake>
GranaIterator<TipOznake>& GranaIterator<TipOznake>::operator++()
{


    if(*this == graph -> dajGraneKraj()) throw std::logic_error("Posljednja grana u grafu!");

    auto temp = graph -> sljedecaGrana(izvor, ponor);
    izvor = temp.izvor;
    ponor = temp.ponor;

    return *this;
}

template <typename TipOznake>
GranaIterator<TipOznake> GranaIterator<TipOznake>::operator++(int)
{
    auto kopija (*this);
    if(*this == graph -> dajGraneKraj()) throw std::logic_error("Posljednja grana u grafu!");
    ++(*this);

    return kopija;
}

template <typename TipOznake>
void traziPoDubini(UsmjereniGraf<TipOznake>* graf, std::vector<Cvor<TipOznake>> &obilazak, Cvor<TipOznake> pocetni, std::vector<bool> &posjecen)
{

    posjecen[pocetni.dajRedniBroj()] = true;
    obilazak.emplace_back(pocetni);

    for(int i = 0; i < posjecen.size(); i++)
        if(graf -> postojiGrana(pocetni.dajRedniBroj(), i) && !posjecen[i])
            traziPoDubini(graf, obilazak, graf->dajCvor(i), posjecen);

}

template<typename TipOznake>
static void dfs(UsmjereniGraf<TipOznake>* graf,std::vector<Cvor<TipOznake>>&obilazak, Cvor<TipOznake> pocetni)
{
    std::vector<bool> posjecen(graf -> dajBrojCvorova(), false);
    for(int i = 0; i < posjecen.size(); i++)
        if(!posjecen[i]) traziPoDubini(graf, obilazak, pocetni, posjecen);
}

template <typename TipOznake>
void traziPoSirini(UsmjereniGraf<TipOznake>* graf, std::vector<Cvor<TipOznake>> &obilazak, Cvor<TipOznake> pocetni, std::vector<bool> &posjecen,
                   std::queue<Cvor<TipOznake>> &red)
{

    posjecen[pocetni.dajRedniBroj()] = true;
    obilazak.emplace_back(pocetni);
    red.push(pocetni);

    while(!red.empty()) {
        auto v = red.front();
        red.pop();
        for(int i = 0; i < posjecen.size(); i++)
            if(graf -> postojiGrana(v.dajRedniBroj(), i) && !posjecen[i]) {
                posjecen[i] = true;
                red.push(graf -> dajCvor(i));
                obilazak.emplace_back(graf -> dajCvor(i));


            }
    }

}

template<typename TipOznake>
static void bfs(UsmjereniGraf<TipOznake>* graf,std::vector<Cvor<TipOznake>>&obilazak, Cvor<TipOznake> pocetni)
{
    std::vector<bool> posjecen(graf -> dajBrojCvorova(), false);
    std::queue<Cvor<TipOznake>> red;
    for(int i = 0; i < posjecen.size(); i++)
        if(!posjecen[i]) traziPoSirini(graf, obilazak, pocetni, posjecen, red);
}

int main()
{

    /* UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 2, 1.2);
    g->dodajGranu(1, 3, 0.1);
    g->dodajGranu(3, 3, -4.0);
    g->dodajGranu(2, 4, 3.14);
    g->dodajGranu(2, 3, 2.73);
    g->dodajGranu(3, 5, 1);
    g->dodajGranu(5, 2, -5.0);
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 6; j++)
            cout << g->postojiGrana(i, j);
    delete g;

    std::cout << endl <<  "010000001100000110000101000000001000" << std::endl; */

    /* UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
      g->dodajGranu(0, 1, 2.5);
      g->dodajGranu(1, 2, 1.2);
      g->dodajGranu(1, 3, 0.1);
      g->dodajGranu(3, 3, -4.0);
      g->dodajGranu(2, 4, 3.14);
      g->dodajGranu(2, 3, 2.73);
      g->dodajGranu(3, 5, 1);
      g->dodajGranu(5, 2, -5.0);
      g->obrisiGranu(1,2);
      g->obrisiGranu(3,5);
      for (int i = 0; i < 6; i++)
        for (int j = 0; j < 6; j++)
          cout << g->postojiGrana(i, j);
      delete g;

      std::cout << endl <<  "010000000100000110000100000000001000" << std::endl;*/


    /*UsmjereniGraf<bool> *g = new MatricaGraf<bool>(4);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 2, 1.2);
    g->dodajGranu(1, 3, 0.1);
    g->dodajGranu(3, 3, -4.0);
    g->postaviTezinuGrane(1, 2, 0.2);
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        if (g->postojiGrana(i,j))
          cout << "(" << i << "," << j << ") -> " << g->dajTezinuGrane(i, j) << "; ";
    delete g;

    std::cout << endl <<  "(0,1) -> 2.5; (1,2) -> 0.2; (1,3) -> 0.1; (3,3) -> -4;" << std::endl; */

    /*  UsmjereniGraf<std::string> *g = new MatricaGraf<std::string>(4);
      g->dodajGranu(0, 1, 2.5);
      g->dodajGranu(1, 2, 1.2);
      g->dodajGranu(1, 3, 0.1);
      g->dodajGranu(3, 3, -4.0);
      g->postaviOznakuGrane(1, 3, "aa");
      for (int i = 0; i < 4; i++)
          for (int j = 0; j < 4; j++)
              if (g->postojiGrana(i,j))
                  cout << "(" << i << "," << j << ") -> '" << g->dajOznakuGrane(i, j) << "'; ";
      delete g;
      std::cout << endl <<  "(0,1) -> ''; (1,2) -> ''; (1,3) -> 'aa'; (3,3) -> '';" << std::endl; */

    /* UsmjereniGraf<std::string> *g = new MatricaGraf<std::string>(4);
     g->postaviOznakuCvora(0,"a");
     g->postaviOznakuCvora(1,"bcd");
     for (int i = 0; i < 4; i++)
         cout << "(" << i << ") -> '" << g->dajOznakuCvora(i) << "'; ";
     delete g;

     cout << endl << "(0) -> 'a'; (1) -> 'bcd'; (2) -> ''; (3) -> '';" << endl;
       return 0;*/

    /*  UsmjereniGraf<std::string> *g = new MatricaGraf<std::string>(4);
     cout << g->dajCvor(0).dajRedniBroj() << "; ";
     g->postaviOznakuCvora(1, "aaa");
     g->dajCvor(2).postaviOznaku("bbb");
     for (int i = 0; i < 4; i++)
         cout << "(" << i << ") -> '" << g->dajOznakuCvora(i) << "','" << g->dajCvor(i).dajOznaku() << "'; ";
     delete g;

     cout << endl << "0; (0) -> '',''; (1) -> 'aaa','aaa'; (2) -> 'bbb','bbb'; (3) -> '','';" << endl;*/

    /*UsmjereniGraf<std::string> *g = new MatricaGraf<std::string>(4);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 2, 1.2);
    g->dodajGranu(1, 3, 0.1);
    g->dodajGranu(3, 3, -4.0);
    g->postaviOznakuGrane(3, 3, "aa");
    g->dajGranu(0,1).postaviOznaku("bb");
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        if (g->postojiGrana(i,j))
          cout << "(" << i << "," << j << ") -> '" << g->dajOznakuGrane(i, j) << "','" << g->dajGranu(i, j).dajOznaku() << "'; ";
    delete g;

    cout << endl << "(0,1) -> 'bb','bb'; (1,2) -> '',''; (1,3) -> '',''; (3,3) -> 'aa','aa';" << endl;*/

    /*UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
      g->dodajGranu(0, 1, 2.5);
      g->dodajGranu(1, 0, 1.2);
      g->dodajGranu(1, 2, 0.1);
      g->dodajGranu(0, 0, 3.14);
      for (GranaIterator<bool> iter = g->dajGranePocetak();
           iter != g->dajGraneKraj(); ++iter)
        cout << "(" << (*iter).dajPolazniCvor().dajRedniBroj() << ","
             << (*iter).dajDolazniCvor().dajRedniBroj() << ") -> "
             << (*iter).dajTezinu() << "; ";
    delete g;

    cout << endl << "(0,0) -> 3.14; (0,1) -> 2.5; (1,0) -> 1.2; (1,2) -> 0.1;";
    cout << endl << "(0,1) -> 2.5; (0,0) -> 3.14; (1,0) -> 1.2; (1,2) -> 0.1;";
    */

    /*MatricaGraf<bool> g(6);
    g.dodajGranu(0, 1, 0);
    cout << g.postojiGrana(0,1);
    cout << g.postojiGrana(0,2); */

    /*UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
     g->dodajGranu(0, 1, 2.5);
     g->dodajGranu(1, 2, 1.2);
     g->dodajGranu(1, 3, 0.1);
     g->dodajGranu(3, 3, -4.0);
     g->dodajGranu(2, 4, 3.14);
     g->dodajGranu(2, 3, 2.73);
     g->dodajGranu(3, 5, 1);
     g->dodajGranu(5, 2, -5.0);
     for (int i = 0; i < 6; i++)
       for (int j = 0; j < 6; j++)
         cout << g->postojiGrana(i, j);
     delete g;

     cout << endl << "010000001100000110000101000000001000"; */

    /*UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
     g->dodajGranu(0, 1, 2.5);
     g->dodajGranu(1, 2, 1.2);
     g->dodajGranu(1, 3, 0.1);
     g->dodajGranu(3, 3, -4.0);
     g->dodajGranu(2, 4, 3.14);
     g->dodajGranu(2, 3, 2.73);
     g->dodajGranu(3, 5, 1);
     g->dodajGranu(5, 2, -5.0);
     g->obrisiGranu(1,2);
     g->obrisiGranu(3,5);
     for (int i = 0; i < 6; i++)
       for (int j = 0; j < 6; j++)
         cout << g->postojiGrana(i, j);
     delete g;

     cout << endl << "010000000100000110000100000000001000"; */

    /*UsmjereniGraf<bool> *g = new MatricaGraf<bool>(4);
     g->dodajGranu(0, 1, 2.5);
     g->dodajGranu(1, 2, 1.2);
     g->dodajGranu(1, 3, 0.1);
     g->dodajGranu(3, 3, -4.0);
     g->postaviTezinuGrane(1, 2, 0.2);
     for (int i = 0; i < 4; i++)
       for (int j = 0; j < 4; j++)
         if (g->postojiGrana(i,j))
           cout << "(" << i << "," << j << ") -> " << g->dajTezinuGrane(i, j) << "; ";
     delete g;

    cout << endl << "(0,1) -> 2.5; (1,2) -> 0.2; (1,3) -> 0.1; (3,3) -> -4;";*/

    /*UsmjereniGraf<std::string> *g = new MatricaGraf<std::string>(4);
      g->dodajGranu(0, 1, 2.5);
      g->dodajGranu(1, 2, 1.2);
      g->dodajGranu(1, 3, 0.1);
      g->dodajGranu(3, 3, -4.0);
      g->postaviOznakuGrane(1, 3, "aa");
      for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
          if (g->postojiGrana(i,j))
            cout << "(" << i << "," << j << ") -> '" << g->dajOznakuGrane(i, j) << "'; ";
      delete g;

      cout << endl << "(0,1) -> ''; (1,2) -> ''; (1,3) -> 'aa'; (3,3) -> '';"; */

    /*UsmjereniGraf<std::string> *g = new MatricaGraf<std::string>(4);
    g->postaviOznakuCvora(0,"a");
    g->postaviOznakuCvora(1,"bcd");
    for (int i = 0; i < 4; i++)
        cout << "(" << i << ") -> '" << g->dajOznakuCvora(i) << "'; ";
    delete g;

    cout << endl << "(0) -> 'a'; (1) -> 'bcd'; (2) -> ''; (3) -> '';";*/

    /*UsmjereniGraf<std::string> *g = new MatricaGraf<std::string>(4);
    cout << g->dajCvor(0).dajRedniBroj() << "; ";
    g->postaviOznakuCvora(1, "aaa");
    g->dajCvor(2).postaviOznaku("bbb");
    for (int i = 0; i < 4; i++)
        cout << "(" << i << ") -> '" << g->dajOznakuCvora(i) << "','" << g->dajCvor(i).dajOznaku() << "'; ";
    delete g;

    cout << endl << "0; (0) -> '',''; (1) -> 'aaa','aaa'; (2) -> 'bbb','bbb'; (3) -> '','';"; */

    /* UsmjereniGraf<std::string> *g = new MatricaGraf<std::string>(4);
     g->dodajGranu(0, 1, 2.5);
     g->dodajGranu(1, 2, 1.2);
     g->dodajGranu(1, 3, 0.1);
     g->dodajGranu(3, 3, -4.0);
     g->postaviOznakuGrane(3, 3, "aa");
     g->dajGranu(0,1).postaviOznaku("bb");
     for (int i = 0; i < 4; i++)
       for (int j = 0; j < 4; j++)
         if (g->postojiGrana(i,j))
           cout << "(" << i << "," << j << ") -> '" << g->dajOznakuGrane(i, j) << "','" << g->dajGranu(i, j).dajOznaku() << "'; ";
    delete g;

    cout << endl << "(0,1) -> 'bb','bb'; (1,2) -> '',''; (1,3) -> '',''; (3,3) -> 'aa','aa';"; */

    /* UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
        g->dodajGranu(0, 1, 2.5);
        g->dodajGranu(1, 0, 1.2);
        g->dodajGranu(1, 2, 0.1);
        g->dodajGranu(0, 0, 3.14);
        for (GranaIterator<bool> iter = g->dajGranePocetak();
             iter != g->dajGraneKraj(); ++iter)
          cout << "(" << (*iter).dajPolazniCvor().dajRedniBroj() << ","
               << (*iter).dajDolazniCvor().dajRedniBroj() << ") -> "
               << (*iter).dajTezinu() << "; ";
      delete g;

      cout << endl << "(0,0) -> 3.14; (0,1) -> 2.5; (1,0) -> 1.2; (1,2) -> 0.1;" ;
      cout << endl << "(0,1) -> 2.5; (0,0) -> 3.14; (1,0) -> 1.2; (1,2) -> 0.1;" ; */

    /*
    MatricaGraf<bool> g(6);
    g.dodajGranu(0, 1, 0);
    cout << g.postojiGrana(0,1);
    cout << g.postojiGrana(0,2);
      */


    /* UsmjereniGraf<std::string> *g = new MatricaGraf<std::string>(4);
     g->dodajGranu(0, 1, 2.5);
     g->dodajGranu(1, 2, 1.2);
     g->dodajGranu(1, 3, 0.1);
     g->dodajGranu(3, 3, -4.0);
     g->postaviOznakuGrane(3, 3, "aa");
     Grana<std::string> grana = g->dajGranu(0,1);
     grana.postaviOznaku("bb");
     grana.postaviTezinu(3.5);
     for (int i = 0; i < 4; i++)
       for (int j = 0; j < 4; j++)
         if (g->postojiGrana(i,j)) {
           cout << "(" << i << "," << j << ") -> '" << g->dajOznakuGrane(i, j) << "','" << g->dajGranu(i, j).dajOznaku();
           cout << "'," << g->dajTezinuGrane(i, j) << "; ";
       }
    delete g;

    cout << endl << "(0,1) -> 'bb','bb',3.5; (1,2) -> '','',1.2; (1,3) -> '','',0.1; (3,3) -> 'aa','aa',-4;";*/

    /* UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 2, 1.2);
    g->dodajGranu(1, 3, 0.1);
    g->dodajGranu(2, 4, 3.14);
    g->dodajGranu(2, 3, 2.73);
    g->dodajGranu(3, 5, 1);
    g->dodajGranu(5, 2, -5.0);
    std::vector<Cvor<bool> > dfs_obilazak;
    dfs(g, dfs_obilazak, g->dajCvor(0));
    for (int i = 0; i < dfs_obilazak.size(); i++)
        cout << dfs_obilazak[i].dajRedniBroj() << ",";
    delete g;

    std::cout << std::endl << "0,1,2,3,5,4," << endl;
     cout <<  	"0,1,2,4,3,5,"; */

    UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 2, 1.2);
    g->dodajGranu(1, 3, 0.1);
    g->dodajGranu(2, 4, 3.14);
    g->dodajGranu(2, 3, 2.73);
    g->dodajGranu(3, 5, 1);
    g->dodajGranu(5, 2, -5.0);
    std::vector<Cvor<bool> > bfs_obilazak;
    bfs(g, bfs_obilazak, g->dajCvor(0));
    for (int i = 0; i < bfs_obilazak.size(); i++)
        cout << bfs_obilazak[i].dajRedniBroj() << ",";
    delete g;

    cout << endl << "0,1,2,3,4,5,";

    return 0;
}
