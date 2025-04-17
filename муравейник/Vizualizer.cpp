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
    if (!font.loadFromFile("C:\\Users\\Professional\\Desktop\\c++\\antsproject\\x64\\Debug\\Arial.ttf")) {
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
void Visualizer::render() {
    window.clear(sf::Color(192, 192, 192)); 
    drawAnthill();
    drawAnts();
    drawLarvae();
    drawEnemies();
    if (showResources) drawResources();
    if (showStats) drawStatistics();
    if (showLegend) drawLegend();
    window.display();
}

void Visualizer::drawAnthill() {
    sf::CircleShape anthillShape(anthill.GetSize() * 20.f);
    anthillShape.setPosition(
        window.getSize().x * 0.4f - anthill.GetSize() * 20.f,
        window.getSize().y * 0.5f - anthill.GetSize() * 20.f
    );
    anthillShape.setFillColor(sf::Color(139, 69, 19));
    window.draw(anthillShape);
}

void Visualizer::drawAnts() {
    const auto& ants = anthill.GetAnts();
    if (ants.empty()) {
        std::cout << "No ants to draw!" << std::endl;
        return;
    }

    const float centerX = window.getSize().x * 0.4f;
    const float centerY = window.getSize().y * 0.5f;
    const float anthillRadius = anthill.GetSize() * 20.f;

    static const std::map<std::string, sf::Color> roleColors = {
        {"NoRole",   sf::Color(180, 180, 180)},//серый
        {"Nurse",    sf::Color(255, 153, 204)}, //розовый
        {"Soldier",  sf::Color(128, 128, 0)},   //оливковый
        {"Shepherd", sf::Color(255, 165, 0)},   //оранжевый
        {"Forager",  sf::Color(255, 255, 0)},   //жёлтый
        {"Builder",  sf::Color(80, 80, 220)},   //синий
        {"Cleaner",  sf::Color(180, 100, 220)},//фиолетовый
        {"Enemy",  sf::Color(220, 80, 80)}  //красный
    };

    for (size_t i = 0; i < ants.size(); ++i) {
        if (!ants[i]->IsAlive()) continue;

        std::string roleName = ants[i]->GetRoleName();
        sf::Color antColor = roleColors.at(roleName);

        float angle = 2 * 3.14159f * i / ants.size();
        float distance = anthillRadius + 30.f;
        float x = centerX + cos(angle) * distance;
        float y = centerY + sin(angle) * distance;

        sf::CircleShape body(8.f);
        body.setPosition(x, y);
        body.setFillColor(antColor);
        body.setOutlineThickness(1.f);
        body.setOutlineColor(sf::Color::Black);
        window.draw(body);

        sf::CircleShape head(5.f);
        head.setPosition(x + 10.f, y - 3.f);
        head.setFillColor(antColor);
        head.setOutlineThickness(1.f);
        body.setOutlineColor(sf::Color::Black);
        window.draw(head);

        if (showAntRoles) {
            sf::Text roleText;
            roleText.setFont(font);
            roleText.setString(roleName);
            roleText.setCharacterSize(10);
            roleText.setFillColor(sf::Color::Black);
            roleText.setPosition(x - 10.f, y - 15.f);
            window.draw(roleText);
        }

        if (showAntHealth) {
            float healthPercent = ants[i]->GetHealth() / 100.f;
            sf::RectangleShape healthBar(sf::Vector2f(16.f * healthPercent, 2.f));
            healthBar.setPosition(x - 8.f, y + 15.f);
            healthBar.setFillColor(
                healthPercent > 0.6f ? sf::Color::Green :
                healthPercent > 0.3f ? sf::Color::Yellow : sf::Color::Red);
            window.draw(healthBar);
        }
    }
}
void Visualizer::drawLarvae() {
    const auto& ants = anthill.GetAnts();
    for (const auto& ant : ants) {
        if (ant->GetRoleName() == "NoRole") {
            sf::CircleShape larva(10.f);
            larva.setFillColor(sf::Color(200, 200, 200));
            window.draw(larva);
        }
    }
}

void Visualizer::drawEnemies() {

    const auto& enemies = anthill.GetEnemies();

    const float centerX = window.getSize().x * 0.4f;
    const float centerY = window.getSize().y * 0.5f;
    const float anthillRadius = anthill.GetSize() * 20.f;

    const sf::Color enemyColor(255, 50, 50);

    for (const auto& enemy : enemies) {
        if (!enemy.IsAlive()) continue;

        float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.f;
        float distance = anthillRadius + 50.f + rand() % 100;
        float x = centerX + cos(angle) * distance;
        float y = centerY + sin(angle) * distance;

        sf::CircleShape body(12.f);
        body.setPosition(x, y);
        body.setFillColor(enemyColor);
        body.setOutlineThickness(2.f);
        body.setOutlineColor(sf::Color::Black);
        window.draw(body);

        sf::CircleShape eye1(3.f);
        eye1.setPosition(x + 5.f, y + 2.f);
        eye1.setFillColor(sf::Color::White);
        window.draw(eye1);

        sf::CircleShape eye2(3.f);
        eye2.setPosition(x + 15.f, y + 2.f);
        eye2.setFillColor(sf::Color::White);
        window.draw(eye2);

        float strengthPercent = enemy.GetStrength() / 100.f;
        sf::RectangleShape strengthBar(sf::Vector2f(24.f * strengthPercent, 3.f));
        strengthBar.setPosition(x - 2.f, y + 25.f);
        strengthBar.setFillColor(
            strengthPercent > 0.6f ? sf::Color::Red :
            strengthPercent > 0.3f ? sf::Color(255, 165, 0) : sf::Color::Yellow);
        window.draw(strengthBar);

        if (showAntRoles) {
            sf::Text enemyText;
            enemyText.setFont(font);
            enemyText.setString("Enemy");
            enemyText.setCharacterSize(12);
            enemyText.setFillColor(sf::Color::Black);
            enemyText.setPosition(x - 10.f, y - 20.f);
            window.draw(enemyText);
        }
    }
}
void Visualizer::drawResources() {

    sf::Text foodText;
    foodText.setFont(font);
    foodText.setString("Food: " + std::to_string(anthill.GetFoodAmount()));
    foodText.setPosition(20.f, 20.f);
    window.draw(foodText);

    sf::Text materialsText;
    materialsText.setFont(font);
    materialsText.setString("Materials: " + std::to_string(anthill.GetBuildingMaterials()));
    materialsText.setPosition(20.f, 50.f);
    window.draw(materialsText);
}

void Visualizer::drawStatistics() {

    sf::Text statsText;
    statsText.setFont(font);

    std::stringstream statsStream;
    statsStream << "Ants: " << anthill.GetAntCount() << "\n"
        << "Day: " << currentDay;

    statsText.setString(statsStream.str());
    statsText.setPosition(20.f, 80.f);
    statsText.setFillColor(sf::Color::Black);
    window.draw(statsText);
}

void Visualizer::drawLegend() {
    static const std::vector<std::pair<std::string, sf::Color>> legendItems = {
        {"NoRole",   sf::Color(180, 180, 180)},//серый
        {"Nurse",    sf::Color(255, 153, 204)}, //розовый
        {"Soldier",  sf::Color(128, 128, 0)},   //оливковый
        {"Shepherd", sf::Color(255, 165, 0)},   //оранжевый
        {"Forager",  sf::Color(255, 255, 0)},   //жёлтый
        {"Builder",  sf::Color(80, 80, 220)},   //синий
        {"Cleaner",  sf::Color(180, 100, 220)},//фиолетовый
        {"Enemy",  sf::Color(220, 80, 80)}  //красный
    };

    sf::RectangleShape legendBg(sf::Vector2f(160.f, 210.f));
    legendBg.setPosition(window.getSize().x - 170.f, 20.f);
    legendBg.setFillColor(sf::Color(255, 255, 255, 200));
    legendBg.setOutlineThickness(1.f);
    legendBg.setOutlineColor(sf::Color::Black);
    window.draw(legendBg);

    sf::Text legendTitle("Ant Roles:", font, 16);
    legendTitle.setPosition(window.getSize().x - 120.f, 15.f);
    legendTitle.setFillColor(sf::Color::Black);
    window.draw(legendTitle);

    for (size_t i = 0; i < legendItems.size(); ++i) {
        sf::CircleShape icon(8.f);
        icon.setPosition(window.getSize().x - 160.f, 55.f + i * 22.f);
        icon.setFillColor(legendItems[i].second);
        icon.setOutlineThickness(1.f);
        icon.setOutlineColor(sf::Color::Black);
        window.draw(icon);

        sf::Text label(legendItems[i].first, font, 14);
        label.setPosition(window.getSize().x - 145.f, 52.f + i * 22.f);
        label.setFillColor(sf::Color::Black);
        window.draw(label);
    }
}