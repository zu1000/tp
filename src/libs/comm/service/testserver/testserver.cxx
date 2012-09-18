#include "testserver.hpp"

using namespace tp::comm::service::testapp;

int main(void)
{
    testapp testapp;

    testapp.create_service();

    while(true)
        sleep(10);
}
