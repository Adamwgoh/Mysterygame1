//
//  TemplateRecognizer.h
//  battleout
//
//  Created by Adam on 9/5/14.
//
//

#ifndef __battleout__TemplateRecognizer__
#define __battleout__TemplateRecognizer__

#include <iostream>
#include <limits>

#include "GestureSample.h"
#include "GeometricUtils.h"

using namespace std;

namespace DollarRecognizer {
    class TemplateRecognizer{
    public:
        TemplateRecognizer();
        
        void loadTemplates();
        void addTemplate(string name, Path2D points);
        Path2D resample(Path2D points);
        Path2D normalizePath(Path2D points);
        Path2D normalizePathRecog(Path2D points);
        double distanceAtAngle(Path2D points, GestureTemplate tmplate, double rotation);
        double distanceAtBestAngle(Path2D points, GestureTemplate tmplate);
        RecognitionResult recognize(Path2D points);
        
        void setRotateInvariance(bool ignoreRotation);
        bool getRotateInvariance(){ return ignoreRotation; }
        
        double convertDegreeToRadian(double degrees);
        
    protected:
        double half_diagonal;
        double angle_range;
        double angle_precision;
        double golden_ratio;
        
        bool ignoreRotation = true;
        int square_size;
        int nof_gesture_points;
        
        GestureTemplates templates;
    };
}
#endif /* defined(__battleout__TemplateRecognizer__) */
