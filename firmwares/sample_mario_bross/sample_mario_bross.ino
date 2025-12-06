/*
 * ===============================================
 * PROTO-SYNTH V2.0 - MELODIC SAMPLER
 * ===============================================
 * 
 * INSTRUCCIONES DE USO:
 * 
 * 🎤 GRABACIÓN:
 * 1. Conecta un micrófono al Pin 33
 * 2. Presiona BOTÓN 1 (Pin 18) para iniciar grabación
 * 3. El LED 4 (Pin 2) se encenderá fijo y los otros LEDs parpadearán
 * 4. Graba tu sample base (máximo 1.5 segundos)
 * 5. Presiona BOTÓN 1 nuevamente para detener la grabación
 * 
 * 🎵 REPRODUCCIÓN:
 * 1. Después de grabar, presiona BOTÓN 2 (Pin 4) para reproducir
 * 2. Tu sample se reproducirá siguiendo la melodía de Super Mario Bros
 * 3. Cada nota de la melodía usa tu sample con diferentes tonos
 * 4. La melodía se repite en loop automáticamente
 * 5. Presiona BOTÓN 2 nuevamente para detener
 * 
 * 🎛️ CONTROLES:
 * - POTENCIÓMETRO 3 (Pin 12): Control de TEMPO (60-200 BPM)
 * - POTENCIÓMETRO 1 (Pin 13): Control de PITCH (+/- 24 semitonos)
 * - POTENCIÓMETRO 2 (Pin 14): Control de VOLUMEN MASTER (0-100%)
 * - POTENCIÓMETRO 4 (Pin 27): EXTRA (reservado para futuras funciones)
 * 
 * 🔄 EFECTOS:
 * - BOTÓN 3 (Pin 15): Activa/desactiva modo REVERSA del sample
 * - BOTÓN 4 (Pin 19): (No usado - solo hay una melodía disponible)
 * 
 * 💡 INDICADORES LED:
 * - LED 1 (Pin 23): Parpadea con cada nota (beat indicator)
 * - LED 2 (Pin 32): Encendido durante reproducción
 * - LED 3 (Pin 5): Indica melodía actual (no usado con una sola melodía)
 * - LED 4 (Pin 2): Encendido durante grabación
 * 
 * 🔊 SALIDA DE AUDIO:
 * - Pin 25: Salida DAC para amplificador o auriculares
 * 
 * 📝 NOTAS TÉCNICAS:
 * - Sample Rate: 22kHz
 * - Resolución: 12 bits
 * - Duración máxima de grabación: 1.5 segundos
 * - La melodía base está 4 octavas abajo del original
 * - El control de pitch permite subir/bajar 2 octavas adicionales
 * 
 * 🎼 MELODÍA INCLUIDA:
 * - Super Mario Bros - Overworld Theme (completa)
 * 
 * ===============================================
 */

#include <driver/adc.h>
#include <driver/dac.h>
#include <Arduino.h>

// Pines Proto-Synth V2.0
const int MIC_PIN = 33;          // Micrófono
const int AUDIO_OUT_PIN = 25;    // Salida DAC

// Botones
const int REC_BTN = 18;          // Botón grabar
const int PLAY_BTN = 4;          // Botón play/stop
const int REVERSE_BTN = 15;      // Botón reversa sample
const int CYCLE_BTN = 19;        // Cambio de melodías

// Potenciómetros
const int TEMPO_POT = 12;        // Control de tempo
const int PITCH_POT = 13;        // Control de pitch (+/- 24 semitonos)
const int VOLUME_POT = 14;       // Control de volumen master
const int EXTRA_POT = 27;        // Extra control (por definir)

// LEDs para feedback visual
const int LED1 = 23;  // Beat indicator
const int LED2 = 32;  // Playing indicator
const int LED3 = 2;   // Melody indicator
const int LED4 = 5;   // Recording indicator

