#include "Framework.h"
#include <png\png.h>
#include <cstring>

#define		RETURN(code) return 0

void user_read_data_fcn(png_structp png_ptr,png_bytep data, png_size_t length) 
{ 
	png_size_t check; 

	DataStream* input=(DataStream*)png_ptr->io_ptr; 
	check = input->Read(data, length);
	//memcpy();
	//   check=(png_size_t) file->read((void*)data,length); 

	if (check != length) 
		 png_error(png_ptr, "Read Error"); 
} 

glTexture2D* loadPngTexture(const char* path)
{
	DataStreamPtr stream = vfsMapFile(path);
	if(!stream)	{RETURN("Could not open the file");}

	//unsigned char* input = stream->MapStream();
	png_uint_32 png_width, png_height;
	int bit_depth, color_type, interlace_type;
	unsigned int width, height, type, format, internalFormat;

	png_structp png_ptr = png_create_read_struct(
	  PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (png_ptr == NULL)
	{
		RETURN("Error creating png read struct");
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		RETURN("Error creating png info struct");
	}

	if (setjmp(png_ptr->jmpbuf))
	{
		/* Free all of the memory associated with the png_ptr and info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		RETURN("Error reading file");
	}

	png_set_read_fn(png_ptr, stream.get(), user_read_data_fcn); 

	/* The call to png_read_info() gives us all of the information from the
	* PNG file before the first IDAT (image data chunk).  REQUIRED
	*/
	png_read_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, &png_width, &png_height, &bit_depth,
				&color_type, &interlace_type, NULL, NULL);
	width = png_width;
	height = png_height;

	/* Extract multiple pixels with bit depths of 1, 2, and 4 from a single
	* byte into separate bytes (useful for paletted and grayscale images).
	*/
	png_set_packing(png_ptr);

	/* Expand paletted colors into true RGB triplets */
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_expand(png_ptr);

	/* Expand grayscale images to the full 8 bits from 1, 2, or 4 bits/pixel */
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand(png_ptr);

	/* Expand paletted or RGB images with transparency to full alpha channels
	* so the data will be available as RGBA quartets.
	*/
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_expand(png_ptr);

	/* Tell libpng to handle the gamma conversion for you.  The final call
	* is a good guess for PC generated images, but it should be configurable
	* by the user at run time by the user.  It is strongly suggested that
	* your application support gamma correction.
	*/
	double screen_gamma = 2.0;
	int intent;
	if (png_get_sRGB(png_ptr, info_ptr, &intent))
		png_set_gamma(png_ptr, screen_gamma, 0.45455);
	else
	{
		double image_gamma;
		if (png_get_gAMA(png_ptr, info_ptr, &image_gamma))
			png_set_gamma(png_ptr, screen_gamma, image_gamma);
		else
			png_set_gamma(png_ptr, screen_gamma, 0.45455);
	}

	/* Optional call to gamma correct and add the background to the palette
	* and update info structure.  REQUIRED if you are expecting libpng to
	* update the palette for you (ie you selected such a transform above).
	*/
	png_read_update_info(png_ptr, info_ptr);

	/* Allocate the memory to hold the image using the fields of info_ptr. */
	unsigned int size = width * height;

	if (info_ptr->bit_depth == 8)
	{
		type = GL_UNSIGNED_BYTE;
	}
	else if (info_ptr->bit_depth == 16)
	{
		type = GL_UNSIGNED_SHORT;
		size *= sizeof(unsigned short);
	}
	else
	{
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		RETURN("Invalid bit depth");
	}

	switch (info_ptr->color_type)
	{
		case PNG_COLOR_TYPE_GRAY:
			format = GL_LUMINANCE;
			internalFormat = GL_LUMINANCE8;
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			format = GL_LUMINANCE_ALPHA;
			internalFormat = GL_LUMINANCE8_ALPHA8;
			size *= 2;
			break;
		case PNG_COLOR_TYPE_RGB:
			format = GL_RGB;
			internalFormat = GL_RGB8;
			size *= 3;
			break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			format = GL_RGBA;
			internalFormat = GL_RGBA8;
			size *= 4;
			break;
		default:
			png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
			RETURN("Unknown color type");
	}

	if (height*info_ptr->rowbytes != size)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		RETURN("Invalid rowbytes");
	}

	png_bytep image = new png_byte[size];//(png_bytep)malloc(size);

	png_bytep *row_ptrs = new png_bytep[height*sizeof(png_bytep)];//(png_bytep*)malloc(height*sizeof(png_bytep));
	for (unsigned int i=0; i<height; ++i)
	{
		//flip???????
		//row_ptrs[i] = &image[(height-i-1)*info_ptr->rowbytes];
		row_ptrs[i] = &image[i*info_ptr->rowbytes];
	}

	/* read the image */
	png_read_image(png_ptr, row_ptrs);

	//free(row_ptrs);
	delete [] row_ptrs;

	/* clean up after the read, and free any memory allocated - REQUIRED */
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

	glTexture2D *texture = new glTexture2D(internalFormat, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	texture->setImage2D(width, height, format, type, image);
	
	delete [] image;

	return texture;
}