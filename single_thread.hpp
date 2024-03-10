#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <chrono>

#define title "Improved Particle Simulation"
#define substeps 4

struct Particle {
    sf::Vector2f position;
    sf::Vector2f prevPosition;
    sf::Vector2f velocity;
    float mass;
    float radius;
    float invMass; // Inverse of mass for efficient computation
    float restitution; // Coefficient of restitution for collision response
    sf::Color color;

    Particle(double x, double y, double vx, double vy, double m, double r) 
        : position(x, y), prevPosition(x, y), velocity(vx, vy), mass(m), radius(r),
          color(sf::Color(rand() % 255, rand() % 255, rand() % 255)) {
        invMass = (mass > 0) ? 1.0 / mass : 0;
        restitution = 0.85; // Adjust this value as needed
    }
};

class VerletPhysicsSimulator {
public:

    VerletPhysicsSimulator(int w, int h, int maxParticles) : width(w), height(h), maxParticles(maxParticles) {
        particles.reserve(maxParticles);
    }

    void addParticle(double x, double y, double vx, double vy, double mass, double radius) {
        if (canSpawnParticle(x, y, radius)) {
            particles.emplace_back(x, y, vx, vy, mass, radius);
        }
    }

    void update() {
        applyGravity();

        for (auto &particle : particles) {
            // Verlet integration
            sf::Vector2f tempPos = particle.position;
            particle.position += particle.velocity * timeStep;
            particle.prevPosition = tempPos;

            handleBoundaryCollision(particle);
        }

        //handleParticleCollisions();
    }

    void applyGravity() {
        for (auto &particle : particles) {
            particle.velocity.y += gravity * timeStep;
        }
    }

    void handleBoundaryCollision(Particle& particle) {
    // Corrected boundary collision handling with proper type casting
    if (particle.position.x - particle.radius < 0 || particle.position.x + particle.radius > width) {
        particle.velocity.x *= -bounceFactor;
        particle.position.x = std::max(particle.radius, std::min<float>(particle.position.x, width - particle.radius));
    }
    if (particle.position.y - particle.radius < 0 || particle.position.y + particle.radius > height) {
        particle.velocity.y *= -bounceFactor;
        particle.position.y = std::max(particle.radius, std::min<float>(particle.position.y, height - particle.radius));
    }
}

    void handleParticleCollisions() {
    for (std::size_t i = 0; i < particles.size(); ++i) {
        for (std::size_t j = i + 1; j < particles.size(); ++j) {
            Particle &p1 = particles[i];
            Particle &p2 = particles[j];
            sf::Vector2f delta = p2.position - p1.position;
            float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y); // Use float instead of double
            float minDist = p1.radius + p2.radius; // Use float instead of double

            if (dist < minDist) {
                sf::Vector2f normal = delta / dist; // No need for type casting, both delta and dist are floats
                sf::Vector2f relativeVelocity = p2.velocity - p1.velocity;
                float velocityAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y; // Use float

                if (velocityAlongNormal > 0) continue; // They are moving apart

                // Collision response
                float e = std::min(p1.restitution, p2.restitution); // Use float
                float j = (-(1 + e) * velocityAlongNormal) / (p1.invMass + p2.invMass); // Use float
                sf::Vector2f impulse = j * normal; // No type conflict

                p1.velocity -= p1.invMass * impulse;
                p2.velocity += p2.invMass * impulse;

                // Positional correction for sinking
                const float percent = 0.8f; // Use float
                const float slop = 0.1f; // Use float
                //sf::Vector2f correction = std::max(dist - minDist - slop, 0.0f) / (p1.invMass + p2.invMass) * percent * normal; // Use float
                sf::Vector2f correction = std::max(- dist + minDist + slop, 0.0f) / (p1.invMass + p2.invMass) * percent * normal; // Use float

                /*
                if (dist - minDist - slop < 0)
                    std::cout << "dist: " << dist << " minDist: " << minDist << " slop: " << slop << std::endl;

                if (std::hypot(correction.x, correction.y) > 0 )
                    std::cout << "correction: " << std::hypot(correction.x, correction.y) << std::endl;
                */

                p1.position -= p1.invMass * correction;
                p2.position += p2.invMass * correction;
            }
        }
    }
}

    bool canSpawnParticle(double x, double y, double radius) {
        for (const auto& particle : particles) {
            double dx = x - particle.position.x;
            double dy = y - particle.position.y;
            double distance = std::hypot(dx, dy);
            if (distance < radius + particle.radius) {
                return false;
            }
        }
        return true;
    }

    const std::vector<Particle>& getParticles() const {
        return particles;
    }

private:
    std::vector<Particle> particles;

    int width;
    int height;

    int maxParticles;
    float gravity = 9.81f;  // Use float instead of double
    float timeStep = 0.16666667f / substeps;  // Use float instead of double
    float bounceFactor = 0.90f;  // Use float instead of double
};

