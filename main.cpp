#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <boost/filesystem.hpp>
#include "resources/LocalResources.h"
#include "Button.h"

void drawMain(sf::RenderWindow& mainMenu, std::vector<sf::Sprite>& sprites) {
    mainMenu.clear();
    for (sf::Sprite& sprite : sprites) {
        mainMenu.draw(sprite);
    }
    mainMenu.display();
}

void handleTransition(sf::RenderWindow& splash, const uint16_t width, const uint16_t height) {
    sf::Clock clock;
    std::unique_ptr<sf::Music> introMusic = Locator::getResource()->loadMusic("main-menu", "intro.wav");
    introMusic -> setVolume(6); // TODO: think about this
    introMusic -> setLoop(true);
    introMusic -> play();

    std::string configPath = Locator::getResource()->loadYAML("config.yaml");
    YAML::Node config = YAML::LoadFile(configPath);

    for (auto it = Locator::defaultConfig.begin(); it != Locator::defaultConfig.end(); ++it) {
        std::string key = it->first.as<std::string>();
        if (!config[key]) {
            config[key] = Locator::defaultConfig[key];
        }
    }

    std::ofstream fout(configPath);
    fout << config;

    sf::RenderWindow mainMenu;

    // load main menu background
    sf::Texture background = Locator::getResource()->loadTexture("main-menu", "background.png");
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(background);
    const float backgroundScale = std::max((float) width / background.getSize().x, (float) height / background.getSize().y);
    backgroundSprite.setScale(backgroundScale, backgroundScale);

    std::array<Button, 2> buttons{Button("Chapter Select", ButtonType::mainMenu), Button("Credits", ButtonType::mainMenu)};

    for (uint8_t i = 0; i < buttons.size(); i++) {
        Button& menuButton = buttons[i];
        const uint16_t oldBtnWidth = (uint16_t) menuButton.getTexture()->getSize().x;
        const uint16_t oldBtnHeight = (uint16_t) menuButton.getTexture()->getSize().y;
        const uint8_t gap = 5;
        const float ratio = (float) oldBtnHeight / oldBtnWidth;
        std::cout << ratio << "\n";
        const float scale = .2;
        const uint16_t btnWidth = (uint16_t) (width * scale);
        const uint16_t btnHeight = (uint16_t) (btnWidth * ratio);
        std::cout << btnHeight;
        menuButton.setScale((float) btnWidth / oldBtnWidth, (float) btnHeight / oldBtnHeight);
        menuButton.setPosition(0, i * btnHeight + (i + 1) * gap);
    }

    while (splash.isOpen()) {
        const float timePassed = clock.getElapsedTime().asSeconds();
        if (timePassed >= 3) {
            splash.close();
        } else {
            sf::Event event;
            while (splash.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    splash.close();
                    exit(0);
                }
            }
        }
    }

    const bool useFullScreen = config["video"]["fullscreen"].as<bool>();
    mainMenu.create(useFullScreen ? sf::VideoMode::getFullscreenModes()[0] : sf::VideoMode(width, height), "I Am Synthetic", useFullScreen ? sf::Style::Fullscreen : sf::Style::Titlebar + sf::Style::Close);
    mainMenu.clear();
    while (mainMenu.isOpen()) {
        sf::Event event;
        while (mainMenu.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                mainMenu.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (useFullScreen)
                        mainMenu.create(sf::VideoMode(width, height), "I Am Synthetic", sf::Style::Titlebar + sf::Style::Close);
                }
            }
        }
        std::vector<sf::Sprite> sprites{backgroundSprite};
        sprites.insert(sprites.end(), &buttons[0], &buttons[buttons.size()]);
        drawMain(mainMenu, sprites);
    }
}

int main(int argc, char** argv) {
    const float scale = .6;

    const uint16_t width = (uint16_t) (sf::VideoMode::getDesktopMode().width * scale);
    const uint16_t height = (uint16_t) (sf::VideoMode::getDesktopMode().height * scale);
    const uint16_t windowDim = (uint16_t) (std::min(width, height) * scale);

    Locator::provideArgs(argv[0]);

    Locator::provide(std::make_unique<LocalResources>());
    std::string defaultConfigPath = Locator::getResource()->loadYAML("default-config.yaml");
    Locator::provideConfig(defaultConfigPath);

    sf::Texture splash = Locator::getResource()->loadTexture("main-menu", "splash.png");
    sf::Sprite splashSprite;
    splashSprite.setScale((float) windowDim / splash.getSize().x, (float) windowDim / splash.getSize().y);
    splashSprite.setTexture(splash);

    sf::RenderWindow window(sf::VideoMode(windowDim, windowDim), "(Loading) I Am Synthetic", sf::Style::None);
    window.clear();
    window.draw(splashSprite);
    window.display();

    handleTransition(window, (uint16_t) (width / scale), (uint16_t) (height / scale));
    return 0;
}
