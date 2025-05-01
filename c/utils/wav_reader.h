#ifndef WAV_READER_H
#define WAV_READER_H

#include <stdint.h>

typedef struct {
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint16_t bits_per_sample;
    uint16_t bytes_per_sample;
    uint32_t data_size;
    uint32_t *sample_data;
} WavInfo;

int read_wav_header(const char *filename, WavInfo *info);

#endif