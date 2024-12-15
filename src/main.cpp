#include <Arduino.h>

// Define notes as frequencies
#define NOTE_C 2441
#define NOTE_D 2741
#define NOTE_E 3048
#define NOTE_F 3255
#define NOTE_G 3654
#define NOTE_A 4058
#define NOTE_B 4562

// Pin for piezo buzzer
const int piezoPin = 2;

// Variables for music playback
const char* musicString = nullptr;
size_t musicIndex = 0;

// Play a single tone
void playTone(int freq, int duration) {
    ledcWriteTone(0, freq);
    delay(duration);
    ledcWriteTone(0, 0);
    delay(50); // Small pause to separate notes
}

// Play the next note in the music string
void playNextNote() {
    if (!musicString || musicIndex >= strlen(musicString)) {
        return; // End of music string
    }

    char note = musicString[musicIndex];
    int durationFactor = musicString[musicIndex + 1] - '0';
    musicIndex += 2; // Move to the next note

    int freq = 0;
    switch (note) {
        case 'C': freq = NOTE_C; break;
        case 'D': freq = NOTE_D; break;
        case 'E': freq = NOTE_E; break;
        case 'F': freq = NOTE_F; break;
        case 'G': freq = NOTE_G; break;
        case 'A': freq = NOTE_A; break;
        case 'B': freq = NOTE_B; break;
        default: freq = 0; break; // Invalid note, skip
    }

    if (freq > 0) {
        int duration = 325 * durationFactor; // Calculate duration based on factor
        // int duration = 50 * durationFactor; // Calculate duration based on factor
        playTone(freq, duration);
    }

    // Schedule the next note
    if (musicIndex < strlen(musicString)) {
        playNextNote();
    }
}

// Play music from a string
void playMusic(const char* mstr) {
    musicString = mstr;
    musicIndex = 0; // Reset index
    playNextNote();
}

void setup() {
    // Initialize LEDC channel for tone generation
    ledcSetup(0, 500, 13); // Channel 0, 500 Hz, 13-bit resolution
    ledcAttachPin(piezoPin, 0);

    // Example usage
    playMusic("E1E1E2E1E1E2E1G1C1D1E4F1F1F1F1F1E1E1E1E1D1D1E1D2G2");
    // // mary had a little lamb
    // playMusic("B1A1G1A1B1B1B2A1A1A2B1B1B2B1A1G1A1B1B1B2A1A1B1A1G2G2");
    // // twinkle twinkle little star
    // playMusic("C1C1G1G1A1A1G2F1F1E1E1D1D1C2");
    // Jessie J price tag
    // playMusic("F2F2F2F2A1A1F2F2A1A1");

}

void loop() {
    // Nothing to do here
}
