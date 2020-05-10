#include <iostream>                                                 // NAPOMENA: kako je c9 server bio preopterecen, zadaci su uradjeni u drugom 
// okruzenju, te je kopirana na c9
int fib2_0(int n) {

    static int last(1), beforeLast(0);
    int result;

    if (n == 0) {
        result = beforeLast;
        last = 1, beforeLast = 0;                                  //kako bi se funkcija mogla pozivati u petlji

        return result;
    }

    result = last + beforeLast;
    beforeLast = last;
    last = result;

    return fib2_0(--n);

}

int nzd(int x, int y) {
    if (!y) return x;
    return nzd(y, x % y);
}

void testFib2_0() {
    std::cout << "Ispis prvih 25 Fibonaccijevih brojeva:" << std::endl;
    for (int i = 1; i <= 25; i++) {
        std::cout << fib2_0(i);
        if (i != 25) std::cout << ",";
    }
    std::cout << std::endl;
}

void testNzd(){
    std::cout << "Treba ispisati 25, 1, 49:" << std::endl;
    std::cout << nzd(1250,225) << ", " << nzd(25320, 17777) << ", " << nzd(2499,5764801) << std::endl;
}

int main() {
    testFib2_0();
    testNzd();

    return 0;
}