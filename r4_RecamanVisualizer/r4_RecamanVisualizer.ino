/*
Note: This code was enhanced for readability and clarity with the assistance of OpenAI's GPT model.
*/

#include <Arduino_LED_Matrix.h>
ArduinoLEDMatrix matrix;

// Class to visualize the Recamán sequence on the LED matrix
class RecamanVisualizer {
private:
    // Dimensions of the LED matrix
    static const int ROWS = 8;
    static const int COLS = 12;

    // Frame buffer to store LED states
    byte frame[ROWS][COLS];

    int current = 0; // Current number in the sequence
    int step = 1;    // Current step in the sequence

    bool used[100] = {false}; // Track used numbers to avoid repetition

public:
    // Update the visualization for the next number in the sequence
    void update() {
        memset(frame, 0, ROWS * COLS); // Clear the frame buffer

        // Calculate the next number in the Recamán sequence
        int next = current - step;
        if (next < 0 || used[next]) {
            next = current + step;
        }
        used[next] = true; // Mark the number as used

        // Map numbers to matrix coordinates
        int x1 = (current * COLS / 50) % COLS;
        int y1 = (current * ROWS / 50) % ROWS;
        int x2 = (next * COLS / 50) % COLS;
        int y2 = (next * ROWS / 50) % ROWS;

        // Draw a line between the current and next positions
        drawLine(x1, y1, x2, y2);

        matrix.renderBitmap(frame, ROWS, COLS); // Render the frame to the LED matrix
        current = next; // Update the current number
        step++;         // Increment the step

        delay(200); // Control the speed of the animation
    }

private:
    // Draw a line between two points using Bresenham's algorithm
    void drawLine(int x1, int y1, int x2, int y2) {
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            if (x1 >= 0 && x1 < COLS && y1 >= 0 && y1 < ROWS) {
                frame[y1][x1] = 1; // Set the pixel in the frame buffer
            }

            if (x1 == x2 && y1 == y2) break; // Exit when the line is complete

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }
};

// Setup function to initialize the LED matrix
void setup() {
    matrix.begin();
}

// Main loop to animate the Recamán sequence
void loop() {
    RecamanVisualizer recaman;
    for (int i = 0; i < 200; i++) { // Limit the number of updates for this demo
        recaman.update();
    }
}
