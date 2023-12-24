#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "atom.hpp"

#define W 800
#define H 600
#define Max_Atoms 1

int main() {
    sf::RenderWindow window(sf::VideoMode(W, H), "Collision detection");

    //new std::vector<Atom*> atoms;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        //renderer.render(&window);
        window.display();
    }

    return 0;
}