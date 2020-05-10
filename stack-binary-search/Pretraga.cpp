#include <iostream>
#include <vector>
// klasa Stek je kopirana iz proslih zadataka

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

    bool isPrazan() const
    {
        return velicina == 0;
    }
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


void pretraga(Stek<std::vector<int>>& s, int trazeni, int brojac = -1)
{
    if(brojac == -1) brojac = s.brojElemenata();
    int search = - 1;

    if(!s.isPrazan() && !(s.vrh().size() == 0 || trazeni < s.vrh().at(0) ||             // problem prazne kolekcije i elementa van granica
                          trazeni > s.vrh().at(s.vrh().size() - 1))) {

        int top(s.vrh().size()), bot(0), mid;

        while(top >= bot) {
            mid = (top + bot)/2;                                                         //koristimo binarnu pretragu za pronalazak trazenog elementa
            if(s.vrh().at(mid) == trazeni) {
                search = mid;
                break;
            } else if(s.vrh().at(mid) < trazeni)
                bot = mid + 1;
            else
                top = mid - 1;
        }
    }

    if(brojac == 0 || (s.vrh().size() != 0 && trazeni > s.vrh().at(s.vrh().size() - 1))) { //optimizacija, dalje u steku su manji elementi pa prekidamo
        std::cout << "Nema elementa";
        return;
    }
    brojac--;

    if(search!= -1) {
        std::cout << search << " " << brojac;
    }

    else {
        if(!s.isPrazan()) {

            const std::vector<int> &temp = s.skini();                           // koristimo const referencu (a ne kolekciju) da sačuvamo informaciju
            pretraga(s, trazeni, brojac);
            s.stavi(temp);
        }
        return ;
    }
}

void testPretraga1()
{
    Stek<std::vector<int>> s;
    std::vector<int> prvi;
    for(int i = 0; i < 10; i++) prvi.emplace_back(i*i);
    std::vector<int> drugi;
    for(int i = 11; i < 22; i++) drugi.emplace_back(i*i);
    std::vector<int> treci;
    for(int i = 25; i < 40; i++) treci.emplace_back(i*i);

    s.stavi(prvi);
    s.stavi(drugi);
    s.stavi(treci);

    std::cout << "Treba ispisati 2 1:" << std::endl;
    pretraga(s, 169);

    std::cout << std::endl << "---------------------------------------------------------------" << std::endl;
}

void testPretraga2()
{
    Stek<std::vector<int>> s;
    std::vector<int> prvi;
    for(int i = 0; i < 10; i++) prvi.emplace_back(i*i);
    std::vector<int> drugi;
    for(int i = 11; i < 22; i++) drugi.emplace_back(i*i);
    std::vector<int> treci;
    for(int i = 25; i < 40; i++) treci.emplace_back(i*i);
    std::vector<int> cetvrti;
    for(int i = 50; i < 55; i++) cetvrti.emplace_back(i*i*i);

    s.stavi(prvi);
    s.stavi(drugi);
    s.stavi(treci);
    s.stavi(cetvrti);

    std::cout << "Test van granica:" << std::endl;
    pretraga(s, 10000000);

    std::cout << std::endl << "---------------------------------------------------------------" << std::endl;
}

void testPretraga3()
{
    Stek<std::vector<int>> s;

    std::vector<int> prvi;
    for(int i = 0; i < 10; i++) prvi.emplace_back(i);
    std::vector<int> drugi;
    for(int i = 11; i < 22; i++) drugi.emplace_back(i);
    std::vector<int> treci;
    for(int i = 25; i < 40; i++) treci.emplace_back(i);
    std::vector<int> cetvrti;
    for(int i = 50; i < 55; i++) cetvrti.emplace_back(i);
    std::vector<int> peti;
    for(int i = 60; i < 70; i++) peti.emplace_back(i);
    std::vector<int> sesti;
    for(int i = 80; i < 90; i++) sesti.emplace_back(i);

    s.stavi(prvi);
    s.stavi(drugi);
    s.stavi(treci);
    s.stavi(cetvrti);
    s.stavi(peti);
    s.stavi(sesti);

    std::cout << "Treba ispisati 3 3" << std::endl;
    pretraga(s, 53);


    std::cout << std::endl << "---------------------------------------------------------------" << std::endl;
}


int main()
{
    testPretraga1();
    testPretraga2();
    testPretraga3();
    return 0;
}
