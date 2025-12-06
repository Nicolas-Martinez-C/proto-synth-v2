/*
SAMPLE - PROTO-SYNTH

un codigo basico de sample para el proto-synth

Permite grabar un sample con el boton un presionado (no puede ser muy largo)
luego lo reproduces con el segundo boton (se reproduce en loop)
con el tercer boton se reproduce solo una vez
con el cuarto boton se configura el sample en modo reverse

el potenciometro 3 sirve para el pitch del sample





*/
#include <driver/adc.h>
#include <driver/dac.h>
#include <Arduino.h>

// Pines adaptados para Proto-Synth V2.0
const int MIC_PIN = 33;          // Micrófono en pin 33
const int AUDIO_OUT_PIN = 25;    // Salida DAC
const int REC_BTN = 18;          // Botón grabar (Pin 18)
const int PLAY_STOP_BTN = 4;     // Botón play/stop (Pin 4) 
const int PLAY_ONCE_BTN = 15;    // Botón play once (Pin 15)
const int EFFECT_BTN = 19;       // Botón efecto (Pin 19)
const int PITCH_POT = 12;        // Pot pitch (Pin 12)
const int LED_REC = 23;          // LED grabación
const int LED_PLAY = 32;         // LED reproducción

const int MAX_SAMPLES = 24000;
const int SEQUENCE_STEP_MS = 500;
const int NUM_STEPS = 8;

uint16_t audioBuffer[MAX_SAMPLES];
volatile int sampleCount = 0;
volatile int playIndex = 0;
volatile bool hasRecording = false;
volatile bool isPlaying = false;
volatile bool isRunning = false;
bool loopMode = false;
unsigned long lastStepTime = 0;
int currentStep = 0;
bool effectActive = false;

void startPlayback(bool loop) {
  playIndex = 0;
  currentStep = 0;
  lastStepTime = millis();
  isPlaying = true;
  loopMode = loop;
}

void playSample() {
   if (!isPlaying || !hasRecording) return;
   
   unsigned long currentTime = millis();
   if (currentTime - lastStepTime >= SEQUENCE_STEP_MS) {
       lastStepTime = currentTime;
       playIndex = 0;
       
       if (!loopMode) {
           isPlaying = false;
           return;
       }
       
       currentStep = (currentStep + 1) % NUM_STEPS;
   }
   
   int potValue = analogRead(PITCH_POT);
   int playbackDelay = map(potValue, 0, 4095, 5, 250);
   
   if (playIndex < sampleCount) {
       uint16_t sample;
       if (effectActive) {
           sample = audioBuffer[sampleCount - 1 - playIndex]; // Reverse
       } else {
           sample = audioBuffer[playIndex]; // Normal
       }
       
       dac_output_voltage(DAC_CHANNEL_1, sample >> 4);
       delayMicroseconds(playbackDelay);
       playIndex++;
   }
}

void setup() {
  // Deshabilitar logging para evitar interferencias
  esp_log_level_set("*", ESP_LOG_NONE);
  
  pinMode(LED_REC, OUTPUT);
  pinMode(LED_PLAY, OUTPUT);
  pinMode(REC_BTN, INPUT_PULLUP);
  pinMode(PLAY_STOP_BTN, INPUT_PULLUP);
  pinMode(PLAY_ONCE_BTN, INPUT_PULLUP);
  pinMode(EFFECT_BTN, INPUT_PULLUP);
  
  digitalWrite(LED_PLAY, LOW);
  
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  analogSetWidth(12);
  
  dac_output_enable(DAC_CHANNEL_1);
}

void loop() {
  if (digitalRead(REC_BTN) == LOW) {
      isPlaying = false;
      sampleCount = 0;
      digitalWrite(LED_REC, HIGH);
      
      while(digitalRead(REC_BTN) == LOW && sampleCount < MAX_SAMPLES) {
          audioBuffer[sampleCount++] = analogRead(MIC_PIN);
          delayMicroseconds(62);
      }
      
      digitalWrite(LED_REC, LOW);
      hasRecording = true;
  }

  if (digitalRead(PLAY_STOP_BTN) == LOW) {
      delay(50);
      if (digitalRead(PLAY_STOP_BTN) == LOW) {
          isRunning = !isRunning;
          if (isRunning) {
              startPlayback(true);
          } else {
              isPlaying = false;
          }
      }
  }
  
  if (digitalRead(PLAY_ONCE_BTN) == LOW) {
      startPlayback(false);
  }
  
  if (digitalRead(EFFECT_BTN) == LOW) {
   delay(50);
   if (digitalRead(EFFECT_BTN) == LOW) {
       effectActive = !effectActive;
       while(digitalRead(EFFECT_BTN) == LOW) {
           delay(1);
       }
       delay(100);
   }
}
  
  if (isPlaying) {
      playSample();
      digitalWrite(LED_PLAY, HIGH);
  } else {
      digitalWrite(LED_PLAY, LOW);
  }
}