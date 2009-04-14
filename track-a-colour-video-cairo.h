
// called to create a cairo 2d canvas
void tm_cairo_init(int width, int height);

// called to plot out a co-ordinate on the cairo 2d canvas
void tm_cairo_move(int x, int y);

// called to write the canvas to png & release the cairo resources from memory
void tm_cairo_release();
