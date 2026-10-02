#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <cmath>
#include <iostream>

const unsigned int WINDOW_WIDTH = 1024;
const unsigned int WINDOW_HEIGHT = 1024;

int main() {

	sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Perlin Noise");
	window.setFramerateLimit(60);

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		window.clear();
		window.display();
	}

	return 0;
}