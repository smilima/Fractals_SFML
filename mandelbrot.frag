#version 330 core

uniform float x_min;
uniform float x_max;
uniform float y_min;
uniform float y_max;
uniform int max_iterations;
uniform vec2 resolution;

out vec4 fragColor;

void main() {
    // Map pixel coordinates to the complex plane
    vec2 c;
    c.x = x_min + gl_FragCoord.x * (x_max - x_min) / resolution.x;
    c.y = y_min + gl_FragCoord.y * (y_max - y_min) / resolution.y;

    // Mandelbrot calculation
    vec2 z = vec2(0.0, 0.0);
    int iterations = 0;

    while (length(z) <= 2.0 && iterations < max_iterations) {
        float x = z.x * z.x - z.y * z.y + c.x;
        float y = 2.0 * z.x * z.y + c.y;
        z = vec2(x, y);
        iterations++;
    }

    // Map iterations to a color
    if (iterations == max_iterations) {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0); // Black for points inside the set
    } else {
        float t = float(iterations) / float(max_iterations);
        fragColor = vec4(t, t * 0.6, 1.0 - t, 1.0); // Gradient color
    }
}

