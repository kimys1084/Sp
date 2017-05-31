/*
 *  phong.vert
 *  Chapter 13 - Example 01
 *
 *  Created by Graphics and Medea Lab (GML).
 *  Copyright 2010 Seoul National University. All rights reserved.
 *
 */

varying vec3 normal, lightDir, halfVector;
varying float x;
void main() {
	normal = normalize(gl_NormalMatrix*gl_Normal);
	lightDir = normalize(gl_LightSource[0].position.xyz);
	halfVector = normalize(gl_LightSource[0].halfVector.xyz);
	x = gl_Vertex.x;
	gl_Position = ftransform();
}