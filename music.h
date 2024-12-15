#ifndef MUSIC_H
#define MUSIC_H

#include <Arduino.h>

// Define notes as frequencies
#define NOTE_C 2441
#define NOTE_D 2741
#define NOTE_E 3048
#define NOTE_F 3255
#define NOTE_G 3654
#define NOTE_A 4058
#define NOTE_B 4562

// Variables for music playback
extern const char *musicString;
extern size_t musicIndex;
extern bool playingMusic;

// FreeRTOS Task Handle
extern TaskHandle_t playMusicTaskHandle;

// Task function for music playback
void playMusicTask(void *parameter);

// Play music (enqueue playback)
void playMusic(const char *mstr);

// Play a single tone
void playTone(int freq, int duration);

// Ensure the music task is running
void ensureMusicTaskRunning();

#endif // MUSIC_H
