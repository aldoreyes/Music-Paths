//
//  Particle.cpp
//  RoboticaProject
//
//  Created by Arnoldo Reyes on 11/19/11.
//  Copyright (c) 2011 Arnoldo Reyes. All rights reserved.
//
#include "cinder/app/AppBasic.h"
#include "Particle.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/Color.h"

using namespace ci::app;

using namespace ci;


extern void renderImage( Vec2f _loc, float _diam, Color _col, float _alpha );
extern bool ALLOWVECTORS, ALLOWINERTIA;


Particle::Particle( const Vec2f &_loc, const Vec2f &_vel, float _mass)
{
    loc = _loc;
    vel = _vel;
    mass = _mass;
    color = Color(1,1,1);
    radius = mass * 32;
}

void Particle::render(){
    renderImage(loc,radius, color, 1);
    
    if(ALLOWVECTORS){
        glDisable( GL_TEXTURE_2D );
        gl::color( Color( 1.0f, 1.0f, 1.0f ) );
        Vec3f l = Vec3f(loc.x, loc.y, 0);
        Vec3f v = Vec3f(vel.x*10, vel.y*10, 0);
        gl::drawVector( l, l+v, 6, 3 );
        glEnable( GL_TEXTURE_2D );
    }
}

void Particle::update(const ci::Vec2f &_velRepl, const ci::Vec2f &_velAttr){
    if(ALLOWINERTIA){
        if(_velRepl.length() == 0){
            vel*=.995;
            if(vel.length() < 1){
                vel.normalize();
            }
        }else{
            vel += _velRepl;
        }
    
        vel+=_velAttr;
    }else{
        vel = _velRepl + _velAttr;
    }
    
    loc += vel;
    
    /*if(loc.x < 0){
        loc.x = -loc.x ;
        vel.x = -vel.x;
    }else if(loc.x > ci::app::getWindowWidth()){
        loc.x = ci::app::getWindowWidth() - (ci::app::getWindowWidth() -loc.x);
        vel.x = -vel.x;
    }
    
    if(loc.y < 0){
        vel.y = -vel.y;
        loc.y = -loc.y;
    }else if(loc.y > ci::app::getWindowHeight()){
        loc.y = ci::app::getWindowHeight() - (ci::app::getWindowHeight() -loc.y);
        vel.y = -vel.y;
    }*/
    
    
    
}