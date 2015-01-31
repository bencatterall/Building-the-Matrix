#version 330
uniform mat4 matProjection;
uniform mat4 matModelView;

attribute vec4 a_position;
//attribute vec2 a_texCoord;

//varying vec2 v_texCoord;

void main()
{
	gl_Position = matProjection * matModelView * a_position;
//v_texCoord = a_texCoord;
}