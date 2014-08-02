#include <fwk/CameraDirector.h>

#include <stdio.h>

CameraDirector::CameraDirector(): index(0)
{
    FILE* cam = fopen("cameras.txt", "r");
    if (cam)
    {
        while (!feof(cam))
        {
            ml::vec4 q;
            ml::vec4 p;

            fscanf(cam, "%f %f %f %f %f %f %f\n", &q.x, &q.y, &q.z, &q.w, &p.x, &p.y, &p.z);

            savedCamRotation.push_back(q);
            savedCamLocation.push_back(p);
        }
        fclose(cam);
    }
}

CameraDirector::~CameraDirector()
{
    FILE* cam = fopen("cameras.txt", "w");
    if (cam)
    {
        for (size_t i=0; i<savedCamRotation.size(); ++i)
        {
            ml::vec4 q = savedCamRotation[i];
            ml::vec4 p = savedCamLocation[i];

            fprintf(cam, "%f %f %f %f %f %f %f\n", q.x, q.y, q.z, q.w, p.x, p.y, p.z);
        }
        fclose(cam);
    }
}
