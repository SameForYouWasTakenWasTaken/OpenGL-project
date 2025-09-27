#shader fragment
#version 330 core

out vec4 Color4;

void main()
{
    Color4 = vec4(0.1f, 0.5f, 0.5f, 1.0f);
}

#shader vertex
#version 330 core

in vec3 aPos;        // no explicit layout
uniform mat4 model;

void main()
{
    gl_Position = model * vec4(aPos, 1.0);
}
