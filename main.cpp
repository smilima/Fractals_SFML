#include <SFML/Graphics.hpp>
#include <complex>

const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_ITERATIONS = 100;

// Mandelbrot function
sf::Color mandelbrot(std::complex<double> c, int max_iterations) {
    std::complex<double> z = 0;
    int iterations = 0;

    while (std::abs(z) <= 2.0 && iterations < max_iterations) {
        z = z * z + c;
        iterations++;
    }

    // Map iterations to a color
    if (iterations == max_iterations) {
        return sf::Color::Black; // Inside the set
    }
    else {
        int hue = static_cast<int>(255.0 * iterations / max_iterations);
        return sf::Color(hue, hue, 255 - hue); // Gradient color
    }
}

void renderMandelbrot(sf::Image& image, double x_min, double x_max, double y_min, double y_max) {
    double dx = (x_max - x_min) / WIDTH;
    double dy = (y_max - y_min) / HEIGHT;

    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            std::complex<double> c(x_min + x * dx, y_min + y * dy);
            image.setPixel(x, y, mandelbrot(c, MAX_ITERATIONS));
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Zoomable Mandelbrot Set");
    sf::Image image;
    image.create(WIDTH, HEIGHT, sf::Color::Black);

    sf::Texture texture;
    sf::Sprite sprite;

    double x_min = -2.0, x_max = 1.0;
    double y_min = -1.5, y_max = 1.5;

    renderMandelbrot(image, x_min, x_max, y_min, y_max);
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseWheelScrolled) {
                // Zoom on mouse wheel scroll
                double zoomFactor = (event.mouseWheelScroll.delta > 0) ? 0.9 : 1.1;
                double x_center = (x_min + x_max) / 2.0;
                double y_center = (y_min + y_max) / 2.0;

                double x_range = (x_max - x_min) * zoomFactor;
                double y_range = (y_max - y_min) * zoomFactor;

                x_min = x_center - x_range / 2.0;
                x_max = x_center + x_range / 2.0;
                y_min = y_center - y_range / 2.0;
                y_max = y_center + y_range / 2.0;

                renderMandelbrot(image, x_min, x_max, y_min, y_max);
                texture.loadFromImage(image);
                sprite.setTexture(texture);
            }
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
