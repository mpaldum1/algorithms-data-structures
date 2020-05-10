#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <stdexcept>

using namespace std;


void radixSort (vector<int> &a)
{
    std::vector<std::list<int>> row(10);

    int i = 1;

    for(;;) {

        for(auto num : a) {
            if(num < 0) num*= -1;
            row[(num/i)%10].emplace_back(num);
        }

        i *= 10;

        int k = 0;

        auto sup = row.size();
        for(int j = 0; j < sup; j++) {
            for(;;) {

                if(row[j].empty())
                    break;

                a[k++] = row[j].front();

                row[j].pop_front();

            }
            if(!j && k == a.size()) return;
        }
    }

}

void popraviDolje(std::vector<int> &a, int i, int size)
{

    for(;;) {

        if(i >= size/2 && i < size) break;

        int veci = 2 * i + 1;
        int desno = 2 * i + 2;

        if(desno < size && a[desno] > a[veci]) veci = desno;
        if(a[i] > a[veci]) return;

        std::swap(a[veci], a[i]);
        i = veci;
    }
}


void stvoriGomilu (vector<int> &a)
{
    int size = a.size();
    for(int i = (size - 1)/2; i >= 0; i--) {
        popraviDolje(a, i, size);
    }
}

void popraviGore(std::vector<int> &a, int size)
{

    int parent = (size - 1)/2;
    while(size && a[size] > a[parent]) {
        std::swap(a[size], a[parent]);
        size = parent;
        parent = (size - 1)/2;
    }

}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina)
{
    a.emplace_back(umetnuti);
    velicina++;
    popraviGore(a, velicina - 1);
}

int izbaciPrvi(vector<int> &a, int &velicina)
{

    if(a.empty()) throw std::logic_error("Prazna gomila!");

    std::swap(a[0], a[--velicina]);
    if(velicina)
        popraviDolje(a,0,velicina);

    return a[velicina];

}

void gomilaSort(vector<int> &a)
{

    stvoriGomilu(a);
    int size = a.size();

    for(int i = 0; i < a.size() - 1; i++)
        izbaciPrvi(a, size);
}

void jeLiRastuci(vector<int> a)
{
    for(int i = 1; i < a.size(); i++)
        if(a[i] < a[i - 1]) {
            cout << "Netacno!" << endl;
            return;
        }

    cout << "Tacno!" << endl;
}

int main()
{

    vector<int> a;
    vector<int> b;

    for(int i = 0; i < 25; i++) {
        a.emplace_back(rand() % 100);
        b.emplace_back((i*i - rand()) %100);
    }

    radixSort(b);
    for(auto x: b) cout << x << " ";
    jeLiRastuci(b);


    gomilaSort(a);
    for(auto x: a) cout << x << " ";
    jeLiRastuci(a);




    return 0;
}
