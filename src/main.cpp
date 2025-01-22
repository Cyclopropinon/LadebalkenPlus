#include "ladebalkenplus.cpp"

int main()
{
    // Parameters
    std::vector<int> l1_values{2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Create window
    auto window = LadebalkenErstellen(800, 800);

    // Simulation loop in a separate thread
    std::thread simulation([&]()
    {
        for (int i = 1; i <= 1000; ++i)
        {
            progress.store(static_cast<float>(i) / 1000.0f);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
        int completed = static_cast<int>(progressValue * 1000);
        std::ostringstream oss;
        oss.imbue(std::locale("de_DE.UTF-8")); // Hier wird die deutsche Locale genutzt
        oss.precision(1);
        oss << std::fixed << progressValue * 100 << "% " << completed << "/1000";

        // Render the scene
        render(*window, l1_values, oss.str());
    }

    LadebalkenFertig(simulation);
    return 0;
}
