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

Particle::Particle( const Vec2f &_loc, const Vec2f &_vel, float _mass)
{
    loc = _loc;
    vel = _vel;
    mass = _mass;
    color = Color(1,1,1);

}

void Particle::render(){
    renderImage(loc,mass*32, color, 1);
}

void Particle::update(const ci::Vec2f &_vel){
    vel = _vel;
    loc = loc + _vel;
}