#include <windows.h>
#include <shellapi.h>

int main() {
    ShellExecute(nullptr, "open", ".\\bin\\run.bat", nullptr, nullptr, SW_SHOWNORMAL);

    return 0;
}
