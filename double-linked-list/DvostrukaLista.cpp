#include <iostream>
#include <stdexcept>
#include <string>

template <typename Tip>
class Iterator;

template <typename Tip>
class Lista
{


public:
    Lista () {};
    virtual ~Lista() {};
    virtual int brojElemenata () const = 0;
    virtual const Tip& trenutni() const = 0;
    virtual Tip& trenutni() = 0;
    virtual bool prethodni () = 0;
    virtual bool sljedeci () = 0;
    virtual Lista<Tip>& pocetak() = 0;
    virtual Lista<Tip>& kraj() = 0;
    virtual Lista<Tip>& obrisi() = 0;
    virtual Lista<Tip>& dodajIspred(const Tip& el) = 0;
    virtual Lista<Tip>& dodajIza(const Tip& el) = 0;

    virtual Tip& operator[] (int i) = 0;
    virtual Tip operator[](int i) const = 0;

};

template <typename Tip>
class DvostrukaLista: public Lista<Tip>
{
    struct Cvor {
        Cvor *next;
        Cvor *previous;
        Tip info;
        Cvor (const Tip &info, Cvor* previous, Cvor *next = 0):
                next(next), previous(previous), info (info)
        {
            if (previous) previous -> next = this;
            if (next) next -> previous = this;
        }
        explicit Cvor(Cvor* previous = 0, Cvor* next = 0): next(next), previous(previous)
        {
            if (previous) previous -> next = this;
            if (next)  next -> previous = this;
        }
    };

    Cvor *begin, *current, *end;
    int llength, rlength;

    void Iniciraj();
    void Unisti();
    void izuzetakPrazna() const;
    Cvor* dajPokNtog(int i) const;
    void Kopiraj(const DvostrukaLista &l);

public:

    DvostrukaLista ();
    DvostrukaLista(const DvostrukaLista &l);
    DvostrukaLista& operator= (const DvostrukaLista &l);
    ~DvostrukaLista();
    int brojElemenata () const;
    const Tip& trenutni() const;
    Tip& trenutni();
    bool prethodni ();
    bool sljedeci ();
    DvostrukaLista<Tip>& pocetak();
    DvostrukaLista<Tip>& kraj();
    DvostrukaLista<Tip>& obrisi();
    DvostrukaLista<Tip>& dodajIspred(const Tip& info);
    DvostrukaLista<Tip>& dodajIza(const Tip& info);

    Tip& operator[] (int i);
    Tip operator[](int i) const;

    friend class Iterator<Tip>;

};

template <typename Tip>
void DvostrukaLista<Tip>::Iniciraj()
{
    begin = current = new Cvor(0,0);
    begin -> next = end = new Cvor(begin,0);
    llength = rlength = 0;
}

template <typename Tip>
void DvostrukaLista<Tip>::Unisti()
{
    for(;;) {

        current = begin;
        begin = begin -> next;
        delete current;

        if(!begin) break;
    }

    current = end = 0;
}

template <typename Tip>
void DvostrukaLista<Tip>::izuzetakPrazna() const
{
    if (!brojElemenata()) throw "Prazna lista!";
}

template <typename Tip>
auto DvostrukaLista<Tip>::dajPokNtog(int i) const -> Cvor*
{
    if(i < 0 || i >= brojElemenata()) throw "Nedozvoljena pozicija!";

    Cvor *temp = nullptr;

    if (i < brojElemenata() / 2) {
        temp = begin;

        for (int j = 0;; j++) {
            temp = temp->next;
            if (j == i) break;

        }
    } else {
        temp = end;
        int x = brojElemenata() - 1;
        for (int j = x;; j--) {
            temp = temp->previous;
            if (j == i) break;
        }
    }
    return temp;

}

template <typename Tip>
void DvostrukaLista<Tip>::Kopiraj(const DvostrukaLista &l)
{

    Iniciraj();

    for (int i = 0; i < l.brojElemenata(); i++) {
        dodajIspred(l[i]);
        if(i == 0) current = current -> next;
    }

    pocetak();
    llength = l.llength;
    rlength = l.rlength;
}

template <typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(const DvostrukaLista &l)
{
    Kopiraj(l);
}

template <typename Tip>
DvostrukaLista<Tip>& DvostrukaLista<Tip>::operator= (const DvostrukaLista &l)
{
    if(this != &l) {
        Unisti();
        Kopiraj(l);
    }

    return *this;
}

template <typename Tip>
DvostrukaLista<Tip>::DvostrukaLista()
{
    Iniciraj();
}

template <typename Tip>
DvostrukaLista<Tip>::~DvostrukaLista()
{
    Unisti();
}

template <typename Tip>
const Tip& DvostrukaLista<Tip>::trenutni() const
{
    izuzetakPrazna();
    return current -> next -> info;
}

