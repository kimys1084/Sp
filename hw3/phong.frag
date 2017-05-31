/*
 *  phong.frag
 *  Chapter 13 - Example 01
 *
 *  Created by Graphics and Medea Lab (GML).
 *  Copyright 2010 Seoul National University. All rights reserved.
 *
 */

varying vec3 normal, lightDir, halfVector;
varying float x;
uniform float freq;
uniform int enableStripe;
void main() {
	vec3 n, h;
	float NdotL, NdotH;
	vec4 color = gl_FrontMaterial.ambient*gl_LightSource[0].ambient + gl_LightModel.ambient*gl_FrontMaterial.ambient;
	n = normalize(normal);
	NdotL = max(dot(n,lightDir),0.0);
	if (NdotL > 0.0) {
		color += gl_FrontMaterial.diffuse*gl_LightSource[0].diffuse*NdotL;
		h = normalize(halfVector);
		NdotH = max(dot(n,h),0.0);
		color += gl_FrontMaterial.specular*gl_LightSource[0].specular*pow(NdotH, gl_FrontMaterial.shininess)*(gl_FrontMaterial.shininess+1.0)/(8.0*3.14);
	}
	if (enableStripe != 0) {
		float stripe = sin(freq*x);
		if (stripe < 0.0) stripe = 0.25;
		else stripe = 1.0;
		color.xyz *= stripe;
	}
	gl_FragColor = color;
}