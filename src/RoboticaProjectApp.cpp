#include "cinder/app/AppBasic.h"
#include "cinder/audio/Io.h"
#include "cinder/audio/Output.h"
#include "cinder/audio/FftProcessor.h"
#include "cinder/audio/PcmBuffer.h"
#include "cinder/vector.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "Resources.h"
#include "PotentialField.h"
#include "ParticleController.h"


using namespace ci;
using namespace ci::app;
using namespace std;

//globals
gl::Texture *particleImg, *potFldImg;
bool ALLOWMOVE(true), ALLOWVECTORS, ALLOWFULLSCREEN, ALLOWINERTIA, ALLOWMOVEATTRACTORS, ALLOWEQUALIZER;

class RoboticaProjectApp : public AppBasic {
  public:
	void setup();
	void update();
	void draw();
    void drawFft();
    void prepareSettings(Settings *settings);
    void mouseUp( MouseEvent event );
    void keyDown( KeyEvent event );	
    
    audio::TrackRef mTrack;
	audio::PcmBuffer32fRef mPcmBuffer;
    ParticleController *particleC;
    
private:
    bool mAddParticle;
    Vec2f mAddPos;
    std::shared_ptr<float> fftRef;
    uint16_t bandCount;
};


void RoboticaProjectApp::prepareSettings( Settings *settings ){
    settings->setWindowSize( 640, 360 );
    settings->setFrameRate( 60.0f );
    
}

void RoboticaProjectApp::keyDown( KeyEvent event )
{
	char key = event.getChar();
	
	if( ( key == 'm' ) || ( key == 'M' ) )
		ALLOWMOVE = ! ALLOWMOVE;
    else if( ( key == 'a' ) || ( key == 'A' ) )
		ALLOWMOVEATTRACTORS = ! ALLOWMOVEATTRACTORS;
	else if( ( key == 'v' ) || ( key == 'V' ) )
		ALLOWVECTORS = ! ALLOWVECTORS;
    else if( ( key == 'i' ) || ( key == 'I' ) )
		ALLOWINERTIA = ! ALLOWINERTIA;
    else if( ( key == 'e' ) || ( key == 'E' ) )
		ALLOWEQUALIZER = ! ALLOWEQUALIZER;
    else if( ( key == 'f' ) || ( key == 'F' ) ){
		ALLOWFULLSCREEN = ! ALLOWFULLSCREEN;
        setFullScreen(ALLOWFULLSCREEN);
    }
}

void RoboticaProjectApp::setup()
{
    
    
    
    bandCount = 12;
    
    particleImg = new gl::Texture( loadImage( loadResource( RES_PARTICLE ) ) );
    
    //add the audio track the default audio output
	mTrack = audio::Output::addTrack( audio::load( loadResource( RES_WOLFGANG ) ) );
    mTrack.get()->setLooping(true);
    
    audio::Output::setVolume(2);
	//you must enable enable PCM buffering on the track to be able to call getPcmBuffer on it later
	mTrack->enablePcmBuffering( true );

    particleC = new ParticleController(getWindowSize(), bandCount);
    
}


void RoboticaProjectApp::mouseUp( MouseEvent event )
{
	//particleC.addParticle(event.getPos());
    mAddPos = Vec2f(event.getX(), event.getY());
    mAddParticle = true;
}


void RoboticaProjectApp::update()
{
    //get the latest pcm buffer from the track
	mPcmBuffer = mTrack->getPcmBuffer();
    
    if( mPcmBuffer ){
       // mPcmBuffer->getChannelData( audio::CHANNEL_FRONT_LEFT )
        fftRef = audio::calculateFft( mPcmBuffer->getChannelData( audio::CHANNEL_FRONT_LEFT ), bandCount );
        if(mAddParticle){
            particleC->addParticle(mAddPos);
            mAddParticle = false;
        }
        
        particleC->update(fftRef, bandCount, getElapsedSeconds());
    }    
        
    
    
}


void renderImage( Vec2f _loc, float _diam, Color _col, float _alpha )
{
	glPushMatrix();
	glTranslatef( _loc.x, _loc.y, 0 );
	glScalef( _diam, _diam, _diam );
	glColor4f( _col.r, _col.g, _col.b, _alpha );
	glBegin( GL_QUADS );
    glTexCoord2f(0, 0);    glVertex2f(-.5, -.5);
    glTexCoord2f(1, 0);    glVertex2f( .5, -.5);
    glTexCoord2f(1, 1);    glVertex2f( .5,  .5);
    glTexCoord2f(0, 1);    glVertex2f(-.5,  .5);
	glEnd();
	glPopMatrix();
}

void RoboticaProjectApp::draw()
{
	gl::clear( Color( 0.0f, 0.0f, 0.0f ) );
    glClear( GL_COLOR_BUFFER_BIT );
	
#if defined( CINDER_MAC )
    if(ALLOWEQUALIZER){
        drawFft();
    }
#endif
    
    
    glDepthMask( GL_FALSE );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    
    particleC->render();
    
}


#if defined(CINDER_MAC)
void RoboticaProjectApp::drawFft()
{
	float ht = 100.0f;
	
	
	if( ! mPcmBuffer ) return;
	
	//use the most recent Pcm data to calculate the Fft
	
	if( ! fftRef ) {
		return;
	}
	
	float * fftBuffer = fftRef.get();
	
	//draw the bands
	for( int i = 0; i < ( bandCount ); i++ ) {
		float barY = fftBuffer[i] / bandCount * ht;
		glBegin( GL_QUADS );
        glColor3f( 255.0f, 255.0f, 0.0f );
        glVertex2f( i * 3, ht );
        glVertex2f( i * 3 + 1, ht );
        glColor3f( 0.0f, 255.0f, 0.0f );
        glVertex2f( i * 3 + 1, ht - barY );
        glVertex2f( i * 3, ht - barY );
		glEnd();
	}
}
#endif

CINDER_APP_BASIC( RoboticaProjectApp, RendererGl )