template <typename Tip>
Tip& DvostrukaLista<Tip>::trenutni()
{
    izuzetakPrazna();
    return current ->next -> info;
}

template <typename Tip>
int DvostrukaLista<Tip>::brojElemenata() const
{
    return llength + rlength;
}

template <typename Tip>
DvostrukaLista<Tip>& DvostrukaLista<Tip>::pocetak()
{
    izuzetakPrazna();
    current = begin;

    rlength = rlength + llength;
    llength = 0;

    return *this;
}

template <typename Tip>
DvostrukaLista<Tip>& DvostrukaLista<Tip>::kraj()
{
    izuzetakPrazna();
    if(brojElemenata() > 1)
        current = end -> previous -> previous;

    else current = end -> previous;
    llength = llength + rlength;
    rlength = 0;

    return *this;
}

template <typename Tip>
bool DvostrukaLista<Tip>::prethodni ()
{
    if(current != begin) {
        current = current -> previous;
        llength--;
        rlength++;
        return true;
    }
    return false;
}

template <typename Tip>
bool DvostrukaLista<Tip>:: sljedeci ()
{
    if(current->next && current->next->next != end) {
        current = current -> next;
        llength++;
        rlength--;
        return true;
    }
    return false;
}

template <typename Tip>
DvostrukaLista<Tip>& DvostrukaLista<Tip>::obrisi()
{
    izuzetakPrazna();
    Cvor* temp = nullptr;

    if(current -> next == end) {
        temp = current -> previous;
        temp -> next = end;
        end -> previous = temp;

        delete current;
        current = temp;

    } else {
        temp = current -> next;
        current -> next = temp -> next;
        temp -> next -> previous = current;

        delete temp;
    }

    rlength--;
    return *this;

}

template <typename Tip>
DvostrukaLista<Tip>& DvostrukaLista<Tip>::dodajIza(const Tip& info)
{
    if(brojElemenata() != 0)
        new Cvor(info, current -> next, current -> next -> next);

    else {
        if(current -> next == end) prethodni();
        {
            dodajIspred(info);
            llength--;                               //dekrementiramo jer se llength inkrementira pozivom dodajIspred()
        }
    }
    rlength++;

    return *this;

}

template <typename Tip>
DvostrukaLista<Tip>& DvostrukaLista<Tip>::dodajIspred(const Tip& info)
{
    new Cvor(info, current, current -> next);
    if(brojElemenata() != 0)
        current = current -> next;                  // samo ukoliko je lista prazna ne pomjeramo tekuci, kako bi ostao poziciju ispred

    llength++;

    return *this;

}

template <typename Tip>
Tip& DvostrukaLista<Tip>::operator[] (int i)
{
    return dajPokNtog(i) -> info;

}

template <typename Tip>
Tip DvostrukaLista<Tip>::operator[](int i) const
{
    return dajPokNtog(i) ->  info;
}


template <typename Tip>
class Iterator
{
    const DvostrukaLista<Tip>* list;
    typename DvostrukaLista<Tip>::Cvor* current;


public:
    Iterator(const DvostrukaLista<Tip> &list);
    Iterator(const Lista<Tip> &list);
    Tip& trenutni();
    bool prethodni();
    bool sljedeci();
    void pocetak();
    void kraj();
};

template <typename Tip>
Iterator<Tip>::Iterator(const DvostrukaLista<Tip> &list): list(list), current(list->begin->next) {}

template <typename Tip>
Iterator<Tip>::Iterator(const Lista<Tip> &list)
{
    this -> list = (const DvostrukaLista<Tip> *) &list;
    current = this -> list -> begin -> next;
}

template <typename Tip>
Tip& Iterator<Tip>::trenutni()
{
    list->izuzetakPrazna();
    return current -> info;
}

template <typename Tip>
bool Iterator<Tip>::prethodni()
{
    list->izuzetakPrazna();
    if(list -> begin == current) return false;

    current = current -> previous;
    return true;

}

template <typename Tip>
bool Iterator<Tip>::sljedeci()
{
    list->izuzetakPrazna();
    if(list -> end == current) return false;

    current = current -> next;
    return true;
}

template <typename Tip>
void Iterator<Tip>::pocetak()
{
    list->izuzetakPrazna();
    current = list -> begin;

}
template <typename Tip>
void Iterator<Tip>::kraj()
{
    list->izuzetakPrazna();
    current = list -> end;

}

template<typename Tip>
Tip dajMaksimum(const Lista<Tip>& n)
{
    Iterator<Tip> it(n);
    Tip maximum (it.trenutni());

    auto num = n.brojElemenata();
    for(int i = 0; i < num; i++) {
        if(it.trenutni() > maximum) maximum = it.trenutni();
        it.sljedeci();
    }

    return maximum;
}


