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
void Visualizer::run() {
    std::cout << "Entering main loop" << std::endl;

    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        handleEvents();

        if (!paused) {
            GenerateRandomEvents();
            anthill.Update();
            currentDay++;
        }

        render();
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    std::cout << "Main loop exited" << std::endl;
}

void Visualizer::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}
void Visualizer::GenerateRandomEvents() {
    int eventType = rand() % 100;

    if (eventType < 10) {
        Enemy enemy(rand() % 20 + 10);
        anthill.AddEnemy(enemy);
        globalInformant.Notify(EventType::EnemySpotted);
        cout << "!!! Атака врага началась !!!" << endl;
    }
    else if (eventType < 20) {
        anthill.AddBuildingMaterials(20);
        globalInformant.Notify(EventType::HeavyBranchFound);
        cout << "Обнаружена тяжелая ветка!" << endl;
    }
    else if (eventType < 30) {
        anthill.AddFood(50);
        globalInformant.Notify(EventType::LargeFoodFound);
        cout << "Обнаружено много еды!" << endl;
    }
    else if (eventType < 35 && anthill.GetFoodAmount() < 20) {
        globalInformant.Notify(EventType::StorageEmpty);
        cout << "Внимание! Запасы пусты!" << endl;
    }
}

void Visualizer::PrintAnthillStatus() {
    std::cout << "\n=== Статус муравейника ===" << std::endl;
    std::cout << "Размер: " << anthill.GetSize() << std::endl;
    std::cout << "Еда: " << anthill.GetFoodAmount() << std::endl;
    std::cout << "Материалы: " << anthill.GetBuildingMaterials() << std::endl;
    std::cout << "Муравьи: " << anthill.GetAntCount() << std::endl;
    std::cout << "===============================\n" << std::endl;
}
