#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;

// Wave sources
const float source1X = 2.0;  // First wave source position
const float source1Y = 4.0;
const float source2X = 9.0;  // Second wave source position
const float source2Y = 4.0;

// Wave parameters
float frequency = 0.5;    // Wave frequency
float wavelength = 4.0;   // Wavelength
float amplitude = 1.0;    // Wave amplitude
float damping = 0.15;     // Damping factor for distance
float timeStep = 0.2;     // Time increment per frame
float currentTime = 0.0;  // Current time

// LED Matrix dimensions
const int ROWS = 8;
const int COLS = 12;

void setup() {
  matrix.begin();
}

// Calculate wave amplitude at a point from a source
float waveFunction(float x, float y, float sourceX, float sourceY, float time) {
  float distance = sqrt(pow(x - sourceX, 2) + pow(y - sourceY, 2));
  float dampingFactor = exp(-damping * distance);
  return amplitude * dampingFactor * 
         sin(2 * PI * (distance / wavelength - frequency * time));
}

// Combine wave amplitudes and map to LED brightness
byte calculateBrightness(float x, float y, float time) {
  // Calculate individual wave contributions
  float wave1 = waveFunction(x, y, source1X, source1Y, time);
  float wave2 = waveFunction(x, y, source2X, source2Y, time);
  
  // Combine waves
  float combinedWave = wave1 + wave2;
  
  // Map the combined amplitude to LED brightness (0 or 1)
  // Using a threshold to create a more distinct pattern
  return (combinedWave > 0) ? 1 : 0;
}

void updateMatrix() {
  byte frame[ROWS][COLS];
  
  // Calculate the interference pattern for each LED
  for(int y = 0; y < ROWS; y++) {
    for(int x = 0; x < COLS; x++) {
      frame[y][x] = calculateBrightness(x, y, currentTime);
    }
  }
  
  // Update the LED matrix
  matrix.renderBitmap(frame, ROWS, COLS);
}

void loop() {
  updateMatrix();
  currentTime += timeStep;
  
  // Reset time to prevent overflow
  if(currentTime > 1000) {
    currentTime = 0;
  }
  
  delay(50);  // Control animation speed
}