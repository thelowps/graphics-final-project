/*
*
*	David Lopes, 2012
*       for CSE 40166, University of Notre Dame
*/

varying vec3 lightDir, normal;
attribute vec4 target;
uniform float lerper;

void main(void) {

  // Pass vertex normal on to fragment shader
  lightDir = normalize( vec3(gl_LightSource[0].position) );
  normal = gl_NormalMatrix * gl_Normal;

  // Pass the texture coordinate to the fragment shader
  gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

  // Set vertex position
  // interpolate between its position and the texture
  vec4 newVertex = mix(gl_Vertex, target, lerper);
  gl_Position = gl_ModelViewProjectionMatrix * newVertex;
  
}
