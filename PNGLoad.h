/**
 *	@file PNGLoad.h
 *	@date 2011
 *	@author Corthezz
 *	@brief A simple PNG image loader.
 */

#ifndef __PNG_LOAD_INCLUDED
#define __PNG_LOAD_INCLUDED

#include "Tga.h"

class CPngCodec {
public:
	/**
	 *	@brief information about a PNG image
	 */
	struct TImageInfo {
		bool b_alpha; /**< @brief alpha channel flag */
		bool b_grayscale; /**< @brief grayscale flag */
		int n_bit_depth; /**< @brief number of bits per pixel (or per palette color) */
		int n_palette_entry_num; /**< @brief number of palette entries or 0 for true color */
		int n_width; /**< @brief image width */
		int n_height; /**< @brief image height */
	};

protected:
	/*static TBmp *p_Load_PNG_int(png_structp p_png_ptr);
	static bool GetInfo_int(TImageInfo &r_t_info, png_structp p_png_ptr);*/
	class CInternal;

public:
	/**
	 *	@brief reads image header from a file and returns image properties
	 *
	 *	@param[out] r_t_info is structure to be filled with image information
	 *	@param[in] p_s_filename is input file name
	 *
	 *	@return Returns true on success, false on failure.
	 */
	static bool Get_ImageInfo(TImageInfo &r_t_info, const char *p_s_filename);

	/**
	 *	@brief reads image header from memory and returns image properties
	 *
	 *	@param[out] r_t_info is structure to be filled with image information
	 *	@param[in] p_data is image data
	 *	@param[in] n_size is image data size, in bytes
	 *
	 *	@return Returns true on success, false on failure.
	 */
	static bool Get_ImageInfo(TImageInfo &r_t_info, const void *p_data, size_t n_size);

	/**
	 *	@brief loads jpeg image from file
	 *
	 *	@param[in] p_s_filename is input file name
	 *
	 *	@return Returns pointer to loaded bitmap on success, or 0 on failure.
	 */
	static TBmp *p_Load_PNG(const char *p_s_filename);

	/**
	 *	@brief loads jpeg image from memory
	 *
	 *	@param[in] p_data is image data
	 *	@param[in] n_size is image data size, in bytes
	 *
	 *	@return Returns pointer to loaded bitmap on success, or 0 on failure.
	 */
	static TBmp *p_Load_PNG(const void *p_data, size_t n_size);
};

#endif //__PNG_LOAD_INCLUDED
