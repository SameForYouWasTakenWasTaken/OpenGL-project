#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos; // Get the position from location 0
layout(location = 1) in vec3 aColor; // Get the color from location 1

out vec3 vertexColor; // Output the vertex color

uniform mat4 model; // CAREFUL! Uniforms must be used at least SOMEWHERE in your code, or else it will crash

void main()
{
    gl_Position = model * vec4(aPos, 1.0); // Add the position and offset it by the model (transform)
    vertexColor = aColor; // Assign the vertexColor, which will be passed over to the fragment shader below.
}

#shader fragment
#version 330 core

in vec3 vertexColor; // Grab the vertexColor from the vertex shader above
out vec4 Color4; // We will output the color

void main()
{
    Color4 = vec4(vertexColor, 1.0f); // Color4 will output now to the screen. :)
}

