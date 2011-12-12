//
//  ParticleController.cpp
//  RoboticaProject
//
//  Created by Arnoldo Reyes on 11/20/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//
#include "Particle.h"
#include "PotentialField.h"
#include "ParticleController.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/app/AppBasic.h"

using std::list;
using namespace ci;

extern gl::Texture *particleImg, *emitterImg;
extern bool ALLOWMOVEATTRACTORS;
Color colors[] =  {Color(1,0,0), Color(1,0,0),Color(1,0,0),Color(1,0,0),
                    Color(1,.25,.25),Color(1,.25,.25),Color(1,.25,.25),Color(1,.25,.25),
                    Color(1,.5,.5),Color(1,.5,.5),Color(1,.5,.5),Color(1,.5,.5),
                    Color(1,.75, .75),Color(1,.75, .75),Color(1,.75, .75),Color(1,.75, .75)};


ParticleController::ParticleController(const ci::Vec2i &windowSize, const uint16_t bandCount)
{
    int segment = bandCount/3;
    double c = 0;
    Vec2f l;
    Vec2f v;
    for(int i = 0; i< bandCount; i++){
        l = ci::Vec2f( Rand::randInt(0, windowSize.x), Rand::randInt(0, windowSize.y) );
        
        c = floor(i/segment)*.25;
        v = Rand::randVec2f()*(1-c);
        fields.push_back( PotentialField( l, v, 3.0f, Color(1,c,c), true ));
    }
    l = Vec2f(windowSize.x/2, windowSize.y/2);
    v = Rand::randVec2f()*.1;;
    fieldsAttr.push_back(PotentialField(l, v, 2.0f, Color(.75, 1, .75), false));
}

void ParticleController::render(){
    
    for (list<PotentialField>::iterator f = fields.begin(); f != fields.end(); f++) {
        f->render();
    }
    
    for (list<PotentialField>::iterator f = fieldsAttr.begin(); f != fieldsAttr.end(); f++) {
        f->render();
    }
    
    glEnable( GL_TEXTURE_2D );
    particleImg->bind();
    for (list<Particle>::iterator p = particles.begin(); p != particles.end(); p++) {
        p->render();
    }
    
    glDisable( GL_TEXTURE_2D );
}

void ParticleController::update(std::shared_ptr<float> fft, const uint16_t bandCount, const double elapsedSeconds){
    int ind = 0;
    float * fftBuffer = fft.get();
    
    //update force from sound
    for (list<PotentialField>::iterator f = fields.begin(); f != fields.end(); f++) {
        f->update(fftBuffer[ind]*10.0*(ind+1.0)/(bandCount-ind) , elapsedSeconds);
        ind++;
    }
    
    for (list<PotentialField>::iterator f = fieldsAttr.begin(); f != fieldsAttr.end(); f++) {
        f->update(1 , elapsedSeconds);
        ind++;
    }
    
    Vec2f vRepl = Vec2f(0,0);
    Vec2f vAttr = Vec2f(0,0);
    
    //update position for particles
    for (list<Particle>::iterator p = particles.begin(); p != particles.end(); p++) {
        calculateVelOnPos(vRepl, vAttr,  p->loc);
        p->update(vRepl, vAttr);
        vRepl.x = 0;
        vRepl.y = 0;
        vAttr.x = 0;
        vAttr.y = 0;
    }
}


void ParticleController::calculateVelOnPos(ci::Vec2f &_velRepl, ci::Vec2f &_velAttr, const ci::Vec2f &_loc){
    for (list<PotentialField>::iterator f = fields.begin(); f != fields.end(); f++) {
        f->calculateForceOnPos(_loc, _velRepl);
    }
    
    for (list<PotentialField>::iterator f = fieldsAttr.begin(); f != fieldsAttr.end(); f++) {
        f->calculateForceOnPos(_loc, _velAttr);
    }
}

void ParticleController::addParticle( const ci::Vec2f &_loc){
    

    particles.push_back(Particle(_loc, ci::Vec2f(Rand::randFloat(-1, 1), Rand::randFloat(-1,1)), 1));
}