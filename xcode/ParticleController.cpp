//
//  ParticleController.cpp
//  RoboticaProject
//
//  Created by Arnoldo Reyes on 11/20/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//
#include "Particle.h"
#include "ParticleController.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using std::list;
using namespace ci;

extern gl::Texture *particleImg, *emitterImg;

ParticleController::ParticleController()
{
    
}

void ParticleController::render(){
    
    glEnable( GL_TEXTURE_2D );
	particleImg->bind();
    
    for (list<Particle>::iterator p = particles.begin(); p != particles.end(); p++) {
        p->render();
    }
    
    glDisable( GL_TEXTURE_2D );
}

void ParticleController::update(){
    for (list<Particle>::iterator p = particles.begin(); p != particles.end(); p++) {
        p->update(ci::Vec2f(Rand::randFloat(1, 2), Rand::randFloat(1,2)));
    }
}

void ParticleController::addParticle( const ci::Vec2f &_loc){
    

    particles.push_back(Particle(_loc, ci::Vec2f(Rand::randFloat(1, 2), Rand::randFloat(1,2)), 1));
}