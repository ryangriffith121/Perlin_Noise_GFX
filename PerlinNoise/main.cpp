#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <random>
#include <iostream>

const unsigned int WINDOW_WIDTH = 1024;
const unsigned int WINDOW_HEIGHT = 1024;

const unsigned int PERLIN_WIDTH = 16;
const unsigned int PERLIN_HEIGHT = 16;

std::vector<float> normalizeVector(std::vector<float> vector2) {
	std::vector<float> normalizedVector = {0.0, 0.0};
	float vectorMagnitude = std::sqrt(std::pow(vector2[0], 2) + std::pow(vector2[1], 2));
	normalizedVector = { vector2[0] / vectorMagnitude, vector2[1] / vectorMagnitude };
	return normalizedVector;
}

std::vector<std::vector<std::vector<float>>> createPerlinNoise(int width, int height, int randSeed) {
	std::mt19937 gen(randSeed);
	std::uniform_real_distribution<float> distrib(-1.0, 1.0);

	std::vector< std::vector< std::vector<float> > > perlinArray(width, std::vector< std::vector<float> >(height, std::vector<float>(2,0.0)));
	
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			perlinArray[i][j] = normalizeVector({distrib(gen), distrib(gen)});
		}
	}


	return perlinArray;
}

void renderPerlinVectors(sf::RenderWindow& drawSurface, std::vector<std::vector<std::vector<float>>> pArray) {
	float vectorMarginX = static_cast<float>(WINDOW_WIDTH) / PERLIN_WIDTH;
	float vectorMarginY = static_cast<float>(WINDOW_HEIGHT) / PERLIN_HEIGHT;

	int screenMargin = int(vectorMarginX / 2);

	int radius = 5;
	int lineSize = 35;

	std::vector<sf::CircleShape> circles;

	std::vector<std::vector<sf::Vertex>> lines;

	for (int i = 0; i < pArray.size(); i++) {
		for (int j = 0; j < pArray[i].size(); j++) {
			sf::CircleShape shape(radius);
			shape.setFillColor(sf::Color(255, 0, 0));
			shape.setPosition({(i * vectorMarginX) - radius + screenMargin, (j * vectorMarginY) - radius + screenMargin});
			std::vector<sf::Vertex> line = { sf::Vertex{ sf::Vector2f((i * vectorMarginX) + screenMargin, (j * vectorMarginY) + screenMargin) }, sf::Vertex{ sf::Vector2f(((i * vectorMarginX) + (pArray[i][j][0] * lineSize)) + screenMargin, ((j * vectorMarginY) + (pArray[i][j][1] * lineSize)) + screenMargin)}};
			circles.push_back(shape);
			lines.push_back(line);
		}
		std::cout << std::endl;
	}

	for (const auto& circle : circles) {
		drawSurface.draw(circle);
	}

	for (const auto& line : lines) {
		drawSurface.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
	}
}

int main() {
	std::random_device randomSeed;

	std::vector<std::vector<std::vector<float>>> perlinArray = createPerlinNoise(PERLIN_WIDTH, PERLIN_HEIGHT, 0);

	for (int i = 0; i < PERLIN_WIDTH; i++) {
		for (int j = 0; j < PERLIN_HEIGHT; j++) {
			std::cout << "(" << perlinArray[i][j][0] << "," << perlinArray[i][j][1] << "), ";
		}
		std::cout << std::endl;
	}

	sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Perlin Noise");
	window.setFramerateLimit(60);

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		window.clear();

		renderPerlinVectors(window, perlinArray);

		window.display();
	}

	return 0;
}