#include <stdio.h>
#include <stdlib.h> // for system() call
#include <cairo.h>
#include "track-a-colour-video-cairo.h"

cairo_surface_t *TmCairoSurface = 0;
cairo_t *TmCairoCanvas = 0;
int TmCairoSizeWidth = 0;
int TmCairoSizeHeight = 0;

// INTERNAL: draw the background
void __tm_cairo_draw_bg() {
	cairo_pattern_t *linearGrad = cairo_pattern_create_linear(0, 0, 0, TmCairoSizeHeight);
	cairo_pattern_add_color_stop_rgb(linearGrad, 0.0, 0.2, 0.2, 0.2);
	cairo_pattern_add_color_stop_rgb(linearGrad, 1.0, 0.0, 0.0, 0.0);
	cairo_rectangle(TmCairoCanvas, 0, 0, TmCairoSizeWidth, TmCairoSizeHeight);
	cairo_set_source(TmCairoCanvas, linearGrad);
	cairo_fill(TmCairoCanvas);
}

void tm_cairo_init(int width, int height) {
	TmCairoSizeWidth = width;
	TmCairoSizeHeight = height;
	//fprintf(stdout, "Building Cairo graphic of dimensions: %dpx x %dpx\n", width, height);
	TmCairoSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
	TmCairoCanvas = cairo_create(TmCairoSurface);
	cairo_set_antialias(TmCairoCanvas, CAIRO_ANTIALIAS_SUBPIXEL);
	__tm_cairo_draw_bg();
	cairo_set_source_rgb(TmCairoCanvas, 1.0, 1.0, 1.0);
	cairo_move_to(TmCairoCanvas, 0.0, 0.0);
}

void tm_cairo_move(int x, int y) {
	if (!TmCairoCanvas) {
		fprintf(stderr, "cairo extenions has not been initialized (call tm_cairo_init(int width, int height))\n");
	}
	cairo_line_to(TmCairoCanvas, (float)x, (float)y);
}

void tm_cairo_release() {
	if (TmCairoCanvas) {
		cairo_set_line_width(TmCairoCanvas, 2);
		cairo_stroke(TmCairoCanvas);
		cairo_destroy(TmCairoCanvas);
		cairo_surface_write_to_png(TmCairoSurface, "bin/output.png");
		cairo_surface_destroy(TmCairoSurface);
	}
}
