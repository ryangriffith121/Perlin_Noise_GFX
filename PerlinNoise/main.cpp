#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdint>
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

std::vector<float> vector2DotProduct(std::vector<float> vec1, std::vector<float> vec2) {
	std::vector<float> productVec = {vec1[0] * vec2[0], vec1[1] * vec2[1]};
	return productVec;
}

int vector2Magnitude(std::vector<float> vec) {
	int vecMagnitude = std::sqrt(std::pow(vec[0], 2) + std::pow(vec[1], 2));
	return vecMagnitude;
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

sf::Color hsv(float hue, float saturation, float value) {
	hue = std::fmod(hue, 360.0f);
	if (hue < 0.0f) hue += 360.0f;
	saturation = std::max(0.0f, std::min(1.0f, saturation));
	value = std::max(0.0f, std::min(1.0f, value));

	float c = value * saturation;
	float x = c * (1.0f - std::abs(std::fmod(hue / 60.0f, 2.0f) - 1.0f));
	float m = value - c;

	float r = 0, g = 0, b = 0;

	if (hue >= 0 && hue < 60) {
		r = c; g = x; b = 0;
	}
	else if (hue >= 60 && hue < 120) {
		r = x; g = c; b = 0;
	}
	else if (hue >= 120 && hue < 180) {
		r = 0; g = c; b = x;
	}
	else if (hue >= 180 && hue < 240) {
		r = 0; g = x; b = c;
	}
	else if (hue >= 240 && hue < 300) {
		r = x; g = 0; b = c;
	}
	else if (hue >= 300 && hue < 360) {
		r = c; g = 0; b = x;
	}

	return sf::Color(
		static_cast<std::uint8_t>((r + m) * 255),
		static_cast<std::uint8_t>((g + m) * 255),
		static_cast<std::uint8_t>((b + m) * 255)
	);
}

void renderPerlinGradient(sf::RenderWindow& drawSurface, std::vector<std::vector<std::vector<float>>> pArray) {
	float vectorMarginX = static_cast<float>(WINDOW_WIDTH) / PERLIN_WIDTH;
	float vectorMarginY = static_cast<float>(WINDOW_HEIGHT) / PERLIN_HEIGHT;

	int screenMargin = int(vectorMarginX / 2);

	sf::Image perlinGradient({WINDOW_WIDTH, WINDOW_HEIGHT}, sf::Color(0, 0, 255));
	
	for (int i = 0; i < WINDOW_WIDTH; i++) {
		for (int j = 0; j < WINDOW_HEIGHT; i++) {

			std::vector<float> tlVec;
			std::vector<float> blVec;
			std::vector<float> trVec;
			std::vector<float> brVec;

			std::vector<float> tlVecDistance = { i - tlVec[0], j - tlVec[1] };
			std::vector<float> blVecDistance = { i - blVec[0], blVec[1] - j };
			std::vector<float> trVecDistance = { trVec[0] - i, j - trVec[1] };
			std::vector<float> brVecDistance = { brVec[0] - i, brVec[1] - j };

			std::vector<float> tlVecProduct = vector2DotProduct(tlVec, tlVecDistance);
			std::vector<float> blVecProduct = vector2DotProduct(blVec, blVecDistance);
			std::vector<float> trVecProduct = vector2DotProduct(trVec, trVecDistance);
			std::vector<float> brVecProduct = vector2DotProduct(brVec, brVecDistance);

			float tlVecProductMagnitude;
			float blVecProductMagnitude;
			float trVecProductMagnitude;
			float brVecProductMagnitude;

			float pixelValue = (tlVecProductMagnitude + blVecProductMagnitude + trVecProductMagnitude + brVecProductMagnitude) / 4;

			perlinGradient.setPixel(sf::Vector2u(i, j), hsv(pixelValue * 255, 255.0, 255.0));
		}
	}

	sf::Texture texture;
	texture.loadFromImage(perlinGradient);
	sf::Sprite sprite(texture);

	drawSurface.draw(sprite);
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