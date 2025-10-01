#pragma once
#include <string>
#include <fstream>
#include <sstream>

#include <spdlog/spdlog.h>
#include <glad/glad.h>

static void display_error(int succ, GLuint ID, const unsigned int buf_len, char* buf, const char* type)
{
    if (!succ)
    {
        glGetProgramInfoLog(ID, buf_len, nullptr, buf);
        spdlog::error("Shader Program {} Error: {}", type, buf);
    }
}


struct ShaderSources {
    std::string VertexSource;
    std::string FragmentSource;
};

inline ShaderSources ParseShaderFile(const std::string& path) {
    std::ifstream file(path);


    // Couldn't open file
    if (!file) {
        throw std::runtime_error("Failed to open shader file: " + path);
    }

    // Temporary enums
    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
    ShaderType type = ShaderType::NONE;

    std::stringstream ss[2];
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        } else {
            if (type != ShaderType::NONE)
                ss[static_cast<int>(type)] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

class Shader {
    GLuint vertexShader;
    GLuint fragmentShader;
    void Create(const char* vertexSrc, const char* fragmentSrc)
    {
        // Create the program
        ID = glCreateProgram();
        // Create the shaders
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        
        // Add the vertex and fragment paths to the shaders
        glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
        glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
        // Compile the shaders
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        // Attach the shaders to the program
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        
        // Bind attrib locations
        glBindAttribLocation(ID, 0, "aPos");  
        glBindAttribLocation(ID, 1, "aColor");  
        
        // Link the program
        glLinkProgram(ID);
        
        GLint program_success;
        GLint vert_success;
        GLint frag_success;
        
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vert_success);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &frag_success);
        glGetProgramiv(ID, GL_LINK_STATUS, &program_success);
        
        
        if (vert_success != GL_TRUE || frag_success != GL_TRUE || program_success != GL_TRUE)
        {
            char program[512], vert[512], frag[512];
            glGetShaderInfoLog(vertexShader, 512, nullptr, vert);
            glGetShaderInfoLog(fragmentShader, 512, nullptr, frag);
            glGetProgramInfoLog(ID, 512, nullptr, program);
            spdlog::error("Vertex Shader: {}\nFragment Shader: {}\nProgram Link: {}", vert, frag, program);
        }
        // Delete the shaders as they're no longer necessary
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
public:
    GLuint ID;
    GLint locProjection;
    GLint locView;
    void CacheUniforms()
    {
        Use();
        locProjection = glGetUniformLocation(ID, "projection");
        locView = glGetUniformLocation(ID, "view");
    }
    Shader(const char* vertexSrc, const char* fragmentSrc)
    {
        Create(vertexSrc, fragmentSrc);
    }
    void Use() 
    {
        if (glIsProgram(ID))
            glUseProgram(ID); 
    }
    void Delete() 
    { 
        glDeleteProgram(ID); 
    }
    void Unuse()
    {
        glUseProgram(0);
    }
    bool Available()
    {
        if (ID > 0)
        {
            return true;
        }

        return false;
    }
    void Update(const char* vertexPath, const char* fragmentPath)
    {
        Delete();
        Create(vertexPath, fragmentPath);
        Use();
    }
};