// Configuración de audio
const int SAMPLE_RATE = 22000;   
const int MAX_SAMPLES = 33000;   // 1.5 segundos a 22kHz
const int REFERENCE_FREQ = 110;  // LA2 = 110Hz

// Definiciones de notas (frecuencias en Hz)
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define REST     0

// Estructura para una nota musical
struct MelodyNote {
  int frequency;  // Frecuencia de la nota
  int duration;   // Duración (4=quarter, 8=eighth, etc., negativo=dotted)
};

// Super Mario Bros melody - extraída del código proporcionado
const MelodyNote MARIO_MELODY[] = {
  {NOTE_E5,8}, {NOTE_E5,8}, {REST,8}, {NOTE_E5,8}, {REST,8}, {NOTE_C5,8}, {NOTE_E5,8},
  {NOTE_G5,4}, {REST,4}, {NOTE_G4,8}, {REST,4}, 
  {NOTE_C5,-4}, {NOTE_G4,8}, {REST,4}, {NOTE_E4,-4},
  {NOTE_A4,4}, {NOTE_B4,4}, {NOTE_AS4,8}, {NOTE_A4,4},
  {NOTE_G4,-8}, {NOTE_E5,-8}, {NOTE_G5,-8}, {NOTE_A5,4}, {NOTE_F5,8}, {NOTE_G5,8},
  {REST,8}, {NOTE_E5,4}, {NOTE_C5,8}, {NOTE_D5,8}, {NOTE_B4,-4},
  {NOTE_C5,-4}, {NOTE_G4,8}, {REST,4}, {NOTE_E4,-4},
  {NOTE_A4,4}, {NOTE_B4,4}, {NOTE_AS4,8}, {NOTE_A4,4},
  {NOTE_G4,-8}, {NOTE_E5,-8}, {NOTE_G5,-8}, {NOTE_A5,4}, {NOTE_F5,8}, {NOTE_G5,8},
  {REST,8}, {NOTE_E5,4}, {NOTE_C5,8}, {NOTE_D5,8}, {NOTE_B4,-4},
  
  {REST,4}, {NOTE_G5,8}, {NOTE_FS5,8}, {NOTE_F5,8}, {NOTE_DS5,4}, {NOTE_E5,8},
  {REST,8}, {NOTE_C5,8}, {NOTE_A4,8}, {NOTE_C5,8}, {REST,8}, {NOTE_A4,8}, {NOTE_C5,8}, {NOTE_D5,8},
  {REST,4}, {NOTE_DS5,4}, {REST,8}, {NOTE_D5,-4},
  {NOTE_C5,2}, {REST,2},

  {REST,4}, {NOTE_G5,8}, {NOTE_FS5,8}, {NOTE_F5,8}, {NOTE_DS5,4}, {NOTE_E5,8},
  {REST,8}, {NOTE_C5,8}, {NOTE_A4,8}, {NOTE_C5,8}, {REST,8}, {NOTE_A4,8}, {NOTE_C5,8}, {NOTE_D5,8},
  {REST,4}, {NOTE_DS5,4}, {REST,8}, {NOTE_D5,-4},
  {NOTE_C5,2}, {REST,2}
};

// Twinkle Twinkle Little Star
const MelodyNote TWINKLE_MELODY[] = {
  {NOTE_C4,4}, {NOTE_C4,4}, {NOTE_G4,4}, {NOTE_G4,4},
  {NOTE_A4,4}, {NOTE_A4,4}, {NOTE_G4,2},
  {NOTE_F4,4}, {NOTE_F4,4}, {NOTE_E4,4}, {NOTE_E4,4},
  {NOTE_D4,4}, {NOTE_D4,4}, {NOTE_C4,2}
};

