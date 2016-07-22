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
		
		void load(const std::string& fragmentShader, const std::string& vertexShader);
		
		inline bool isValid() const { return m_valid; }
		
		void use(Window* window);
		
		Attribute getAttribute(const std::string& attributeName);
		Uniform getUniform(const std::string& uniformName);
		
		void addInputTexture(const Texture& inputTexture);
		
	protected:
		void assertValid();
		
		GLuint compileProgram(GLuint fragmentShaderId, GLuint vertexShaderId);
		GLuint compileShader(const std::string& shader, GLuint shaderType);
		
		void checkProgram(GLuint programId);
		void checkShader(const std::string& shaderFile, GLuint shaderId);
		
		void readCode(const std::string& shader, std::string& code);
		
		void loadAttributes();
		void loadUniforms();
		
	protected:
		std::map<std::string, Uniform> m_uniforms;
		std::map<std::string, Attribute> m_attributes;
		
		std::vector<Texture> m_inputTextures;
		
		std::string m_fragmentShader;
		std::string m_vertexShader;
		
		GLuint m_programId;
		bool m_valid;
};

} // video
} // flat

#endif // FLAT_VIDEO_PROGRAM_H


