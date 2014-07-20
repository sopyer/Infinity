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
        impl::buildFillGeometry(*newPath, numSegments, pathSegments, numSegments*6, pathData);

        return newPath;
    }

    void destroyPath(Path path)
    {
        delete path;
    }
}