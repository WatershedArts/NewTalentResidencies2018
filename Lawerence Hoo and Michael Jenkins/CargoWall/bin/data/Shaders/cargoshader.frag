#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect u_main_tex;
uniform sampler2DRect maskTex;

void main (void){
	vec2 pos = gl_TexCoord[0].st;

	vec3 src = texture2DRect(u_main_tex, pos).rgb;
	float mask = texture2DRect(maskTex, pos).r;

	gl_FragColor = vec4( src , mask);
}

//#version 120
//#extension GL_ARB_texture_rectangle : enable
//
//uniform sampler2D 	u_main_tex;
//uniform float 			u_time;
//uniform vec2 			u_resolution;
//
//void main( void ) {
//	vec2 texCoord = gl_FragCoord.xy / u_resolution;
//	vec4 t = texture2D (u_main_tex,texCoord);
//
//	gl_FragColor = vec4(t.r,texCoord.y,0.0,1.0);
//}

//#ifdef GL_ES
//// define default precision for float, vec, mat.
//precision highp float;
//#endif
//
//uniform sampler2D tex0;
//uniform sampler2D maskTex;
//uniform vec4 globalColor;
//
//varying vec2 texCoordVarying;
//
//void main (void)
//{
//	vec2 pos = texCoordVarying;
////	vec3 src = texture2D(tex0, pos).rgb;
////	float mask = texture2D(maskTex, pos).r;
//	gl_FragColor = vec4( pos.x,pos.y,1.0 , 1.0);
//}

