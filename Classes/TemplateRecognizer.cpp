//
//  TemplateRecognizer.cpp
//  battleout
//
//  Created by Adam on 9/5/14.
//
//

#include "TemplateRecognizer.h"
#include <cmath>

namespace DollarRecognizer {
    TemplateRecognizer::TemplateRecognizer(){
        
        nof_gesture_points = 128;
        
        square_size = 10;
        half_diagonal = 0.5* (sqrt((square_size * square_size) + (square_size * square_size)));
        //setRotateInvariance(false);
        angle_range = convertDegreeToRadian(45.0);
        angle_precision = convertDegreeToRadian(2.0);
        golden_ratio = 0.5 *(-1.0 + sqrt(5.0));
        
    }
    
    void TemplateRecognizer::loadTemplates(){
        SampleGestures samples;
        
		addTemplate("Arrow", samples.getGestureArrow());
        addTemplate("Caret", samples.getGestureCaret());
        addTemplate("CheckMark", samples.getGestureCheckMark());
		addTemplate("Circle", samples.getGestureCircle());
        addTemplate("Delete", samples.getGestureDelete());
        //addTemplate("Diamond", samples.getGestureDiamond());
        addTemplate("LeftCurlyBrace", samples.getGestureLeftCurlyBrace());
        addTemplate("LeftSquareBracket", samples.getGestureLeftSquareBracket());
        //addTemplate("LeftToRightLine", samples.getGestureLeftToRightLine());
        //addTemplate("LineDownDiagonal", samples.getGestureLineDownDiagonal());
		addTemplate("Pigtail", samples.getGesturePigtail());
        addTemplate("QuestionMark", samples.getGestureQuestionMark());
        addTemplate("Rectangle", samples.getGestureRectangle());
        addTemplate("RightCurlyBrace", samples.getGestureRightCurlyBrace());
        addTemplate("RightSquareBracket", samples.getGestureRightSquareBracket());
        //addTemplate("RightToLeftLine", samples.getGestureRightToLeftLine());
		//addTemplate("RightToLeftLine2", samples.getGestureRightToLeftLine2());
		//addTemplate("RightToLeftSlashDown", samples.getGestureRightToLeftSlashDown());
        addTemplate("Spiral", samples.getGestureSpiral());
		addTemplate("Star", samples.getGestureStar());
        addTemplate("Triangle", samples.getGestureTriangle());
        addTemplate("V", samples.getGestureV());
        addTemplate("X", samples.getGestureX());
        
//        printf("Arrow score %f\n",recognize(samples.getGestureArrow()).score);
//        printf("Care score %f\n",recognize(samples.getGestureCaret()).score);
//        printf("CheckMark score %f\n",recognize(samples.getGestureCheckMark()).score);
//        printf("Circle score %f\n",recognize(samples.getGestureCircle()).score);
//        printf("Delete score %f\n",recognize(samples.getGestureDelete()).score);
//        printf("Diamond score %f\n",recognize(samples.getGestureDiamond()).score);
//        printf("LeftCurlyBrace score %f\n",recognize(samples.getGestureLeftCurlyBrace()).score);
//        printf("LeftSquareBracket score %f\n",recognize(samples.getGestureLeftSquareBracket()).score);
//        printf("LeftToRightLine %f\n",recognize(samples.getGestureLeftToRightLine()).score);
//        printf("Pigtail score %f\n",recognize(samples.getGesturePigtail()).score);
//        printf("QuestionMark Score %f\n",recognize(samples.getGestureQuestionMark()).score);
//        printf("Rectangle score %f\n",recognize(samples.getGestureRectangle()).score);
//        printf("Right Curly Brace Score %f\n",recognize(samples.getGestureRightCurlyBrace()).score);
//        printf("RightSquareBracket Score %f \n",recognize(samples.getGestureRightSquareBracket()).score);
//        printf("Star score %f\n",recognize(samples.getGestureStar()).score);
//        printf("Triangle score %f\n",recognize(samples.getGestureTriangle()).score);
//        printf("V score %f\n",recognize(samples.getGestureV()).score);
//        printf("X Score %f\n",recognize(samples.getGestureX()).score);

        
    }
    
    void TemplateRecognizer::addTemplate(string name, Path2D points){
        points = normalizePath(points);
//        Path2D new_points;
//        for(Path2DIterator i = points.begin(); i < points.end(); i++){
//            Point2D point = *i;
//            point.x *= 10;
//            point.y *= 10;
//            new_points.push_back(point);
//        }
        templates.push_back(GestureTemplate(name,points) );
    }
    
