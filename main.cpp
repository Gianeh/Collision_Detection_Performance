#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "atom.hpp"
#include "renderer.hpp"

#define W 800
#define H 600
#define Max_Atoms 1

int main() {
    sf::RenderWindow window(sf::VideoMode(W, H), "Collision detection");
    Renderer renderer(W, H, Max_Atoms, "Collision detection");

    renderer.render();

    return 0;
}