// Happy Birthday
const MelodyNote BIRTHDAY_MELODY[] = {
  {NOTE_C4,8}, {NOTE_C4,8}, {NOTE_D4,4}, {NOTE_C4,4}, {NOTE_F4,4}, {NOTE_E4,2},
  {NOTE_C4,8}, {NOTE_C4,8}, {NOTE_D4,4}, {NOTE_C4,4}, {NOTE_G4,4}, {NOTE_F4,2},
  {NOTE_C4,8}, {NOTE_C4,8}, {NOTE_C5,4}, {NOTE_A4,4}, {NOTE_F4,4}, {NOTE_E4,4}, {NOTE_D4,4}
};

// Ode to Joy (Beethoven)
const MelodyNote ODE_MELODY[] = {
  {NOTE_E4,4}, {NOTE_E4,4}, {NOTE_F4,4}, {NOTE_G4,4},
  {NOTE_G4,4}, {NOTE_F4,4}, {NOTE_E4,4}, {NOTE_D4,4},
  {NOTE_C4,4}, {NOTE_C4,4}, {NOTE_D4,4}, {NOTE_E4,4},
  {NOTE_E4,4}, {NOTE_D4,8}, {NOTE_D4,2}
};

// Mary Had a Little Lamb
const MelodyNote MARY_MELODY[] = {
  {NOTE_E4,4}, {NOTE_D4,4}, {NOTE_C4,4}, {NOTE_D4,4},
  {NOTE_E4,4}, {NOTE_E4,4}, {NOTE_E4,2},
  {NOTE_D4,4}, {NOTE_D4,4}, {NOTE_D4,2},
  {NOTE_E4,4}, {NOTE_G4,4}, {NOTE_G4,2}
};

// Solo una melodía: Super Mario Bros completa
const MelodyNote* MELODIES[] = {
  MARIO_MELODY
};

const int MELODY_SIZES[] = {
  sizeof(MARIO_MELODY) / sizeof(MelodyNote)
};

const char* MELODY_NAMES[] = {
  "Super Mario Bros - Overworld Theme"
};

const int NUM_MELODIES = 1;

// Variables del sampler
uint16_t* audioBuffer;
volatile int totalSamples = 0;
volatile bool hasRecording = false;
volatile bool isRecording = false;

// Variables del secuenciador melódico
volatile bool isPlaying = false;
volatile bool reverseMode = false;
int currentMelody = 0;
int currentNote = 0;
int baseTempo = 120; // BPM base
float masterVolume = 1.0;
int pitchBend = 0; // semitonos +/- 24 (centro en 0)

// Timing
unsigned long lastNoteTime = 0;
unsigned long noteDuration = 0;
unsigned long lastButtonTime = 0;
const int DEBOUNCE_MS = 100;

// LEDs
unsigned long lastLedUpdate = 0;
bool isNotePlaying = false;

float calculatePitchRatio(int noteFreq) {
  if (noteFreq == REST) return 1.0;
  
  // Bajar toda la melodía 4 octavas por defecto (-48 semitonos)
  // Luego aplicar el pitch bend del potenciómetro (+/- 24 semitonos)
  float totalPitch = -48.0 + pitchBend; // Base -4 octavas + control del pot
  float adjustedFreq = noteFreq * pow(2.0, totalPitch / 12.0);
  
  return adjustedFreq / REFERENCE_FREQ;
}

void updateLEDs() {
  digitalWrite(LED1, isNotePlaying ? HIGH : LOW);
  digitalWrite(LED2, isPlaying ? HIGH : LOW);
  digitalWrite(LED3, (currentMelody % 2) ? HIGH : LOW);
  digitalWrite(LED4, isRecording ? HIGH : LOW);
}

void showRecordingAnimation() {
  static unsigned long lastBlink = 0;
  static bool blinkState = false;
  
  if (millis() - lastBlink > 150) {
    blinkState = !blinkState;
    lastBlink = millis();
    
    digitalWrite(LED1, blinkState);
    digitalWrite(LED2, blinkState);
    digitalWrite(LED3, blinkState);
    digitalWrite(LED4, HIGH);
  }
}

void startRecording() {
  isRecording = true;
  isPlaying = false;
  totalSamples = 0;
  hasRecording = false;
  Serial.println("Grabando en LA2 (110Hz)...");
}

