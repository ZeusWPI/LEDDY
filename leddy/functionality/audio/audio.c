
#include <arduinoFFT.h>

arduinoFFT FFT = arduinoFFT();

double realComponent[64];
double imagComponent[64];
int spectralHeight[] = {0b00000000, 0b10000000, 0b11000000,
                        0b11100000, 0b11110000, 0b11111000,
                        0b11111100, 0b11111110, 0b11111111
                       };

int index, c, value;


void renderAudio() {
  for (int i = 0; i < 64; i++) {
    realComponent[i] = analogRead(A3);
    imagComponent[i] = 0;
  }

  FFT.Windowing(realComponent, 64, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(realComponent, imagComponent, 64, FFT_FORWARD);
  FFT.ComplexToMagnitude(realComponent, imagComponent, 64);

  for (int i = 0; i < 32; i++) {
    realComponent[i] = constrain(realComponent[i], 0, 35);
    realComponent[i] = map(realComponent[i], 0, 35, 0, 8);
    index = realComponent[i];
    value = spectralHeight[index];

    c = (31 - i)*3;
    setRow(c>>3, c%8, value);
    c++;
    setRow(c>>3, c%8, value);
    c++;
    setRow(c>>3, c%8, value);
  }
}