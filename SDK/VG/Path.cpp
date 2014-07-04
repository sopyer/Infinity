#include <vector>
#include <glm/glm.h>
#include <utils.h>

#include "VG.h"
#include "impl/Rasterizer.h"

namespace vg
{
    void getPathBounds(Path path, float& x1, float& y1, float& x2, float& y2)
    {
        x1 = path->xmin; y1 = path->ymin;
        x2 = path->xmax; y2 = path->ymax;
    }

    Path createPath(VGint numSegments, const VGubyte* pathSegments, const VGfloat* pathData)
    {
        Path newPath;

        newPath = new impl::Geometry();
        impl::buildFillGeometry(*newPath, numSegments, pathSegments, pathData);

        return newPath;
    }

    Path createUnitQuad()
    {
        const VGubyte quadSegs[] = {VG_MOVE_TO_ABS, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_REL, VG_LINE_TO_REL};
        const VGfloat quadData[] = {0.5f, 0.5f, -1, 0, 0, -1, 1, 0, 0, 1};
        return createPath(ARRAY_SIZE(quadSegs), quadSegs, quadData);
    }

    void destroyPath(Path path)
    {
        delete path;
    }
}