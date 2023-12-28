#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "atom.hpp"
#include "renderer.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>

#define W 800
#define H 600
#define Max_Atoms 300
using namespace std;
int main() {
    sf::RenderWindow window(sf::VideoMode(W, H), "Collision detection");
    Renderer renderer(W, H, Max_Atoms, "Collision detection");
    for(int i=0;i<200;i++){
        renderer.render();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}