#include <Arduino_LED_Matrix.h>
ArduinoLEDMatrix matrix;

class QuantumTeleporter {
private:
    static const int ROWS = 8;
    static const int COLS = 12;
    byte frame[ROWS][COLS];
    float time = 0;
    float amplitudes[ROWS][COLS];
    
    // Teleportation parameters
    int sourceX, sourceY;
    int targetX, targetY;
    bool teleporting = false;
    float teleportPhase = 0;
    
public:
    QuantumTeleporter() {
        memset(frame, 0, ROWS * COLS);
        randomSeed(analogRead(0));
        initializeNewTeleport();
    }
    
    void update() {
        // Clear frame
        memset(frame, 0, ROWS * COLS);
        
        if (!teleporting) {
            // Start new teleportation if not active
            initializeNewTeleport();
            teleporting = true;
            teleportPhase = 0;
        }
        
        // Update teleportation effect
        updateTeleportation();
        
        // Render frame
        renderToFrame();
        matrix.renderBitmap(frame, ROWS, COLS);
        
        // Update time and phase
        time += 0.1;
        teleportPhase += 0.15;
        
        // Reset teleportation after completion
        if (teleportPhase > PI) {
            teleporting = false;
            delay(200); // Pause between teleports
        }
        
        delay(50);
    }

private:
    void initializeNewTeleport() {
        // Set random source position
        sourceX = random(COLS);
        sourceY = random(ROWS);
        
        // Set target position at a reasonable distance
        float angle = random(2 * PI);
        int distance = min(ROWS, COLS) / 2;
        targetX = sourceX + int(cos(angle) * distance);
        targetY = sourceY + int(sin(angle) * distance);
        
        // Ensure target is within bounds using Arduino's constrain macro
        targetX = constrain(targetX, 0, COLS-1);
        targetY = constrain(targetY, 0, ROWS-1);
    }
    
    void updateTeleportation() {
        // Calculate quantum state for each LED
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                // Calculate distances to source and target
                float distSource = distance(x, y, sourceX, sourceY);
                float distTarget = distance(x, y, targetX, targetY);
                
                // Create teleportation effect
                float sourceAmp = exp(-distSource * 0.5) * (1 - sin(teleportPhase));
                float targetAmp = exp(-distTarget * 0.5) * sin(teleportPhase);
                
                // Add quantum interference pattern
                float interference = cos(distSource - distTarget + time * 2) * 0.3;
                
                // Add entanglement effect
                float entangle = exp(-(distSource + distTarget) * 0.25) * 
                                sin(teleportPhase * 2) * 0.5;
                
                // Combine effects
                amplitudes[y][x] = sourceAmp + targetAmp + interference + entangle;
            }
        }
        
        normalizeAmplitudes();
    }
    
    void renderToFrame() {
        // Convert amplitudes to binary display with multiple thresholds
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                float amp = abs(amplitudes[y][x]);
                frame[y][x] = (amp > 0.15) ? 1 : 0;  // Lowered threshold for more visible effects
            }
        }
    }
    
    // Helper functions
    float distance(int x1, int y1, int x2, int y2) {
        float dx = x1 - x2;
        float dy = y1 - y2;
        return sqrt(dx*dx + dy*dy);
    }
    
    void normalizeAmplitudes() {
        float maxAmp = 0;
        // Find maximum amplitude
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                float amp = abs(amplitudes[y][x]);
                if (amp > maxAmp) maxAmp = amp;
            }
        }
        // Normalize
        if (maxAmp > 0) {
            for (int y = 0; y < ROWS; y++) {
                for (int x = 0; x < COLS; x++) {
                    amplitudes[y][x] /= maxAmp;
                }
            }
        }
    }
    
    float exp(float x) {
        if (x > 10) return 0;
        if (x < -10) return 0;
        return 1.0 / (1.0 + x + x*x/2 + x*x*x/6);
    }
    
    float sin(float x) {
        return cos(x - PI/2);
    }
    
    float cos(float x) {
        while (x > PI) x -= 2*PI;
        while (x < -PI) x += 2*PI;
        float x2 = x*x;
        return 1 - x2/2 + x2*x2/24;
    }
    
    float sqrt(float x) {
        if (x <= 0) return 0;
        float res = x;
        for (int i = 0; i < 4; i++) {
            res = (res + x/res) * 0.5;
        }
        return res;
    }
    
   
};

void setup() {
    matrix.begin();
}

void loop() {
    QuantumTeleporter teleporter;
    while (true) {
        teleporter.update();
    }
}