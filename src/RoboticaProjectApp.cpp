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


//globals
gl::Texture *particleImg, *potFldImg;

class RoboticaProjectApp : public AppBasic {
  public:
	void setup();
	void update();
	void draw();
    void drawFft();
    void mouseUp( MouseEvent event );
    
    audio::TrackRef mTrack;
	audio::PcmBuffer32fRef mPcmBuffer;
    ParticleController particleC;
    
    
    bool mAddParticle;
    Vec2f mAddPos;
};

void RoboticaProjectApp::setup()
{
    
    particleImg = new gl::Texture( loadImage( loadResource( RES_PARTICLE ) ) );
    
    //add the audio track the default audio output
	mTrack = audio::Output::addTrack( audio::load( loadResource( RES_I_NEED_A_GUIDE ) ) );
	
	//you must enable enable PCM buffering on the track to be able to call getPcmBuffer on it later
	mTrack->enablePcmBuffering( true );

    
    
	//potFldImg = new gl::Texture( loadImage( loadResource( RES_POT_FIELD ) ) );
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

    if(mAddParticle){
        particleC.addParticle(mAddPos);
        mAddParticle = false;
    }
    
    particleC.update();
    
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
	
	glPushMatrix();
    glTranslatef( 0.0, 0.0, 0.0 );
#if defined( CINDER_MAC )
        drawFft();
#endif
	glPopMatrix();
    
    
    glDepthMask( GL_FALSE );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    
    particleC.render();
    
}


#if defined(CINDER_MAC)
void RoboticaProjectApp::drawFft()
{
	float ht = 200.0f;
	uint16_t bandCount = 12;
	
	if( ! mPcmBuffer ) return;
	
	//use the most recent Pcm data to calculate the Fft
	std::shared_ptr<float> fftRef = audio::calculateFft( mPcmBuffer->getChannelData( audio::CHANNEL_FRONT_LEFT ), bandCount );
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
