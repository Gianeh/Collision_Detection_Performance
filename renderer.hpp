#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <random>
#include "atom.hpp"
#include "solver.hpp"

class Renderer{
    private:
        sf::RenderWindow window;
        sf::Clock clock;
        int max_fps;
        std::vector<Atom*> atoms;
        int max_atoms;
        float width;
        float height;
    public:
        Renderer(int width, int height, int max_atoms, int max_fps, std::string title);
        void render();

};