void stopRecording() {
  isRecording = false;
  hasRecording = (totalSamples > 0);
  Serial.print("Grabación terminada. Samples: ");
  Serial.println(totalSamples);
}

void startMelody() {
  if (!hasRecording) return;
  
  isPlaying = true;
  currentNote = 0;
  lastNoteTime = millis();
  
  Serial.print("Reproduciendo: ");
  Serial.println(MELODY_NAMES[currentMelody]);
}

void playMelodyStep() {
  if (!isPlaying || !hasRecording) return;
  
  unsigned long currentTime = millis();
  
  // Si es momento de la siguiente nota
  if (currentTime - lastNoteTime >= noteDuration) {
    
    // Si llegamos al final de la melodía
    if (currentNote >= MELODY_SIZES[currentMelody]) {
      currentNote = 0; // Loop de la melodía
    }
    
    MelodyNote note = MELODIES[currentMelody][currentNote];
    
    // Calcular duración de la nota basada en tempo (como en el código original)
    int wholenote = (60000 * 4) / baseTempo;
    int divider = note.duration;
    
    if (divider > 0) {
      // Nota regular
      noteDuration = wholenote / divider;
    } else if (divider < 0) {
      // Notas con puntillo (dotted notes)
      noteDuration = wholenote / abs(divider);
      noteDuration *= 1.5; // Aumenta duración 50% para notas con puntillo
    }
    
    isNotePlaying = (note.frequency != REST);
    
    if (note.frequency != REST) {
      // Calcular pitch ratio para esta nota
      float pitchRatio = calculatePitchRatio(note.frequency);
      
      // Reproducir sample con el pitch de la nota
      playSampleAtPitch(pitchRatio, noteDuration * 0.9); // 90% de duración
    }
    
    lastNoteTime = currentTime;
    currentNote++;
    
    updateLEDs();
  }
}

void playSampleAtPitch(float pitchRatio, unsigned long maxDuration) {
  if (!hasRecording) return;
  
  unsigned long startTime = millis();
  float samplePosition = 0.0;
  
  while (samplePosition < totalSamples - 1 && 
         (millis() - startTime) < maxDuration) {
    
    if (!isPlaying) break;
    
    int currentSample = (int)samplePosition;
    float fraction = samplePosition - currentSample;
    
    uint16_t sample;
    
    if (reverseMode) {
      int reverseIndex = totalSamples - 1 - currentSample;
      if (reverseIndex > 0) {
        uint16_t sample1 = audioBuffer[reverseIndex];
        uint16_t sample2 = audioBuffer[reverseIndex - 1];
        sample = sample1 + (uint16_t)(fraction * (sample2 - sample1));
      } else {
        sample = audioBuffer[0];
      }
    } else {
      if (currentSample < totalSamples - 1) {
        uint16_t sample1 = audioBuffer[currentSample];
        uint16_t sample2 = audioBuffer[currentSample + 1];
        sample = sample1 + (uint16_t)(fraction * (sample2 - sample1));
      } else {
        sample = audioBuffer[currentSample];
      }
    }
    
    // Aplicar volumen master
    sample = (uint16_t)(sample * masterVolume);
    if (sample > 4095) sample = 4095;
    
    // Output al DAC
    dac_output_voltage(DAC_CHANNEL_1, sample >> 4);
    
    // Avanzar posición con pitch ratio
    samplePosition += pitchRatio;
    
    // Delay fijo para mantener calidad
    delayMicroseconds(45);
  }
}

void readControls() {
  // Leer potenciómetros
  int tempoPot = analogRead(TEMPO_POT);
  int pitchPot = analogRead(PITCH_POT);
  int volumePot = analogRead(VOLUME_POT);
  
  // Mapear valores
  baseTempo = map(tempoPot, 0, 4095, 60, 200); // 60-200 BPM
  pitchBend = map(pitchPot, 0, 4095, -24, 24); // -24 a +24 semitonos (4 octavas rango)
  masterVolume = map(volumePot, 0, 4095, 0, 100) / 100.0;
}

