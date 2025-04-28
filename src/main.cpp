#include <iostream>
#include "collector.h"
#include "../ui/cli.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./minimon [cpu|mem|disk|net]" << endl;
        return 1;
    }

    string command = argv[1];
    handleUserCommand(command);

    return 0;
}

