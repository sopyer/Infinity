#pragma once

#include <boost\smart_ptr.hpp>
#include <boost\bind.hpp>
#include <boost\mem_fn.hpp>
#include <algorithm>
#include "vfs.h"
#include <map>
#include "texture.h"
#include "pipeline.h"
#include "debug.h"

class Resource
{
	friend class ResourceManager;
public:
	//Acquire/Release
	virtual ~Resource(){Unload();};
protected:
	virtual void Load(DataStreamPtr stream)=0;
	virtual void Unload(){}
};

typedef boost::shared_ptr<Resource> ResourcePtr;

class Image: public Resource
{
protected:
	Texture* tex;
	void Unload(){if( tex )delete tex;}
public:
	virtual Texture* GetTexture(){return tex;};
};

class JpegImage:public Image
{
protected:
	//Texture GetTexture()
	virtual void Load(DataStreamPtr stream)
	{
		tex = new Texture();
		BYTE Bpp8;
		int RetVal;

		JPEG_CORE_PROPERTIES image;
		ZeroMemory( &image, sizeof( JPEG_CORE_PROPERTIES ) );

		if( ijlInit( &image ) != IJL_OK )
		{
			return;
		}

		image.JPGBytes = stream->MapStream();
		image.JPGSizeBytes = stream->GetSize();
		if ((RetVal = ijlRead(&image,IJL_JBUFF_READPARAMS)) == IJL_OK)
		{
			tex->Height = image.JPGHeight;
			tex->Width = image.JPGWidth;
			Bpp8 = 3;
			tex->Bpp = Bpp8*8;
			UINT ImageSize=tex->Height*tex->Width*Bpp8;
			BYTE *img = new BYTE[ImageSize];
			if (img)
			{
				image.DIBBytes = img;
				image.DIBColor = IJL_RGB;
				image.DIBHeight = tex->Height;
				image.DIBWidth = tex->Width;
				image.DIBChannels = Bpp8;

				if ((RetVal = ijlRead(&image,IJL_JBUFF_READWHOLEIMAGE)) == IJL_OK)
				{
					GLuint type;
					switch (tex->Bpp)
					{
						case 24:
							type=GL_RGB;break;
						case 32:
							type=GL_RGBA;break;
					};
					glBindTexture(GL_TEXTURE_2D, tex->TextureID);
				    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
					glTexImage2D(GL_TEXTURE_2D, 0, type, tex->Width, tex->Height, 0, type, GL_UNSIGNED_BYTE, img);
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
			}
			delete img;
		}
		ijlFree(&image);
	}

public:
	JpegImage(){}
};

class Shader: public Resource
{
public:
	ShaderObject* GetShader(){return CompiledShader;}
protected:
	ShaderObject* CompiledShader;
	virtual void Unload(){if( CompiledShader ) delete CompiledShader;}
};

class VertShader: public Shader
{
protected:
	virtual void Load(DataStreamPtr stream)
	{
		CompiledShader = new VertexShader();
		CompiledShader->Compile((char*)stream->MapStream());
		debug(CompiledShader->GetCompilerLog().c_str());
	}
public:
	VertShader(){}
};

class FragShader: public Shader
{
protected:
	virtual void Load(DataStreamPtr stream)
	{
		CompiledShader = new FragmentShader();
		CompiledShader->Compile((char*)stream->MapStream());
		debug(CompiledShader->GetCompilerLog().c_str());
	}
public:
	FragShader(){}
};

class Factory
{
public:
	static Resource* Create(std::string type)
	{
		if(type=="frag")
			return new FragShader();
		else if( type=="vert" )
			return new VertShader();
		else if( type=="jpeg" )
			return new JpegImage();
		return 0;
	}
};

class ResourceManager
{
public:
	ResourceManager(VFS& vfs):m_VFS(vfs)
	{
	}

	void AddResource(std::string name, std::string type, std::string file)
	{
		MapType::iterator it=m_ResMap.find(name);
		if( it==m_ResMap.end())
		{
			m_ResMap[name]=ResourcePtr(Factory::Create(type));
			m_NameMap[name]=file;
		}
	}

	void LoadAll()
	{
		MapType::iterator it=m_ResMap.begin(), end=m_ResMap.end();
		while( it!=end )
		{
			if( m_VFS.Exist(m_NameMap[it->first]) )
				it->second->Load(m_VFS.Open(m_NameMap[it->first]));
			else
				debug("Can't load resource \"%s\" at file \"%s\"",it->first.c_str(), m_NameMap[it->first].c_str());
			it++;
		}
	}

	void UnloadAll()
	{
		MapType::iterator it=m_ResMap.begin(), end=m_ResMap.end();
		while( it!=end )
		{
			it->second->Unload();
			it++;
		}
	}

	ResourcePtr GetResource(std::string name)
	{
		MapType::iterator it=m_ResMap.find(name);
		if( it!=m_ResMap.end())
			return it->second;
		return ResourcePtr();
	}

private:
	VFS&	m_VFS;
	typedef std::map<std::string, ResourcePtr> MapType;		
	typedef std::map<std::string, std::string> NMapType;		
	MapType m_ResMap;
	NMapType m_NameMap;
};