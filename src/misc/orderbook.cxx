#include "orderbook.h"
#include <iostream>

using namespace Optiver::EFast::BVSPFixFast;

int main(void)
{
    Orderbook book;

    book.Inc(1.1, 10, 1);
    book.Inc(1.2, 10, 2);
    book.Inc(1.3, 10, 3);
    book.Inc(1.4, 10, 4);
    book.Inc(1.5, 10, 5);
    book.Inc(1.6, 10, 6);
    book.Dump();

    std::cout << std::endl;

    book.Reset();

    book.Inc(1.1, 10, 1);
    book.Inc(1.3, 10, 2);
    book.Inc(1.2, 10, 2);
    book.Inc(1.5, 10, 4);
    book.Inc(1.4, 10, 4);
    book.Inc(1.6, 10, 6);
    book.Dump();

    book.Reset();

    std::cout << std::endl;
    book.Inc(1.1, 10, 1);
    book.Inc(1.1, 20, 1);
    book.Inc(1.1, 30, 1);
    book.Inc(1.1, 40, 1);
    book.Inc(1.1, 50, 1);
    book.Inc(1.1, 60, 1);
    book.Inc(1.1, 70, 1);
    book.Dump();

    book.Reset();
}
