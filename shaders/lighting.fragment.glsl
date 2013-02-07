#version 150
#extension GL_ARB_explicit_attrib_location : enable

uniform vec3 camera_position;

in vec2 uv;
in vec3 position;
in vec3 normal;

out vec4 FragColor;

vec3 PointLight(vec3 lightPosition, vec3 lightColor, float LightIntensity, float spec, vec3 n)
{
	vec3 l =  lightPosition - position;
	vec3 v = position - camera_position;
	vec3 h = normalize(l-v);
	float n_dot_l = clamp(dot(n, l), 0, 1.0);
	float n_dot_h = clamp(dot(n, h), 0, 1.0);
	float d = length(l);
	float light_coef = 1.0 / (1.0 + (d*d) * 1.5);
	vec3 color = lightColor * light_coef * LightIntensity * (1 * n_dot_l + spec * vec3(1.0, 1.0, 1.0));
	return color;
}

void main(void)
{
	vec3 n = normalize(normal);

	vec3  lightColor = vec3(1.0, 1.0, 1.0);
	vec3  lightPosition = vec3(1.0, 1.0, 3.5);
	float lightIntensity = 4.2;
	float specular = 1.5;
	
	vec3 pl1 = PointLight(lightPosition,lightColor,lightIntensity,specular,n);

	FragColor = vec4(pl1, 1.0);
}
