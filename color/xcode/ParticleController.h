//
//  ParticleController.h
//  chapter5
//
//  Created by Mike Newell on 3/17/13.
//
//


#pragma once
#include "cinder/Channel.h"
#include "Particle.h"
#include "cinder/Perlin.h"
#include <list>

class ParticleController {
public:
    ParticleController();
    void repulseParticles();
    void pullToCenter();
    void applyPerlin( const ci::Perlin &perlin );
    void update( const ci::Channel32f &channel, const ci::Surface32f &surface, const ci::Vec2i &mouseLoc );
    void draw();
    void addParticles( int amt, const ci::Vec2i &mouseLoc, const ci::Vec2f &mouseVel );
    void removeParticles( int amt );
    
    std::list<Particle> mParticles;
    
};
