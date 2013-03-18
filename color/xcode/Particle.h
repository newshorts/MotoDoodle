//
//  Particle.h
//  chapter5
//
//  Created by Mike Newell on 3/17/13.
//
//
#pragma once
#include "cinder/Channel.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/Perlin.h"
#include "cinder/Surface.h"
#include <vector>

class Particle {
public:
    Particle();
    Particle( ci::Vec2f, ci::Vec2f );
    void pullToCenter();
    void applyPerlin( const ci::Perlin &perlin );
    void update( const ci::Channel32f &channel, const ci::Surface32f &surface, const ci::Vec2i &mouseloc );
    void draw();
    
    ci::Vec2f mLoc;
    ci::Vec2f mVel;
    ci::Vec2f mAccel;
    
    float mDecay;
    float mRadius, mRadiusDest;
    float mScale;
    
    float mMass;
    
    float mAge;
    float mLifespan;
    bool mIsDead;
    
    float mR, mG, mB;
    
};