/*
Note: This code was enhanced for readability and clarity with the assistance of OpenAI's GPT model.
*/

// Include the Arduino_LED_Matrix library
#include <Arduino_LED_Matrix.h>
ArduinoLEDMatrix matrix;

// Class to simulate a drunkard's walk on the LED matrix
class DrunkardWalkSimulator {
private:
    // Constants for the dimensions of the LED matrix
    static const int ROWS = 8;
    static const int COLS = 12;
    
    // Frame buffer to store LED states (1 = ON, 0 = OFF)
    byte frame[ROWS][COLS];

    // Current position of the drunkard
    int currentX;
    int currentY;

    // Trail-related variables
    const int TRAIL_LENGTH = 5;  // Number of previous positions to keep lit
    int trail[5][2];  // Stores previous positions
    int trailIndex = 0;  // Index for maintaining the trail

public:
    // Constructor to initialize the simulator
    DrunkardWalkSimulator() {
        // Start in the middle of the matrix
        currentX = COLS / 2;
        currentY = ROWS / 2;

        // Initialize trail with the starting position
        for (int i = 0; i < TRAIL_LENGTH; i++) {
            trail[i][0] = currentX;
            trail[i][1] = currentY;
        }

        // Clear the frame buffer
        memset(frame, 0, ROWS * COLS);
    }

    // Update method to perform a step in the drunkard's walk
    void update() {
        // Clear the frame buffer for the new frame
        memset(frame, 0, ROWS * COLS);

        // Randomly choose a direction (0: up, 1: right, 2: down, 3: left)
        int direction = random(4);

        // Store the current position in the trail
        trail[trailIndex][0] = currentX;
        trail[trailIndex][1] = currentY;
        trailIndex = (trailIndex + 1) % TRAIL_LENGTH;  // Cycle through trail indices

        // Update the position based on the chosen direction
        switch (direction) {
            case 0: // Move up
                currentY = max(0, currentY - 1);
                break;
            case 1: // Move right
                currentX = min(COLS - 1, currentX + 1);
                break;
            case 2: // Move down
                currentY = min(ROWS - 1, currentY + 1);
                break;
            case 3: // Move left
                currentX = max(0, currentX - 1);
                break;
        }

        // Draw the trail with dimmer lights
        for (int i = 0; i < TRAIL_LENGTH; i++) {
            int x = trail[i][0];
            int y = trail[i][1];
            if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
                frame[y][x] = 1; // Light up the trail
            }
        }

        // Highlight the current position (brightest)
        frame[currentY][currentX] = 1;

        // Render the frame on the LED matrix
        matrix.renderBitmap(frame, ROWS, COLS);

        // Delay to control the speed of the drunkard's movement
        delay(200);
    }
};

// Setup function to initialize the matrix and random seed
void setup() {
    matrix.begin(); // Initialize the LED matrix
    randomSeed(analogRead(0)); // Seed the random number generator
}

// Main loop to continuously simulate the drunkard's walk
void loop() {
    DrunkardWalkSimulator walker; // Create an instance of the simulator

    // Continuously update and display the drunkard's walk
    while (true) {
        walker.update();
    }
}