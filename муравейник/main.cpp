#include "Visualizer.h"
#include <iostream>
#include <locale>

int main() {
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());
    std::cout << "Starting program" << std::endl;
    Visualizer visualizer;
    std::cout << "Visualizer created" << std::endl;
    visualizer.run();
    std::cout << "Visualizer run completed" << std::endl;

    return 0;
}