#include "pch.h"
#include "WWI.h"
#include <cstdio>
#include "png.h "
#include "jpeglib.h"
#include <iostream>
#include <setjmp.h>

struct my_error_mgr {
	struct jpeg_error_mgr pub;	/* "public" fields */

	jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef  struct my_error_mgr* my_error_ptr;

unsigned char* myPixels;
int Iheight;
int Iwidth;

void my_error_exit(j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	my_error_ptr myerr = (my_error_ptr)cinfo->err;

	/* Always display the message. */
	/* We could postpone this until after returning, if we chose. */
	(*cinfo->err->output_message) (cinfo);

	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);
}

FILE* infile;

int read_PNG_file(const char* filename) {
	std::cout << 'f' << std::endl;
	int sig_read = 0;
	png_structp png_ptr;
	png_infop info_ptr;
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;

	fopen_s(&infile, filename, "rb");

	if (infile == NULL) {
		//std::cout << "can't open %s" << std::endl;
		return 0;
	}
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
		NULL, NULL, NULL);

	//png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
	//	png_voidp user_error_ptr, user_error_fn, user_warning_fn);

	if (png_ptr == NULL)
	{
		fclose(infile);
		return 0;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fclose(infile);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return 0;
	}

	//if (setjmp(png_jmpbuf(png_ptr)))
	//{
	//	/* Free all of the memory associated with the png_ptr and info_ptr */
	//	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	//	fclose(infile);
	//	/* If we get here, we had a problem reading the file */
	//	return 0;
	//}

	png_init_io(png_ptr, infile);

	png_read_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		&interlace_type, NULL, NULL);

	// Read any color_type into 8bit depth, RGBA format.

	if (bit_depth == 16)
		png_set_strip_16(png_ptr);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png_ptr);

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	// These color_type don't have an alpha channel then fill it with 0xff.
	if (color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);

	if (color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);

	if (color_type & PNG_COLOR_MASK_COLOR)
		png_set_bgr(png_ptr);

	png_read_update_info(png_ptr, info_ptr);


	png_bytep* row_pointers;
	row_pointers = new png_bytep[height];

	/* Clear the pointer array */
	for (int row = 0; row < height; row++)
		row_pointers[row] = NULL;

	for (int row = 0; row < height; row++)
		row_pointers[row] = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));

	png_read_image(png_ptr, row_pointers);
	myPixels = new unsigned char[width * bit_depth * height];
	png_read_end(png_ptr, info_ptr);
	int tek = 0;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width*4; i++) {
			myPixels[tek] = row_pointers[j][i];
			tek++;
			/*myPixels[tek] = (buffer[0][i * 3 + 2]);
			tek++;
			myPixels[tek] = (buffer[0][i * 3 + 1]);
			tek++;
			myPixels[tek] = (buffer[0][i * 3 + 0]);
			tek++;
			myPixels[tek] = 255;
			tek++;*/
		}

		//put_scanline_someplace(buffer[0], );
	}
	Iheight = height;
	Iwidth = width;

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	fclose(infile);

	delete[] row_pointers;
	return 1;
}

int read_JPEG_file(const char* filename)
{
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	JSAMPARRAY buffer;
	fopen_s(&infile, filename, "rb");
	if (infile == NULL) {
		std::cout << "can't open %s" << std::endl;
		return 0;
	}
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return 0;
	}
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);
	/*(void)*/jpeg_start_decompress(&cinfo);
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, cinfo.output_width * cinfo.output_components, 1);

	Iwidth = cinfo.output_width;
	Iheight = cinfo.output_height;

	myPixels = new unsigned char[cinfo.output_width * 4 * cinfo.output_height];
	int tek = 0;
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, buffer, 1);

		for (int i = 0; i < cinfo.output_width; i++) {
			myPixels[tek] = (buffer[0][i * 3 + 2]);
			tek++;
			myPixels[tek] = (buffer[0][i * 3 + 1]);
			tek++;
			myPixels[tek] = (buffer[0][i * 3 + 0]);
			tek++;
			myPixels[tek] = 255;
			tek++;
		}

		//put_scanline_someplace(buffer[0], );
	}
	/*(void)*/jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	fclose(infile);

	return 1;
}

unsigned char* load_image(const char* filename, int& width, int& height) {
	if (read_PNG_file(filename)) {
		width = Iwidth;
		height = Iheight;
		return myPixels;
	}
	else
		return NULL;
}