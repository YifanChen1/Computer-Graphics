#version 330 core

in vec3 fragPos;
in vec3 fragColor;
in vec3 n;

uniform vec3 light;
uniform sampler2D sampler;

out vec4 color;

void main() {
	vec3 lightDir = normalize(light - fragPos);
    vec3 normal = normalize(n);
    float diff = max(dot(lightDir, normal), 0.0);
	vec4 d = texture(sampler, tc);
	if(d.a < 0.01) {discard;}
	color = vec4(d * (diff + 0.01f)s);
}
