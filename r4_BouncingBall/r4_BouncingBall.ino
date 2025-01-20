#include <Arduino_LED_Matrix.h>
ArduinoLEDMatrix matrix;

class BouncingBallSimulator {
private:
    static const int ROWS = 8;
    static const int COLS = 12;
    byte frame[ROWS][COLS];
    
    // Physics parameters
    float x, y;           // Position (as floats for smooth physics)
    float vx, vy;        // Velocity components
    const float dt = 0.1; // Time step
    const float g = 6.0;  // Reduced gravity for bouncier movement
    const float e = 0.95; // Higher coefficient of restitution for bouncier bounces
    
    // Kick parameters
    unsigned long lastKickTime;
    const unsigned long KICK_INTERVAL = 5000; // 5 seconds in milliseconds
    const float KICK_STRENGTH = 8.0;  // Increased kick strength
    const float MIN_VELOCITY = 2.0;   // Minimum velocity to maintain movement
    
public:
    BouncingBallSimulator() {
        // Start in the middle
        x = COLS / 2.0f;
        y = ROWS / 2.0f;
        
        // Random initial velocity
        float angle = random(360) * PI / 180.0;
        float speed = 6.0;
        vx = speed * cos(angle);
        vy = speed * sin(angle);
        
        // Initialize kick timer
        lastKickTime = millis();
        
        // Clear frame
        memset(frame, 0, ROWS * COLS);
    }
    
    void update() {
        // Clear previous frame
        memset(frame, 0, ROWS * COLS);
        
        // Check if it's time for a kick
        unsigned long currentTime = millis();
        if (currentTime - lastKickTime >= KICK_INTERVAL) {
            // Apply random kick with upward bias
            float kickAngle = random(180, 360) * PI / 180.0; // Upward-biased angle
            vx = KICK_STRENGTH * cos(kickAngle);
            vy = -abs(KICK_STRENGTH * sin(kickAngle)); // Ensure upward kick
            lastKickTime = currentTime;
        }
        
        // Update position using velocity verlet integration
        float ay = g;     // Acceleration from gravity
        
        // Update position
        x += vx * dt;
        y += vy * dt + 0.5 * ay * dt * dt;
        
        // Update velocity
        vy += ay * dt;
        
        // Handle collisions with walls
        if (x < 0) {
            x = 0;
            vx = -vx * e;
        }
        if (x >= COLS) {
            x = COLS - 0.1;
            vx = -vx * e;
        }
        if (y < 0) {
            y = 0;
            vy = -vy * e;
        }
        if (y >= ROWS) {
            y = ROWS - 0.1;
            vy = -vy * e;
            
            // Add a small upward impulse if the ball is moving too slowly
            if (abs(vy) < MIN_VELOCITY) {
                vy = -MIN_VELOCITY;
            }
        }
        
        // Draw ball at current position (rounded to nearest LED)
        int drawX = round(x);
        int drawY = round(y);
        if (drawX >= 0 && drawX < COLS && drawY >= 0 && drawY < ROWS) {
            frame[drawY][drawX] = 1;
        }
        
        // Render frame
        matrix.renderBitmap(frame, ROWS, COLS);
        delay(20);
    }
};

void setup() {
    matrix.begin();
    randomSeed(analogRead(0));
}

void loop() {
    BouncingBallSimulator ball;
    while(true) {
        ball.update();
    }
}