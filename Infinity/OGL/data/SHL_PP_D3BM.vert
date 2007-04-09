//------------------------------------------------------------------//
// Spherical harmonic lighting, per-pixel illumination effect       //
//                                                                  //
// (c) Nathaniel Hoffman 2003                                       //
//                                                                  //
// Based on 'An Efficient Representation for Irradiance             //
// Environment Maps', SIGGRAPH 2001, by Ravi Ramamoorthi and Pat    //
// Hanrahan from Stanford University                                //
// Converted to GLSL by ATI.
//------------------------------------------------------------------//

varying vec3 vNormal;
varying vec2 vTex;
varying vec3 vTangent;
varying vec3 vBinormal;

attribute vec3 rm_Tangent;
attribute vec3 rm_Binormal;

void main(void)
{
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   vTex = vec2(gl_MultiTexCoord0);

   // Rotate tangent basis from object/world space to view
   // space (in RenderMonkey lights are defined in view space)
   vNormal    = vec3(gl_NormalMatrix * gl_Normal);
   vTangent   = vec3(gl_NormalMatrix * rm_Tangent);
   vBinormal  = vec3(gl_NormalMatrix * rm_Binormal);
   
}