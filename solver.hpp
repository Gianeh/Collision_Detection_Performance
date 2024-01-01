#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include "atom.hpp"  // Assuming this is the correct path to your Atom class header

class PhysicsSolver {
    private:
        std::vector<Atom*>& atoms;
        int width;
        int height;
        float dt;
        int substeps;
        float elasticity;

    public:
        PhysicsSolver(std::vector<Atom*>& atoms, int width = 800, int height = 600, float dt = 0.1, float elasticity = 0.99, int substeps = 8);
        void apply_gravity(Atom& atom);
        void verlet_integration(Atom& atom);
        void handle_collisions(Atom& atom);
        void solve();
};