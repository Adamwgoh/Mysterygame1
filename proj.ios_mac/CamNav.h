#ifndef __battleout__Camera__
#define __battleout__Camera__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;
#endif /* defined(__battleout__Camera__) */

class CamNav
{
public:
    
    void setLayer(Layer* layer);
    void ScrollMap(float dx, float dy);
    bool checkState();
    void resetView();
    Layer* getLayer();
private:
    Layer* scene;
};