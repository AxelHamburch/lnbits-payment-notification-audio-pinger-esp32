#include "music.h"

// Task Handle
TaskHandle_t playMusicTaskHandle = nullptr;

// Global Variables
const char *musicString = nullptr;
size_t musicIndex = 0;
bool playingMusic = false;

// Task Function for Music Playback
void playMusicTask(void *parameter) {
    while (true) {
        if (musicString == nullptr) {
            vTaskDelay(100 / portTICK_PERIOD_MS); // Wait if no music is set
            continue;
        }

        playingMusic = true;
        while (musicString[musicIndex] != '\0') {
            char note = musicString[musicIndex];
            int durationFactor = musicString[musicIndex + 1] - '0';
            musicIndex += 2;

            int freq = 0;
            switch (note) {
                case 'C': freq = NOTE_C; break;
                case 'D': freq = NOTE_D; break;
                case 'E': freq = NOTE_E; break;
                case 'F': freq = NOTE_F; break;
                case 'G': freq = NOTE_G; break;
                case 'A': freq = NOTE_A; break;
                case 'B': freq = NOTE_B; break;
                default: freq = 0; break;
            }

            if (freq > 0) {
                int duration = 325 * durationFactor;
                playTone(freq, duration);
            }
        }

        musicString = nullptr; // Reset after playback
        playingMusic = false;
    }
}

// Play a Single Tone
void playTone(int freq, int duration) {
    ledcWriteTone(0, freq);
    delay(duration);
    ledcWriteTone(0, 0);
    delay(50); // Small pause between notes
}

// Ensure Music Task Is Running
void ensureMusicTaskRunning() {
    if (playMusicTaskHandle == nullptr) {
        xTaskCreate(playMusicTask, "PlayMusicTask", 2048, NULL, 1, &playMusicTaskHandle);
    }
}

// Enqueue Music Playback
void playMusic(const char *mstr) {
    if (playingMusic) {
        Serial.println("Music is already playing. Ignoring request.");
        return;
    }

    musicString = mstr;
    musicIndex = 0; // Reset index
    ensureMusicTaskRunning();
}