// testovi

template <typename Tip>
void testBrojElemenata(DvostrukaLista<Tip> l)
{
    std::cout << l.brojElemenata() << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}
template<typename Tip>
void testTrenutni (DvostrukaLista<Tip> l)
{
    std::cout << "Trenutni element glasi: " << l.trenutni() << std::endl;
    l.trenutni() = l.trenutni() + 2;

    std::cout << "Trenutni mora biti uvecan za 2 a glasi: " << l.trenutni() << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

}
template<typename Tip>
void testPrethodniSljedeci(DvostrukaLista<Tip> l)
{
    std::cout << l.trenutni() << std::endl;
    l.sljedeci();
    l.sljedeci();
    l.prethodni();
    l.prethodni();
    std::cout << "Treba biti ispisan isti element kao prethodni! " << l.trenutni() << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}
template <typename Tip>
void testPocetakKraj(DvostrukaLista<Tip> l)
{

    for(int i = 0; i < l.brojElemenata(); i++)
        std::cout << l[i] << " ";

    std::cout << std::endl;
    l.pocetak();
    std::cout << "Ispisi se moraju poklapati..." << std::endl;;

    for(int i = 0; i < l.brojElemenata(); i++) {
        std::cout << l.trenutni() << " ";
        l.sljedeci();
    }


    std::cout << std::endl;
    l.kraj();
    std::cout << "Posljednji element je: " << l.trenutni() << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

}

template <typename Tip>
void testObrisi (DvostrukaLista<Tip> l)
{

    std::cout << "Originalna lista: " << std::endl;
    for(int i = 0; i < l.brojElemenata(); i++)
        std::cout << l[i] << " ";

    std::cout << std::endl << "Lista nakon brisanja 1. i posljednjeg elementa glasi :" << std::endl;

    l.pocetak();
    l.obrisi();
    l.kraj();
    l.obrisi();

    for(int i = 0; i < l.brojElemenata(); i++)
        std::cout << l[i] << " ";

    std::cout << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}

template <typename Tip>
void testDodajIspredDodajIza (DvostrukaLista<Tip> prazna)
{
    prazna.pocetak();
    auto x = prazna.brojElemenata();
    for(int i = 0; i < x; i++)
        prazna.obrisi();

    std::cout << "Punjenje sa DodajIspred" << std::endl;
    for(int i = 1; i <= 10; i++) prazna.dodajIspred(i*i);

    std::cout << "Ispis kvadrata: "<< std::endl;
    for(int i = 0; i < prazna.brojElemenata(); i++)
        std::cout << prazna[i] << " ";



    prazna.kraj();
    std::cout  << std::endl;
    for(int i = 1; i <= 10 ; i++)
        prazna.dodajIza(i*i);

    std::cout << std::endl;

    std::cout << "Ispis dodatnih kvadrata: "<< std::endl;
    for(int i = 0; i < prazna.brojElemenata(); i++)
        std::cout << prazna[i] << " ";

    std::cout << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}

template <typename Tip>
void testIndeksiranja(DvostrukaLista<Tip> l)
{
    std::cout << "Ispisana lista glasi: " << std::endl;
    for(int i = 0; i < l.brojElemenata(); i++)
        std::cout << l[i] << " ";

    std::cout << std::endl;
    l[3] = 7;
    l.pocetak();
    l.sljedeci();
    l.sljedeci();
    l.sljedeci();

    std::cout << "Cetvrti element treba sada da bude 7, a glasi: " << l.trenutni() << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}

void testDajMaksimum()
{
    DvostrukaLista<int> testna;
    for(int i = 0; i < 50; i++)
        testna.dodajIza(i);

    testna.dodajIza(777);

    for(int i = 1; i < 25; i++)
        testna.dodajIza(i*i);

    std::cout << "Lista glasi: " << std::endl;
    testna.pocetak();
    while (testna.sljedeci()) {
        std::cout << testna.trenutni() << " ";
    }
    std::cout << std::endl;

    std::cout << "Maksimalni element treba da bude 777, a glasi: " << dajMaksimum(testna) << std::endl;
}

int main()
{
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 10; i++)
        lista.dodajIspred(i);

    std::cout << "Broj elemenata mora biti 10 a gasi: " << std::endl;
    testBrojElemenata(lista);
    std::cout << std::endl;

    testDodajIspredDodajIza(lista);
    testIndeksiranja(lista);
    testObrisi(lista);
    testPocetakKraj(lista);

    lista.pocetak();
    lista.sljedeci();
    lista.sljedeci();
    testPrethodniSljedeci(lista);
    testTrenutni(lista);

    testDajMaksimum();

    return 0;
}
