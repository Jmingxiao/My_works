// triangle.fs.txt

#version 330

uniform sampler2D u_diffuse;

in  vec2 f_texcoord;

out vec4 frag_color;

void main() {
	vec4 textColor = texture(u_diffuse, f_texcoord);
	frag_color = textColor;
}
