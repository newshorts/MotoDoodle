#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/Perlin.h"
#include "cinder/Channel.h"
#include "cinder/Vector.h"
#include "cinder/Utilities.h"
#include "ParticleController.h"

#define RESOLUTION 10
#define NUM_PARTICLES_TO_SPAWN 12
#define APP_WIDTH 800
#define APP_HEIGHT 600
#define FRAME_RATE 60.0f

#include <sstream>

using std::stringstream;
using namespace ci;
using namespace ci::app;
using namespace std;

class colorApp : public AppBasic {
public:
    void prepareSettings(Settings *settings);
    
    void keyDown( KeyEvent event );
    void mouseUp( MouseEvent event );
    void mouseDown( MouseEvent event );
//    void mouseMove( MouseEvent event );
    void mouseDrag( MouseEvent event );
    
	void setup();
	void update();
	void draw();
    
    Perlin mPerlin;
    
    Channel32f mChannel;
    Surface32f mSurface;
    gl::Texture mTexture;
    
    Vec2i mMouseLoc;
    Vec2f mMouseVel;
    bool mIsPressed;
    
    ParticleController mParticleController;
    
    bool mDrawImage;
    bool mDrawParticles;
    bool mSaveFrames;
    bool mCentralGravity;
    bool mAllowPerlin;
    
    int mSaveFrameCount;
};

void colorApp::prepareSettings(cinder::app::AppBasic::Settings *settings) {
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setFrameRate(FRAME_RATE);
}

void colorApp::keyDown(cinder::app::KeyEvent event) {
    if (event.getChar() == '1') {
        mDrawImage = ! mDrawImage;
    } else if (event.getChar() == '2') {
        mDrawParticles = ! mDrawParticles;
    }
    
    if (event.getChar() == 's') {
        mSaveFrames = !mSaveFrames;
    } else if (event.getChar() == 'g') {
        mCentralGravity = !mCentralGravity;
    } else if (event.getChar() == 'p') {
        mAllowPerlin = !mAllowPerlin;
    }
}

void colorApp::mouseDown(cinder::app::MouseEvent event) {
    mIsPressed = true;
}

void colorApp::mouseUp(cinder::app::MouseEvent event) {
    mIsPressed = false;
}

//void colorApp::mouseMove(cinder::app::MouseEvent event) {
//    mMouseVel = event.getPos() - mMouseLoc;
//    mMouseLoc = event.getPos();
//}

void colorApp::mouseDrag(cinder::app::MouseEvent event) {
    mMouseVel = event.getPos() - mMouseLoc;
    mMouseLoc = event.getPos();
}

void colorApp::setup()
{
    mPerlin = Perlin();
    
    Url url( "http://www.thefullsignal.com/siteimage/scale/0/0/114851.jpg" );
    mSurface = Surface32f(loadImage( loadUrl( url ) ) );
    mChannel = Channel32f( mSurface );
    mTexture = mSurface;
    
    mMouseLoc = Vec2i(0,0);
    mMouseVel = Vec2f::zero();
    
    mDrawParticles = true;
    mDrawImage = false;
    mAllowPerlin = true;
    mCentralGravity = true;
    mSaveFrames = false;
    mSaveFrameCount = 0;
    
}

void colorApp::update()
{
    if (! mChannel) {
        return;
    }
    
    if (mIsPressed) {
        mParticleController.addParticles(NUM_PARTICLES_TO_SPAWN, mMouseLoc, mMouseVel);
    }
    
    mParticleController.repulseParticles();
    
    if (mCentralGravity) {
        mParticleController.pullToCenter();
    }
    
    if (mAllowPerlin) {
        mParticleController.applyPerlin(mPerlin);
    }
    
    mParticleController.update(mChannel, mSurface, mMouseLoc);
}

void colorApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ), true );
    
    if (mDrawImage) {
        mTexture.enableAndBind();
        gl::draw(mTexture, app::getWindowBounds());
    }
    
    if (mDrawParticles) {
        glDisable(GL_TEXTURE_2D);
        mParticleController.draw();
    }
    
    if (mSaveFrames) {
        writeImage(getHomeDirectory() / ("image_" + toString(getElapsedFrames()) + ".png" ), copyWindowSurface());
    }
}

CINDER_APP_BASIC( colorApp, RendererGl )