void handleButtons() {
  unsigned long currentTime = millis();
  if (currentTime - lastButtonTime < DEBOUNCE_MS) return;
  
  // Botón grabar
  static bool lastRecState = HIGH;
  bool recState = digitalRead(REC_BTN);
  if (recState == LOW && lastRecState == HIGH) {
    if (!isRecording) {
      startRecording();
    } else {
      stopRecording();
    }
    lastButtonTime = currentTime;
  }
  lastRecState = recState;
  
  // Botón play/stop
  static bool lastPlayState = HIGH;
  bool playState = digitalRead(PLAY_BTN);
  if (playState == LOW && lastPlayState == HIGH) {
    if (!isPlaying) {
      startMelody();
    } else {
      isPlaying = false;
      isNotePlaying = false;
      Serial.println("Melodía detenida");
    }
    lastButtonTime = currentTime;
  }
  lastPlayState = playState;
  
  // Botón reversa
  static bool lastReverseState = HIGH;
  bool reverseState = digitalRead(REVERSE_BTN);
  if (reverseState == LOW && lastReverseState == HIGH) {
    reverseMode = !reverseMode;
    Serial.print("Modo reversa: ");
    Serial.println(reverseMode ? "ON" : "OFF");
    lastButtonTime = currentTime;
  }
  lastReverseState = reverseState;
  
  // Botón cambio de melodía (deshabilitado ya que solo hay una)
  // Ya no es necesario cambiar melodías
}

void setup() {
  Serial.begin(115200);
  Serial.println("🎵 MELODIC SAMPLER v2.0");
  
  // Allocar buffer de audio
  audioBuffer = (uint16_t*)malloc(MAX_SAMPLES * sizeof(uint16_t));
  if (!audioBuffer) {
    Serial.println("ERROR: No se pudo allocar memoria!");
    while(1) delay(1000);
  }
  Serial.println("✓ Buffer de audio allocado");
  
  // Configurar pines
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  
  pinMode(REC_BTN, INPUT_PULLUP);
  pinMode(PLAY_BTN, INPUT_PULLUP);
  pinMode(REVERSE_BTN, INPUT_PULLUP);
  pinMode(CYCLE_BTN, INPUT_PULLUP);
  
  // Configurar ADC y DAC
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  analogSetWidth(12);
  dac_output_enable(DAC_CHANNEL_1);
  dac_output_voltage(DAC_CHANNEL_1, 128);
  
  // Test de LEDs
  for (int i = 0; i < 2; i++) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    delay(200);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(200);
  }
  
  Serial.println("🎼 MELODÍA: Super Mario Bros - Overworld Theme completa");
  
  Serial.println("🎛️ CONTROLES:");
  Serial.println("  REC(18): Grabar sample base");
  Serial.println("  PLAY(4): Reproducir melodía");
  Serial.println("  REVERSE(15): Toggle reversa sample");
  Serial.println("  CYCLE(19): (No usado - solo hay una melodía)");
  Serial.println("  TEMPO(12): 60-200 BPM");
  Serial.println("  PITCH(13): +/- 24 semitonos desde base (-4 octavas)");
  Serial.println("  VOLUME(14): 0-100%");
  Serial.println("🚀 ¡READY TO MAKE MUSIC!");
}

void loop() {
  readControls();
  handleButtons();
  
  if (isRecording) {
    showRecordingAnimation();
    
    // Grabar con sampling rate apropiado
    if (totalSamples < MAX_SAMPLES) {
      uint16_t sample = analogRead(MIC_PIN);
      audioBuffer[totalSamples++] = sample;
      delayMicroseconds(45); // ~22kHz sampling
    } else {
      stopRecording();
    }
  } else if (isPlaying) {
    playMelodyStep();
  } else {
    updateLEDs();
  }
  
  delayMicroseconds(10);
}