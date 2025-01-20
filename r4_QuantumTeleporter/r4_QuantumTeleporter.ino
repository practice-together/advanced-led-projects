/*
Note: This code was enhanced for readability and clarity with the assistance of OpenAI's GPT model.
*/

#include <Arduino_LED_Matrix.h>
ArduinoLEDMatrix matrix;

// Class to simulate quantum teleportation effects on the LED matrix
class QuantumTeleporter {
private:
    // Dimensions of the LED matrix
    static const int ROWS = 8;
    static const int COLS = 12;

    // Frame buffer to store LED states
    byte frame[ROWS][COLS];

    // Teleportation effect parameters
    float time = 0;
    float amplitudes[ROWS][COLS];

    int sourceX, sourceY; // Source position of teleportation
    int targetX, targetY; // Target position of teleportation

    bool teleporting = false; // Whether a teleportation event is active
    float teleportPhase = 0;  // Current phase of teleportation

public:
    // Constructor to initialize the quantum teleporter
    QuantumTeleporter() {
        memset(frame, 0, ROWS * COLS); // Clear the frame buffer
        randomSeed(analogRead(0));     // Seed the random number generator
        initializeNewTeleport();      // Initialize the first teleportation
    }

    // Update the teleportation effect
    void update() {
        memset(frame, 0, ROWS * COLS); // Clear the frame buffer

        if (!teleporting) {
            initializeNewTeleport(); // Start a new teleportation
            teleporting = true;
            teleportPhase = 0;
        }

        updateTeleportation(); // Update the quantum teleportation effect
        renderToFrame();       // Render the effect to the frame buffer

        matrix.renderBitmap(frame, ROWS, COLS); // Render the frame to the LED matrix

        time += 0.1;         // Advance the time
        teleportPhase += 0.15; // Advance the teleportation phase

        // End teleportation after completing the phase
        if (teleportPhase > PI) {
            teleporting = false;
            delay(200); // Pause between teleportations
        }

        delay(50); // Control animation speed
    }

private:
    // Initialize a new teleportation event
    void initializeNewTeleport() {
        sourceX = random(COLS); // Random source position
        sourceY = random(ROWS);

        // Calculate a random target position at a reasonable distance
        float angle = random(2 * PI);
        int distance = min(ROWS, COLS) / 2;
        targetX = sourceX + int(cos(angle) * distance);
        targetY = sourceY + int(sin(angle) * distance);

        // Constrain the target position to the matrix bounds
        targetX = constrain(targetX, 0, COLS - 1);
        targetY = constrain(targetY, 0, ROWS - 1);
    }

    // Update the teleportation effect by calculating quantum amplitudes
    void updateTeleportation() {
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                float distSource = distance(x, y, sourceX, sourceY);
                float distTarget = distance(x, y, targetX, targetY);

                float sourceAmp = exp(-distSource * 0.5) * (1 - sin(teleportPhase));
                float targetAmp = exp(-distTarget * 0.5) * sin(teleportPhase);

                float interference = cos(distSource - distTarget + time * 2) * 0.3;
                float entangle = exp(-(distSource + distTarget) * 0.25) * sin(teleportPhase * 2) * 0.5;

                amplitudes[y][x] = sourceAmp + targetAmp + interference + entangle;
            }
        }

        normalizeAmplitudes(); // Normalize amplitudes for rendering
    }

    // Render the calculated amplitudes to the frame buffer
    void renderToFrame() {
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                float amp = abs(amplitudes[y][x]);
                frame[y][x] = (amp > 0.15) ? 1 : 0; // Convert amplitudes to binary display
            }
        }
    }

    // Calculate the distance between two points
    float distance(int x1, int y1, int x2, int y2) {
        float dx = x1 - x2;
        float dy = y1 - y2;
        return sqrt(dx * dx + dy * dy);
    }

    // Normalize the amplitude values for display
    void normalizeAmplitudes() {
        float maxAmp = 0;
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                float amp = abs(amplitudes[y][x]);
                if (amp > maxAmp) maxAmp = amp;
            }
        }
        if (maxAmp > 0) {
            for (int y = 0; y < ROWS; y++) {
                for (int x = 0; x < COLS; x++) {
                    amplitudes[y][x] /= maxAmp;
                }
            }
        }
    }
};

// Setup function to initialize the LED matrix
void setup() {
    matrix.begin();
}

// Main loop to continuously update the quantum teleporter
void loop() {
    QuantumTeleporter teleporter;
    while (true) {
        teleporter.update();
    }
}
