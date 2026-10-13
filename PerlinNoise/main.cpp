#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdint>
#include <random>
#include <iostream>

const unsigned int WINDOW_WIDTH = 256;
const unsigned int WINDOW_HEIGHT = 256;

const unsigned int NOISE_SCALE = 1;

std::vector<float> normalizeVector(std::vector<float> vector2) {
	std::vector<float> normalizedVector = { 0.0, 0.0 };
	float vectorMagnitude = std::sqrt(std::pow(vector2[0], 2) + std::pow(vector2[1], 2));
	normalizedVector = { vector2[0] / vectorMagnitude, vector2[1] / vectorMagnitude };
	return normalizedVector;
}

std::vector<std::vector<float>> addPerlinGradients(const std::vector<std::vector<float>>& gradient1, const std::vector<std::vector<float>>& gradient2) {
	if (gradient1.empty()) {
		return {};
	}
	size_t rows = gradient1.size();
	size_t cols = gradient1[0].size();
	std::vector<std::vector<float>> sumGradient(rows, std::vector<float>(cols, 0.0f));

	for (size_t i = 0; i < rows; i++) {
		size_t rowCols = gradient1[i].size();
		for (size_t j = 0; j < rowCols; j++) {
			float a = 0.0f;
			float b = 0.0f;
			if (i < gradient1.size() && j < gradient1[i].size()) a = gradient1[i][j];
			if (i < gradient2.size() && j < gradient2[i].size()) b = gradient2[i][j];
			sumGradient[i][j] = a + b;
		}
	}

	return sumGradient;
}

std::vector<std::vector<float>> multiplyPerlinGradient(const std::vector<std::vector<float>>& gradient, float factor) {
	if (gradient.empty()) {
		return {};
	}
	size_t rows = gradient.size();
	size_t cols = gradient[0].size();
	std::vector<std::vector<float>> productGradient(rows, std::vector<float>(cols, 0.0f));

	for (size_t i = 0; i < cols; i++) {
		for (size_t j = 0; j < rows; j++) {
			productGradient[i][j] = gradient[i][j] * factor;
		}
	}

	return productGradient;
}


float vector2DotProduct(std::vector<float> vec1, std::vector<float> vec2) {
	return vec1[0] * vec2[0] + vec1[1] * vec2[1];
}

float fade(float t) {
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float interpolate(float a, float b, float t) {
	return a + t * (b - a);
}

int vector2Magnitude(std::vector<float> vec) {
	int vecMagnitude = std::sqrt(std::pow(vec[0], 2) + std::pow(vec[1], 2));
	return vecMagnitude;
}

std::vector<std::vector<std::vector<float>>> createPerlinNoise(int width, int height, int randSeed) {
	std::mt19937 gen(randSeed);
	std::uniform_real_distribution<float> distrib(-1.0, 1.0);

	std::vector< std::vector< std::vector<float> > > perlinArray(width, std::vector< std::vector<float> >(height, std::vector<float>(2, 0.0)));

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			perlinArray[i][j] = normalizeVector({ distrib(gen), distrib(gen) });
		}
	}


	return perlinArray;
}

std::vector<std::vector<std::vector<float>>> rotatePerlinVectors(const std::vector<std::vector<std::vector<float>>>& pArray, float time) {
	std::vector<std::vector<std::vector<float>>> rotatedArray = pArray;

	for (int i = 0; i < pArray.size(); i++) {
		for (int j = 0; j < pArray[i].size(); j++) {
			float speed = 0.5f + ((i * 7 + j * 13) % 10) / 10.0f;
			float angle = std::atan2(pArray[i][j][1], pArray[i][j][0]) + time * speed;
			rotatedArray[i][j] = { std::cos(angle), std::sin(angle) };
		}
	}

	return rotatedArray;
}

