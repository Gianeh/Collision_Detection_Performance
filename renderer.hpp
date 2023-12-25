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
    public:
        Renderer(float width, float height, int max_atoms, std::string title);
        void render();

};