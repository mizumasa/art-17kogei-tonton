#include "ofMain.h"
#include "ofApp.h"

#include "ofGLProgrammableRenderer.h"

/* simple model space
 ^ tilt+
 |
 z+  y+
 |  /        . ->pan+
 | /     .
 |/  .
 *C  *--------x+
 cameraPos(0,-300,300), lookAt((0,0,0),up=(0,0,1))
 /
 /
 /
 y-
 */

//========================================================================
int main( ){
	ofGLWindowSettings s;
	s.setGLVersion(3,2);

	ofCreateWindow(s);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}
