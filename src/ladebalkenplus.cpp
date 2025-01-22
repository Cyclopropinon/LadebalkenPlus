#include <atomic>
#include <cmath>
#include <locale>
#include <memory>
#include <sstream>
#include <thread>
#include <vector>

#include <SFML/Graphics.hpp>

constexpr float PI = 3.14159265359f;
constexpr float RADIUS_SCALE = 100.0f; // Scaling factor for the polar radius.
constexpr float TIME_INCREMENT = 0.01f;

std::atomic<float> progress(0.0f);

float sign(float value)
{
    return (value > 0) - (value < 0);
}

sf::Vector2f polarToCartesian(float r, float theta)
{
    return {r * std::cos(theta), r * std::sin(theta)};
}

void render(sf::RenderWindow &window, const std::vector<int> &l1_values, const std::string &progressText)
{
    // Center of the window
    sf::Vector2f center(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    // Clear the window
    window.clear(sf::Color::Black);

    // Draw each l1 graph
    float v15 = progress.load() * 2 * PI;
    for (int l1 : l1_values)
    {
        sf::VertexArray graph(sf::LineStrip);

        for (float theta = 0.0f; theta < 2 * PI; theta += 0.01f)
        {
            float r = l1 * sign(std::cos(5 * theta - l1 * v15))
                    + std::sin(v15 + 5 * theta + l1)
                    - std::cos(v15);
            sf::Vector2f cartesian = polarToCartesian(r * RADIUS_SCALE, theta);
            graph.append(sf::Vertex(center + cartesian, sf::Color::Green));
        }

        window.draw(graph);
    }

    // Draw progress text
    sf::Font font;
    if (!font.loadFromFile("MartianMono-StdMd.ttf"))
    {
        // Handle error
        return;
    }

    sf::Text text(progressText, font, 20);
    text.setFillColor(sf::Color::Cyan);
    text.setPosition(10.0f, window.getSize().y - 30.0f);
    window.draw(text);

    // Display the frame
    window.display();
}

std::unique_ptr<sf::RenderWindow> LadebalkenErstellen(unsigned int width, unsigned int height)
{
    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), "LadebalkenPlus", sf::Style::Close | sf::Style::Resize);
    window->setFramerateLimit(60);
    return window;
}

void LadebalkenFertig(std::thread &simulation)
{
    simulation.join();
}
