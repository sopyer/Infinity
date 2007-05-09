/**
 * \ingroup glClasses
 * Клас glTexture.
 * Цей клас є базовим для представлення текстур
 * та містить їх характеристики: тип, дескриптор,
 * опис внутрішнього представлення, а також задає
 * атоматичну генерацію mipmap рівнів та параметри 
 * фільтрування.
**/
class glTexture
{
	public:
		/**
		 * Конструктор.
		 * \param target тип текстури, як це вказується в OpenGL
		 * \param internalFormat задає формат внутрішнього представлення зображення
		 * \param minFilter задає фільтр, що використовується при зменшенні
		 * \param magFilter задає фільтр, що використовується при збільшенні
		 * \param forceMipmap вказує, чи генерувати примусово рівні деталізації
		**/
		glTexture(GLenum target, GLint internalFmt, GLint minFilter, GLint magFilter, bool forceMipmap=true)
		{
			internalFormat_ = internalFmt;
			target_ = target;
			glGenTextures(1, &handle_);
			glBindTexture(target, handle_);
			glTexParameteri(target, GL_GENERATE_MIPMAP, forceMipmap);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);

		}
		/**
		 * Деструктор
		**/
		~glTexture(void){glDeleteTextures(1, &handle_);}
		
	public:
		/**
		 * Тип текстури.
		**/
		GLenum	target_;
		/**
		 * Дескриптор текстури.
		**/
		GLuint	handle_;
		/**
		 * Внутрішній формат текстури.
		**/
		GLint	internalFormat_;
};

/**
 * \ingroup glClasses
 * Клас двовимірної текстури.
 * Містить методи для роботи з цим типом текстур:
 * Встановлення параметрів розширення координат, а
 * також зображення текстури.
**/
class glTexture2D: public glTexture
{
	public:
		/**
		 * Конструктор.
		 * \param internalFormat задає формат внутрішнього представлення зображення
		 * \param minFilter задає фільтр, що використовується при зменшенні
		 * \param magFilter задає фільтр, що використовується при збільшенні
		 * \param forceMipmap вказує, чи генерувати примусово рівні деталізації
		**/
		glTexture2D(GLint internalFmt, GLint minFilter, GLint magFilter, bool forceMipmap=true):
		  glTexture(GL_TEXTURE_2D, internalFmt, minFilter, magFilter, forceMipmap){}

		/**
		 * Задає спосіб розширення текстурних координат за межі [0..1]
		 * \param wrapS розширення по осі S(горизонтальна)
		 * \param wrapT розширення по осі T(вертикальна)
		**/
		void setWrapMode(GLint wrapS, GLint wrapT)
		{
			glBindTexture(target_, handle_);
			glTexParameteri(target_, GL_TEXTURE_WRAP_S, wrapS);
			glTexParameteri(target_, GL_TEXTURE_WRAP_T, wrapT);
		}

		/**
		 * Задає зображення текстури
		 * \param width ширина зображення
		 * \param height висота зображення
		 * \param format формат зображення - тип та кількість каналів
		 * \param type тип елементу каналу
		 * \param pixels вказівник на дані
		**/
		void setImage2D(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)//maybe this is not usefullas
		{
			glBindTexture(target_, handle_);
			glTexImage2D(target_, 0, internalFormat_, width, height, 0, format, type, pixels);
		}
};