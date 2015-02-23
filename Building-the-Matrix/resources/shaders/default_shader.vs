#version 330
uniform mat4 matProjection;
uniform mat4 matModelView;

attribute vec4 a_position;
attribute vec4 a_colour;
attribute vec2 a_texCoord;

varying vec2 v_texCoord;
out vec4 colour;
void main()
{
	gl_Position = matProjection * matModelView * a_position;
	v_texCoord = a_texCoord;
	colour = a_colour;
}