#include <vec.h>

int main(int argc, char **argv)
{
    vec v1(1,0,0),v2(0,1,0);
    return v1*v2 == 0 ? 0 : -1;
}

