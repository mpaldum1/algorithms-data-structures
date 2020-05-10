#include <iostream>

template <typename Tip>
class DvostraniRed
{
    struct Cvor {

        Tip info;
        Cvor *previous;
        Cvor *next;

        Cvor (const Tip &info, Cvor* previous, Cvor *next = 0):
                info(info), previous(previous), next(next)
        {
            if (previous) previous -> next = this;
            if (next) next -> previous = this;
        }
        explicit Cvor(Cvor* previous = 0, Cvor* next = 0): previous(previous), next(next)
        {
            if (previous) previous -> next = this;
            if (next)  next -> previous = this;
        }
    };

    Cvor* begin;
    Cvor* end;
    int size;

    void Unisti();
    void Kopiraj(const DvostraniRed &d);
    bool isEmpty();
    void izuzetakPrazan();

public:

    DvostraniRed(): begin(nullptr), end(nullptr), size(0) {}
    ~DvostraniRed();
    DvostraniRed(const DvostraniRed &d);
    DvostraniRed& operator= (const DvostraniRed &d);
    void brisi();
    int brojElemenata();

    void staviNaVrh(Tip info);
    Tip skiniSaVrha();
    void staviNaCelo(Tip info);
    Tip skiniSaCela();
    Tip& vrh();
    Tip &celo();

    void Ispisi() const;
};

template <typename Tip>
void DvostraniRed<Tip>::Unisti()
{
    while (!isEmpty()) skiniSaCela();
}

template <typename Tip>
void DvostraniRed<Tip>::Kopiraj(const DvostraniRed &d)
{
    Cvor *temp (d.begin);
    while (temp != nullptr) {
        staviNaVrh(temp->info);
        temp = temp -> next;
    }
}

template <typename Tip>
bool DvostraniRed<Tip>:: isEmpty()
{
    return brojElemenata() == 0;
}

template <typename Tip>
void DvostraniRed<Tip>::izuzetakPrazan()
{
    if(isEmpty()) throw "Kolekcija je prazna!";
}

template <typename Tip>
DvostraniRed<Tip>::~DvostraniRed()
{
    Unisti();
}

template <typename Tip>
DvostraniRed<Tip>::DvostraniRed(const DvostraniRed &d)
{
    begin = end = nullptr;
    size = 0;
    Kopiraj(d);
}

template <typename Tip>
DvostraniRed<Tip>& DvostraniRed<Tip>::operator= (const DvostraniRed &d)
{

    if(this != &d) {
        Unisti();
        Kopiraj(d);
    }
    return *this;
}

template <typename Tip>
void DvostraniRed<Tip>::brisi()
{
    Unisti();
}

template <typename Tip>
int DvostraniRed<Tip>::brojElemenata()
{
    return size;
}

template <typename Tip>
void DvostraniRed<Tip>::staviNaCelo(Tip element)
{

    Cvor* add (new Cvor(element,nullptr, nullptr));
    if(!begin) begin = end = add;
    else {
        begin -> previous = add;
        add -> next = begin;
        begin = add;
    }
    size++;
}


template <typename Tip>
void DvostraniRed<Tip>::staviNaVrh(Tip element)
{

    Cvor* add (new Cvor(element,nullptr,nullptr));
    if(!begin) begin = end = add;
    else {
        add -> previous = end;
        end -> next = add;
        end = end -> next;
    }
    size++;
}

template <typename Tip>
Tip DvostraniRed<Tip>::skiniSaCela()
{

    izuzetakPrazan();

    Tip info(begin -> info);
    Cvor *temp = begin;

    if(begin == end) begin = end = nullptr;
    else
        begin = begin -> next, begin -> previous = nullptr;
    delete temp;
    size--;
    return  info;
}

template <typename Tip>
Tip DvostraniRed<Tip>::skiniSaVrha()
{

    izuzetakPrazan();

    Tip info(end->info);
    Cvor *temp = end;

    if(begin == end) begin = end = nullptr;
    else end = end -> previous, end -> next = nullptr;

    delete temp;
    size--;

    return info;
}

