
/*#include "../UberLame_src/NewFix.h"
#include "../UberLame_src/CallStack.h"*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <exception>
#include <new>

#include "PNGLoad.h"

#if defined(_MSC_VER) && !defined(__MWERKS__) && !defined(for)
#define for if(0) {} else for
#endif

#define PNG_EXPORT(a,b) a b
extern "C" {
#include "png.h"
}

/*
 *								=== CPngCodec::CInternal ===
 */

class CPngCodec::CInternal {
public:
	struct TDataBuffer {
		const uint8_t *p_ptr, *p_end;

		inline TDataBuffer(const void *_p_ptr, size_t n_size)
			:p_ptr((const uint8_t*)_p_ptr), p_end(((const uint8_t*)_p_ptr) + n_size)
		{}
	};

public:
	static TBmp *p_Load_PNG_int(png_structp p_png_ptr);
	static bool GetInfo_int(TImageInfo &r_t_info, png_structp p_png_ptr);

	static void ReadData_Callback(png_structp p_png_ptr, png_bytep p_out_buffer,
	   png_size_t n_byte_count_to_read);
	static void png_my_error(png_structp p_png_ptr, png_const_charp p_s_message);
	static void png_my_warning(png_structp p_png_ptr, png_const_charp p_s_message);
};


void CPngCodec::CInternal::ReadData_Callback(png_structp p_png_ptr, png_bytep p_out_buffer,
   png_size_t n_byte_count_to_read)
{
	if(p_png_ptr->io_ptr == NULL) {
		throw new std::exception("png reader error: p_png_ptr->io_ptr == NULL");
		return;
	}
	// make sure the reader object was specified

	TDataBuffer &r_buffer = *(TDataBuffer*)(p_png_ptr->io_ptr);
	if(r_buffer.p_ptr + n_byte_count_to_read > r_buffer.p_end) {
		throw new std::exception("png reader error: insufficient data");
		return;
	}
	// make sure there's enough data

	memcpy(p_out_buffer, r_buffer.p_ptr, n_byte_count_to_read);
	r_buffer.p_ptr += n_byte_count_to_read;
	// "read" the data
}

void CPngCodec::CInternal::png_my_error(png_structp p_png_ptr, png_const_charp p_s_message)
{
	throw new std::exception(p_s_message);
}

void CPngCodec::CInternal::png_my_warning(png_structp p_png_ptr, png_const_charp p_s_message)
{
#ifdef _DEBUG
	printf("libpng warning: %s - %s\n", p_s_message, (char*)png_get_error_ptr(p_png_ptr)); // debug
#endif // _DEBUG
}

bool CPngCodec::CInternal::GetInfo_int(TImageInfo &r_t_info, png_structp p_png_ptr)
{
	png_infop p_png_info;
	if(!(p_png_info = png_create_info_struct(p_png_ptr))) {
		png_destroy_read_struct(&p_png_ptr, NULL, NULL);
		return 0;
	}

	png_infop p_end_info;
	if(!(p_end_info = png_create_info_struct(p_png_ptr))) {
		png_destroy_read_struct(&p_png_ptr, &p_png_info, NULL);
		return 0;
	}

	unsigned long n_width, n_height;
	int n_palette_entry_num, n_orig_bit_depth, n_color_type;
	try {
		png_read_info(p_png_ptr, p_png_info);

		int n_interlace_type;
		png_get_IHDR(p_png_ptr, p_png_info, &n_width, &n_height,
			&n_orig_bit_depth, &n_color_type, &n_interlace_type, NULL, NULL);
		// get image header

		n_palette_entry_num = 0;
		if(n_color_type == PNG_COLOR_TYPE_PALETTE) {
			png_get_PLTE(p_png_ptr, p_png_info, 0, &n_palette_entry_num);
			n_orig_bit_depth = 8; // palettes are always stored as 24bit RGB (8 bit / pixel)
		}

		if(png_get_valid(p_png_ptr, p_png_info, PNG_INFO_tRNS)) {
			png_set_palette_to_rgb(p_png_ptr); // can't have palette & alpha, have to convert to RGB
			png_set_tRNS_to_alpha(p_png_ptr);
		}
		// transparency to alpha

		n_color_type = png_get_color_type(p_png_ptr, p_png_info);
		// update new values

		//png_read_end(p_png_ptr, NULL); // fires exception, maybe it needs png_read_image() to be called
		// note this doesn't perform any cleanup, it just 
	} catch(std::exception &exc) {
		fprintf(stderr, "error: %s\n", exc.what());
		png_destroy_read_struct(&p_png_ptr, &p_png_info, &p_end_info);
		return 0;
	}
	// read the png from a file

	r_t_info.b_alpha = (n_color_type & PNG_COLOR_MASK_ALPHA) != 0;
	r_t_info.b_grayscale = (n_color_type == PNG_COLOR_TYPE_GRAY || n_color_type == PNG_COLOR_TYPE_GRAY_ALPHA);
	r_t_info.n_bit_depth = n_orig_bit_depth * (((r_t_info.b_grayscale)? 1 : 3) + ((r_t_info.b_alpha)? 1 : 0));
	r_t_info.n_width = n_width;
	r_t_info.n_height = n_height;
	r_t_info.n_palette_entry_num = n_palette_entry_num;
	// set bitmap properties

	png_destroy_read_struct(&p_png_ptr, &p_png_info, &p_end_info);
	// free scanlines

	return true;
}

