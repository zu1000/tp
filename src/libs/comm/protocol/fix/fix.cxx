#include "fix.h"

namespace blade { namespace fix {

void test()
{
    simple_builder<1024> sb;
    sb.set_value(1, 1.1, 7);

    char buf[10];
    sb.set_value(1, buf);
}

}}
