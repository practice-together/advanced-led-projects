#include <Arduino_LED_Matrix.h>
ArduinoLEDMatrix matrix;

class SpiralVisualizer {
private:
    static const int ROWS = 8;
    static const int COLS = 12;
    byte frame[ROWS][COLS];
    float time = 0;
    float randomOffset = 0;
    
public:
    SpiralVisualizer() {
        memset(frame, 0, ROWS * COLS);
    }
    
    void update() {
        memset(frame, 0, ROWS * COLS);
        
        time += 0.1;
        randomOffset = (random(10) - 5) / 100.0;
        
        // Draw multiple spiral arms with offset starting positions
        for (int arm = 0; arm < 3; arm++) {
            float angleOffset = (2 * PI * arm) / 3;
            drawSpiralArm(angleOffset);
        }
        
        matrix.renderBitmap(frame, ROWS, COLS);
        delay(50);
    }
    
private:
    void drawSpiralArm(float angleOffset) {
        float centerX = COLS / 2.0;
        float centerY = ROWS / 2.0;
        
        // Start the spiral further from center
        for (float t = 0.8; t < 4 * PI; t += 0.2) {
            // Modified radius calculation with minimum distance from center
            float radius = max(1.0, exp(t / 4.0) * 0.3);
            
            // Add slight wobble to the radius for more organic movement
            radius += sin(time * 2 + t) * 0.2;
            
            float angle = t + time + randomOffset;
            
            int x = centerX + radius * cos(angle + angleOffset);
            int y = centerY + radius * sin(angle + angleOffset);
            
            // Skip points too close to center
            float distFromCenter = sqrt(pow(x - centerX, 2) + pow(y - centerY, 2));
            if (distFromCenter < 1.5) continue;
            
            // Only draw if within bounds
            if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
                // Alternate points for less density
                if (int(t * 5) % 2 == 0) {
                    frame[int(y)][int(x)] = 1;
                }
            }
        }
    }
};

void setup() {
    matrix.begin();
    randomSeed(analogRead(0));
}

void loop() {
    SpiralVisualizer spiral;
    while (true) {
        spiral.update();
    }
}