#include "ErrorReporting.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

//https://learnopengl.com/In-Practice/Debugging

void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

	spdlog::error("---------------");
	spdlog::error("Debug message ({}): {}", id, message);

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             spdlog::error("Source: API"); break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   spdlog::error("Source: Window System"); break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: spdlog::error("Source: Shader Compiler"); break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     spdlog::error("Source: Third Party"); break;
		case GL_DEBUG_SOURCE_APPLICATION:     spdlog::error("Source: Application"); break;
		case GL_DEBUG_SOURCE_OTHER:           spdlog::error("Source: Other"); break;
	}

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               spdlog::error("Type: Error"); break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: spdlog::error("Type: Deprecated Behaviour"); break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  spdlog::error("Type: Undefined Behaviour"); break; 
		case GL_DEBUG_TYPE_PORTABILITY:         spdlog::error("Type: Portability"); break;
		case GL_DEBUG_TYPE_PERFORMANCE:         spdlog::error("Type: Performance"); break;
		case GL_DEBUG_TYPE_MARKER:              spdlog::error("Type: Marker"); break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          spdlog::error("Type: Push Group"); break;
		case GL_DEBUG_TYPE_POP_GROUP:           spdlog::error("Type: Pop Group"); break;
		case GL_DEBUG_TYPE_OTHER:               spdlog::error("Type: Other"); break;
	}
	
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         spdlog::error("Severity: high"); break;
		case GL_DEBUG_SEVERITY_MEDIUM:       spdlog::error("Severity: medium"); break;
		case GL_DEBUG_SEVERITY_LOW:          spdlog::error("Severity: low"); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: spdlog::error("Severity: notification"); break;
	}
	spdlog::error("");
}

void enableReportGlErrors()
{
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	//glDebugMessageCallback(0, nullptr);
}