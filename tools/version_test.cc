#include <iostream>
#include <votca/tools/version.h>

using namespace std;

int main(int argc, char **argv)
{
    cout << "libtools version: " << votca::tools::ToolsVersionStr() << endl;
}

