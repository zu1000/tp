#include "application.hpp"

using namespace tp::database::indicator;

int main(int argc, const char** argv)
{
    application app;
    app.initialize(argc, argv);
    app.run();
    return 0;
}