    void TemplateRecognizer::setRotateInvariance(bool ignoreRotation){
        this->ignoreRotation = ignoreRotation;
        
        if(ignoreRotation){
            angle_range = 45.0;
        }else{
            angle_range = 15.0;
        }
    }

    Path2D TemplateRecognizer::normalizePath(Path2D points){
        points = resample(points);
        points = rotateToZero(points);
        points = scaletoSquare(points);
        points = translatToOrigin(points);
        
        return points;
    }
    
    Path2D TemplateRecognizer::normalizePathRecog(Path2D points){
        points = resample(points);
        if(getRotateInvariance()){
            points = rotateToZero(points);
        }
    
        points = translatToOrigin(points);
        
        return points;
    }
    
    double TemplateRecognizer::distanceAtAngle(Path2D points, GestureTemplate tmplate, double rotation){
        Path2D new_points = rotateBy(points, rotation);
        
        return getPathDistance(new_points, tmplate.points);
    }
    
    double TemplateRecognizer::distanceAtBestAngle(Path2D points, GestureTemplate tmplate){
        double start_range = -angle_range;
        double end_range = angle_range;
        double x1 = golden_ratio * start_range + (1.0 - golden_ratio) * end_range;
        double f1 = distanceAtAngle(points, tmplate, x1);
        double x2 = (1.0 - golden_ratio) * start_range + golden_ratio * end_range;
        double f2 = distanceAtAngle(points, tmplate, x2);

        
        while( abs(end_range - start_range) > angle_precision){
            if(f1 < f2){
                end_range = x2;
                x2 = x1;
                f2 = f1;
                x1 = golden_ratio * start_range + (1.0 - golden_ratio) * end_range;
                f2 = distanceAtAngle(points, tmplate, x1);
                
            }else{
                start_range = x1;
                x1 = x2;
                f1 = f2;
                x2 = (1.0 - golden_ratio) * start_range + golden_ratio * end_range;
                f2 = distanceAtAngle(points, tmplate, x2);
            }
        }
        
        return min(f1, f2);
    }

    RecognitionResult TemplateRecognizer::recognize(Path2D points){
        if(templates.empty()){
            printf("empty template\n");
            return RecognitionResult("empty template", NULL);
        }
        points = normalizePath(points);
        
        double best_distance = std::numeric_limits<double>::max();
        int best_match_index = -1;
        
        for(int i = 0; i < (int) templates.size(); i++){
            double distance = distanceAtBestAngle(points, templates[i]);
            if( distance < best_distance){
                best_distance = distance;
                best_match_index = i;
            }
        }
        
        printf("distance : %f\n",best_distance);
        double score = 1.0 - (best_distance / half_diagonal);
        if( best_match_index == -1){
            printf("no good match\n");
            return RecognitionResult("No good match", 1);
        }
        RecognitionResult bestMatch(templates[best_match_index].gesture_name, score);
        return bestMatch;
    }
    
    Path2D TemplateRecognizer::resample(Path2D points){
		double interval = getPathLength(points) / (nof_gesture_points - 1); // interval length
		double D = 0.0;
		Path2D newPoints;
        
		//--- Store first point since we'll never resample it out of existence
		newPoints.push_back(points.front());
	    for(int i = 1; i < (int)points.size(); i++)
		{
			Point2D currentPoint  = points[i];
			Point2D previousPoint = points[i-1];
			double d = getDistance(previousPoint, currentPoint);
			if ((D + d) >= interval)
			{
				double qx = previousPoint.x + ((interval - D) / d) * (currentPoint.x - previousPoint.x);
				double qy = previousPoint.y + ((interval - D) / d) * (currentPoint.y - previousPoint.y);
				Point2D point(qx, qy);
				newPoints.push_back(point);
				points.insert(points.begin() + i, point);
				D = 0.0;
			}
			else D += d;
		}
        
		// somtimes we fall a rounding-error short of adding the last point, so add it if so
		if (newPoints.size() == (nof_gesture_points - 1))
		{
			newPoints.push_back(points.back());
		}
        

		return newPoints;
    }
    
    double TemplateRecognizer::convertDegreeToRadian(double degrees){
        
        return degrees * M_PI / 180.0;
    }
}