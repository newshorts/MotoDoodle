//
//  ParticleController.cpp
//  chapter5
//
//  Created by Mike Newell on 3/17/13.
//
//

#include "ParticleController.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"

using namespace ci;
using std::list;

ParticleController::ParticleController() {
    
}

void ParticleController::repulseParticles() {
    for (std::list<Particle>::iterator p1 = mParticles.begin(); p1 != mParticles.end(); ++p1) {
        std::list<Particle>::iterator p2 = p1;
        for (++p2; p2 != mParticles.end(); ++p2) {
            Vec2f dir = p1->mLoc - p2->mLoc;
            
            float thresh = (p1->mRadius + p2->mRadius) * 5.0f;
            
            if (dir.x > -thresh && dir.x < thresh && dir.y > -thresh && dir.y < thresh) {
                float distSqrd = dir.lengthSquared() * dir.length();
                
                if (distSqrd > 0.0f) {
                    float F = 1.0f/distSqrd;
                    dir.normalize();
                    
                    // acceleration = force / mass
                    p1->mAccel += (F * dir) / p1->mMass;
                    p2->mAccel -= (F * dir) / p2->mMass;
                    
                }
            }
        }
    }
}

void ParticleController::pullToCenter() {
    for (std::list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p) {
        p->pullToCenter();
    }
}

void ParticleController::applyPerlin(const Perlin &perlin) {
    for (std::list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p) {
        p->applyPerlin(perlin);
    }
}

void ParticleController::update(const Channel32f &channel, const Surface32f &surface, const Vec2i &mouseLoc) {
    for (std::list<Particle>::iterator p = mParticles.begin(); p != mParticles.end();) {
        if (p->mIsDead) {
            p = mParticles.erase(p);
        } else {
            p->update(channel, surface, mouseLoc);
            ++p;
        }
    }
}

void ParticleController::draw() {
    for (std::list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p) {
        p->draw();
    }
}

void ParticleController::addParticles(int amt, const Vec2i &mouseLoc, const Vec2f &mouseVel) {
    for (int i = 0; i < amt; i++) {
        Vec2f loc = mouseLoc + Rand::randVec2f() * Rand::randFloat( 5.0f );
        Vec2f velOffset = Rand::randVec2f() * Rand::randFloat(1.0f, 3.0f);
        Vec2f vel = mouseVel * 5.0f + velOffset;
        mParticles.push_back(Particle(loc, vel));
        
    }
    
//    std::cout << mParticles.size() << std::endl;
}

void ParticleController::removeParticles(int amt) {
    for (int i = 0; i < amt; i++) {
        mParticles.pop_back();
    }
}


