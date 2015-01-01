//
//  GeometricUtils.h
//  battleout
//
//  Created by Adam on 9/5/14.
//
//

#ifndef __battleout__GeometricUtils__
#define __battleout__GeometricUtils__

#include <iostream>
#include "GeometricTypes.h"

using namespace std;

namespace DollarRecognizer {
    double getDistance(Point2D point1, Point2D point2);
    double getPathLength(Path2D points);
    double getPathDistance(Path2D path1, Path2D path2);
    Point2D centroid(Path2D points);
    Path2D rotateBy(Path2D points, double rotation);
    Path2D translatToOrigin(Path2D objects);
    Rectangle boundingBox(Path2D points);
    Path2D scaletoSquare(Path2D points);
    Path2D rotateToZero(Path2D points);
}
#endif /* defined(__battleout__GeometricUtils__) */
