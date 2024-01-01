#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include "atom.hpp"


class Renderer{
    private:
        sf::RenderWindow window;
        std::vector<Atom*> atoms;
        int max_atoms;
        float width;
        float height;
        sf::Clock clock;
        int framerate;
    public:
        Renderer(int width, int height, int max_atoms, std::string title, int framerate);
        void render();

};