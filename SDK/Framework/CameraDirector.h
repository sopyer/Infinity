#ifndef __CAMERADIRECTOR_H_INCLUDED__
#   define __CAMERADIRECTOR_H_INCLUDED__

#include <vector>
#include <ml.h>

//TODO: use circular queue!!!!
struct CameraDirector
{
    CameraDirector();
    ~CameraDirector();

    std::vector<ml::vec4> savedCamRotation;
    std::vector<ml::vec4> savedCamLocation;
    int                   index;
};

#endif