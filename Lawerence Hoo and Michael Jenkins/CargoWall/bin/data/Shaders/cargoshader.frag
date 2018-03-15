#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect u_main_tex;
uniform sampler2DRect maskTex;

void main (void){
	vec2 pos = gl_TexCoord[0].st;

	// Get the Texture from the screen and return all the colors
	vec3 src = texture2DRect(u_main_tex, pos).rgb;
	
	// Return the Masks Red channel as the alpha pass through
	float mask = texture2DRect(maskTex, pos).r;

	gl_FragColor = vec4( src , mask);
}