template <typename Tip>
Tip& DvostraniRed<Tip>::vrh()
{
    izuzetakPrazan();
    return end->info;
}

template <typename Tip>
Tip& DvostraniRed<Tip>::celo()
{
    izuzetakPrazan();
    return begin -> info;
}

template <typename Tip>
void DvostraniRed<Tip>::Ispisi() const
{

    Cvor* point(begin);
    while(point) {
        std::cout << point -> info << " ";
        point = point -> next;
    }
    std::cout << std::endl;
}

//testovi


void testKonstruktorDodjela()
{
    DvostraniRed<char> prvi, drugi, treci;

    for(int i(0); i < 10; i++) prvi.staviNaVrh(i + 65);
    for(int i(0); i < 10; i++) treci.staviNaVrh('Z' - i);

    std::cout << "Sljedeci ispisi moraju biti isti!"<< std::endl;
    drugi = treci;
    drugi.Ispisi();
    treci = treci;
    treci.Ispisi();

    DvostraniRed<char> novi (prvi);

    std::cout << "Sljedeci ispisi moraju biti isti!"<< std::endl;
    novi.Ispisi();
    prvi.Ispisi();
    std::cout << "--------------------------------" << std::endl;

}

void testBrisi()
{
    DvostraniRed<int> testni;
    for(int i = 0; i < 10; i++)
        testni.staviNaVrh(i*i);

    std::cout << "Ispis deka prije brisanja" << std::endl;
    testni.Ispisi();

    testni.brisi();
    std::cout << "Ispis deka nakon brisanja" << std::endl;
    testni.Ispisi();
    std::cout << "--------------------------------" << std::endl;
}

void testStaviSkiniNaVrh()
{
    DvostraniRed<char> novi;
    for(int i = 0; i < 5; i++) novi.staviNaVrh(i+65);

    std::cout << "Ispis treba biti A B C D E, a glasi" << std::endl;
    novi.Ispisi();

    novi.skiniSaVrha();
    novi.skiniSaVrha();

    std::cout << "Ispis treba biti A B C, a glasi" << std::endl;
    novi.Ispisi();

    std::cout << "--------------------------------" << std::endl;
}

void testStaviSkiniNaCelo()
{
    DvostraniRed<char> novi;
    for(int i = 0; i < 5; i++) novi.staviNaCelo(i+65);

    std::cout << "Ispis treba biti E D C B A, a glasi" << std::endl;
    novi.Ispisi();

    novi.skiniSaCela();
    novi.skiniSaCela();

    std::cout << "Ispis treba biti C B A, a glasi" << std::endl;
    novi.Ispisi();

    std::cout << "--------------------------------" << std::endl;
}

void testBrojElemenata()
{
    DvostraniRed<double> novi;
    for(int i = 0; i < 7; i++)
        novi.staviNaCelo(i*i);

    std::cout << "Broj elemenata je 7, a metoda brojElemenata() daje " << novi.brojElemenata()<< std::endl;
    std::cout << "--------------------------------" << std::endl;
}
void testCelo()
{
    DvostraniRed<char> novi;
    for(int i = 0; i < 3; i++)
        novi.staviNaCelo(i+65);

    std::cout << "Dek glasi: " << std::endl;
    novi.Ispisi();
    std::cout << "Celo deka je " << novi.celo()<< std::endl;
    std::cout << "--------------------------------" << std::endl;

}

void testVrh()
{
    DvostraniRed<char> novi;
    for(int i = 0; i < 3; i++)
        novi.staviNaCelo(i+65);

    std::cout << "Dek glasi: " << std::endl;
    novi.Ispisi();
    std::cout << "Vrh deka je " << novi.vrh()<< std::endl;
    std::cout << "--------------------------------" << std::endl;

}



int main()
{

    testKonstruktorDodjela();
    testBrisi();
    testStaviSkiniNaVrh();
    testStaviSkiniNaCelo();
    testBrojElemenata();
    testCelo();
    testVrh();

    return 0;
}