TBmp *CPngCodec::CInternal::p_Load_PNG_int(png_structp p_png_ptr)
{
	png_infop p_png_info;
	if(!(p_png_info = png_create_info_struct(p_png_ptr))) {
		png_destroy_read_struct(&p_png_ptr, NULL, NULL);
		return 0;
	}

	png_infop p_end_info;
	if(!(p_end_info = png_create_info_struct(p_png_ptr))) {
		png_destroy_read_struct(&p_png_ptr, &p_png_info, NULL);
		return 0;
	}

	unsigned long n_width, n_height;
	int n_bit_depth, n_orig_bit_depth, n_color_type;
	png_bytep *p_scanline_list;
	try {
		png_read_info(p_png_ptr, p_png_info);

		int n_interlace_type;
		png_get_IHDR(p_png_ptr, p_png_info, &n_width, &n_height,
			&n_bit_depth, &n_color_type, &n_interlace_type, NULL, NULL);
		n_orig_bit_depth = n_bit_depth;
		// get image header

		if(!(p_scanline_list = new(std::nothrow) png_bytep[n_height])) {
			png_destroy_read_struct(&p_png_ptr, &p_png_info, &p_end_info);
			return 0;
		}
		// alloc scanline list

		for(unsigned long i = 0; i < n_height; i ++) {
			p_scanline_list[i] = (png_bytep)png_malloc(p_png_ptr,
				png_get_rowbytes(p_png_ptr, p_png_info));	
		}
		png_read_image(p_png_ptr, p_scanline_list);
		// decompress image

		if(n_color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(p_png_ptr);
		// don't want palette

		if(n_color_type == PNG_COLOR_TYPE_GRAY && n_bit_depth < 8)
			png_set_gray_1_2_4_to_8(p_png_ptr);
		// gray -> 8 bits

		if(png_get_valid(p_png_ptr, p_png_info, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(p_png_ptr);
		// transparency to alpha

		if(n_bit_depth == 16)
			png_set_strip_16(p_png_ptr);
		// 16 bit -> 8 bit

		n_bit_depth = png_get_bit_depth(p_png_ptr, p_png_info);
		n_color_type = png_get_color_type(p_png_ptr, p_png_info);
		// update new values

		switch(n_color_type) {
		case PNG_COLOR_TYPE_PALETTE: // (bit depths 1, 2, 4, 8)
			break;
		case PNG_COLOR_TYPE_GRAY:
		case PNG_COLOR_TYPE_RGB_ALPHA:
		case PNG_COLOR_TYPE_GRAY_ALPHA:
		case PNG_COLOR_TYPE_RGB:
			if(n_bit_depth == 8) // make sure we have 8bits per pixel
				break;
			// this case intentionally falls trough
		default:
			for(unsigned long i = 0; i < n_height; i ++)
				png_free(p_png_ptr, p_scanline_list[i]);	
			delete[] p_scanline_list;
			png_destroy_read_struct(&p_png_ptr, &p_png_info, &p_end_info);
			return 0;
		}
		// see what kind of color do we have

		png_read_end(p_png_ptr, p_png_info);
	} catch(std::exception &exc) {
		fprintf(stderr, "error: %s\n", exc.what());
		png_destroy_read_struct(&p_png_ptr, &p_png_info, &p_end_info);
		return 0;
	}
	// read the png from a file

	TBmp *p_bmp;
	if(!(p_bmp = new(std::nothrow) TBmp))
		return 0;
	if(!(p_bmp->p_buffer = new(std::nothrow) uint32_t[n_width * n_height])) {
		delete p_bmp;
		for(unsigned long i = 0; i < n_height; i ++)
			png_free(p_png_ptr, p_scanline_list[i]);	
		delete[] p_scanline_list;
		png_destroy_read_struct(&p_png_ptr, &p_png_info, &p_end_info);
		return 0;
	}
	// alloc bitmap

	p_bmp->b_alpha = (n_color_type & PNG_COLOR_MASK_ALPHA) != 0;
	p_bmp->b_grayscale = (n_color_type == PNG_COLOR_TYPE_GRAY || n_color_type == PNG_COLOR_TYPE_GRAY_ALPHA);
	p_bmp->n_former_bpp = n_orig_bit_depth;
	p_bmp->n_width = n_width;
	p_bmp->n_height = n_height;
	// set bitmap properties

	uint32_t *p_buffer = p_bmp->p_buffer;
	switch(n_color_type) {
	case PNG_COLOR_TYPE_GRAY:
		{
			for(unsigned long y = 0; y < n_height; ++ y) {
				for(unsigned long x = 0; x < n_width; ++ x, ++ p_buffer) {
					uint32_t n_gray = (uint32_t)(p_scanline_list[y])[x];
					*p_buffer = 0xff000000 | n_gray | (n_gray << 8) | (n_gray << 16);
				}
			}
		}
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		{
			for(unsigned long y = 0; y < n_height; y ++) {
				uint8_t *p_scanline = (uint8_t*)p_scanline_list[y];
				for(unsigned long x = 0; x < n_width; x ++, p_buffer ++, p_scanline += 4)
					*p_buffer = p_scanline[0] | (p_scanline[1] << 8) | (p_scanline[2] << 16) | (p_scanline[3] << 24);
			}
		}
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		{
			for(unsigned long y = 0; y < n_height; y ++) {
				uint8_t *p_scanline = (uint8_t*)p_scanline_list[y];
				for(unsigned long x = 0; x < n_width; x ++, p_buffer ++, p_scanline += 2) {
					*p_buffer = p_scanline[0] | (p_scanline[0] << 8) | (p_scanline[0] << 16) | (p_scanline[1] << 24);
				}
			}
		}
		break;
	case PNG_COLOR_TYPE_RGB:
		{
			for(unsigned long y = 0; y < n_height; y ++) {
				uint8_t *p_scanline = (uint8_t*)p_scanline_list[y];
				for(unsigned long x = 0; x < n_width; x ++, p_buffer ++, p_scanline += 3) {
					*p_buffer = 0xff000000 | p_scanline[2] | (p_scanline[1] << 8) | (p_scanline[0] << 16);
				}
			}
		}
		break;
	case PNG_COLOR_TYPE_PALETTE:
		if(n_bit_depth == 8) {
			for(unsigned long y = 0; y < n_height; y ++) {
				uint8_t *p_scanline = (uint8_t*)p_scanline_list[y];
				for(unsigned long x = 0; x < n_width; x ++, p_buffer ++, p_scanline ++) {
					*p_buffer = 0xff000000 | (p_png_ptr->palette[*p_scanline].red << 16) |
						(p_png_ptr->palette[*p_scanline].green << 8) |
						(p_png_ptr->palette[*p_scanline].blue);
				}
			}
		} else if(n_bit_depth == 4) {
			for(unsigned long y = 0; y < n_height; y ++) {
				uint8_t *p_scanline = (uint8_t*)p_scanline_list[y];
				for(unsigned long x = 0; x < n_width; x ++, p_buffer ++, p_scanline += (x & 1)) {
					uint8_t n_pal_entry = (*p_scanline >> (4 * (x & 1))) & 0xf;
					*p_buffer = 0xff000000 | (p_png_ptr->palette[n_pal_entry].red << 16) |
						(p_png_ptr->palette[n_pal_entry].green << 8) |
						(p_png_ptr->palette[n_pal_entry].blue);
				}
			}
		} else if(n_bit_depth == 2) {
			for(unsigned long y = 0; y < n_height; y ++) { // todo - find such an image! it's untested
				uint8_t *p_scanline = (uint8_t*)p_scanline_list[y];
				for(unsigned long x = 0; x < n_width; x ++, p_buffer ++, p_scanline += !(x & 3)) {
					uint8_t n_pal_entry = (*p_scanline >> (2 * (x & 3))) & 0x3;
					*p_buffer = 0xff000000 | (p_png_ptr->palette[n_pal_entry].red << 16) |
						(p_png_ptr->palette[n_pal_entry].green << 8) |
						(p_png_ptr->palette[n_pal_entry].blue);
				}
			}
		} else if(n_bit_depth == 1) {
			for(unsigned long y = 0; y < n_height; y ++) {
				uint8_t *p_scanline = (uint8_t*)p_scanline_list[y];
				for(unsigned long x = 0; x < n_width; x ++, p_buffer ++, p_scanline += !(x & 7)) {
					uint8_t n_pal_entry = (*p_scanline >> (7 - (x & 7))) & 1;
					*p_buffer = 0xff000000 | (p_png_ptr->palette[n_pal_entry].red << 16) |
						(p_png_ptr->palette[n_pal_entry].green << 8) |
						(p_png_ptr->palette[n_pal_entry].blue);
				}
			}
		}
		break;
	}
	// copy the image (convert to ARGB8)

	for(unsigned long i = 0; i < n_height; i ++)
		png_free(p_png_ptr, p_scanline_list[i]);	
	delete[] p_scanline_list;
	png_destroy_read_struct(&p_png_ptr, &p_png_info, &p_end_info);
	// free scanlines

	return p_bmp;
}

/*
 *								=== ~CPngCodec::CInternal ===
 */

/*
 *								=== CPngCodec ===
 */

bool CPngCodec::Get_ImageInfo(TImageInfo &r_t_info, const char *p_s_filename)
{
	memset(&r_t_info, 0, sizeof(TImageInfo));
	// clear first

	FILE *p_fr;
#if defined(_MSC_VER) && !defined(__MWERKS__) && _MSC_VER >= 1400
	if(fopen_s(&p_fr, p_s_filename, "rb"))
#else //_MSC_VER && !__MWERKS__ && _MSC_VER >= 1400
	if(!(p_fr = fopen(p_s_filename, "rb")))
#endif //_MSC_VER && !__MWERKS__ && _MSC_VER >= 1400
		return false;
	// open input file

	png_structp p_png_ptr;
	if(!(p_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
	   &CPngCodec::CInternal::png_my_error, &CPngCodec::CInternal::png_my_warning))) {
		fclose(p_fr);
		return 0;
	}
	// create png reader

	png_init_io(p_png_ptr, p_fr);
	// read from here

	bool b_result = CInternal::GetInfo_int(r_t_info, p_png_ptr);
	// get info

	fclose(p_fr);
	// close the file

	return b_result;
}

bool CPngCodec::Get_ImageInfo(TImageInfo &r_t_info, const void *p_data, size_t n_size)
{
	memset(&r_t_info, 0, sizeof(TImageInfo));
	// clear first

	png_structp p_png_ptr;
	if(!(p_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
	   &CPngCodec::CInternal::png_my_error, &CPngCodec::CInternal::png_my_warning)))
		return 0;
	// create png reader

	CPngCodec::CInternal::TDataBuffer t_data(p_data, n_size);
	png_set_read_fn(p_png_ptr, &t_data, CPngCodec::CInternal::ReadData_Callback);
	// read from here

	return CInternal::GetInfo_int(r_t_info, p_png_ptr);
	// get info
}

TBmp *CPngCodec::p_Load_PNG(const char *p_s_filename)
{
	FILE *p_fr;
#if defined(_MSC_VER) && !defined(__MWERKS__) && _MSC_VER >= 1400
	if(fopen_s(&p_fr, p_s_filename, "rb"))
#else //_MSC_VER && !__MWERKS__ && _MSC_VER >= 1400
	if(!(p_fr = fopen(p_s_filename, "rb")))
#endif //_MSC_VER && !__MWERKS__ && _MSC_VER >= 1400
		return 0;
	// open input file

	png_structp p_png_ptr;
	if(!(p_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
	   &CPngCodec::CInternal::png_my_error, &CPngCodec::CInternal::png_my_warning))) {
		fclose(p_fr);
		return 0;
	}
	// create png reader

	png_init_io(p_png_ptr, p_fr);
	// read from here

	TBmp *p_result = CInternal::p_Load_PNG_int(p_png_ptr);
	// load png

	fclose(p_fr);
	// close the file

	return p_result;
}

TBmp *CPngCodec::p_Load_PNG(const void *p_data, size_t n_size)
{
	png_structp p_png_ptr;
	if(!(p_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
	   &CPngCodec::CInternal::png_my_error, &CPngCodec::CInternal::png_my_warning)))
		return 0;
	// create png reader

	CPngCodec::CInternal::TDataBuffer t_data(p_data, n_size);
	png_set_read_fn(p_png_ptr, &t_data, CPngCodec::CInternal::ReadData_Callback);
	// read from here

	return CInternal::p_Load_PNG_int(p_png_ptr);
	// load png
}

/*
 *								=== ~CPngCodec ===
 */
