#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_t_coordinate;
in vec3 frag_color;

// Output
//layout(location = 0) out vec4 out_position;
//layout(location = 3) out vec4 out_normal;
//layout(location = 2) out vec3 out_coordinate;
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    //out_color = vec4(1);
//    out_position = vec4(frag_position, 1);
//    out_normal = vec4(frag_normal, 1);
//    out_coordinate = vec3(frag_t_coordinate, 1);
    out_color = vec4(abs(frag_color), 1);

    ////////////task 5//////////////////
    //out_color = vec4(frag_normal, 1);

}
