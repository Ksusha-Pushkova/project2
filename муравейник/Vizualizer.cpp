#include "Visualizer.h"
#include <cmath>
#include <sstream>
#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <random> 
using namespace std;

const unsigned char Visualizer::defaultFontData[] = { 0 };
const unsigned int Visualizer::defaultFontDataSize = 0;

Visualizer::Visualizer()
    : window(sf::VideoMode(1280, 720), "Anthill Simulation"),
    anthill(5),
    simulationSpeed(1.0f),
    paused(false),
    showAntRoles(true),
    showAntHealth(true),
    showLegend(true),
    showResources(true),
    showStats(true) {

    initFont();
    currentDay = 1;
    for (int i = 0; i < 10; ++i) {
        int age = rand() % 50;
        int health = rand() % 50 + 50;
        anthill.AddAnt(std::make_unique<Ant>(age, health, &globalInformant, &anthill));
        std::cout << "Создан муравей №" << i + 1 << ", Возраст: " << age << ", Здоровье: " << health << std::endl;
    }
    window.setFramerateLimit(60);
}

Visualizer::~Visualizer() {
}

void Visualizer::initFont() {
    if (!font.loadFromFile("C:\\Users\\Professional\\Desktop\\c++\\муравейник\\x64\\Debug\\resources\\fonts\\arial.ttf")) {
        std::cerr << "Failed to load arial.ttf! Using fallback font." << std::endl;
        if (!font.loadFromMemory(defaultFontData, defaultFontDataSize)) {
            std::cerr << "Failed to load defaultFontData!" << std::endl;
        }
    }
}
