//
//  ScrapeVisualSystem.cpp
//

#include "ScrapeVisualSystem.h"

#include "CloudsRGBDVideoPlayer.h"
#ifdef AVF_PLAYER
#include "ofxAVFVideoPlayer.h"
#endif

static const GLint kFormats[] =
{
    GL_RGB,
    GL_RGBA,
    GL_LUMINANCE,
    GL_LUMINANCE_ALPHA
};

static const int kNumFormats = 4;

static ofxEasingQuad easing;

//These methods let us add custom GUI parameters and respond to their events
void ScrapeVisualSystem::selfSetupGui()
{    
	customGui = new ofxUISuperCanvas("SCRAPE", gui);
	customGui->copyCanvasStyle(gui);
	customGui->copyCanvasProperties(gui);
	customGui->setName("Scrape");
	customGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
	
    fadeInDuration = 500;
    fadeInDelay = 1000;
    fadeOutDuration = 500;
    fadeOutDelay = 500;
	customGui->addSlider("Fade In Duration", 1, 3000, &fadeInDuration);
	customGui->addSlider("Fade In Delay", 0, 3000, &fadeInDelay);
	customGui->addSlider("Fade Out Duration", 1, 3000, &fadeOutDuration);
	customGui->addSlider("Fade Out Delay", 0, 3000, &fadeOutDelay);
	customGui->addButton("Custom Button", false);
	customGui->addToggle("Custom Toggle", &customToggle);
	
	ofAddListener(customGui->newGUIEvent, this, &ScrapeVisualSystem::selfGuiEvent);
	
	guis.push_back(customGui);
	guimap[customGui->getName()] = customGui;
}

void ScrapeVisualSystem::selfGuiEvent(ofxUIEventArgs &e){
	if(e.widget->getName() == "Custom Button"){
		cout << "Button pressed!" << endl;
	}
}

//Use system gui for global or logical settings, for exmpl
void ScrapeVisualSystem::selfSetupSystemGui(){
	
}

void ScrapeVisualSystem::guiSystemEvent(ofxUIEventArgs &e){
	
}
//use render gui for display settings, like changing colors
void ScrapeVisualSystem::selfSetupRenderGui(){
    
}

void ScrapeVisualSystem::guiRenderEvent(ofxUIEventArgs &e){
	
}

// selfSetup is called when the visual system is first instantiated
// This will be called during a "loading" screen, so any big images or
// geometry should be loaded here
void ScrapeVisualSystem::selfSetup()
{
//	someImage.loadImage( getVisualSystemDataPath() + "images/someImage.png";
//    tex.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
}

// selfPresetLoaded is called whenever a new preset is triggered
// it'll be called right before selfBegin() and you may wish to
// refresh anything that a preset may offset, such as stored colors or particles
void ScrapeVisualSystem::selfPresetLoaded(string presetPath){
	
}

// selfBegin is called when the system is ready to be shown
// this is a good time to prepare for transitions
// but try to keep it light weight as to not cause stuttering
void ScrapeVisualSystem::selfBegin()
{
    doGrow();
}

//do things like ofRotate/ofTranslate here
//any type of transformation that doesn't have to do with the camera
void ScrapeVisualSystem::selfSceneTransformation()
{
    
}

//normal update call
void ScrapeVisualSystem::selfUpdate()
{
    //    if (ofGetFrameNum() % 60 == 0) {
    //        float coin = ofRandomuf();
    //        int type;
    //        if (coin < 0.25) type = GL_RGB;
    //        else if (coin < 0.5) type = GL_RGBA;
    //        else if (coin < 0.75) type = GL_LUMINANCE;
    //        else type = GL_LUMINANCE_ALPHA;
    //        tex.allocate(ofGetWidth(), ofGetHeight(), type);
    //    }
    
    if (bComplete) {
        if (bGrowing) {
            doShrink();
        }
        else {
            doGrow();
        }
    }
    
    // Update the Scrape box scales, checking if all the animations are complete.
    bComplete = true;
    for (int i = 0; i < boxes.size(); i++) {
        boxes[i]->scale = boxes[i]->tween.update();
        
        if (!boxes[i]->tween.isCompleted()) {
            bComplete = false;
        }
    }
}

