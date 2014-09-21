#include "pass.h"

namespace flat
{
namespace video
{

Pass::Pass() : Program()
{
	
}

Pass::~Pass()
{
	
}

void Pass::load(FrameBuffer* frameBuffer, std::string fragmentShader, std::string vertexShader)
{
	m_frameBuffer = frameBuffer;
	Program::load(fragmentShader, vertexShader);
}

void Pass::use()
{
	m_frameBuffer->use();
	glUseProgram(m_programId);
}

} // video
} // flat


