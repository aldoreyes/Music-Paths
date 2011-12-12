//
//  ParticleController.h
//  RoboticaProject
//
//  Created by Arnoldo Reyes on 11/20/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef RoboticaProject_ParticleController_h
#define RoboticaProject_ParticleController_h
#include "Particle.h"
#include "PotentialField.h"
#include "cinder/gl/Texture.h"
#include <list>

class ParticleController {
    
    
public:
    ParticleController(const ci::Vec2i &windowSize, const uint16_t bandCount);
    void update( const std::shared_ptr<float> fft, const uint16_t bandCount, const double elapsedSeconds);
    void render();
    void addParticle(const ci::Vec2f &_loc);
    void calculateVelOnPos(ci::Vec2f &_velRepl, ci::Vec2f &_velAttr, const ci::Vec2f &_loc);
    
    std::list<Particle> particles;
    std::list<PotentialField> fields;
    std::list<PotentialField> fieldsAttr;
    
};

#endif
