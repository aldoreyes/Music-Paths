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
#include "cinder/Color.h"
#include "cinder/Easing.h"
#include <list>

class PotentialField {
    
public:
    PotentialField(ci::Vec2f &_loc, ci::Vec2f &_vel, float _mass, ci::Color _color, bool repulsive = true);
    void render();
    void update(float _force, const double elapsedSeconds);
    void calculateForceOnPos(const ci::Vec2f &_pos, ci::Vec2f &_vel);
    
private:
    float force;
    float easingForce;
    float mass;
    ci::Vec2f loc;
    ci::Vec2f vel;
    ci::Color color;
    float time;
    cinder::EaseOutQuad ease;
    float areaOfInfluence;
    float radius;
    std::list<ci::Vec2f> vectors;
    bool isRepulsive;
    
};

#endif
