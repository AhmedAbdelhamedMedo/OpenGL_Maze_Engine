#version 330

float ka = 0.4;		//Ambient reflection coefficient
float kd = 0.8;		//Diffuse reflection coefficient
float ks = 5.0;		//Specular reflection coefficient
float sh = 50.0;	//Shininess

vec4 light_color = vec4(1, 1, 1, 1);

in vec3 v;
in vec3 n;
in vec3 l;
in vec3 h;

in vec2 UV;

uniform sampler2D Diffuse_texture;

void main() {
    vec3 V = normalize(v);
    vec3 N = normalize(n);
    vec3 L = normalize(l);
    vec3 H = normalize(h);

	vec4 I = (ka * light_color) +
	(kd * max(dot(L,N),0) * light_color) +
	(ks * pow(max(dot(H, N), 0.0), sh) * light_color);

	gl_FragColor = texture(Diffuse_texture, UV) * I;
}