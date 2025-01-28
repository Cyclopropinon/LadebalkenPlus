#include <atomic>
#include <cmath>
#include <locale>
#include <memory>
#include <sstream>
#include <thread>
#include <vector>

#include <SFML/Graphics.hpp>

constexpr float PI = 3.141592653589793f;
constexpr float RADIUS = 40.0f; // Scaling factor for the polar radius.
constexpr float D_THETA = 0.01f;
constexpr float MAX_LINIENLÄNGE = 1.0f;
constexpr float LADEBALKEN_VOLL = 2 * PI;

std::atomic<float> progress(0.0f);
std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();
float deltaTimeMs = 0.0f;
float currentFps = 0.0f;

inline float sign(float value)
{
    return (value > 0) - (value < 0);
}

inline sf::Vector2f polarToCartesian(float r, float theta)
{
    return {r * std::cos(theta), r * std::sin(theta)};
}

void render(sf::RenderWindow &window, const std::vector<int> &l1_values, const std::string &progressText)
{
    // Calculate delta time and FPS
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    deltaTimeMs = deltaTime.count();
    currentFps = 1000.0f / deltaTimeMs;

    // Center of the window
    sf::Vector2f center(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    // Clear the window
    window.clear(sf::Color::Black);

    // Draw each l1 graph
    float v15 = progress.load() * LADEBALKEN_VOLL;
    for (int l1 : l1_values)
    {
        sf::VertexArray graph(sf::LineStrip);
        float old_r = 0.0f;

        for (float theta = 0.0f; theta < 2 * PI; theta += D_THETA)
        {
            float r = l1 * sign(std::cos(5 * theta - l1 * v15))
                    + std::sin(v15 + 5 * theta + l1)
                    - std::cos(v15);
            sf::Vector2f cartesian = polarToCartesian(r * RADIUS, theta);

            // If this is not the first vertex, check the distance
            if (!graph.getVertexCount() || std::abs(r - old_r) < MAX_LINIENLÄNGE)
                {
                // Append the new vertex if the distance is within the limit
                graph.append(sf::Vertex(center + cartesian, sf::Color::Green));
            } else {
                // If the distance is too large, start a new vertex array
                window.draw(graph);  // Draw the old graph
                graph.clear();  // Clear the old vertices
                graph.append(sf::Vertex(center + cartesian, sf::Color::Green));  // Start a new vertex array
            }

            old_r = r;
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

    // Draw FPS and delta time
    std::ostringstream stats;
    stats.imbue(std::locale("de_DE.UTF-8")); // Hier wird die deutsche Locale genutzt
    stats.precision(3);
    stats << "Frame Time: " << std::fixed << deltaTimeMs << " ms\nFPS: " << currentFps;

    sf::Text statsText(stats.str(), font, 20);
    statsText.setFillColor(sf::Color::Yellow);
    statsText.setPosition(10.0f, window.getSize().y - 78.0f);
    window.draw(statsText);

    // Display the frame
    window.display();
}

std::unique_ptr<sf::RenderWindow> LadebalkenErstellen(unsigned int width, unsigned int height)
{
    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), "LadebalkenPlus", sf::Style::Close | sf::Style::Resize);
    window->setFramerateLimit(60); // MaxFPS
    return window;
}

void LadebalkenFertig(std::thread &simulation)
{
    simulation.join();
}
