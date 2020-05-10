#include <iostream>
#include <stdexcept>

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

template <typename TipNiz>
class NizLista: public Lista<TipNiz>
{
    TipNiz** L;
    int  duzina, tekuci, kapacitet;
    void dozvoljenIndex (int i) const
    {
        if(!(i >= 0 && i < duzina))
            throw std::domain_error("Nedozvoljen indeks!");
    }
    void praznaIzuzetak() const
    {
        if(duzina == 0) throw std::domain_error("Lista je prazna!");
    }
public:

    NizLista(): L(new TipNiz*[10]), duzina(0), tekuci(0), kapacitet(10) {}
    NizLista (const NizLista &l)
    {
        duzina = l.duzina;
        tekuci = l.tekuci;
        kapacitet = l.kapacitet;
        L = new TipNiz*[kapacitet];
        for(int i = 0; i < duzina; i++)
            L[i] = new TipNiz (*(l.L[i]));                                          //osiguravamo duboku kopiju
    }
    NizLista& operator= (const NizLista<TipNiz>& l)
    {

        kapacitet = l.kapacitet;
        tekuci = l.tekuci;
        for(int i = 0; i < duzina; i++) delete L[i];                                //brisanje starog niza, izbjegavanje curenja memorije
        delete [] L;

        L = new TipNiz*[kapacitet];
        for(int i = 0; i < l.duzina; i++)
            L[i] = new TipNiz (*(l.L[i]));

        duzina = l.duzina;

        return *this;
    }
    NizLista (NizLista &&r):duzina(r.duzina),kapacitet(r.kapacitet),tekuci(r.tekuci)
    {
        L = r.L;
    }
    NizLista& operator =(NizLista &&r)
    {
        duzina = r.duzina;
        kapacitet = r.kapacitet;
        tekuci = r.tekuci;
        std::swap(L, r.L);
    }

    ~NizLista() override
    {
        for(int i = 0; i < duzina; i++) delete L[i];
        delete[] L;
    }
    int brojElemenata() const override
    {
        return duzina;

    }
    const TipNiz& trenutni() const override
    {
        praznaIzuzetak();
        return *L[tekuci];
    }
    TipNiz &trenutni() override
    {
        praznaIzuzetak();
        return *L[tekuci];
    }

    bool prethodni() override
    {
        praznaIzuzetak();
        if(tekuci == 0) return false;
        tekuci--;
        return true;
    }
    bool sljedeci() override
    {
        praznaIzuzetak();

        if(tekuci < duzina - 1) {
            tekuci++;
            return true;
        }
        return false;
    }

    NizLista<TipNiz>& pocetak() override
    {
        praznaIzuzetak();
        tekuci = 0;
        return *this;
    }
    NizLista<TipNiz>& kraj() override
    {
        praznaIzuzetak();
        tekuci = duzina - 1;
        return *this;
    }
    NizLista<TipNiz>& obrisi() override
    {
        praznaIzuzetak();

        if(tekuci != duzina) {
            delete L[tekuci];
            for(int i = tekuci; i + 1< duzina; i++)
                L[i] = L[i + 1 ];

            if(tekuci == duzina - 1) tekuci--;
            duzina--;
        }
        if(duzina == 0) tekuci = 0;
        return *this;
    }

    NizLista<TipNiz>& dodajIspred(const TipNiz& el) override
    {
        if(kapacitet <= duzina) {
            kapacitet+= 100;                                                    //probijanjem kapaciteta realociramo memoriju prostor za 100 novih el
            // broj 100 je proizvoljan i s ciljem izbjegavanja ceste realokacije
            auto pok = L;
            L = new TipNiz*[kapacitet];
            for(int i = 0; i <= tekuci - 1; i++)
                L[i] = pok[i];
            L[tekuci++] = new TipNiz(el);
            for(int i = tekuci - 1; i < duzina; i++)
                L[i + 1] = pok[i];

            delete[] pok;
            duzina++;

        } else {
            if(duzina == 0) {

                L[duzina++] = new TipNiz(el);
            } else {
                for(int i = duzina; i > tekuci; i--)
                    L[i] = L[i - 1];
                L[tekuci++] = new TipNiz(el);
                duzina++;
            }
        }
        return *this;
    }

