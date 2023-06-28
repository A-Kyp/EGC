#version 330

// Input
// TODO(student): Get vertex attributes from each location

layout(location = 0) in vec3 vertex_position;
layout(location = 3) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coordinate;
layout(location = 1) in vec3 vertex_color;


///////////////task 5//////////////////////
//layout(location = 0) in vec3 vertex_position;
//layout(location = 1) in vec3 vertex_normal;
//layout(location = 2) in vec2 texture_coordinate;
//layout(location = 1) in vec3 vertex_color;

///////////////task 6/////////////////////
//layout(location = 0) in vec3 vertex_position;
//layout(location = 1) in vec3 vertex_color;
//layout(location = 2) in vec2 texture_coordinate;
//layout(location = 3) in vec3 vertex_normal;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_t_coordinate;
out vec3 frag_color;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_position = vertex_position;
    frag_normal = vertex_normal;
    frag_t_coordinate = texture_coordinate;
    frag_color = vertex_color;

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(vertex_position, 1.0);

}
