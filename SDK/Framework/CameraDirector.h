#ifndef __CAMERADIRECTOR_H_INCLUDED__
#   define __CAMERADIRECTOR_H_INCLUDED__

#include <vector>
#include <glm/glmext.h>

struct CameraDirector
{
    CameraDirector();
    ~CameraDirector();

    std::vector<glm::__quatGTX> savedCamRotation;
    std::vector<glm::vec3>      savedCamLocation;
    int                         index;
};

class SpectatorCamera;

void processCameraDirectorInput(CameraDirector* camDirector, SpectatorCamera* camera);

#endif