    NizLista<TipNiz>& dodajIza(const TipNiz& el) override
    {
        if(kapacitet <= duzina) {
            kapacitet+= 100;
            TipNiz **pok = new TipNiz*[kapacitet];
            for(int i = 0; i <= tekuci; i++)
                pok[i] = L[i];
            pok[++tekuci] =  new TipNiz (el);
            for(int i = tekuci; i < duzina; i++)
                pok[i + 1] = L[i];
            L = pok;
            duzina++;

        } else {
            if(duzina == 0)
                L[duzina++] = new TipNiz(el);
            else {
                for(int i = duzina; i > tekuci + 1; i--)
                    L[i] = L[i - 1];

                L[tekuci + 1] = new TipNiz(el);
                duzina++;
            }
        }
        return *this;
    }

    TipNiz& operator[] (int i) override
    {
        dozvoljenIndex(i);
        return **(L + i);
    }
    TipNiz operator[](int i) const override
    {
        dozvoljenIndex(i);
        return **(L + i);
    }
};

template <typename Tip>
class JednostrukaLista :public Lista<Tip>
{
private:
    struct Cvor {
        Tip element;
        Cvor* next;
        Cvor(const Tip &element, Cvor* next): element(element), next(next) {}
    };
    int lduzina, dduzina;
    Cvor *p;
    Cvor *k;
    Cvor *tekuci;

    void praznaIzuzetak() const
    {
        if(brojElemenata() == 0) throw std::domain_error("Lista je prazna!");
    }

public:
    JednostrukaLista(): lduzina(0), dduzina(0), p(nullptr), k(nullptr), tekuci(nullptr)  {}
    JednostrukaLista(const JednostrukaLista& l): lduzina(l.lduzina), dduzina(l.dduzina)
    {

        p = new Cvor(l.p->element,0);

        Cvor *prolaz = p;
        for(auto pok = l.p; pok != l.k; pok = pok->next) {
            prolaz->next = new Cvor(pok->element, nullptr);
            prolaz = prolaz -> next;
            if (pok == l.tekuci) tekuci = prolaz;
        }
        k = prolaz;
    }
    JednostrukaLista& operator = (const JednostrukaLista &l)
    {
        Cvor* prvi;

        if(p)
            prvi = p -> next;
        else prvi = nullptr;                                                                    //oslobađanje stare memorije

        for(auto pok = p; prvi != nullptr; prvi = prvi->next) {
            delete pok;
            pok = prvi;

        }
        delete k;

        p = tekuci = k = nullptr;

        Cvor* novi = new Cvor(l.p->element, nullptr);
        p = novi;

        for(auto pok = l.p;; pok = pok->next) {

            if(pok-> next == nullptr) break;
            novi -> next = new Cvor(pok->next->element, nullptr);
            novi = novi -> next;
        }

        k = novi;

        dduzina = l.dduzina;
        lduzina = l.lduzina;

        return *this;
    }

    JednostrukaLista (JednostrukaLista &&r):dduzina(r.dduzina), lduzina(r.lduzina)
    {
        std::swap(p, r.p);
        std::swap(k,r.k);
        std::swap(tekuci, r.tekuci);
    }
    JednostrukaLista& operator =(JednostrukaLista &&r)
    {
        dduzina = r.dduzina;
        lduzina =r.lduzina;
        std::swap(p, r.p);
        std::swap(k,r.k);
        std::swap(tekuci, r.tekuci);
    }

    ~JednostrukaLista() override
    {
        Cvor* prvi;

        if(p)
            prvi = p -> next;
        else prvi = nullptr;

        for(auto pok = p; prvi != nullptr; prvi = prvi->next) {
            delete pok;
            pok = prvi;

        }
        delete k;
        p = tekuci = k = nullptr;

    }
    int brojElemenata() const override
    {
        return lduzina + dduzina;
    }
    const Tip &trenutni() const override
    {
        praznaIzuzetak();
        if(!tekuci) return p->element;
        else if(tekuci == k) return k->element;
        else return tekuci->next->element;

    }
    Tip &trenutni() override
    {
        praznaIzuzetak();
        if(!tekuci) return p->element;
        else if(tekuci == k) return k->element;
        else return tekuci->next->element;

    }
    bool prethodni() override
    {
        praznaIzuzetak();

        if(lduzina == 0) return false;
        else {
            if(lduzina == 1) tekuci = nullptr;
            else {
                Cvor *pok(p);
                while (pok->next != tekuci) pok = pok -> next;
                tekuci = pok;
            }
            lduzina--;
            dduzina++;
            return true;
        }
    }

