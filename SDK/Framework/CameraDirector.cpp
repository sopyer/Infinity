#include "CameraDirector.h"

#include <stdio.h>
#include <utils.h>

#include "ui.h"
#include <SpectatorCamera.h>

CameraDirector::CameraDirector(): index(0)
{
    FILE* cam = fopen("cameras.txt", "r");
    if (cam)
    {
        while (!feof(cam))
        {
            glm::__quatGTX  q;
            glm::vec3       p;
            
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
            glm::__quatGTX  q = savedCamRotation[i];
            glm::vec3       p = savedCamLocation[i];
            
            fprintf(cam, "%f %f %f %f %f %f %f\n", q.x, q.y, q.z, q.w, p.x, p.y, p.z);
        }
        fclose(cam);
    }
}

void processCameraDirectorInput(CameraDirector* camDirector, SpectatorCamera* camera)
{
    assert(camDirector && camera);

    if (ui::keyWasReleased(SDLK_0))
    {
        camDirector->savedCamRotation.push_back(camera->getOrientation());
        camDirector->savedCamLocation.push_back(camera->getPosition());
    }

    if (ui::keyWasReleased(SDLK_EQUALS) &&
        ui::keyIsPressed(SDLK_LALT)     &&
        camDirector->savedCamRotation.size())
    {
        int max = camDirector->savedCamRotation.size() - 1;
        int idx = camDirector->index;

        idx = idx + 1;
        idx = idx > max ? 0 : idx;
        
        camDirector->index = idx;

        camera->setOrientation(camDirector->savedCamRotation[camDirector->index]);
        camera->setPosition   (camDirector->savedCamLocation[camDirector->index]);
    }

    if (ui::keyWasReleased(SDLK_MINUS) &&
        ui::keyIsPressed(SDLK_LALT)    &&
        camDirector->savedCamRotation.size())
    {
        int max = camDirector->savedCamRotation.size() - 1;
        int idx = camDirector->index;

        idx = idx - 1;
        idx = idx < 0 ? max : idx;
        
        camDirector->index = idx;
        
        camera->setOrientation(camDirector->savedCamRotation[camDirector->index]);
        camera->setPosition   (camDirector->savedCamLocation[camDirector->index]);
    }
}
