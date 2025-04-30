#ifndef PLOT_H
#define PLOT_H

#include <cairo/cairo.h>
#include <stddef.h>

#define IMAGE_WIDTH 1920
#define IMAGE_HEIGHT 1080
#define MAX_SAMPLES_PER_LINE 7100
#define GRAPH_PADDING 50
#define LINE_SPACING 0 

/**
 * @brief Plots the given data as a graph and saves it to a PNG file.
 * 
 * @param y_values Array of y-axis values to plot.
 * @param num_samples Number of samples in the y_values array.
 * @param x_min Minimum value of the x-axis.
 * @param x_max Maximum value of the x-axis.
 * @param output_file Path to the output PNG file.
 */
void plot_data(const int *y_values, size_t num_samples, int x_min, int x_max, const char *output_file);

#endif // PLOT_H