//
//  PotentialField.cpp
//  RoboticaProject
//
//  Created by Arnoldo Reyes on 11/20/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "PotentialField.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"

using namespace ci;

extern bool ALLOWMOVE, ALLOWVECTORS, ALLOWMOVEATTRACTORS;

PotentialField::PotentialField(Vec2f &_loc, Vec2f &_vel, float _mass, Color _color, bool repulsive){
    mass = _mass;
    loc = _loc;
    easingForce = force = .1;
    color = _color;
    radius = mass *4;
    vel = _vel;
    isRepulsive = repulsive;
    
    areaOfInfluence = easingForce * mass + 32.0;
}

void PotentialField::render(){
    glColor4f(color.r, color.g, color.b, .2);
    gl::drawSolidCircle(loc, radius);
    
    
    gl::drawSolidCircle(loc, areaOfInfluence);
    gl::drawSolidCircle(loc, areaOfInfluence*.8);
    gl::drawSolidCircle(loc, areaOfInfluence*.6);
    gl::drawSolidCircle(loc, areaOfInfluence*.4);
    
    
    if(ALLOWVECTORS){
        glColor4f(color.r, color.g, color.b, 1);
        
        for (std::list<Vec2f>::iterator v = vectors.begin(); v != vectors.end(); v++) {
            Vec3f l = Vec3f(loc.x, loc.y, 0);
            Vec3f myVel = Vec3f(v->x, v->y, 0);
            gl::drawVector( l, l+myVel, 6, 3 );
            
        }
    }
    
}

void PotentialField::update(float _force, const double elapsedSeconds){
    if(isRepulsive){
        if(ALLOWMOVE){
            loc += vel;
    
            if(loc.x < 0){
                loc.x = -loc.x ;
                vel.x = -vel.x;
            }else if(loc.x > ci::app::getWindowWidth()){
                loc.x = ci::app::getWindowWidth() - (ci::app::getWindowWidth() -loc.x);
                vel.x = -vel.x;
            }
        }
    
        
    
        force = _force;
    
        if(force <  easingForce){
            easingForce += (force - easingForce)*.2;
        }else{
            easingForce = force;
        }
        //16 radius of particle
        areaOfInfluence = easingForce * mass + 32;
    }else{
        if(ALLOWMOVEATTRACTORS){
            loc += vel;
        }
    }
    
    
    if(loc.y < 0){
        vel.y = -vel.y;
        loc.y = -loc.y;
    }else if(loc.y > ci::app::getWindowHeight()){
        loc.y = ci::app::getWindowHeight() - (ci::app::getWindowHeight() -loc.y);
        vel.y = -vel.y;
    }
    
    if(ALLOWVECTORS){
        vectors.clear();
    }
}

void PotentialField::calculateForceOnPos(const Vec2f &_pos, Vec2f &_vel){
    Vec2f toV;
    float l;
    
    if(isRepulsive){
    
        if((l = (toV = (_pos-loc)).length()) < areaOfInfluence){
            /*toV.safeNormalize();
             toV *= (areaOfInfluence - l)*.01;
             _vel += toV;*/
        
            toV.safeNormalize();
            toV *= 2*(areaOfInfluence - l) / (areaOfInfluence-radius);
            _vel += toV;
        
        
            if(ALLOWVECTORS){
                toV.normalize();
                toV *=l;
                vectors.push_back(toV);
            }
        }
    }else{
        _vel += (loc-_pos)*.002;
    }
}
