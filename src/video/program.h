#ifndef FLAT_VIDEO_PROGRAM_H
#define FLAT_VIDEO_PROGRAM_H

#include <map>
#include <string>
#include <vector>
#include <iostream>
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
		Program(const Program&) = delete;
		Program(Program&&) = delete;
		virtual ~Program();
		void operator=(const Program&) = delete;
		
		void load(const std::string& fragmentShader, const std::string& vertexShader);
		
		inline bool isValid() const { return m_valid; }
		
		void use(const Window& window) const;
		void use(const Vector2& size) const;
		
		Attribute getAttribute(const std::string& attributeName) const;

		template <class T>
		Uniform<T> getUniform(const std::string& uniformName) const
		{
			std::map<std::string, GLint>::const_iterator it = m_uniforms.find(uniformName);

			if (it != m_uniforms.end())
			{
				return Uniform<T>(it->second);
			}
			else
			{
				std::cerr << "Warning: uniform '" << uniformName << "' does not exist in '" << m_fragmentShader << "' and '" << m_vertexShader << "'" << std::endl;
				return Uniform<T>(0);
			}
		}
		
		void addInputTexture(const Texture& inputTexture);

		inline GLuint getProgramId() const { return m_programId; }

		inline std::uint32_t getHash() const { return static_cast<std::uint32_t>(m_programId); }
		
	protected:
		void assertValid() const;
		
		GLuint compileProgram(GLuint fragmentShaderId, GLuint vertexShaderId);
		GLuint compileShader(const std::string& shader, GLuint shaderType);
		
		void checkProgram(GLuint programId);
		void checkShader(const std::string& shaderFile, GLuint shaderId);
		
		void readCode(const std::string& shader, std::string& code);
		
		void loadAttributes();
		void loadUniforms();
		
	protected:
		std::map<std::string, GLint> m_uniforms;
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


