//------------------------------------------------------------------//
// Spherical harmonic lighting, per-pixel illumination effect       //
//                                                                  //
// (c) Nathaniel Hoffman 2003                                       //
//                                                                  //
// Based on 'An Efficient Representation for Irradiance             //
// Environment Maps', SIGGRAPH 2001, by Ravi Ramamoorthi and Pat    //
// Hanrahan from Stanford University                                //
// Ported to GLSL by ATI.
//------------------------------------------------------------------//



uniform sampler2D bump_map;
uniform sampler2D rgb_map;

uniform mat4 r_sh_matrix;
uniform mat4 b_sh_matrix;
uniform mat4 g_sh_matrix;

varying vec3 vNormal;
varying vec2 vTex;
varying vec3 vTangent;
varying vec3 vBinormal;


void main(void)
{
   vec4 c;
   mat3 rotation;
   vec3 normal3;
   vec4 normal4;

   // Matrix to transform from tangent space into light space)
   rotation = mat3(normalize(vTangent) , normalize(vBinormal), normalize(vNormal));

   // Get normal
   normal3 = texture2D(bump_map, vTex).xyz * 2.0 - 1.0;
   // the *_sh_matrix'es below are the wrong handedness for opengl, so we need to flip z
   normal3.z *= -1.0;

   // Transform normal into light space
   normal4 = vec4((normal3 * rotation), 1.0);
   
   //Evaluate spherical harmonic
   c.r = dot((r_sh_matrix * normal4), normal4);
   c.g = dot((g_sh_matrix * normal4), normal4);
   c.b = dot((b_sh_matrix * normal4), normal4);
   c.a = 1.0;

   // Multiply by rgb factor (and scale by two)
   gl_FragColor = c * texture2D(rgb_map, vTex) * 2.0;
}