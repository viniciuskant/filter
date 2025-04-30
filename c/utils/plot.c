#include <cairo/cairo.h>
#include <stdio.h>
#include <stdlib.h>
#include "plot.h"

#define TICKS_Y 5

static void find_y_min_max(const int *y_values, size_t num_samples, int *y_min, int *y_max) {
    if (!y_values || num_samples == 0 || !y_min || !y_max) return;

    *y_min = y_values[0];
    *y_max = y_values[0];

    for (size_t i = 1; i < num_samples; ++i) {
        if (y_values[i] < *y_min) *y_min = y_values[i];
        if (y_values[i] > *y_max) *y_max = y_values[i];
    }

}

void plot_data(const int *y_values, size_t num_samples, int x_min, int x_max, const char *output_file) {
    if (!y_values) {
        fprintf(stderr, "Error: y_values is NULL.\n");
        return;
    }

    if (num_samples == 0) {
        fprintf(stderr, "Error: num_samples is zero.\n");
        return;
    }

    if (!output_file) {
        fprintf(stderr, "Error: output_file is NULL.\n");
        return;
    }

    if (x_min >= x_max) {
        fprintf(stderr, "Error: x_min is greater than or equal to x_max.\n");
        return;
    }

    size_t max_samples_per_line = (num_samples < MAX_SAMPLES_PER_LINE) ? num_samples : MAX_SAMPLES_PER_LINE;

    int y_min, y_max;
    find_y_min_max(y_values, num_samples, &y_min, &y_max);

    double graph_height = IMAGE_HEIGHT - 2 * GRAPH_PADDING;
    double graph_width = IMAGE_WIDTH - 2 * GRAPH_PADDING;

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, IMAGE_WIDTH, IMAGE_HEIGHT);
    cairo_t *cr = cairo_create(surface);

    // Background
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    // Axes
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 2.0);

    // X-axis
    cairo_move_to(cr, GRAPH_PADDING, IMAGE_HEIGHT - GRAPH_PADDING);
    cairo_line_to(cr, IMAGE_WIDTH - GRAPH_PADDING, IMAGE_HEIGHT - GRAPH_PADDING);
    cairo_stroke(cr);

    // X-axis ticks and labels for multiple lines
    size_t lines = (num_samples + max_samples_per_line - 1) / max_samples_per_line; // Calculate number of lines
    double x_scale = graph_width / max_samples_per_line;

    for (size_t line = 0; line < lines; ++line) {
        size_t start_index = line * max_samples_per_line;
        size_t end_index = (line + 1) * max_samples_per_line;
        if (end_index > num_samples) end_index = num_samples;

        double y_offset = (lines - line - 1) * (graph_height / lines);

        // X-axis ticks and labels for the current line
        size_t x_ticks = 10; // Number of ticks on the X-axis
        double x_tick_spacing = graph_width / (x_ticks - 1);
        for (size_t i = 0; i < x_ticks; ++i) {
            size_t sample_index = start_index + i * (end_index - start_index) / (x_ticks - 1);
            if (sample_index >= end_index) break;

            int x_value = x_min + sample_index * (x_max - x_min) / num_samples;
            double x = GRAPH_PADDING + i * x_tick_spacing;

            cairo_move_to(cr, x, IMAGE_HEIGHT - GRAPH_PADDING - y_offset + 15);
            char label[16];
            snprintf(label, sizeof(label), "%d", x_value);
            cairo_show_text(cr, label);

            cairo_move_to(cr, x, IMAGE_HEIGHT - GRAPH_PADDING - y_offset);
            cairo_line_to(cr, x, IMAGE_HEIGHT - GRAPH_PADDING - y_offset - 5);
        }
        cairo_stroke(cr);
    }

    // Plot data as vertical bars starting from 0 with a limit of max_samples_per_line per line
    cairo_set_source_rgb(cr, 0.1, 0.2, 0.8);

    double y_scale = (graph_height / (y_max - y_min)) / lines;

    for (size_t line = 0; line < lines; ++line) {
        size_t start_index = line * max_samples_per_line;
        size_t end_index = (line + 1) * max_samples_per_line;
        if (end_index > num_samples) end_index = num_samples;

        double y_offset = (lines - line - 1) * (graph_height / lines);

        for (size_t i = start_index; i < end_index; ++i) {
            double x = GRAPH_PADDING + (i - start_index) * x_scale;
            double y = IMAGE_HEIGHT - GRAPH_PADDING - y_offset - (y_values[i] - y_min) * y_scale;
            double bar_height = (y_values[i] - y_min) * y_scale;

            cairo_rectangle(cr, x, y, x_scale * 0.8, bar_height);
            cairo_fill(cr);
        }
    }

    // Export image
    cairo_surface_write_to_png(surface, output_file);

    // Free resources
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}
