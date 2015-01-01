
#include "CamNav.h"

void CamNav::setLayer(Layer* layer){
    scene = layer;
}

Layer* CamNav::getLayer(){
    return scene;
}

