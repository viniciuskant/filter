#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "wav_reader.h"

int read_wav_header(const char *filename, WavInfo *info) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];

    // Read RIFF header
    fread(chunk_id, 1, 4, file);
    fread(&chunk_size, 4, 1, file);
    fread(format, 1, 4, file);

    if (strncmp(chunk_id, "RIFF", 4) != 0 || strncmp(format, "WAVE", 4) != 0) {
        fprintf(stderr, "Arquivo não é um WAV válido.\n");
        fclose(file);
        return -1;
    }

    // Read fmt subchunk
    fread(chunk_id, 1, 4, file);
    fread(&chunk_size, 4, 1, file);

    if (strncmp(chunk_id, "fmt ", 4) != 0) {
        fprintf(stderr, "Subchunk fmt não encontrado.\n");
        fclose(file);
        return -1;
    }

    fread(&info->audio_format, 2, 1, file);
    fread(&info->num_channels, 2, 1, file);
    fread(&info->sample_rate, 4, 1, file);
    fseek(file, 6, SEEK_CUR); // Skip byte rate and block align
    fread(&info->bits_per_sample, 2, 1, file);

    // Read data subchunk
    while (1) {
        fread(chunk_id, 1, 4, file);
        fread(&chunk_size, 4, 1, file);

        if (strncmp(chunk_id, "data", 4) == 0) {
            info->data_size = chunk_size;
            break;
        } else {
            fseek(file, chunk_size, SEEK_CUR); // Skip unknown subchunk
        }
    }

    // Allocate memory for sample data
    info->sample_data = (uint32_t *)malloc(info->data_size);
    if (!info->sample_data) {
        fprintf(stderr, "Erro ao alocar memória para os dados de amostra.\n");
        fclose(file);
        return -1;
    }

    // Calculate the number of samples
    size_t num_samples = info->data_size / (info->bits_per_sample / 8);

    // Allocate memory for sample data
    info->sample_data = (uint32_t *)malloc(num_samples * sizeof(uint32_t));
    if (!info->sample_data) {
        fprintf(stderr, "Erro ao alocar memória para os dados de amostra.\n");
        fclose(file);
        return -1;
    }

    // Read sample data
    for (size_t i = 0; i < num_samples; i++) {
        uint32_t sample = 0;
        fread(&sample, info->bits_per_sample / 8, 1, file);
        info->sample_data[i] = sample;
    }

    fclose(file);
    return 0;
}