#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Anthill.h"
#include "Informant.h"

class Visualizer {
public:
    Visualizer();
    ~Visualizer();
    void run();

private:
    int currentDay;
    sf::RenderWindow window;

    Anthill anthill;
    Informant globalInformant;

    sf::Font font;

    float simulationSpeed;
    bool paused;
    bool showAntRoles;
    bool showAntHealth;
    bool showLegend;
    bool showResources;
    bool showStats;

    static const unsigned char defaultFontData[];
    static const unsigned int defaultFontDataSize;

    void initTextures();
    void initFont();

    void handleEvents();

    void render();
    void drawAnthill();
    void drawAnts();
    void drawLarvae();
    void drawEnemies();
    void drawResources();
    void drawStatistics();
    void drawLegend();
    void drawControlsInfo();
    void GenerateRandomEvents();
    void PrintAnthillStatus();
};

#endif // VISUALIZER_H
