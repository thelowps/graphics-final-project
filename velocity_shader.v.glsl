/*
*
*	David Lopes, 2012
*       for CSE 40166, University of Notre Dame
*/


// received from the outside world
uniform vec3 center;
uniform vec3 velocity;

uniform int amtCelLights;
uniform vec3 lightPos [4];

uniform float x_index;
uniform float y_index;

// Passed to the fragment shader
varying vec3 lightDir[4], normal;

void main(void) {

  //////////////////////
  // SQUEEZE POSITION // 
  //////////////////////

  vec3 yvec = vec3(0,1,0);
  vec3 zvec = vec3(0,0,1);

  vec3 pos = vec3(gl_Vertex.xyz);
  vec3 dist = pos - center;
  float lenv = sqrt(dot (velocity, velocity) );
  vec3 normV = velocity / lenv;
  float PprojV = abs(dot(dist, normV));
  float distMag = sqrt(dot(dist,dist));  
  //if (lenv >= 5.0){lenv = 5.0;}
  lenv = 0.5;
  if (lenv == 0.0) {lenv = 0.01;}
  float a = distMag /(1.0 + ((lenv/ 5.0) * 2.0));
  float b = ((lenv/5.0) * 5.0) * distMag + distMag;
 
  float u = acos(dot(dist, yvec)/ distMag);
  float v = acos(dot(dist, zvec)/ distMag);

  float x = b * cos(u) * cos(v);
  float y = a * cos(u) * sin(v);
  float z = a * sin(u);

  //float S = 1.0;
  //float cosine = ( PprojV/distMag );

  //float extraLength = (PprojV/lenv * 0.2);
  
  //vec3 vertexnew = (dist + extraLength) + center;
  vec3 vertexnew = vec3(x,y,z) + center;
  gl_Position = gl_ModelViewProjectionMatrix * vec4(vertexnew.xyz, 1);  

  //gl_Position = ftransform();

  
  /////////////////
  // CEL SHADING //
  /////////////////

  // Pass vertex normal on to fragment shader
  for (int i = 0; i < amtCelLights; ++i) {
    lightDir[i] = normalize( lightPos[i] );
  }
  normal = gl_NormalMatrix * gl_Normal;
  
}
