#include "Framework.h"
#include <jpeg/jpeglib.h>

// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This stream is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

void init_source (j_decompress_ptr cinfo)
{
	// DO NOTHING
}

boolean fill_input_buffer (j_decompress_ptr cinfo)
{
	// DO NOTHING
	return 1;
}

void skip_input_data (j_decompress_ptr cinfo, long count)
{
	jpeg_source_mgr * src = cinfo->src;
	if(count > 0)
	{
		src->bytes_in_buffer -= count;
		src->next_input_byte += count;
	}
}

void resync_to_restart (j_decompress_ptr cinfo, long desired)
{
	// DO NOTHING
}

void term_source (j_decompress_ptr cinfo)
{
	// DO NOTHING
}

glTexture2D* loadJpegTexture(const char* path)
{
	DataStreamPtr stream = vfsMapFile(path);
	if(!stream)
	{
		return 0;
	}
//	stream->seek(0);
	unsigned char* input = stream->MapStream();
	//stream->read(input, stream->getSize());

	struct jpeg_decompress_struct cinfo;

	// allocate and initialize JPEG decompression object
	struct jpeg_error_mgr jerr;

	//We have to set up the error handler first, in case the initialization
	//step fails.  (Unlikely, but it could happen if you are out of memory.)
	//This routine fills in the contents of struct jerr, and returns jerr's
	//address which we place into the link field in cinfo.
	
	cinfo.err = jpeg_std_error(&jerr);
	// Now we can initialize the JPEG decompression object. 
	jpeg_create_decompress(&cinfo);

	// specify data source
	jpeg_source_mgr jsrc;

	// Set up data pointer
	jsrc.bytes_in_buffer = stream->GetSize();
	jsrc.next_input_byte = (JOCTET*)input;
	cinfo.src = &jsrc;

	jsrc.init_source = init_source;
	jsrc.fill_input_buffer = fill_input_buffer;
	jsrc.skip_input_data = skip_input_data;
	jsrc.resync_to_restart = jpeg_resync_to_restart;
	jsrc.term_source = term_source;

	// Decodes JPG input from whatever source
	// Does everything AFTER jpeg_create_decompress
	// and BEFORE jpeg_destroy_decompress
	// Caller is responsible for arranging these + setting up cinfo

	// read stream parameters with jpeg_read_header() 
	(void) jpeg_read_header(&cinfo, TRUE);

	// Start decompressor
	(void) jpeg_start_decompress(&cinfo);

	// Get image data
	unsigned short rowspan = cinfo.image_width * cinfo.num_components;
	unsigned width = cinfo.image_width;
	unsigned height = cinfo.image_height;

	bool has_alpha= false;  //(JPEG never has alpha)
	bool greyscale;

	if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
		greyscale = true;
	else
		greyscale = false;

	if ( greyscale )
	{
		//delete [] input;
		jpeg_destroy_decompress(&cinfo);
		print("Greyscale .jpg textures are not supported, please just convert that stream.");
		return 0;
	}

	// Allocate memory for buffer
	unsigned char *output = new unsigned char[rowspan * height];

	// Here we use the library's state variable cinfo.output_scanline as the
	// loop counter, so that we don't have to keep track ourselves.
	// Create array of row pointers for lib
	unsigned char **rowPtr = new unsigned char * [height];

	for( unsigned i = 0; i < height; i++ )
		rowPtr[i] = &output[ i * rowspan ];

	unsigned rowsRead = 0;

	while( cinfo.output_scanline < cinfo.output_height )
		rowsRead += jpeg_read_scanlines( &cinfo, &rowPtr[rowsRead], cinfo.output_height - rowsRead );

	delete [] rowPtr;
	// Finish decompression

	(void) jpeg_finish_decompress(&cinfo);

	// Release JPEG decompression object

	// This is an important step since it will release a good deal of memory.
	jpeg_destroy_decompress(&cinfo);


	// convert image
	 
	glTexture2D *image = new glTexture2D(GL_RGB8, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	image->setImage2D(width, height, GL_RGB, GL_UNSIGNED_BYTE, output);
	
	//delete [] input;
	
	return image;
}