#include <iostream>


template<typename Tip>
class Red
{
    struct Cvor {
        Tip info;
        Cvor *sljedeci;

        Cvor(const Tip &info, Cvor *sljedeci) : info(info), sljedeci(sljedeci) {}
    };

    Cvor *pocetak;
    Cvor *kraj;
    int velicina;

    void Unisti();

    void Kopiraj(const Red<Tip> &q);

    bool jeLiPrazan() const
    {
        return pocetak == 0;
    }

public:
    Red() : pocetak(0), kraj(0), velicina(0) {}
    Red(const Red &r)
    {
        pocetak = kraj = nullptr;
        velicina = 0;
        Kopiraj(r);
    }
    ~Red()
    {
        Unisti();
    }
    Red &operator=(const Red &r);
    void brisi()
    {
        Unisti();
    }
    void stavi(const Tip &el);
    Tip skini();
    Tip &celo();
    int brojElemenata() const;

    void Ispisi() const;
};

template<typename Tip>
void Red<Tip>::Unisti()
{
    while (!jeLiPrazan()) skini();
}

template<typename Tip>
void Red<Tip>::Kopiraj(const Red<Tip> &q)
{


    Cvor *p(q.pocetak);
    while (p != 0) {
        stavi(p->info);
        p = p -> sljedeci;
    }
}

template<typename Tip>
Red<Tip> &Red<Tip>::operator=(const Red &r)
{

    if (&r != this) {
        Unisti();
        Kopiraj(r);
    }
    return *this;
}
template<typename Tip>
void Red<Tip>::stavi(const Tip &el)
{
    Cvor*n (new Cvor(el,0));
    if(pocetak == 0) pocetak = kraj = n;
    else {
        kraj->sljedeci = n;
        kraj = kraj->sljedeci;
    }
    velicina++;
}

template<typename Tip>
Tip Red<Tip>::skini()
{
    if(!pocetak) throw "Prazan red!";
    Tip info(pocetak -> info);
    Cvor * p = pocetak;
    if(pocetak == kraj) pocetak = kraj = 0;
    else
        pocetak = pocetak -> sljedeci;
    delete p;
    velicina--;
    return  info;
}
template<typename Tip>
Tip &Red<Tip>::celo()
{
    if(!pocetak) throw "Prazan red!";
    return pocetak->info;
}
template<typename Tip>
int Red<Tip>::brojElemenata() const
{
    return  velicina;
}

template <typename Tip>
void Red<Tip>::Ispisi() const
{
    Cvor* p(pocetak);
    while(p) {
        std::cout << p -> info << " ";
        p = p -> sljedeci;
    }
    std::cout << std::endl;
}


void testKonstruktorDodjela()
{
    Red<char> prvi, drugi, treci;

    for(int i(0); i < 10; i++) prvi.stavi(i + 65 );
    for(int i(0); i < 10; i++) treci.stavi('Z' - i);

    std::cout << "Sljedeci ispisi moraju biti isti!"<< std::endl;
    drugi = treci;
    drugi.Ispisi();
    treci = treci;
    treci.Ispisi();

    Red<char> novi (prvi);

    std::cout << "Sljedeci ispisi moraju biti isti!"<< std::endl;
    novi.Ispisi();
    prvi.Ispisi();
    std::cout << "--------------------------------" << std::endl;

}

void testBrisi()
{
    Red<int> testni;
    for(int i = 0; i < 10; i++)
        testni.stavi(i*i);

    std::cout << "Ispis reda prije brisanja" << std::endl;
    testni.Ispisi();

    testni.brisi();
    std::cout << "Ispis reda nakon brisanja" << std::endl;
    testni.Ispisi();
    std::cout << "--------------------------------" << std::endl;
}

void testStaviSkini()
{
    Red<char> novi;
    for(int i = 0; i < 5; i++) novi.stavi(i+65);

    std::cout << "Ispis treba biti A B C D E, a glasi" << std::endl;
    novi.Ispisi();

    novi.skini();
    novi.skini();

    std::cout << "Ispis treba biti C D E, a glasi" << std::endl;
    novi.Ispisi();

    std::cout << "--------------------------------" << std::endl;
}

void testBrojElemenata()
{
    Red<double> novi;
    for(int i = 0; i < 7; i++)
        novi.stavi(i*i);

    std::cout << "Broj elemenata je 7, a metoda brojElemenata() daje " << novi.brojElemenata()<< std::endl;
    std::cout << "--------------------------------" << std::endl;
}
void testCelo()
{
    Red<char> novi;
    for(int i = 0; i < 3; i++)
        novi.stavi(i+65);

    std::cout << "Celo reda je A, a metoda celo() daje vrijednost " << novi.celo()<< std::endl;
    std::cout << "--------------------------------" << std::endl;

}


int main()
{

    testKonstruktorDodjela();
    testBrisi();
    testStaviSkini();
    testBrojElemenata();
    testCelo();

    return 0;
}
