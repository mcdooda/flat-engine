#ifndef FLAT_VIDEO_PROGRAM_H
#define FLAT_VIDEO_PROGRAM_H

#include <map>
#include <string>
#include <vector>
#include "attribute.h"
#include "uniform.h"
#include "window.h"
#include "texture.h"

namespace flat
{
namespace video
{

class Program
{
	public:
		Program();
		virtual ~Program();
		
		void load(std::string fragmentShader, std::string vertexShader);
		
		inline bool isValid() const { return m_valid; }
		
		void use(Window* window);
		
		Attribute getAttribute(std::string attributeName);
		Uniform getUniform(std::string uniformName);
		
		void addInputTexture(const Texture& inputTexture);
		
	protected:
		void checkValid();
		
		GLuint compileProgram(GLuint fragmentShaderId, GLuint vertexShaderId);
		GLuint compileShader(std::string shader, GLuint shaderType);
		
		void checkProgram(GLuint programId);
		void checkShader(std::string shaderFile, GLuint shaderId);
		
		const GLchar* readCode(std::string shader);
		
		void loadAttributes();
		void loadUniforms();
		
	protected:
		GLuint m_programId;
		bool m_valid;
		
		std::map<std::string, Attribute> m_attributes;
		std::map<std::string, Uniform> m_uniforms;
		
		std::vector<Texture> m_inputTextures;
};

} // video
} // flat

#endif // FLAT_VIDEO_PROGRAM_H


