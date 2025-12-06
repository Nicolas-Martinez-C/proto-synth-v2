Este documento explica qué hace cada uno de los 20 códigos para que el usuario sepa cuál cargar.

```markdown
# 💾 Lista de Firmwares y Ejemplos

Aquí encontrarás una colección de 20 códigos listos para usar en el Proto-Synth v2, organizados por categoría.

## 🎵 Canciones y Covers (Demos)
Estos códigos reproducen secuencias famosas permitiendo modificar el sonido en tiempo real.

* **`Giorgio_Moroder`**: Sample del icónico intro de Daft Punk. Incluye secuenciador y controles de síntesis para modificar el timbre de la voz.
* **`idioteque_final` (Radiohead)**: Recreación del tema de Radiohead. Los 4 botones disparan los 4 samples clave que componen la base rítmica y melódica de la canción.
* **`no_necesitamos_banderas` (Los Prisioneros)**: Sintetizador configurado para tocar la línea de bajo de este clásico, con controles para modificar la envolvente y filtros.
* **`on_the_run_pink_floyd`**: Secuenciador generativo que emula el arpegio acelerado del álbum *Dark Side of the Moon*. Los potenciómetros controlan velocidad y síntesis.
* **`sample_mario_bross`**: Graba tu propia voz (sample) y luego reproduce la melodía de Super Mario Bros utilizando tu grabación como instrumento.

## 🎛️ Síntesis y Osciladores
Exploración pura de sonido y ondas.

* **`oscilador_cuantizado`**: Código de introducción. Un potenciómetro selecciona notas afinadas (escalas) y los otros controlan parámetros del timbre.
* **`Oscilador_4_escalas`**: **(Mozzi)** Potente motor de 4 osciladores simultáneos. Usa el sensor de luz (LDR) para abrir o cerrar el filtro de frecuencia, creando efectos wah-wah sin tocar la placa. libreria MOZZI
* **`trance_synth_ldr`**: Secuenciador estilo música Trance donde el corte del filtro (cutoff) es controlado por la cantidad de luz que recibe el sensor LDR.
* **`sequenciador`**: Base de secuenciador por pasos con controles completos de síntesis (ataque, decaimiento, forma de onda).

## 🖐️ Control Gestual (IMU MPU6050)
Estos ejemplos requieren que el chip MPU6050 esté soldado. Usan el movimiento para crear música.

* **`Oscilador_IMU`**: Genera notas cuantizadas (afinadas) al inclinar la placa hacia adelante o atrás.
* **`oscilador_IMU_Continuo`**: Similar al anterior, pero sin cuantizar (tipo Theremin), permitiendo glissandos y sirenas al mover el synth.
* **`emotion_trigger_MIDI`**: Detecta golpes o sacudidas bruscas (perturbaciones) en el aire para disparar notas MIDI aleatorias dentro de 4 escalas seleccionables.
* **`secuencial_imu_MIDI`**: Controlador MIDI secuencial donde la inclinación o movimiento afecta la velocidad o las notas de la secuencia enviada.
* **`trance_IMU`**: Secuenciador de música electrónica donde los filtros y efectos se modulan inclinando el dispositivo en el aire.

## 🎤 Samplers y Grabación
Transforma el Proto-Synth en una grabadora y manipuladora de audio lo-fi.

* **`sample`**: Ejemplo básico. Graba un sonido corto con el micrófono y lo reproduce en bucle con control de Pitch (velocidad) y modo Reversa.
* **`Sample_2`**: Versión avanzada del anterior. Suma secuenciador al sample grabado y más efectos de manipulación.
* **`sample_trance_LDR`**: Graba un sample y lo usa rítmicamente en una secuencia trance, añadiendo control de filtro por luz.
* **`trance_sample`**: Secuenciador híbrido que mezcla síntesis generada con reproducción de samples.

## 👾 Gaming y Utilidades
* **`juego_nave_v2`**: ¡Videojuego! **Nota:** Requiere retirar el MPU6050 e instalar una pantalla OLED I2C en su lugar. Los botones controlan la nave.
* **`Monitoreo_Protosynth`**: Herramienta de diagnóstico. Úsalo para verificar si todos los botones, perillas y sensores están funcionando correctamente (imprime valores en el Monitor Serie).

---
**Nota:** Para cargar cualquiera de estos códigos, recuerda girar el **Potenciómetro 3 al máximo**.