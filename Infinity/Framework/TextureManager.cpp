#include <TextureManager.h>
#include <..\vfs\vfs.h>
#include <SOIL.h>

gl::Texture2D* TextureManager::load(const std::string& name)
{
	gl::Texture2D *image = new glTexture2D(GL_RGB8, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

	DataStreamPtr fin;

	if(!(fin = vfsMapFile(name.c_str())))
	{
		return 0;
	}

	int	imgWidth, imgHeight, imgChannels;
	unsigned char*	pixelsPtr = SOIL_load_image_from_memory(fin->MapStream(), fin->GetSize(),
		&imgWidth, &imgHeight, &imgChannels, SOIL_LOAD_RGBA);

	image->setImage2D(imgWidth, imgHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixelsPtr);
	
	SOIL_free_image_data(pixelsPtr);
	
	return image;
}