void renderPerlinVectors(sf::RenderWindow& drawSurface, const std::vector<std::vector<std::vector<float>>>& pArray, int width, int height) {
	float vectorMarginX = static_cast<float>(WINDOW_WIDTH) / width;
	float vectorMarginY = static_cast<float>(WINDOW_HEIGHT) / height;

	int screenMargin = int(vectorMarginX / 2);

	int radius = 5;
	int lineSize = 35;

	std::vector<sf::CircleShape> circles;

	std::vector<std::vector<sf::Vertex>> lines;

	for (int i = 0; i < pArray.size(); i++) {
		for (int j = 0; j < pArray[i].size(); j++) {
			sf::CircleShape shape(radius);
			shape.setFillColor(sf::Color(255, 0, 0));
			shape.setPosition({ (i * vectorMarginX) - radius + screenMargin, (j * vectorMarginY) - radius + screenMargin });
			std::vector<sf::Vertex> line = { sf::Vertex{ sf::Vector2f((i * vectorMarginX) + screenMargin, (j * vectorMarginY) + screenMargin) }, sf::Vertex{ sf::Vector2f(((i * vectorMarginX) + (pArray[i][j][0] * lineSize)) + screenMargin, ((j * vectorMarginY) + (pArray[i][j][1] * lineSize)) + screenMargin)} };
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

std::vector<std::vector<float>> createPerlinGradient(const std::vector<std::vector<std::vector<float>>>& pArray, int width, int height) {
	float vectorMarginX = static_cast<float>(WINDOW_WIDTH) / width;
	float vectorMarginY = static_cast<float>(WINDOW_HEIGHT) / height;

	int screenMargin = int(vectorMarginX / 2);

	std::vector<std::vector<float>> perlinGradient(WINDOW_HEIGHT, std::vector<float>(WINDOW_WIDTH, -1.0));

	for (int i = 0; i < WINDOW_WIDTH / NOISE_SCALE; i++) {
		for (int j = 0; j < WINDOW_HEIGHT / NOISE_SCALE; j++) {

			float gridX = (i * static_cast<float>(NOISE_SCALE) - screenMargin) / vectorMarginX;
			float gridY = (j * static_cast<float>(NOISE_SCALE) - screenMargin) / vectorMarginY;

			int cellX = static_cast<int>(std::floor(gridX));
			int cellY = static_cast<int>(std::floor(gridY));

			float fracX = gridX - cellX;
			float fracY = gridY - cellY;

			int w = width;
			int h = height;

			int x0 = ((cellX % w) + w) % w;
			int x1 = (x0 + 1) % w;
			int y0 = ((cellY % h) + h) % h;
			int y1 = (y0 + 1) % h;

			std::vector<float> tlVec = pArray[x0][y0];
			std::vector<float> trVec = pArray[x1][y0];
			std::vector<float> blVec = pArray[x0][y1];
			std::vector<float> brVec = pArray[x1][y1];

			std::vector<float> tlVecDistance = { fracX, fracY };
			std::vector<float> trVecDistance = { fracX - 1.0f, fracY };
			std::vector<float> blVecDistance = { fracX, fracY - 1.0f };
			std::vector<float> brVecDistance = { fracX - 1.0f, fracY - 1.0f };

			float tlVecProduct = vector2DotProduct(tlVec, tlVecDistance);
			float trVecProduct = vector2DotProduct(trVec, trVecDistance);
			float blVecProduct = vector2DotProduct(blVec, blVecDistance);
			float brVecProduct = vector2DotProduct(brVec, brVecDistance);

			float topValue = interpolate(tlVecProduct, trVecProduct, fade(fracX));
			float bottomValue = interpolate(blVecProduct, brVecProduct, fade(fracX));
			float pixelValue = interpolate(topValue, bottomValue, fade(fracY));

			pixelValue = std::max(0.0f, std::min(1.0f, (pixelValue + 0.707f) / 1.414f));

			perlinGradient[i][j] = pixelValue;


		}
	}

	return perlinGradient;

}

void renderPerlinGradient(sf::RenderWindow& drawSurface, const std::vector<std::vector<float>>& pArray) {
	sf::Image perlinGradient({ WINDOW_WIDTH / NOISE_SCALE, WINDOW_HEIGHT / NOISE_SCALE }, sf::Color(0, 0, 255));

	for (int i = 0; i < WINDOW_WIDTH / NOISE_SCALE; i++) {
		for (int j = 0; j < WINDOW_HEIGHT / NOISE_SCALE; j++) {
			perlinGradient.setPixel(sf::Vector2u(i, j), hsv(std::pow(pArray[i][j] * 300.0f, 1.5), std::pow(pArray[i][j], 1.5), std::pow(pArray[i][j], 3.0)));
			//perlinGradient.setPixel(sf::Vector2u(i, j), hsv(std::pow(pArray[i][j] * 300.0f, 1), std::pow(1.0f , 1), std::pow(1.0f , 1)));
		}
	}

	sf::Texture texture;
	texture.loadFromImage(perlinGradient);
	texture.setSmooth(true);
	sf::Sprite sprite(texture);
	sprite.setScale({ static_cast<float>(NOISE_SCALE), static_cast<float>(NOISE_SCALE) });

	drawSurface.draw(sprite);
}

int main() {
	std::random_device randomSeed;

	//for (int i = 0; i < PERLIN_WIDTH; i++) {
	//	for (int j = 0; j < PERLIN_HEIGHT; j++) {
	//		std::cout << "(" << perlinArray[i][j][0] << "," << perlinArray[i][j][1] << "), ";
	//	}
	//	std::cout << std::endl;
	//}

	sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Perlin Noise");
	window.setFramerateLimit(60);

	sf::Clock clock;

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		window.clear();

		float time = clock.getElapsedTime().asSeconds();
		//std::vector<std::vector<std::vector<float>>> animatedArray = rotatePerlinVectors(createPerlinNoise(10, 10, 0), time);

		std::vector<std::vector<float>> gradient1 = multiplyPerlinGradient(createPerlinGradient(rotatePerlinVectors(createPerlinNoise(3, 3, 1), 1), 3, 3), 0.5);
		std::vector<std::vector<float>> gradient2 = multiplyPerlinGradient(createPerlinGradient(rotatePerlinVectors(createPerlinNoise(6, 6, 2), 1), 6, 6), 0.4);
		std::vector<std::vector<float>> gradient3 = multiplyPerlinGradient(createPerlinGradient(rotatePerlinVectors(createPerlinNoise(9, 9, 3), 1), 9, 9), 0.3);
		std::vector<std::vector<float>> gradient4 = multiplyPerlinGradient(createPerlinGradient(rotatePerlinVectors(createPerlinNoise(12, 12, 4), 1), 12, 12), 0.2);
		std::vector<std::vector<float>> gradient5 = multiplyPerlinGradient(createPerlinGradient(rotatePerlinVectors(createPerlinNoise(15, 15, 5), 1), 15, 15), 0.1);

		std::vector<std::vector<float>> gradient6 = multiplyPerlinGradient(addPerlinGradients(gradient1, addPerlinGradients(gradient2, addPerlinGradients(gradient3, addPerlinGradients(gradient4, gradient5)))), 0.75);

		renderPerlinGradient(window, gradient6);

		//renderPerlinVectors(window, animatedArray);

		window.display();
	}

	return 0;
}