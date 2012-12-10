/*
*  David Lopes, 2012
*  CSE 40166, University of Notre Dame
*/

//uniform vec3 center;
//uniform vec3 velocity;

varying vec3 lightDir [4];
varying vec3 normal;
uniform int amtCelLights;

void main(void) {

  float brightnessFactor = 1.0;
  float intensity = 0.0;
  for (int i = 0; i < amtCelLights; ++i) {
    vec3 normz = normalize(normal);
    float temp = dot(lightDir[i], normz);
    
    intensity += temp * brightnessFactor;
  }
  
  vec4 col;
  if (intensity > 0.9) {
    col = vec4(0.9, 0.6, 0.5, 1.0);
  } else if (intensity > 0.5) {
    col = vec4(0.7, 0.4, 0.3, 1.0);
  } else if (intensity > 0.2) {
    col = vec4(0.4, 0.2, 0.0, 1.0);
  } else {
    col = vec4(0.2, 0.1, 0.0, 1.0);
  }

  //gl_FragColor = col;
  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
