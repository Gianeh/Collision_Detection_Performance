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

#define W 1920
#define H 1080
#define Max_Atoms 100
using namespace std;
int main() {
    Renderer renderer(W, H, Max_Atoms, 60, "Collision detection");
    renderer.render();

    return 0;
}