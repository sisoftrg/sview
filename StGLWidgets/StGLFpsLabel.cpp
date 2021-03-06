/**
 * Copyright © 2013 Kirill Gavrilov <kirill@sview.ru
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file license-boost.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt
 */

#include <StGLWidgets/StGLFpsLabel.h>
#include <StGLWidgets/StGLRootWidget.h>

#include <StGL/StGLContext.h>
#include <StGLCore/StGLCore20.h>

StGLFpsLabel::StGLFpsLabel(StGLWidget* theParent)
: StGLTextArea(theParent,
              -theParent->getRoot()->scale(32),
               theParent->getRoot()->scale(32),
               StGLCorner(ST_VCORNER_TOP, ST_HCORNER_RIGHT),
               theParent->getRoot()->scale(128),
               theParent->getRoot()->scale(32)),
  myPlayFps(-1.0),
  myPlayQueued(0),
  myPlayQueueLen(0),
  myTimer(true),
  myCounter(0) {
    StGLWidget::signals.onMouseUnclick.connect(this, &StGLFpsLabel::doMouseUnclick);

    setupAlignment(StGLTextFormatter::ST_ALIGN_X_CENTER,
                   StGLTextFormatter::ST_ALIGN_Y_CENTER);
    setBorder(true);
    setBackColor(StGLVec3(0.88f, 0.88f, 0.88f));
    setText("0.0");
}

StGLFpsLabel::~StGLFpsLabel() {
    //
}

void StGLFpsLabel::doMouseUnclick(const int ) {
    signals.onBtnClick(getUserData());
}

void StGLFpsLabel::update(const bool   theIsStereo,
                          const double theTargetFps) {
    char aBuffer[128];
    const double aTime = myTimer.getElapsedTimeInSec();
    if(aTime >= 1.0) {
        myTimer.restart();
        const double aFpsCurrent = double(myCounter) / aTime;
        if(myPlayFps <= 0.0) {
            stsprintf(aBuffer, 128, "%c %4.1f (%4.1f)",
                      theIsStereo ? 'S' : 'M', aFpsCurrent, theTargetFps);
        } else {
            stsprintf(aBuffer, 128, "%c %4.1f (%4.1f)\n%d / %d [%4.1f]",
                      theIsStereo ? 'S' : 'M', aFpsCurrent, theTargetFps,
                      myPlayQueued, myPlayQueueLen, myPlayFps);
        }
        setText(aBuffer);
        myCounter = 0;
    }
    ++myCounter;
}
