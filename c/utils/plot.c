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
    if (!y_values || !output_file || num_samples == 0 || x_min >= x_max) {
        fprintf(stderr, "Invalid input data.\n");
        return;
    }

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

    // X-axis ticks and labels
    size_t x_ticks = 10; // Number of ticks on the X-axis
    double x_tick_spacing = graph_width / (x_ticks - 1);
    for (size_t i = 0; i < x_ticks; ++i) {
        int x_value = x_min + i * (x_max - x_min) / (x_ticks - 1);
        double x = GRAPH_PADDING + i * x_tick_spacing;

        cairo_move_to(cr, x, IMAGE_HEIGHT - GRAPH_PADDING + 15);
        char label[16];
        snprintf(label, sizeof(label), "%d", x_value);
        cairo_show_text(cr, label);

        cairo_move_to(cr, x, IMAGE_HEIGHT - GRAPH_PADDING);
        cairo_line_to(cr, x, IMAGE_HEIGHT - GRAPH_PADDING - 5);
    }
    cairo_stroke(cr);

    // Plot data as vertical bars starting from 0
    cairo_set_source_rgb(cr, 0.1, 0.2, 0.8);

    double x_scale = graph_width / num_samples;
    double y_scale = graph_height / (y_max - y_min);

    for (size_t i = 0; i < num_samples; ++i) {
        double x = GRAPH_PADDING + i * x_scale;
        double y = IMAGE_HEIGHT - GRAPH_PADDING - (y_values[i] - y_min) * y_scale;
        double bar_height = (y_values[i] - y_min) * y_scale;

        cairo_rectangle(cr, x, y, x_scale * 0.8, bar_height);
        cairo_fill(cr);
    }

    // Export image
    cairo_surface_write_to_png(surface, output_file);

    // Free resources
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}
