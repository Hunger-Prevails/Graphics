#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <iostream>
#include <map>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

class TexManager
{
private:
	int count;
public:
	map<int, unsigned int> format;
	map<string, int> number;
	map<string, unsigned int> texID;
public:
	TexManager() {
		count = 0;

		format[1] = GL_RED;
		format[3] = GL_RGB;
		format[4] = GL_RGBA;
	}

	int get_uniform(string name) { return number[name]; }

	unsigned int get_ID(string name) { return texID[name]; }

	void load_texture(string filepath, string name) {

		GLuint texture;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width;
		int height;
		int n_channels;
		auto data = stbi_load(filepath.c_str(), &width, &height, &n_channels, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, format[n_channels], width, height, 0, format[n_channels], GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		number[name] = count ++;
		texID[name] = texture;

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void upload(GLuint program, string name) {

		glActiveTexture(GL_TEXTURE0 + number[name]);
    	glBindTexture(GL_TEXTURE_2D, texID[name]);

    	int sampler_uniform = glGetUniformLocation(program, name.c_str());
    	glUniform1i(sampler_uniform, number[name]);
	}
};