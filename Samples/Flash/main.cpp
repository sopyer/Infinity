#include <cstdio>
#include <memory>
#include <cassert>
#include <zlib.h>
#include <set>

typedef unsigned int	uint32;
typedef unsigned char	uint8;

typedef unsigned int	u32;
typedef signed int		s32;
typedef unsigned short	u16;
typedef signed int		s16;
typedef unsigned char	u8;
typedef signed char		s8;

#include <utils/ml.h>

void parseTag(u32 tagType)
{
	switch (tagType)
	{
		case 0:
			printf("EndTag\n");
			break;
		case 1:
			printf("ShowFrameTag\n");
			break;
		case 2:
			printf("DefineShapeTag\n");
			break;
		case 6:
			printf("DefineBits\n");
			break;
		case 8:
			printf("JPEGTables\n");
			break;
		case 9:
			printf("SetBackgroundColorTag\n");
			break;
		case 10:
			printf("DefineFontTag\n");
			break;
		case 11:
			printf("DefineTextTag\n");
			break;
		case 12:
			printf("DoActionTag\n");
			break;
		case 13:
			printf("DefineFontInfoTag\n");
			break;
		case 14:
			printf("DefineSoundTag\n");
			break;
		case 15:
			printf("StartSoundTag\n");
			break;
		case 17:
			printf("DefineButtonSound\n");
			break;
		case 18:
			printf("SoundStreamHeadTag\n");
			break;
		case 19:
			printf("SoundStreamBlockTag\n");
			break;
		case 20:
			printf("DefineBitsLosslessTag\n");
			break;
		case 21:
			printf("DefineBitsJPEG2Tag\n");
			break;
		case 22:
			printf("DefineShape2Tag\n");
			break;
		case 24:
			printf("ProtectTag\n");
			break;
		case 26:
			printf("PlaceObject2Tag\n");
			break;
		case 28:
			printf("RemoveObject2Tag\n");
			break;
		case 32:
			printf("DefineShape3Tag\n");
			break;
		case 34:
			printf("DefineButton2Tag\n");
			break;
		case 35:
			printf("DefineBitsJPEG3\n");
			break;
		case 36:
			printf("DefineBitsLossless2Tag\n");
			break;
		case 37:
			printf("DefineEditTextTag\n");
			break;
		case 39:
			printf("DefineSpriteTag\n");
			break;
		case 41:
			printf("ProductInfoTag\n");
			break;
		case 43:
			printf("FrameLabelTag\n");
			break;
		case 45:
			printf("SoundStreamHead2Tag\n");
			break;
		case 46:
			printf("DefineMorphShapeTag\n");
			break;
		case 48:
			printf("DefineFont2Tag\n");
			break;
		case 56:
			printf("ExportAssetsTag\n");
			break;
		case 58:
			printf("EnableDebuggerTag\n");
			break;
		case 59:
			printf("DoInitActionTag\n");
			break;
		case 60:
			printf("DefineVideoStreamTag\n");
			break;
		case 62:
			printf("DefineFontInfo2\n");
			break;
		case 63:
			printf("DebugIDTag\n");
			break;
		case 64:
			printf("EnableDebugger2Tag\n");
			break;
		case 65:
			printf("ScriptLimitsTag\n");
			break;
		case 69:
			printf("FileAttributesTag\n");
			//FileAttributes tag is mandatory on version>=8 and must be the first tag
			break;
		case 70:
			printf("PlaceObject3Tag\n");
			break;
		case 73:
			printf("DefineFontAlignZonesTag\n");
			break;
		case 74:
			printf("CSMTextSettingsTag\n");
			break;
		case 75:
			printf("DefineFont3Tag\n");
			break;
		case 76:
			printf("SymbolClassTag\n");
			break;
		case 77:
			printf("MetadataTag\n");
			break;
		case 78:
			printf("DefineScalingGridTag\n");
			break;
		case 82:
			printf("DoABCTag\n");
			break;
		case 83:
			printf("DefineShape4Tag\n");
			break;
		case 86:
			printf("DefineSceneAndFrameLabelDataTag\n");
			break;
		case 87:
			printf("DefineBinaryDataTag\n");
			break;
		case 88:
			printf("DefineFontNameTag\n");
			break;
		default:
			printf("UnsupportedTag 0x%x(%d)\n", tagType, tagType);
	}
}

