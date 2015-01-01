//
//  GeometricUtils.cpp
//  battleout
//
//  Created by Adam on 9/5/14.
//
//

#include "GeometricUtils.h"
#include "GeometricTypes.h"

#define MAX_DOUBLE std::numeric_limits<double>::max();

namespace DollarRecognizer {

double getDistance(Point2D point1, Point2D point2){
    float dx = point2.x - point1.x;
    float dy = point2.y - point1.y;
    return sqrt((dx*dx) + (dy*dy));
}

double getPathLength(Path2D points){
    double path_length = 0.0;
    for(int i = 1; i < points.size(); i++){
        path_length += getDistance(points[i-1], points[i]);
    }
    
    return path_length;
}

double getPathDistance(Path2D path1, Path2D path2){
    double distance = 0.0;
    for(int i = 0; i < path1.size(); i++){
        distance += getDistance(path1[i], path2[i]);
    }
    
    return (distance/path1.size());
}
    
Point2D centroid(Path2D points){
    double x = 0.0;
    double y = 0.0;
    
    for(Path2DIterator itr = points.begin(); itr != points.end(); ++itr){
        Point2D point = (*itr);
        x += point.x;
        y += point.y;
    }
    
    x = x/points.size();
    y = y/points.size();
    

    return Point2D(x,y);
}
    
Path2D rotateBy(Path2D points, double rotation){
    Point2D center = centroid(points);
    
    double cosine = cos(rotation);
    double sine   = sin(rotation);
    
    Path2D new_points;
    for(Path2DIterator itr = points.begin(); itr != points.end(); ++itr){
        Point2D point = (*itr);
        
        double rotated_x = (point.x - center.x)*cosine - (point.y - center.y)*sine + center.x;
        double rotated_y = (point.x - center.x)*sine + (point.y - center.y)*cosine + center.y;
        
        new_points.push_back(Point2D(rotated_x,rotated_y));
    }
    
    return new_points;
}
    
Path2D rotateToZero(Path2D points){
    Point2D center = centroid(points);
    double rotation = atan2(center.y - points[0].y, center.x - points[0].x);
    
    return rotateBy(points, -rotation);
}
 
Path2D translatToOrigin(Path2D objects){
    Point2D center = centroid(objects);
    Path2D new_objects;
    for(Path2DIterator itr = objects.begin(); itr != objects.end(); ++itr){
        Point2D point = (*itr);
        double qx = point.x - center.x;
        double qy = point.y - center.y;
        
        new_objects.push_back(Point2D(qx, qy));
    }
        
    return new_objects;
}
    
Rectangle boundingBox(Path2D points){
    double minX = MAX_DOUBLE;
    double minY = MAX_DOUBLE;
    double maxX = -MAX_DOUBLE;
    double maxY = -MAX_DOUBLE;
    
    for(Path2DIterator itr = points.begin(); itr != points.end(); ++itr){
        Point2D point = (*itr);
        if(point.x < minX) minX = point.x;
        if(point.y < minY) minY = point.y;
        if(point.x > maxX) maxX = point.x;
        if(point.y > maxY) maxY = point.y;
            
    }
    
    Rectangle bounding_box = Rectangle(minX, minY, (maxX - minX), (maxY - minX));
    return bounding_box;
}
    
Path2D scaletoSquare(Path2D points){
    Rectangle box = boundingBox(points);
    Path2D new_points = *new Path2D();
    for(Path2DIterator itr = points.begin(); itr != points.end(); ++itr){
        Point2D point = (*itr);
        double scaledX = point.x * (1.0 / box.width);//change to squaresize once it is defined
        double scaledY = point.y * (1.0 / box.height);
        
        new_points.push_back(Point2D(scaledX, scaledY));
    }
    
    return new_points;
}



}//end namespace