    bool sljedeci() override
    {
        praznaIzuzetak();
        if (dduzina == 0 || (tekuci && tekuci->next == k)) return false;

        else {
            if (lduzina == 0) tekuci = p;
            else
                tekuci = tekuci->next;
            lduzina++;
            dduzina--;
            return true;
        }
    }

    JednostrukaLista<Tip>& pocetak() override
    {
        praznaIzuzetak();

        tekuci = nullptr;
        dduzina+= lduzina;
        lduzina = 0;
        return *this;
    }

    JednostrukaLista<Tip>& kraj() override
    {
        praznaIzuzetak();
        Cvor* before;
        if(k == p) {
            before = nullptr;

        } else
            for(before = p; before->next != k; before = before-> next);

        tekuci = before;
        lduzina+= dduzina;
        dduzina = 0;
        return *this;
    }

    JednostrukaLista<Tip>& obrisi() override
    {
        praznaIzuzetak();
        if(!tekuci) tekuci = p;

        if (brojElemenata() == 1) {
            delete p;
            lduzina = dduzina = 0;
            p = k = nullptr;
        }

        else if( tekuci -> next != nullptr && tekuci -> next != k) {
            auto temp = tekuci-> next -> next;
            if(tekuci->next == p) {
                p = temp;
            }
            if(tekuci->next == k) k = temp;
            delete tekuci->next;
            tekuci -> next = temp;
            dduzina--;

        } else {
            Cvor* before;
            if(tekuci == p) {
                before = p;

            } else
                for(before = p; before->next != tekuci; before = before-> next);                //pronanalaženje prethodnog elementa
            Cvor*temp;

            if(tekuci->next == nullptr) temp = nullptr ;
            else temp = tekuci-> next -> next;
            delete tekuci->next;
            k = tekuci;
            k->next = temp;
            tekuci  = before;

            lduzina--;
        }

        if(brojElemenata() == 0) p = k = tekuci = nullptr;
        return *this;
    }

    Lista<Tip>& dodajIspred(const Tip& el) override
    {

        auto pok = new Cvor(el, 0);

        if(lduzina == 0) {
            pok->next = p;
            p = pok;

            if(lduzina + dduzina == 0)
                k   = pok;
            lduzina++;
        } else {

            if(tekuci) {
                pok->next = tekuci->next;
                tekuci -> next = pok;
                tekuci = tekuci->next;
                lduzina++;
            } else {                                                            //specijalni slučaj ukoliko je lista prazna
                pok->next = p;
                p = pok;
                tekuci = pok;
                dduzina++;
            }
        }

        return *this;
    }

    Lista<Tip>& dodajIza(const Tip& el) override
    {

        auto pok = new Cvor(el, 0);

        if(lduzina == 0) {
            if(brojElemenata() == 0) p = k = pok;
            else {
                pok->next = p->next;
                p -> next = pok;
                if(dduzina == 1) k = pok;
            }

        } else {
            if(tekuci != k &&  k != p) {
                pok->next = tekuci->next->next;
                tekuci->next -> next = pok;
                if(!pok->next) k = pok;
            } else {
                pok-> next = tekuci-> next;
                tekuci->next = pok;
                k = pok;
            }
        }
        dduzina++;
        return *this;
    }
    Tip& operator[] (int i) override
    {
        Cvor *pok = nullptr;
        for (int j = 0; j <= i; j++) {
            if (!pok) pok = p;
            else pok = pok->next;
        }
        return pok->element;

    }
    Tip operator[](int i) const override
    {
        Cvor *pok = nullptr;
        for (int j = 0; j <= i; j++) {
            if (!pok) pok = p;
            else pok = pok->next;
        }
        return pok->element;
    }
};

