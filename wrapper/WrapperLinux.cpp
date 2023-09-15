#include <cstdlib>

int main() {
    int ret_code = system("LD_LIBRARY_PATH=./lib ./bin/Emu_Intel8080");

    return ret_code;
}