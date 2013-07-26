//
//  ScrapeVisualSystem.cpp
//

#include "ScrapeVisualSystem.h"

#include "CloudsRGBDCombinedRenderer.h"
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

void ScrapeVisualSystem::selfSetup()
}

void ScrapeVisualSystem::selfPresetLoaded(string presetPath){
	
}

void ScrapeVisualSystem::selfBegin()
{
    doGrow();
}

void ScrapeVisualSystem::selfEnd()
{
    for (int i = 0; i < boxes.size(); i++) {
        delete boxes[i];
    }
    boxes.clear();
}

void ScrapeVisualSystem::selfExit()
{
    
}

void ScrapeVisualSystem::selfSetupSystemGui()
{
   
}

void ScrapeVisualSystem::selfSetupRenderGui()
{
    
}

void ScrapeVisualSystem::guiSystemEvent(ofxUIEventArgs &e)
{
    
}

void ScrapeVisualSystem::selfKeyPressed(ofKeyEventArgs & args)
{
    
}

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
        boxes[i]->scale = boxes[i]->tween.update();
        
        if (!boxes[i]->tween.isCompleted()) {
            bComplete = false;
        }
    }
}

void ScrapeVisualSystem::selfDraw()
{
}

void ScrapeVisualSystem::selfDrawBackground()
{
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

void ScrapeVisualSystem::selfDrawDebug()
{
    
}

void ScrapeVisualSystem::selfSceneTransformation()
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

void ScrapeVisualSystem::selfSetupGui()
{
    
}

void ScrapeVisualSystem::selfGuiEvent(ofxUIEventArgs &e)
{
    
}

void ScrapeVisualSystem::guiRenderEvent(ofxUIEventArgs &e)
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
        box->tween.setParameters(i, easing, ofxTween::easeOut, 0, 1, ofRandom(500), ofRandom(1000));
        box->tween.start();
        
        boxes.push_back(box);
    }
    
    bGrowing = true;
}

void ScrapeVisualSystem::doShrink()
{
    // Shrink all the boxes back to 0 in sync.
    for (int i = 0; i < boxes.size(); i++) {
        boxes[i]->tween.setParameters(i, easing, ofxTween::easeOut, 1, 0, 500, 500);
        boxes[i]->tween.start();
    }
    
    bGrowing = false;
}
