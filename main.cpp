#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

const int WIDTH = 600;
const int HEIGHT = 600;

int main() {
    bool isPanning = false;
    sf::Vector2i lastMousePosition;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set - Shader");
    window.setFramerateLimit(60);

    sf::Shader shader;
    if (!shader.loadFromFile("mandelbrot.frag", sf::Shader::Fragment)) {
        std::cerr << "Failed to load shader!" << std::endl;
        return -1;
    }

    sf::RectangleShape screen(sf::Vector2f(WIDTH, HEIGHT));
    screen.setPosition(0, 0);

    // Mandelbrot parameters
    double x_min = -2.0, x_max = 1.0;
    double y_min = -1.5, y_max = 1.5;
    const int max_iterations = 500;

    // Set initial shader uniforms
    shader.setUniform("resolution", sf::Glsl::Vec2(WIDTH, HEIGHT));
    shader.setUniform("x_min", static_cast<float>(x_min));
    shader.setUniform("x_max", static_cast<float>(x_max));
    shader.setUniform("y_min", static_cast<float>(y_min));
    shader.setUniform("y_max", static_cast<float>(y_max));
    shader.setUniform("max_iterations", max_iterations);

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

                // Update shader uniforms
                shader.setUniform("x_min", static_cast<float>(x_min));
                shader.setUniform("x_max", static_cast<float>(x_max));
                shader.setUniform("y_min", static_cast<float>(y_min));
                shader.setUniform("y_max", static_cast<float>(y_max));
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                // Start panning
                isPanning = true;
                lastMousePosition = sf::Mouse::getPosition(window);
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                // Stop panning
                isPanning = false;
            }
            else if (event.type == sf::Event::MouseMoved && isPanning) {
                // Calculate the drag offset
                sf::Vector2i currentMousePosition = sf::Mouse::getPosition(window);
                sf::Vector2i delta = currentMousePosition - lastMousePosition;

                // Convert screen-space movement to Mandelbrot space
                double dx = (x_max - x_min) / WIDTH * delta.x;
                double dy = (y_max - y_min) / HEIGHT * delta.y;

                x_min -= dx;
                x_max -= dx;
                y_min += dy;
                y_max += dy;

                // Update the last mouse position
                lastMousePosition = currentMousePosition;

                // Update shader uniforms
                shader.setUniform("x_min", static_cast<float>(x_min));
                shader.setUniform("x_max", static_cast<float>(x_max));
                shader.setUniform("y_min", static_cast<float>(y_min));
                shader.setUniform("y_max", static_cast<float>(y_max));
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                x_min = -2.0; x_max = 1.0;
                y_min = -1.5; y_max = 1.5;

                shader.setUniform("x_min", static_cast<float>(x_min));
                shader.setUniform("x_max", static_cast<float>(x_max));
                shader.setUniform("y_min", static_cast<float>(y_min));
                shader.setUniform("y_max", static_cast<float>(y_max));
            }

        }


        window.clear();
        window.draw(screen, &shader);
        window.display();
    }

    return 0;
}
