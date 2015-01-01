//
//  Voice.h
//  battleout
//
//  Created by Adam on 8/28/14.
//
//

#ifndef __battleout__Voice__
#define __battleout__Voice__

#include <iostream>
#include "cocos2d.h"
#include "AnimatedLabel.h"


class Voice{
public:
    bool think(const char* speech);
    bool think(std::vector<std::string> speech);
    
private:
    
    
};
#endif /* defined(__battleout__Voice__) */
