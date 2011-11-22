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
#include "cinder/gl/Texture.h"
#include <list>

class ParticleController {
    
    
public:
    ParticleController();
    void update();
    void render();
    void addParticle(const ci::Vec2f &_loc);
    
    std::list<Particle> particles;
};

#endif
