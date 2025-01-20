#include <Arduino_LED_Matrix.h>
ArduinoLEDMatrix matrix;

class RecamanVisualizer {
private:
    static const int ROWS = 8;
    static const int COLS = 12;
    byte frame[ROWS][COLS];
    int current = 0;
    int step = 1;
    bool used[100] = {false};  // Track used numbers
    
public:
    void update() {
        // Clear frame
        memset(frame, 0, ROWS * COLS);
        
        // Calculate next number in sequence
        int next = current - step;
        if(next < 0 || used[next]) {
            next = current + step;
        }
        used[next] = true;
        
        // Visualize the sequence as a path
        int x1 = (current * COLS / 50) % COLS;
        int y1 = (current * ROWS / 50) % ROWS;
        int x2 = (next * COLS / 50) % COLS;
        int y2 = (next * ROWS / 50) % ROWS;
        
        // Draw line between points using Bresenham's algorithm
        drawLine(x1, y1, x2, y2);
        
        matrix.renderBitmap(frame, ROWS, COLS);
        current = next;
        step++;
        delay(200);
    }
    
private:
    void drawLine(int x1, int y1, int x2, int y2) {
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx - dy;
        
        while(true) {
            if(x1 >= 0 && x1 < COLS && y1 >= 0 && y1 < ROWS) {
                frame[y1][x1] = 1;
            }
            
            if(x1 == x2 && y1 == y2) break;
            int e2 = 2 * err;
            if(e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if(e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }
};

void setup() {
    matrix.begin();
}

void loop() {
    RecamanVisualizer recaman;
    for(int i = 0; i < 200; i++) {
        recaman.update();
    }
}