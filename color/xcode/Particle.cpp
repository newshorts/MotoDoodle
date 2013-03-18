//
//  Particle.cpp
//  chapter5
//
//  Created by Mike Newell on 3/17/13.
//
//

#include "Particle.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"

using namespace ci;

Particle::Particle () {
    
}

Particle::Particle( Vec2f loc, Vec2f vel) {
    mLoc = loc;
    mVel = vel;
    mAccel = Vec2f::zero();
    
    mDecay = Rand::randFloat(0.95f, 0.951f);
    mRadius = 3.0f + Rand::randFloat(0.1f);
    mRadiusDest = 6.0f;
    mMass = mRadius * mRadius * 0.0001f + 0.01f;
    mScale = 10.0f;
    mAge = 0.0f;
//    mLifespan = Rand::randFloat(0.5f, 2.0f);
    mLifespan = 25.0f;
    mIsDead = false;
    
    mR = Rand::randFloat();
    mG = Rand::randFloat();
    mB = Rand::randFloat();
}

void Particle::pullToCenter() {
    Vec2f dirToCenter = mLoc - app::getWindowCenter();
    mVel -= dirToCenter * mMass * 0.025f;
}

void Particle::applyPerlin(const Perlin &perlin) {
    float nX = mLoc.x * 0.005f;
    float nY = mLoc.y * 0.005f;
    float nZ = app::getElapsedFrames() * 0.0025f;
    float noise = perlin.fBm(nX, nY, nZ);
    float angle = noise * 15.0f;
    Vec2f noiseVector ( cos(angle), sin(angle) );
    mVel += noiseVector * mMass * 5.0f;
}

void Particle::update(const Channel32f &channel, const Surface32f &surface, const Vec2i &mouseloc) {
    mVel += mAccel;
    
    float maxVel = mRadius + 0.0025f;
    float velLength = mVel.lengthSquared() + 0.1f;
    if (velLength > maxVel * maxVel) {
        mVel.normalize();
        mVel *= maxVel;
    }
    
    mLoc += mVel;
    mVel *= mDecay;
    mAccel.set(0, 0);
    
    if (mLoc.x >= 0.0f && mLoc.x <= (float)app::getWindowWidth() - 1 &&
        mLoc.y >= 0.0f && mLoc.y <= (float)app::getWindowHeight() - 1) {
        mRadiusDest = channel.getValue(mLoc) * mScale + 0.5f;
        
//        std::cout << surface.getPixel(mouseloc) << std::endl;
        
    } else {
//        mRadiusDest = 0.1f;
        mIsDead = true;
    }
    
    mRadius -= (mRadius - mRadiusDest) * 0.1f;
    mMass = mRadius * mRadius * 0.0001f + 0.01f;
    
//    std::cout << mRadius << std::endl;
    
    mAge += (1.0f/(mRadius * mRadius));
    
    if (mAge > mLifespan) {
        mIsDead = true;
    }
    
    ColorAT<float> surf = surface.getPixel( Vec2i( (int)mLoc.x, (int)mLoc.y ) );
    
//    std::cout << surf.r << std::endl;
    
    mR = (float)surf.r + 0.5f;
    mG = (float)surf.g + 0.5f;
    mB = (float)surf.b + 0.5f;
    
}

void Particle::draw() {
    
    gl::color(mR, mG, mB);
    gl::drawSolidCircle(mLoc, mRadius);
    
}




