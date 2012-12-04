/*
*  David Lopes, 2012
*  CSE 40166, University of Notre Dame
*/

varying vec3 lightDir, normal;
uniform sampler2D tex;

void main(void) {

  vec4 col;

  vec3 normz = normalize(normal);
  float intensity = dot(lightDir, normz);

  if (intensity > 0.9) {
    col = vec4(0.9, 0.6, 0.5, 1.0);
  } else if (intensity > 0.5) {
    col = vec4(0.7, 0.4, 0.3, 1.0);
  } else if (intensity > 0.2) {
    col = vec4(0.4, 0.2, 0.0, 1.0);
  } else {
    col = vec4(0.2, 0.1, 0.0, 1.0);
  }
    
  gl_FragColor = col;

}
