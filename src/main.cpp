#include "ladebalkenplus.cpp"

int main()
{
    // Parameters
    std::vector<int> l1_values{2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Create window
    auto window = LadebalkenErstellen(900, 900);

    constexpr int max = 1729;
    constexpr int fps = 60;
    constexpr int nspf = 1'000'000'000/fps;

    // Simulation loop in a separate thread
    std::thread simulation([&]()
    {
        for (int i = 1; i <= max; ++i)
        {
            progress.store(static_cast<float>(i) / max);
            std::this_thread::sleep_for(std::chrono::nanoseconds(nspf));
        }
    });

    while (window->isOpen())
    {
        // Event handling
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            } else if (event.type == sf::Event::Resized) {
                // Update the viewport to the new window size
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window->setView(sf::View(visibleArea));
            }
        }

        // Update progress text
        float progressValue = progress.load();
        int completed = static_cast<int>(progressValue * max);
        std::ostringstream oss;
        oss.imbue(std::locale("de_DE.UTF-8")); // Hier wird die deutsche Locale genutzt
        oss.precision(2);
        oss << std::fixed << progressValue * 100 << "% " << completed << "/" << max;

        // Render the scene
        render(*window, l1_values, oss.str());
    }

    LadebalkenFertig(simulation);
    return 0;
}
