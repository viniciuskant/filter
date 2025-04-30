#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils/wav_reader.h"
#include <fftw3.h>
#include <math.h>
#include "utils/plot.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s file.wav\n", argv[0]);
        return 1;
    }

    WavInfo info;
    if (read_wav_header(argv[1], &info) != 0) {
        return 1;
    }

    printf("Audio Format: %u\n", info.audio_format);
    printf("Channels: %u\n", info.num_channels);
    printf("Sample Rate: %u Hz\n", info.sample_rate);
    printf("Bytes per Sample: %u\n", info.bytes_per_sample);
    printf("Data Size: %u bytes\n", info.data_size);

    // Allocate memory for FFT input and output
    int num_samples = info.data_size / info.bytes_per_sample;
    fftw_complex *in = fftw_malloc(sizeof(fftw_complex) * num_samples);
    fftw_complex *out = fftw_malloc(sizeof(fftw_complex) * num_samples);
    fftw_plan plan = fftw_plan_dft_1d(num_samples, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    // Prepare input data for FFT
    for (int i = 0; i < num_samples; i++) {
        in[i][0] = (double)info.sample_data[i]; // Real part
        in[i][1] = 0.0;                        // Imaginary part
    }

    // Execute FFT
    fftw_execute(plan);

    // Print FFT results (magnitude)
    int prev_frequency = -1;
    int frequency;
    int magnitude;
    int* frequency_bins = (int*)malloc(num_samples / 2 * sizeof(int));
    int count = 0;
    printf("Frequency Domain Representation:\n");
    for (int i = 0; i < num_samples / 2; i++) { // Only show positive frequencies
        magnitude = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
        frequency = i * info.sample_rate / num_samples;
        if (frequency != prev_frequency) {
            printf("%d Hz: %d\n", frequency, magnitude);
            frequency_bins[count] = magnitude / 1000;
            count++;
        }
        prev_frequency = frequency;
    }

    // Clean up
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    int deslocamento = 1000;

    plot_data(frequency_bins + deslocamento, count, 0, count - deslocamento, "Magnitude");

    return 0;
}
