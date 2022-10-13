/*
Type your name and student ID here
	- Name: Ng Ching Yi
	- Student ID: 1155149111
*/

#version 430

out vec4 Color;
in vec3 theColor;

void main()
{
	Color = vec4(theColor, 1.0);
}
