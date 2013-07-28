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
    
    b3DToggle = false;
    customGui->addToggle("Go 3D!", &b3DToggle);

    customGui->addSpacer();
    
    boxDivWidth = 24;
    boxDivHeight = 24;
    boxMaxSubDivs = 8;
    customGui->addSlider("Box Div Width", 1, 256, &boxDivWidth);
	customGui->addSlider("Box Div Height", 1, 256, &boxDivHeight);
	customGui->addSlider("Box Max Subdivisions", 1, 24, &boxMaxSubDivs);
	
    customGui->addSpacer();
    
    fadeInDuration = 500;
    fadeInDelay = 1000;
    fadeOutDuration = 500;
    fadeOutDelay = 500;
	customGui->addSlider("Fade In Duration", 50, 3000, &fadeInDuration);
	customGui->addSlider("Fade In Delay", 0, 3000, &fadeInDelay);
	customGui->addSlider("Fade Out Duration", 50, 3000, &fadeOutDuration);
	customGui->addSlider("Fade Out Delay", 0, 3000, &fadeOutDelay);
    
	ofAddListener(customGui->newGUIEvent, this, &ScrapeVisualSystem::selfGuiEvent);
	
	guis.push_back(customGui);
	guimap[customGui->getName()] = customGui;
}

void ScrapeVisualSystem::selfGuiEvent(ofxUIEventArgs &e){

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
    fboSize = ofNextPow2(MAX(ofGetWidth(), ofGetHeight()));
    contentFbo.allocate(fboSize, fboSize);
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
        if (bGrowing) {
            boxes[i]->scale = boxes[i]->tween.update();
        }
        else {
            boxes[i]->alpha = boxes[i]->tween.update();
        }
        
        if (!boxes[i]->tween.isCompleted()) {
            bComplete = false;
        }
    }
    
    ofPushStyle();
    
    // Draw the textures into the FBO.
    contentFbo.begin();
    {
        ofClear(0, 0);
        
        // Draw the Scrape boxes.
        for (int i = 0; i < boxes.size(); i++) {
            ofSetColor(255, 255 * boxes[i]->alpha);
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
    contentFbo.end();
    
    ofPopStyle();
}

// selfDraw draws in 3D using the default ofEasyCamera
// you can change the camera by returning getCameraRef()
void ScrapeVisualSystem::selfDraw()
{
    if (!b3DToggle) return;
    
    // Scale up the texture since we're not working with normalized tex coords.
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    ofScale(contentFbo.getWidth(), contentFbo.getHeight());
    glMatrixMode(GL_MODELVIEW);
    
    // Draw the sphere.
    contentFbo.getTextureReference().bind();
    {
        ofSphere(0, 0, 0, 128);
    }
    contentFbo.getTextureReference().unbind();
    
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// draw any debug stuff here
void ScrapeVisualSystem::selfDrawDebug()
{
    
}

// or you can use selfDrawBackground to do 2D drawings that don't use the 3D camera
void ScrapeVisualSystem::selfDrawBackground()
{
    if (b3DToggle) return;
    
    contentFbo.draw(0, 0);
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
    boxFitting.setup(fboSize, fboSize, boxDivWidth, boxDivHeight, boxMaxSubDivs);
    boxFitting.generate(ofGetSystemTime());
    
    // Build some scrape boxes with the box fitting data and grow them.
    for (int i = 0; i < boxFitting.boxes.size(); i++) {
        ScrapeBox * box =  new ScrapeBox();
        box->x = boxFitting.boxes[i].x + boxFitting.boxes[i].w / 2.0;
        box->y = boxFitting.boxes[i].y + boxFitting.boxes[i].h / 2.0;
        box->w = boxFitting.boxes[i].w;
        box->h = boxFitting.boxes[i].h;
        box->scale = 0.0f;
        box->alpha = 1.0f;
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
