// ============================================================
// PHASE 0 - SETUP CHECK
// Goal: confirm compiler works before starting real project.
//
// Compile : g++ -std=c++17 -Wall -Wextra phase0_hello.cpp -o hello
// Run     : ./hello        (Windows: hello.exe)
// ============================================================
#include <iostream>
using namespace std;

int main() {
    cout << "Hello, Browser Simulator!" << endl;

    // __cplusplus tells which C++ standard compiler is using:
    // 201103 = C++11, 201402 = C++14, 201703 = C++17, 202002 = C++20
    cout << "Setup OK. C++ standard = " << __cplusplus << endl;
    return 0;
}
