/*
Note: This code was enhanced for readability and clarity with the assistance of OpenAI's GPT model.
*/

// Include the Arduino_LED_Matrix library
#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;

// Wave sources
const float source1X = 2.0;  // X-coordinate of the first wave source
const float source1Y = 4.0;  // Y-coordinate of the first wave source
const float source2X = 9.0;  // X-coordinate of the second wave source
const float source2Y = 4.0;  // Y-coordinate of the second wave source

// Wave parameters
float frequency = 0.5;    // Wave frequency
float wavelength = 4.0;   // Wavelength
float amplitude = 1.0;    // Wave amplitude
float damping = 0.15;     // Damping factor to reduce wave intensity with distance
float timeStep = 0.2;     // Time increment per frame for animation
float currentTime = 0.0;  // Current time for wave simulation

// LED Matrix dimensions
const int ROWS = 8;
const int COLS = 12;

// Setup function to initialize the LED matrix
void setup() {
  matrix.begin(); // Initialize the LED matrix
}

// Function to calculate wave amplitude at a given point from a wave source
float waveFunction(float x, float y, float sourceX, float sourceY, float time) {
  // Calculate the distance from the point to the wave source
  float distance = sqrt(pow(x - sourceX, 2) + pow(y - sourceY, 2));
  
  // Apply damping factor based on distance
  float dampingFactor = exp(-damping * distance);
  
  // Calculate wave amplitude using the wave equation
  return amplitude * dampingFactor * 
         sin(2 * PI * (distance / wavelength - frequency * time));
}

// Function to combine wave amplitudes and map them to LED brightness
byte calculateBrightness(float x, float y, float time) {
  // Calculate contributions from both wave sources
  float wave1 = waveFunction(x, y, source1X, source1Y, time);
  float wave2 = waveFunction(x, y, source2X, source2Y, time);
  
  // Combine the amplitudes of the two waves
  float combinedWave = wave1 + wave2;
  
  // Map the combined amplitude to LED brightness (0 or 1)
  // Using a threshold to create a distinct interference pattern
  return (combinedWave > 0) ? 1 : 0;
}

// Function to update the LED matrix with the current wave pattern
void updateMatrix() {
  byte frame[ROWS][COLS]; // Frame buffer to store the LED states
  
  // Calculate the wave pattern for each LED
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      frame[y][x] = calculateBrightness(x, y, currentTime);
    }
  }
  
  // Render the frame on the LED matrix
  matrix.renderBitmap(frame, ROWS, COLS);
}

// Main loop to continuously update the wave interference pattern
void loop() {
  updateMatrix(); // Update the LED matrix with the current pattern
  currentTime += timeStep; // Increment time for animation
  
  // Reset time to prevent overflow after long durations
  if (currentTime > 1000) {
    currentTime = 0;
  }
  
  delay(50); // Control animation speed
}