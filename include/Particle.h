//
//  Particle.h
//  RoboticaProject
//
//  Created by Arnoldo Reyes on 11/19/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#ifndef RoboticaProject_Particle_h
#define RoboticaProject_Particle_h

#include "cinder/Color.h"
#include "cinder/Vector.h"
#include <vector>

class Particle{
    
public:
    Particle(const ci::Vec2f &_loc, const ci::Vec2f &_vel, float _mass );
    void render();
    void update(const ci::Vec2f &_velRepl, const ci::Vec2f &_velAttr);
    
    float mass;
    float radius;
    ci::Vec2f loc;
    ci::Vec2f vel;
    ci::Color  color;

};

#endif