template <typename Tip>
void testBrojElemenata1(NizLista<Tip> l)
{
    std::cout << l.brojElemenata() << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}
template<typename Tip>
void testTrenutni1 (NizLista<Tip> l)
{
    std::cout << "Trenutni element glasi: " << l.trenutni() << std::endl;
    l.trenutni() = l.trenutni() + 2;

    std::cout << "Trenutni mora biti uvecan za 2 a glasi: " << l.trenutni() << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

}
template<typename Tip>
void testPrethodniSljedeci1(NizLista<Tip> l)
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
void testPocetakKraj1(NizLista<Tip> l)
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
void testObrisi1 (NizLista<Tip> l)
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
void testDodajIspredDodajIza1 (NizLista<Tip> prazna)
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
void testIndeksiranja1(NizLista<Tip> l)
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

template <typename Tip>
void testBrojElemenata2(JednostrukaLista<Tip> l)
{
    std::cout << l.brojElemenata() << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}
template<typename Tip>
void testTrenutni2 (JednostrukaLista<Tip> l)
{
    std::cout << "Trenutni element glasi: " << l.trenutni() << std::endl;
    l.trenutni() = l.trenutni() + 2;

    std::cout << "Trenutni mora biti uvecan za 2 a glasi: " << l.trenutni() << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

}
template<typename Tip>
void testPrethodniSljedeci2(JednostrukaLista<Tip> l)
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
void testPocetakKraj2(JednostrukaLista<Tip> l)
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
void testObrisi2 (JednostrukaLista<Tip> l)
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

    std::cout << std::endl <<  "-----------------------------------------------" << std::endl;
}

template <typename Tip>
void testDodajIspredDodajIza2 (JednostrukaLista<Tip> prazna)
{
    prazna.pocetak();
    auto x = prazna.brojElemenata();
    for(int i = 0; i < x; i++)
        prazna.obrisi();

    std::cout << "Punjenje sa DodajIspred" << std::endl;
    for(int i = 1; i <= 10; i++) prazna.dodajIspred(i*i);

    std::cout << "Ispis prvih kvadrata: "<< std::endl;
    for(int i = 0; i < prazna.brojElemenata(); i++)
        std::cout << prazna[i] << " ";


    std::cout << std::endl;

    prazna.kraj();
    std::cout  << std::endl;
    for(int i = 1; i <= 10 ; i++)
        prazna.dodajIza(i*i);

    std::cout << "Ispis dodatnih kvadrata: "<< std::endl;
    for(int i = 0; i < prazna.brojElemenata(); i++)
        std::cout << prazna[i] << " ";

    std::cout << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}

template <typename Tip>
void testIndeksiranja2(JednostrukaLista<Tip> l)
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

int main()
{
    // Testovi za NizListu
    NizLista<int> niz;
    for(int i = 1; i <= 10; i++)
        niz.dodajIspred(i);

    std::cout << "Broje elemenata mora biti 10 a gasi: ";
    testBrojElemenata1(niz);
    std::cout << std::endl;

    testDodajIspredDodajIza1(niz);
    testIndeksiranja1(niz);
    testObrisi1(niz);
    testPocetakKraj1(niz);

    niz.pocetak();
    niz.sljedeci();
    niz.sljedeci();
    testPrethodniSljedeci1(niz);
    testTrenutni1(niz);



    //Testovi za JednostrukuListu
    JednostrukaLista<double> lista;
    for(int i = 11; i <= 20; i++)
        lista.dodajIza(i);

    std::cout << "Broj elemenata mora biti 10 a gasi: ";
    testBrojElemenata2(lista);
    std::cout << std::endl;

    testDodajIspredDodajIza2(lista);
    testIndeksiranja2(lista);
    testObrisi2(lista);
    testPocetakKraj2(lista);

    lista.pocetak();
    lista.sljedeci();
    lista.sljedeci();
    testPrethodniSljedeci2(lista);
    testTrenutni2(lista);

    return 0;

}
