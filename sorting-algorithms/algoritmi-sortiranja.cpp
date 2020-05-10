#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <functional>

template <typename Tip>
void substitute (Tip &first, Tip &second)
{
    Tip temp;
    temp = first, first = second, second = temp;
}

template <typename Tip>
void bubble_sort(Tip* niz, int vel)
{

    for(int i = vel - 1; i >= 1; i--) {
        for(int j = 1; j <= i; j++) {
            if(niz[j - 1] > niz[j])
                substitute(niz[j-1], niz[j]);
        }
    }
}

template <typename Tip>
void selection_sort(Tip* niz, int vel)
{

    int index;
    //provjeriti za negativne brojeve
    for(int i = 0; i < vel - 1; i++) {
        index = i;
        for(int j = i + 1; j < vel; j++) {
            if(niz[j] < niz[index])
                index = j;
        }
        substitute(niz[index], niz[i]);
    }
}

template <typename Tip>
Tip* partition (Tip *first, Tip *last)
{
    Tip pivot = *first;
    Tip *p = first + 1;

    while (p <= last) {
        if(*p <= pivot) p++;
        else  break;
    }

    for(Tip *pok = p + 1; pok <= last; pok++) {
        if(*pok < pivot) substitute(*p, *pok), p++;
    }
    substitute(*first, *(p - 1));
    return p - 1;
}

template <typename Tip>
void quick(Tip *first, Tip *last)
{
    if(first < last) {
        Tip *pivot = partition(first, last);
        quick(first, pivot - 1);
        quick(pivot + 1, last);
    }
}


template <typename Tip>
void quick_sort(Tip* niz, int vel)
{
    quick(niz, niz + vel - 1);
}

template <typename Tip>
void merge (Tip* niz, int begin, int p, int e, int end)
{
    int i = 0, j = e - begin, k = begin;
    Tip *cont = new Tip[end - begin + 1];

    for(int m = 0; m <= end - begin; m++)
        cont[m] = niz[begin + m];

    while (i <= p - begin && j <= end - begin) {
        if(cont[i] < cont[j]) niz[k++] = cont[i++];
        else niz[k++] = cont[j++];
    }

    while (i <= p - begin)
        niz[k++] = cont[i], i++;

    while (j <= end - begin) {
        niz[k++] = cont[j], j++;
    }

    delete[] cont;
}

template <typename Tip>
void merge_s(Tip* niz, int begin, int end)
{
    if(begin < end) {
        int p = (begin + end - 1)/2;
        int e = p + 1;

        merge_s(niz, begin, p);
        merge_s(niz, e, end);
        merge (niz, begin, p, e, end);

    }
}

template <typename Tip>
void merge_sort(Tip* niz, int vel)
{

    merge_s(niz, 0, vel - 1);

}

void ucitaj(std::string filename, int*&niz, int &vel)
{

    std::ifstream input (filename);
    if(!input) throw std::logic_error("Trazena datoteka ne postoji");

    int counter = 0;
    vel = 0;
    char count;
    while(input >> count)
        if(count == ',' || count == ' ') counter++;


    input.clear();
    input.seekg(0);
    niz = new int[counter];

    counter = 0;

    int current_number;
    std::string temp;

    while (!input.eof() && input >> count) {
        if(count != ',') temp += count;
        else {
            current_number = std::stoi(temp);
            temp = "";
            niz[counter++] = current_number;
            if(input.bad()) throw std::logic_error("Problem sa datotekom!");
        }
    }

    vel = counter;
}

void generisi(std::string filename, int vel)
{
    srand(time(NULL));
    std::ofstream output(filename);
    for(int i = 0; i < vel; i++)
        output << rand() << ",";


}


bool isSorted(int* container, int size)
{
    for(int i = 0; i < size - 1; i++)
        if(container[i] > container[i + 1]) return false;

    return true;
}


void funkcija (std::string name, int* &niz)
{

    int size(0);
    ucitaj(name, niz, size);
    int option;
    std::cout << "Odaberite algoritam: 1 - Bubble sort; 2 - Selection sort; 3 - Quick sort; 4 - Merge sort: ";
    std::cin >> option;

    clock_t time_1 = clock();

    switch (option) {
        case 1:
            bubble_sort(niz, size);
            break;
        case 2:
            selection_sort(niz, size);
            break;
        case 3:
            quick_sort(niz, size);
            break;
        case 4:
            merge_sort(niz, size);
            break;
    }
    clock_t time_2 = clock();

    int total = (time_2 - time_1)/(CLOCKS_PER_SEC/1000);
    std::cout << "Vrijeme izvrsavanja : " << total << " ms" << std::endl;

    auto test = isSorted(niz, size);

    std::cout << "Provjera sortiranja:" << std::endl;

    if(test) std::cout << "Niz je sortiran!";
    else std::cout << "Niz nije sortiran!";

    std::ofstream output("new_file");
    for(int i = 0; i < size; i++)
        output << niz[i] << ", ";
}
int main()
{

    int *niz = nullptr;
    std::cout << "Unesite ime datoteke: ";
    std::string name;
    std::getline(std::cin, name);
    std::cout << "Unesite broj elemenata: ";
    int number;
    std::cin >> number;
    generisi(name, number);

    funkcija(name,niz);
    delete[] niz;

    return 0;
}
