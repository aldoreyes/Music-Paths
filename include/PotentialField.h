//
//  PotentialField.h
//  RoboticaProject
//
//  Created by Arnoldo Reyes on 11/19/11.
//  Copyright (c) 2011 Arnoldo Reyes. All rights reserved.
//

#ifndef RoboticaProject_PotentialField_h
#define RoboticaProject_PotentialField_h


#include <vector>
#include "cinder/Vector.h"

class PotentialField {
private:
    float force;
    float mass;
    ci::Vec2d loc;
public:
    PotentialField(ci::Vec2d &_loc, float _mass);
    void render();
    void update(float _force);
    void calculateForceOnPos(ci::Vec2d &_pos, ci::Vec2d &_vel);
    
    
};

#endif
