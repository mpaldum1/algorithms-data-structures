#include <iostream>
#include <stdexcept>

template <typename Tip>
class Stek
{
    struct Cvor {
        Tip info;
        Cvor *sljedeci;
        Cvor(const Tip &info, Cvor* sljedeci):
                info(info), sljedeci(sljedeci) {}
    };

    Cvor* Vrh;
    unsigned int velicina;
    void Kopiraj(const Stek &s);
    void izuzetakPrazan()

    {
        if(!Vrh) throw std::out_of_range("Prazan stek!");
    }

public:
    Stek (): Vrh(nullptr),velicina(0) {}
    Stek(const Stek &s)
    {
        Kopiraj(s);
    }
    Stek& operator= (const Stek &s);
    ~Stek()
    {
        brisi();
    }
    void brisi();
    void stavi(const Tip &el);
    Tip skini();
    Tip& vrh();
    unsigned int brojElemenata();
    void Ispisi();
};

template <typename Tip>
void Stek<Tip>::Kopiraj(const Stek &s)
{
    Cvor *q, *p(s.Vrh);
    Vrh = nullptr;
    velicina = s.velicina;

    while(p) {
        Cvor *n(new Cvor(p->info,nullptr));
        if(!Vrh) Vrh = n;
        else q->sljedeci = n;

        p = p -> sljedeci;
        q = n;
    }

}

template <typename Tip>
Stek<Tip>& Stek<Tip>::operator= (const Stek &s)
{
    if(&s == this) return *this; //samododjela
    brisi();
    Kopiraj(s);
    return *this;
}

template <typename Tip>
void Stek<Tip>::brisi()
{
    while(brojElemenata()) skini();
}

template <typename Tip>
void Stek<Tip>::stavi(const Tip &el)
{
    Vrh = new Cvor(el, Vrh);
    velicina++;
}
template <typename Tip>
Tip Stek<Tip>::skini()
{
    izuzetakPrazan();

    Tip temp(Vrh->info);
    Cvor *p = Vrh->sljedeci;

    delete Vrh;
    Vrh = p;
    velicina--;

    return temp;
}

template <typename Tip>
Tip& Stek<Tip>::vrh()
{
    izuzetakPrazan();

    return Vrh->info;
}

template <typename Tip>
unsigned int Stek<Tip>::brojElemenata()
{
    return velicina;
}

template <typename Tip>
void Stek<Tip>::Ispisi()
{

    auto p (Vrh);

    while (p) {
        std::cout << p->info;
        p = p -> sljedeci;
    }
    std::cout << std::endl;
}

void testKonstruktorDodjela()
{
    Stek<char> prvi, drugi, treci;

    for(int i(0); i < 10; i++) prvi.stavi(i + 65 );
    for(int i(0); i < 10; i++) treci.stavi('Z' - i);

    std::cout << "Sljedeci ispisi moraju biti isti!"<< std::endl;
    drugi = treci;
    drugi.Ispisi();
    treci = treci;
    treci.Ispisi();

    Stek<char> novi (prvi);

    std::cout << "Sljedeci ispisi moraju biti isti!"<< std::endl;
    novi.Ispisi();
    prvi.Ispisi();
    std::cout << "--------------------------------" << std::endl;

}

void testBrisi()
{
    Stek<int> testni;
    for(int i = 0; i < 10; i++)
        testni.stavi(i*i);

    std::cout << "Ispis steka prije brisanja" << std::endl;
    testni.Ispisi();

    testni.brisi();
    std::cout << "Ispis steka nakon brisanja" << std::endl;
    testni.Ispisi();
    std::cout << "--------------------------------" << std::endl;
}

void testStaviSkini()
{
    Stek<char> novi;
    for(int i = 0; i < 5; i++) novi.stavi(i+65);

    std::cout << "Ispisi treba biti EDCBA, a glasi" << std::endl;
    novi.Ispisi();

    novi.skini();
    novi.skini();

    std::cout << "Ispisi treba biti CBA, a glasi" << std::endl;
    novi.Ispisi();

    std::cout << "--------------------------------" << std::endl;
}

void testBrojElemenata()
{
    Stek<double> novi;
    for(int i = 0; i < 7; i++)
        novi.stavi(i*i);

    std::cout << "Broj elemenata je 7, a metoda brojElemenata() daje " << novi.brojElemenata()<< std::endl;
    std::cout << "--------------------------------" << std::endl;
}
void testVrh(){
    Stek<char> novi;
    for(int i = 0; i < 3; i++)
        novi.stavi(i+65);

    std::cout << "Vrh steka je C, a metoda vrh() daje vrijednost " << novi.vrh()<< std::endl;
    std::cout << "--------------------------------" << std::endl;

}

int main()
{
    testKonstruktorDodjela();
    testBrisi();
    testStaviSkini();
    testBrojElemenata();
    testVrh();

    return 0;
}
