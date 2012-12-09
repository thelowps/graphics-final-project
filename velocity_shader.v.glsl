/*
*
*	David Lopes, 2012
*       for CSE 40166, University of Notre Dame
*/

uniform vec3 center;
uniform vec3 velocity;

void main(void) {

  vec3 pos = vec3(gl_Vertex.xyz);
  vec3 dist = pos - center;
  float lenv = sqrt(dot (velocity, velocity) );
  vec3 normV = velocity / lenv;
  float PprojV = abs(dot(dist, normV));
  float distMag = sqrt(dot(dist,dist));  
  
  float S = 1.0;
  float cosine = ( PprojV/distMag );

  float extraLength = (PprojV/lenv * 0.2);
  
  vec3 vertexnew = (dist + extraLength) + center;
  
  //gl_Position = gl_ModelViewProjectionMatrix * vec4(vertexnew.xyz, 1);
  gl_Position = ftransform();
  
}
