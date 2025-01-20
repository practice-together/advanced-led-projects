/*
Note: This code was enhanced for readability and clarity with the assistance of OpenAI's GPT model.
*/

// Include the Arduino_LED_Matrix library
#include <Arduino_LED_Matrix.h>
ArduinoLEDMatrix matrix;

// Class to create and display a spiral visualizer on the LED matrix
class SpiralVisualizer {
private:
    // Constants for the dimensions of the LED matrix
    static const int ROWS = 8;
    static const int COLS = 12;
    
    // Frame buffer to store LED states (1 = ON, 0 = OFF)
    byte frame[ROWS][COLS];
    
    // Variables for animation timing and randomness
    float time = 0; // Tracks the current time in the animation
    float randomOffset = 0; // Random offset to add variability to the animation

public:
    // Constructor to initialize the frame buffer
    SpiralVisualizer() {
        memset(frame, 0, ROWS * COLS); // Clear the frame buffer
    }

    // Update method to draw and render the spiral pattern
    void update() {
        memset(frame, 0, ROWS * COLS); // Clear the frame buffer for the new frame

        time += 0.1; // Increment time for animation
        randomOffset = (random(10) - 5) / 100.0; // Generate a small random offset

        // Draw multiple spiral arms, evenly spaced around the center
        for (int arm = 0; arm < 3; arm++) {
            float angleOffset = (2 * PI * arm) / 3; // Offset each arm by 120 degrees
            drawSpiralArm(angleOffset);
        }

        // Render the frame on the LED matrix
        matrix.renderBitmap(frame, ROWS, COLS);

        // Delay to control animation speed
        delay(50);
    }

private:
    // Method to draw a single spiral arm
    void drawSpiralArm(float angleOffset) {
        float centerX = COLS / 2.0; // X-coordinate of the center
        float centerY = ROWS / 2.0; // Y-coordinate of the center

        // Iterate through points along the spiral
        for (float t = 0.8; t < 4 * PI; t += 0.2) {
            // Calculate the radius with a minimum distance from the center
            float radius = max(1.0, exp(t / 4.0) * 0.3);

            // Add slight wobble to the radius for organic movement
            radius += sin(time * 2 + t) * 0.2;

            // Calculate the angle for the current point
            float angle = t + time + randomOffset;

            // Convert polar coordinates to Cartesian coordinates
            int x = centerX + radius * cos(angle + angleOffset);
            int y = centerY + radius * sin(angle + angleOffset);

            // Skip points that are too close to the center
            float distFromCenter = sqrt(pow(x - centerX, 2) + pow(y - centerY, 2));
            if (distFromCenter < 1.5) continue;

            // Only draw the point if it is within the bounds of the matrix
            if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
                // Alternate points to reduce density and improve visual clarity
                if (int(t * 5) % 2 == 0) {
                    frame[int(y)][int(x)] = 1; // Set the point to ON
                }
            }
        }
    }
};

// Setup function to initialize the matrix and random seed
void setup() {
    matrix.begin(); // Initialize the LED matrix
    randomSeed(analogRead(0)); // Seed the random number generator
}

// Main loop to continuously update the spiral visualizer
void loop() {
    SpiralVisualizer spiral; // Create an instance of the SpiralVisualizer class

    // Continuously update and display the spiral animation
    while (true) {
        spiral.update();
    }
}
