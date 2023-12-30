#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define width 800
#define height 600
#define title "collisioni"
#define maxparticle 15

struct Particle {
    double x, y;  // Position
    double prevX, prevY;  // Previous position
    double vx, vy;  // Velocity
    double mass;
    double radius;
};

class VerletPhysicsSimulator {
public:
    void addParticle(double x, double y, double mass,double vx,double vy, double radius) {
        Particle* particle = new Particle{x, y, x, y, vx, vy, mass, radius};
        particles.push_back(particle);
    }

    void update() {
        applyGravity();  // Apply gravity to all particles

        for (Particle* particle : particles) {
            // Verlet integration
            double tempX = particle->x;
            double tempY = particle->y;
            particle->x += particle->vx * timeStep;
            particle->y += particle->vy * timeStep;
            particle->prevX = tempX;
            particle->prevY = tempY;

            // Handle collisions with rectangle boundary
            handleBoundaryCollision(*particle);
        }

        handleParticleCollisions();
    }

    void applyGravity() {
        // Simple gravity effect (adjust as needed)
        for (Particle* particle : particles) {
            particle->vy += gravity * timeStep;
        }
    }

    void handleBoundaryCollision(Particle& particle) {
        // Rectangle boundary coordinates
        double minX = particle.radius, minY = particle.radius;
        double maxX = width - 2*particle.radius, maxY = height - 2*particle.radius;

        // Bounce on the boundary
        if (particle.x < minX || particle.x > maxX) {
            particle.prevX = particle.x;  // Reset previous position to prevent sticking
            particle.x = std::max(minX, std::min(particle.x, maxX));
            particle.prevX += (particle.x - particle.prevX);
            particle.vx *= -bounceFactor;  // Reverse velocity on collision
        }

        if (particle.y < minY || particle.y > maxY) {
            particle.prevY = particle.y;  // Reset previous position to prevent sticking
            particle.y = std::max(minY, std::min(particle.y, maxY));
            particle.prevY += (particle.y - particle.prevY);
            particle.vy *= -bounceFactor;  // Reverse velocity on collision
        }
    }

    void handleParticleCollisions() {
        for (std::size_t i = 0; i < particles.size(); ++i) {
            for (std::size_t j = i + 1; j < particles.size(); ++j) {
                Particle& particleA = *particles[i];
                Particle& particleB = *particles[j];

                double dx = particleB.x - particleA.x;
                double dy = particleB.y - particleA.y;
                double distance = std::hypot(dx, dy);
                double minDistance = particleA.radius + particleB.radius;

                if (distance < minDistance) {
                    // Calculate collision normal
                    double nx = dx / distance;
                    double ny = dy / distance;

                    // Calculate relative velocity
                    double relativeVelocity = (particleB.vx - particleA.vx) * nx + (particleB.vy - particleA.vy) * ny;

                    // Calculate impulse (elastic collision)
                    double impulse = (2.0 * relativeVelocity) / (particleA.mass + particleB.mass);

                    // Update particle velocities
                    particleA.vx += impulse * particleB.mass * nx;
                    particleA.vy += impulse * particleB.mass * ny;
                    particleB.vx -= impulse * particleA.mass * nx;
                    particleB.vy -= impulse * particleA.mass * ny;
                }
            }
        }
    }

    std::vector<Particle*> getParticlesPointers() {
        return particles;
    }

private:
    std::vector<Particle*> particles;
    double gravity = 9.8;  // Adjust as needed (positive for gravity going down)
    double timeStep = 0.0005;  // Adjust as needed
    double bounceFactor = 0.9;  // Adjust for the bounce effect
};

int main() {
    VerletPhysicsSimulator simulator;

    // Add particles with initial positions, masses, and radii
    for(int k = 0;k<maxparticle;k++){
        float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/width));
        float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/height));
        float vx = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/50));
        float vy = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/50));
        double radius=10.0;
        double mass=1.0;
        simulator.addParticle(x,y,mass,vx,vy,radius);
    }

    // Simulate physics over time
    sf::RenderWindow window;
    std::vector<Particle*> particlePointers = simulator.getParticlesPointers();
    window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        simulator.update();  // Adjust time step as needed

        window.clear();  // Clear the window before redrawing
        for (const Particle* particle : particlePointers) {
            sf::CircleShape circle = sf::CircleShape(particle->radius);
            circle.setPosition(sf::Vector2f(particle->x, particle->y));
            window.draw(circle);
        }

        window.display();
    }

    // Clean up allocated memory
    for (Particle* particle : particlePointers) {
        delete particle;
    }

    return 0;
}