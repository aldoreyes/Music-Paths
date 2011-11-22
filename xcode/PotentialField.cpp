//
//  PotentialField.cpp
//  RoboticaProject
//
//  Created by Arnoldo Reyes on 11/20/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "PotentialField.h"
#include "cinder/gl/gl.h"

using namespace ci;

PotentialField::PotentialField(Vec2d &_loc, float _mass){
    mass = _mass;
    loc = _loc;
}

void PotentialField::render(){
    gl::drawSolidCircle(loc, mass * force);
}

