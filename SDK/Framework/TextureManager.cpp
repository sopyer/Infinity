#include <TextureManager.h>
#include <..\vfs++\vfs++.h>
#include <SOIL.h>

//gl::Texture2D* TextureManager::load(const std::string& name)
//{
//	//Texture format - be careful!!! Can cause troubles
//	gl::Texture2D *image = new gl::Texture2D(GL_RGBA8, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
//
//	//DataStreamPtr fin;
//	File	src = VFS::openRead(name.c_str());
//	//if(!(fin = vfsMapFile(name.c_str())))
//	//{
//	//	return 0;
//	//}
//
//	unsigned char* data = new unsigned char[src.size()+1];
//
//	src.read(data, src.size(), 1);
//
//	int	imgWidth, imgHeight, imgChannels;
//	unsigned char*	pixelsPtr = SOIL_load_image_from_memory(data, src.size(),
//		&imgWidth, &imgHeight, &imgChannels, SOIL_LOAD_RGBA);
//
//	image->setImage2D(imgWidth, imgHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixelsPtr);
//	
//	SOIL_free_image_data(pixelsPtr);
//	delete [] data;
//
//	return image;
//}


bool testLoad(GLuint texture)
{
	float x, y;

	glBindTexture(GL_TEXTURE_2D, texture);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &x);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &y);

	return x!=0 && y!=0;
}

void loadTexture(const char* name, GLuint texture)
{
	//Texture format - be careful!!! Can cause troubles
	//gl::Texture2D *image = new gl::Texture2D(GL_RGBA8, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

	//DataStreamPtr fin;
	File	src = VFS::openRead(name);
	//if(!(fin = vfsMapFile(name.c_str())))
	//{
	//	return 0;
	//}

	unsigned char* data = new unsigned char[src.size()+1];

	src.read(data, src.size(), 1);

	int	imgWidth, imgHeight, imgChannels;
	unsigned char*	pixelsPtr = SOIL_load_image_from_memory(data, src.size(),
		&imgWidth, &imgHeight, &imgChannels, SOIL_LOAD_RGBA);

	//image->setImage2D(imgWidth, imgHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixelsPtr);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsPtr);
	
	assert(testLoad(texture));

	SOIL_free_image_data(pixelsPtr);
	delete [] data;
}