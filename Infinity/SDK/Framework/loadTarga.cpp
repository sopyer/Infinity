#include "Framework.h"
//#include <cstdio>
#include <cstring>

/*	Targa header file.  Includes structures and prototypes for Targa code.
	All code by Nutty.
*/

#define		NO_TGA_EXTENSION			1
#define		TGA_FAILED_OPEN				2
#define		TGA_HAS_COLOR_MAP			3
#define		TGA_NOT_24_BIT				4
#define		FAILED_ID_ALLOCATION		5
#define		FAILED_DATA_ALLOCATION		6
#define		TGA_NOT_TYPE_2_OR_10		7
#define		RETURN(code) return 0
#define		FGETC(ptr) *(/*(unsigned char)*/ptr++)

typedef struct __TGA_IMAGE_SPEC__
{
	unsigned short	X_origin;			//Integer (lo-hi) of the X coordinate of the lower left corner of image.
	unsigned short	Y_origin;			//Integer (lo-hi) of the Y coordinate of the lower left corner of image.

	unsigned short	width;				//Integer (lo-hi) of the width of the image in pixels.
	unsigned short	height;				//Integer (lo-hi) of the height of the image in pixels.

	unsigned char	pixel_size;			//Number of bits int he pixel 16 for Targa 16, 24 for targa 24... etc...
	unsigned char	image_descriptor;	//Bits 3-0 - number of attribute bits associated with each  
										//pixel.  For the Targa 16, this would be 0 or   
										//1.  For the Targa 24, it should be 0.  For     
										//Targa 32, it should be 8.                      
										//Bit 4    - reserved.  Must be set to 0.                   
										//Bit 5    - screen origin bit.                             
										//0 = Origin in lower left-hand corner.          
										//1 = Origin in upper left-hand corner.          
										//Must be 0 for Truevision images.               
										//Bits 7-6 - Data storage interleaving flag.                
										//00 = non-interleaved.                          
										//01 = two-way (even/odd) interleaving.          
										//10 = four way interleaving.                    
										//11 = reserved.
}TGA_IMAGE_SPEC;

typedef struct __TGA_HEAD__
{
	unsigned char id_length;		//Length of the id string field.
	unsigned char color_map_type;	//Type of color map included. 0 means no color map.
	unsigned char image_type_code;	//This will always contain 2 or 10, meaning RGB or RLE RGB.

	unsigned char color_map_origin[2];	//Integer (lo-hi) index of 1st color map entry.
	unsigned char color_map_length[2];	//Integer (lo-hi) count of  color map entries.
	unsigned char color_map_size;		//Number of bits in color map entry.  16 for the Targa 16, 24 for the Targa 24, 32 for the Targa 32.                 

	TGA_IMAGE_SPEC		image_info;
}TGA_HEADER;

typedef struct __TGA_FILE__
{
	TGA_HEADER		header;
	unsigned char	data[1];
}TGA_FILE;

//Image descriptor field in targa is not correctly handled
glTexture2D* loadTargaTexture(const char* path)
{
	DataStreamPtr fin;
	int i, bytes_per_pixel;
	unsigned char r, g, b, a;
	unsigned char id, length;
	TGA_FILE	*targa;
	unsigned char*	dataPtr, *imageData;

	if(!(fin = vfsMapFile(path)))
	{
		RETURN(TGA_FAILED_OPEN);
	}
	//read in the header.
	targa = reinterpret_cast<TGA_FILE*>(fin->MapStream());

	//Check a few things out.
	if(targa->header.color_map_type != 0)
	{
		RETURN(TGA_HAS_COLOR_MAP);
	}
	if(targa->header.image_type_code != 2  &&  targa->header.image_type_code != 10)
	{
		RETURN(TGA_NOT_TYPE_2_OR_10);
	}
	//Must be 24 bit images.
	if(targa->header.image_info.pixel_size != 24)
	{
		RETURN(TGA_NOT_24_BIT);
	}

	bytes_per_pixel = targa->header.image_info.pixel_size / 8;

	dataPtr = targa->data;
	if(targa->header.id_length)		//If there is one at all.
	{
		//skip the string id.
		dataPtr += targa->header.id_length;
	}

	//Read in the data,  there is no color map, so the next thing to get should be the data.
	if(targa->header.image_type_code == 2)
	{
		//Just read int the data.
		imageData = dataPtr;
	}
	else
	{
		imageData = new unsigned char[targa->header.image_info.width * targa->header.image_info.height * bytes_per_pixel];
		if(imageData == NULL)
		{
			RETURN(FAILED_DATA_ALLOCATION);
		}
		//RLE decompression.
		for(i=0; i<targa->header.image_info.width*targa->header.image_info.height*bytes_per_pixel; )
		{
			id = FGETC(dataPtr);
			//If bit 7 is set it's a run length data.
			if(id & 128)
			{
				//Find length.
				length = id - 127;		//Remove high bit (128) and add 1.
				//Get the next 3 bytes, which should be the values to duplicate.
				r = FGETC(dataPtr);
				g = FGETC(dataPtr);
				b = FGETC(dataPtr);
				if(bytes_per_pixel == 4)
				{
					a = FGETC(dataPtr);
				}

				//Do a loop to write the number of pels out.
				while(length > 0)
				{
					imageData[i++] = r;
					imageData[i++] = g;
					imageData[i++] = b;
					if(bytes_per_pixel == 4)
					{
						imageData[i++] = a;
					}

					length--;
				}
			}
			else
			{
				//Find number of pixels to get.
				length = id + 1;

				while(length > 0)
				{
					r = FGETC(dataPtr);
					g = FGETC(dataPtr);
					b = FGETC(dataPtr);
					if(bytes_per_pixel == 4)
					{
						a = FGETC(dataPtr);
					}

					imageData[i++] = r;
					imageData[i++] = g;
					imageData[i++] = b;
					if(bytes_per_pixel == 4)
					{
						imageData[i++] = a;
					}

					length--;
				}
			}
		}
	}

	//Upload Texture Here
	glTexture2D*	texture = new glTexture2D(GL_RGB8, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	texture->setImage2D(targa->header.image_info.width,
						targa->header.image_info.height,
						GL_BGR, GL_UNSIGNED_BYTE,
						imageData);
	return texture;
}