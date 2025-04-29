#include <stdint.h>
#include <stdio.h>
#include "utils/wav_reader.h"

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
    printf("Bits per Sample: %u\n", info.bits_per_sample);
    printf("Data Size: %u bytes\n", info.data_size);

    for (uint32_t i = 0; i < info.data_size / sizeof(uint32_t); i++) {
        printf("Sample %06d: %06x\n", i, info.sample_data[i]);
    }

    return 0;
}
