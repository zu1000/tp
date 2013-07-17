#include "fix.h"

namespace blade { namespace fix {

void test()
{
    simple_builder sb;
    sb.set_value<5,7>(1, 1.1);

    char buf[10];
    sb.set_value(1, buf);
}

}}
