#include <Arduino_LED_Matrix.h>
ArduinoLEDMatrix matrix;

class DrunkardWalkSimulator {
private:
    static const int ROWS = 8;
    static const int COLS = 12;
    byte frame[ROWS][COLS];
    int currentX;
    int currentY;
    const int TRAIL_LENGTH = 5;  // Number of previous positions to keep lit
    int trail[5][2];  // Stores previous positions
    int trailIndex = 0;
    
public:
    DrunkardWalkSimulator() {
        // Start in the middle of the matrix
        currentX = COLS / 2;
        currentY = ROWS / 2;
        
        // Initialize trail with starting position
        for(int i = 0; i < TRAIL_LENGTH; i++) {
            trail[i][0] = currentX;
            trail[i][1] = currentY;
        }
        
        // Clear frame
        memset(frame, 0, ROWS * COLS);
    }
    
    void update() {
        // Clear frame
        memset(frame, 0, ROWS * COLS);
        
        // Randomly choose direction (0: up, 1: right, 2: down, 3: left)
        int direction = random(4);
        
        // Store current position in trail
        trail[trailIndex][0] = currentX;
        trail[trailIndex][1] = currentY;
        trailIndex = (trailIndex + 1) % TRAIL_LENGTH;
        
        // Update position based on direction
        switch(direction) {
            case 0: // Up
                currentY = max(0, currentY - 1);
                break;
            case 1: // Right
                currentX = min(COLS - 1, currentX + 1);
                break;
            case 2: // Down
                currentY = min(ROWS - 1, currentY + 1);
                break;
            case 3: // Left
                currentX = max(0, currentX - 1);
                break;
        }
        
        // Draw trail (dimmer)
        for(int i = 0; i < TRAIL_LENGTH; i++) {
            int x = trail[i][0];
            int y = trail[i][1];
            if(x >= 0 && x < COLS && y >= 0 && y < ROWS) {
                frame[y][x] = 1;
            }
        }
        
        // Draw current position (brightest)
        frame[currentY][currentX] = 1;
        
        // Render frame
        matrix.renderBitmap(frame, ROWS, COLS);
        delay(200);  // Adjust speed of movement
    }
};

void setup() {
    matrix.begin();
    randomSeed(analogRead(0));  // Initialize random number generator
}

void loop() {
    DrunkardWalkSimulator walker;
    while(true) {
        walker.update();
    }
}