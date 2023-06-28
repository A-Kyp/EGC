#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 car_position;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;    


void main()
{
    // TODO(student): Send output to fragment shader
	frag_color		= v_color;
	vec3 world_position = (Model * vec4(v_position, 1.f)).xyz;
	float distance = distance(world_position, car_position);
	world_position.y -= distance * distance * 0.005; 
	gl_Position	= Projection * View * vec4(world_position, 1.0);

}
