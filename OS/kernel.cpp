#include <stddef.h>
#include <stdint.h>

#include "io.h"
#include "vga.h"
#include "gfx.h"
#include "vector.h"
#include "raytracer.h"


extern "C" void kernel_main() {
	vga_init();
	
	uint8_t greyscale_palette[256*3];
	for (int i = 0; i < 256; i++) {
		uint8_t brightness = (uint8_t)(((float)i*255.0f) / 255.0f);
		
		greyscale_palette[(i*3) + 0] = brightness;
		greyscale_palette[(i*3) + 1] = brightness;
		greyscale_palette[(i*3) + 2] = brightness;
	}
	
	gfx_set_palette(greyscale_palette);
	
	
	Vector camera_eye(0,8,0);
	Vector camera_target(0,0,0);
	Vector camera_up(0,0,1);
	
	clear();
	
	render(camera_eye, camera_target, camera_up);
	
	flush();
}
