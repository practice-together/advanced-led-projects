/*
Note: This code was enhanced for readability and clarity with the assistance of OpenAI's GPT model.
*/

#include <Arduino_LED_Matrix.h>
ArduinoLEDMatrix matrix;

// Class to simulate a bouncing ball on the LED matrix
class BouncingBallSimulator {
private:
    // Dimensions of the LED matrix
    static const int ROWS = 8;
    static const int COLS = 12;

    // Frame buffer to store LED states
    byte frame[ROWS][COLS];

    // Physics parameters
    float x, y;           // Position of the ball (float for smooth motion)
    float vx, vy;         // Velocity components (x and y directions)
    const float dt = 0.1; // Time step for simulation
    const float g = 6.0;  // Gravity acceleration (reduced for slower fall)
    const float e = 0.95; // Coefficient of restitution (bounciness)

    // Kick parameters
    unsigned long lastKickTime;          // Time of the last kick
    const unsigned long KICK_INTERVAL = 5000; // Interval for kicks (5 seconds)
    const float KICK_STRENGTH = 8.0;     // Strength of the kick
    const float MIN_VELOCITY = 2.0;      // Minimum velocity to maintain motion

public:
    // Constructor to initialize the simulator
    BouncingBallSimulator() {
        // Start the ball in the middle of the matrix
        x = COLS / 2.0f;
        y = ROWS / 2.0f;

        // Assign a random initial velocity
        float angle = random(360) * PI / 180.0; // Random angle in radians
        float speed = 6.0;                      // Initial speed
        vx = speed * cos(angle);
        vy = speed * sin(angle);

        // Initialize the kick timer
        lastKickTime = millis();

        // Clear the frame buffer
        memset(frame, 0, ROWS * COLS);
    }

    // Update method to move the ball and render its position
    void update() {
        // Clear the frame buffer from the previous state
        memset(frame, 0, ROWS * COLS);

        // Check if it's time for a kick
        unsigned long currentTime = millis();
        if (currentTime - lastKickTime >= KICK_INTERVAL) {
            // Apply a random kick with upward bias
            float kickAngle = random(180, 360) * PI / 180.0; // Upward-biased angle
            vx = KICK_STRENGTH * cos(kickAngle);
            vy = -abs(KICK_STRENGTH * sin(kickAngle)); // Ensure upward kick
            lastKickTime = currentTime;
        }

        // Update the ball's position using velocity verlet integration
        float ay = g; // Acceleration due to gravity

        // Update position
        x += vx * dt;
        y += vy * dt + 0.5 * ay * dt * dt;

        // Update velocity
        vy += ay * dt;

        // Handle collisions with the walls
        if (x < 0) {
            x = 0;
            vx = -vx * e; // Reverse velocity with damping
        }
        if (x >= COLS) {
            x = COLS - 0.1;
            vx = -vx * e; // Reverse velocity with damping
        }
        if (y < 0) {
            y = 0;
            vy = -vy * e; // Reverse velocity with damping
        }
        if (y >= ROWS) {
            y = ROWS - 0.1;
            vy = -vy * e; // Reverse velocity with damping

            // Add an upward impulse if the ball's vertical speed is too low
            if (abs(vy) < MIN_VELOCITY) {
                vy = -MIN_VELOCITY;
            }
        }

        // Draw the ball at its current position (rounded to the nearest LED)
        int drawX = round(x);
        int drawY = round(y);
        if (drawX >= 0 && drawX < COLS && drawY >= 0 && drawY < ROWS) {
            frame[drawY][drawX] = 1; // Light up the LED at the ball's position
        }

        // Render the frame to the LED matrix
        matrix.renderBitmap(frame, ROWS, COLS);
        delay(20); // Delay to control the animation speed
    }
};

// Setup function to initialize the matrix and random number generator
void setup() {
    matrix.begin();          // Initialize the LED matrix
    randomSeed(analogRead(0)); // Seed the random number generator
}

// Main loop to run the ball simulation continuously
void loop() {
    BouncingBallSimulator ball; // Create an instance of the simulator
    while (true) {
        ball.update(); // Update and render the ball's position
    }
}