// selfDraw draws in 3D using the default ofEasyCamera
// you can change the camera by returning getCameraRef()
void ScrapeVisualSystem::selfDraw()
{
    //	sharedRenderer->setShaderPath();
    //	sharedRenderer->bind();
    //	sharedRenderer->unbind();
    
    //    sharedRenderer->getPlayer().getAmplitude();
    //
    //    mat->begin();
    //    ofSphere(20);
    //    mat->end();
    
    //    bool mode = true;
    //    float offset = MIN(1.0, MAX(0.01, mouseX / (ofGetWidth() * 1.0)));
    //    hypnoRect(1.0, offset, mode);
}

// draw any debug stuff here
void ScrapeVisualSystem::selfDrawDebug()
{
    
}

// or you can use selfDrawBackground to do 2D drawings that don't use the 3D camera
void ScrapeVisualSystem::selfDrawBackground()
{
    //    tex.draw(0, 0);
    
    // Draw the Scrape boxes.
    if (bGrowing) {
        ofSetColor(255);
    }
    for (int i = 0; i < boxes.size(); i++) {
        if (!bGrowing) {
            ofSetColor(255, 255 * boxes[i]->scale);
        }
        boxes[i]->tex.draw(boxes[i]->x - (boxes[i]->w * boxes[i]->scale) / 2.0, boxes[i]->y - (boxes[i]->h * boxes[i]->scale) / 2.0, boxes[i]->w * boxes[i]->scale, boxes[i]->h * boxes[i]->scale);
    }
    
    // Draw some outlines.
    //    ofNoFill();
    //    ofSetColor(ofColor::white);
    //    for (int i = 0; i < boxes.size(); i++) {
    //        ofRect(boxes[i]->x - boxes[i]->w / 2.0, boxes[i]->y - boxes[i]->h / 2.0, boxes[i]->w, boxes[i]->h);
    //    }
    //    ofFill();
}

// this is called when your system is no longer drawing.
// Right after this selfUpdate() and selfDraw() won't be called any more
void ScrapeVisualSystem::selfEnd()
{
    for (int i = 0; i < boxes.size(); i++) {
        delete boxes[i];
    }
    boxes.clear();
}

// this is called when you should clear all the memory and delet anything you made in setup
void ScrapeVisualSystem::selfExit()
{
    
}

//events are called when the system is active
//Feel free to make things interactive for you, and for the user!
void ScrapeVisualSystem::selfKeyPressed(ofKeyEventArgs & args)
{
    
}

void ScrapeVisualSystem::selfKeyReleased(ofKeyEventArgs & args)
{
    
}

void ScrapeVisualSystem::selfMouseDragged(ofMouseEventArgs& data)
{
    
}

void ScrapeVisualSystem::selfMouseMoved(ofMouseEventArgs& data)
{

}

void ScrapeVisualSystem::selfMousePressed(ofMouseEventArgs& data)
{
    
}

void ScrapeVisualSystem::selfMouseReleased(ofMouseEventArgs& data)
{
    
}

void ScrapeVisualSystem::doGrow()
{
    // Clear the previous boxes.
    if (boxes.size()) {
        for (int i = 0; i < boxes.size(); i++) {
            delete boxes[i];
        }
        boxes.clear();
    }
    
    // Create a box fitting grid.
    ofxMtlBoxFitting boxFitting;
    boxFitting.setup(ofGetWidth(), ofGetHeight(), 24, 24, 8);
    boxFitting.generate(ofGetSystemTime());
    
    // Build some scrape boxes with the box fitting data and grow them.
    for (int i = 0; i < boxFitting.boxes.size(); i++) {
        ScrapeBox * box =  new ScrapeBox();
        box->x = boxFitting.boxes[i].x + boxFitting.boxes[i].w / 2.0;
        box->y = boxFitting.boxes[i].y + boxFitting.boxes[i].h / 2.0;
        box->w = boxFitting.boxes[i].w;
        box->h = boxFitting.boxes[i].h;
        box->scale = 0.0f;
        box->tex.allocate(box->w, box->h, kFormats[(int)ofRandom(kNumFormats)]);
//        box->tex.allocate(box->w, box->h, GL_RGBA);
        box->tween.setParameters(i, easing, ofxTween::easeOut, 0, 1, ofRandom(fadeInDuration), ofRandom(fadeInDelay));
        box->tween.start();
        
        boxes.push_back(box);
    }
    
    bGrowing = true;
}

void ScrapeVisualSystem::doShrink()
{
    // Shrink all the boxes back to 0 in sync.
    for (int i = 0; i < boxes.size(); i++) {
        boxes[i]->tween.setParameters(i, easing, ofxTween::easeOut, 1, 0, fadeOutDuration, fadeOutDelay);
        boxes[i]->tween.start();
    }
    
    bGrowing = false;
}