struct MemReader
{
	const char*	start;
	const char*	end;
	const char*	cur;

	void set(uint32 offset) {cur = start+offset;assert(cur<=end);}

	void move(uint32 offset) {cur += offset; assert(cur<=end);}

	template<typename T>
	const T& read() { const char* ptr=cur; move(sizeof(T)); return *((const T*)ptr);}

	template<typename T>
	const T* as() {return (const T*)cur;}

	template<typename T>
	const T* as(uint32 offset) {return (const T*)(start+offset);}
};

int main()
{
	//Transpose
	vec4 mat[4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};

	__m128i xmm0 = _mm_unpacklo_epi32(_mm_castps_si128(mat[0]), _mm_castps_si128(mat[1]));
	__m128i xmm1 = _mm_unpackhi_epi32(_mm_castps_si128(mat[0]), _mm_castps_si128(mat[1]));
	__m128i xmm2 = _mm_unpacklo_epi32(_mm_castps_si128(mat[2]), _mm_castps_si128(mat[3]));
	__m128i xmm3 = _mm_unpackhi_epi32(_mm_castps_si128(mat[2]), _mm_castps_si128(mat[3]));

	vec4 trans[4];

	trans[0] = _mm_castsi128_ps(_mm_unpacklo_epi64(xmm0, xmm2));
	trans[1] = _mm_castsi128_ps(_mm_unpackhi_epi64(xmm0, xmm2));
	trans[2] = _mm_castsi128_ps(_mm_unpacklo_epi64(xmm1, xmm3));
	trans[3] = _mm_castsi128_ps(_mm_unpackhi_epi64(xmm1, xmm3));

	vec4 trans2[4];

	ml::transpose(trans2, mat);

	FILE* file = fopen("..\\..\\AppData\\VT.swf", "rb");
	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	fseek(file, 0, SEEK_SET);
	unsigned char* fileData = (unsigned char*)malloc(size);
	fread(fileData, 1, size, file);
	fclose(file);

	MemReader data = {(const char*)fileData, (const char*)fileData+size, (const char*)fileData};
	
	//Read SWF header
	const u32 signatureAndVersion	= data.read<u32>();
	const u32 actualSize			= data.read<u32>();

	u32 signature = signatureAndVersion&0x00FFFFFF;
	u8	version = signatureAndVersion>>24;

	bool isCompressed   = signature=='\0SWC';
	bool isUncompressed = signature=='\0SWF';

	//if !isCompressed && !isUncompressed return error;

	MemReader data2 = {0, 0, 0};

	char* uncompressed = 0;
	if (isCompressed)
	{
		 uncompressed = (char*)malloc(actualSize-8);
		 data2.cur = data2.start = uncompressed;
		 data2.end = uncompressed+actualSize-8;
		 uLongf uncompressedSize = actualSize-8;
		 uncompress((Bytef*)uncompressed, &uncompressedSize, data.as<Bytef>(), size-8);
	}
	else if (isCompressed)
	{
		data2.cur = data2.start = data.as<char>();
		data2.end = data2.start+actualSize-8;
	}

	
	u8 bits = data2.read<u8>();
	u8 numBits = bits>>3;

	u32 rectSizeMinusOne = (numBits*4+5)>>3;
	data2.move(rectSizeMinusOne);
	
	const u16 frameRate	 = data2.read<u16>();
	const u16 frameCount = data2.read<u16>();

	std::set<u32>	tagsUsed;
	size_t tagCount = 0;

	while (data2.cur!=data2.end)
	{
		u16 tagHeader = data2.read<u16>();
		u32 tagLength = tagHeader&0x3F;
		u32 tagType = tagHeader>>6;
		tagsUsed.insert(tagType);

		if (tagLength==0x3F)
			tagLength = data2.read<u32>();
		data2.move(tagLength);

		parseTag(tagType);

		++tagCount;
	}

	if (uncompressed) free(uncompressed);

	printf("\nProcessed %d tags\n\n", tagCount);

	printf("        Tags used        \n");
	printf("-------------------------\n");

	std::set<u32>::iterator	it  = tagsUsed.begin(),
							end = tagsUsed.end();

	for (; it!=end; ++it)
	{
		parseTag(*it);
	}

	free(fileData);
}