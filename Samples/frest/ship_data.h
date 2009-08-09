// Data header file.

#ifndef _DATA_H_
#define _DATA_H_


//Taken from output of ship data from 3D Explorer.
typedef struct{
 float ambient[3];
 float diffuse[3];
 float specular[3];
 float emission[3];
 float alpha;
 float phExp;
 int   texture;
}sample_MATERIAL;



// The space ship data.
extern short face_indicies [8686][6];
extern float normals [4481][3];
extern float vertices [4345][3];
extern int material_ref [7][2];
extern sample_MATERIAL materials[5];





#endif