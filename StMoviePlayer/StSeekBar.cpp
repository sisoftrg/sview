/**
 * Copyright © 2009-2014 Kirill Gavrilov <kirill@sview.ru>
 *
 * StMoviePlayer program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * StMoviePlayer program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "StSeekBar.h"

#include <StGL/StGLProgram.h>
#include <StGL/StGLContext.h>
#include <StGLCore/StGLCore20.h>
#include <StGLWidgets/StGLRootWidget.h>

class StSeekBar::StProgramSB : public StGLProgram {

        public:

    StProgramSB() : StGLProgram("StSeekBar") {}

    StGLVarLocation getVVertexLoc() const { return StGLVarLocation(0); }
    StGLVarLocation getVColorLoc()  const { return StGLVarLocation(1); }

    void setProjMat(StGLContext&      theCtx,
                    const StGLMatrix& theProjMat) {
        theCtx.core20fwd->glUniformMatrix4fv(uniProjMatLoc, 1, GL_FALSE, theProjMat);
    }

    void use(StGLContext& theCtx) {
        StGLProgram::use(theCtx);
    }

    void use(StGLContext&  theCtx,
             const GLfloat theOpacityValue,
             const GLfloat theDispX) {
        StGLProgram::use(theCtx);
        theCtx.core20fwd->glUniform1f(uniOpacityLoc, theOpacityValue);
        if(!stAreEqual(myDispX, theDispX, 0.0001f)) {
            myDispX = theDispX;
            theCtx.core20fwd->glUniform4fv(uniDispLoc,  1, StGLVec4(theDispX, 0.0f, 0.0f, 0.0f));
        }
    }

    virtual bool init(StGLContext& theCtx) {
        const char VERTEX_SHADER[] =
           "uniform mat4  uProjMatrix;\n"
           "uniform vec4  uDisp;\n"
           "uniform float uOpacity;\n"
           "attribute vec4 vVertex;\n"
           "attribute vec4 vColor;\n"
           "varying vec4 fColor;\n"
           "void main(void) {\n"
           "    fColor = vec4(vColor.rgb, vColor.a * uOpacity);\n"
           "    gl_Position = uProjMatrix * (vVertex + uDisp);\n"
           "}\n";

        const char FRAGMENT_SHADER[] =
           "varying vec4 fColor;\n"
           "void main(void) {\n"
           "    gl_FragColor = fColor;\n"
           "}\n";

        StGLVertexShader aVertexShader(StGLProgram::getTitle());
        StGLAutoRelease aTmp1(theCtx, aVertexShader);
        aVertexShader.init(theCtx, VERTEX_SHADER);

        StGLFragmentShader aFragmentShader(StGLProgram::getTitle());
        StGLAutoRelease aTmp2(theCtx, aFragmentShader);
        aFragmentShader.init(theCtx, FRAGMENT_SHADER);
        if(!StGLProgram::create(theCtx)
           .attachShader(theCtx, aVertexShader)
           .attachShader(theCtx, aFragmentShader)
           .bindAttribLocation(theCtx, "vVertex", getVVertexLoc())
           .bindAttribLocation(theCtx, "vColor",  getVColorLoc())
           .link(theCtx)) {
            return false;
        }

        uniProjMatLoc = StGLProgram::getUniformLocation(theCtx, "uProjMatrix");
        uniDispLoc    = StGLProgram::getUniformLocation(theCtx, "uDisp");
        uniOpacityLoc = StGLProgram::getUniformLocation(theCtx, "uOpacity");
        return uniProjMatLoc.isValid() && uniOpacityLoc.isValid();
    }

        private:

    GLfloat         myDispX;
    StGLVarLocation uniProjMatLoc;
    StGLVarLocation uniDispLoc;
    StGLVarLocation uniOpacityLoc;

};

StSeekBar::StSeekBar(StGLWidget* theParent,
                     const int   theTop)
: StGLWidget(theParent, 0, theTop, StGLCorner(ST_VCORNER_BOTTOM, ST_HCORNER_CENTER),
             theParent->getRoot()->scale(512), theParent->getRoot()->scale(12 + 8)),
  myProgram(new StProgramSB()),
  myProgress(0.0f),
  myProgressPx(0) {
    StGLWidget::signals.onMouseUnclick.connect(this, &StSeekBar::doMouseUnclick);
}

StSeekBar::~StSeekBar() {
    StGLContext& aCtx = getContext();
    if(!myProgram.isNull()) {
        myProgram->release(aCtx);
    }
    myVertices.release(aCtx);
    myColors.release(aCtx);
}

void StSeekBar::stglResize() {
    StGLWidget::stglResize();
    StGLContext& aCtx = getContext();

    // update pixel coordinates
    changeRectPx().right() = getParent()->getRectPx().width() - myRoot->scale(128);

    stglUpdateVertices();

    // update projection matrix
    if(!myProgram.isNull()) {
        myProgram->use(aCtx);
        myProgram->setProjMat(aCtx, getRoot()->getScreenProjection());
        myProgram->unuse(aCtx);
    }
}

void StSeekBar::stglUpdateVertices() {
    StArray<StGLVec2> aVertices(12);

    // black border quad
    StRectI_t aRectPx(getRectPxAbsolute());
    aRectPx.top()    += getRoot()->scale(4); // make bar virtually bigger remaining the same visible size
    aRectPx.bottom() -= getRoot()->scale(4); // to simplify clicking on touch-screens

    getRoot()->getRectGl(aRectPx, aVertices, 0);

    // inner empty quad
    aRectPx.top()    += 1;
    aRectPx.bottom() -= 1;
    aRectPx.left()   += 1;
    aRectPx.right()  -= 1;
    getRoot()->getRectGl(aRectPx, aVertices, 4);

    // inner filled quad
    myProgressPx = int(myProgress * GLfloat(aRectPx.width()));
    myProgressPx = stClamp(myProgressPx, 0, aRectPx.width());
    aRectPx.right() = aRectPx.left() + myProgressPx;
    getRoot()->getRectGl(aRectPx, aVertices, 8);

    myVertices.init(getContext(), aVertices);
}

bool StSeekBar::stglInit() {
    StGLContext& aCtx = getContext();
    const GLfloat COLORS[4 * 12] = {
        // black border colors
        0.0f, 0.0f, 0.0f, 1.0f, // quad's top-right
        0.0f, 0.0f, 0.0f, 1.0f, // quad's bottom-right
        0.0f, 0.0f, 0.0f, 1.0f, // quad's top-left
        0.0f, 0.0f, 0.0f, 1.0f, // quad's bottom-left
        // empty color
        0.3f, 0.3f, 0.3f, 1.0f, // quad's top-right
        0.3f, 0.3f, 0.3f, 1.0f, // quad's bottom-right
        0.3f, 0.3f, 0.3f, 1.0f, // quad's top-left
        0.3f, 0.3f, 0.3f, 1.0f, // quad's bottom-left
        // fill color
        0.13f, 0.35f, 0.49f, 1.0f, // quad's top-right
        0.13f, 0.35f, 0.49f, 1.0f, // quad's bottom-right
        0.13f, 0.35f, 0.49f, 1.0f, // quad's top-left
        0.13f, 0.35f, 0.49f, 1.0f  // quad's bottom-left
    };

    myVertices.init(aCtx); // just generate buffer
    myColors.init(aCtx, 4, 12, COLORS);

    stglUpdateVertices();

    return myProgram->init(aCtx);
}

void StSeekBar::stglDraw(unsigned int ) {
    StGLContext& aCtx = getContext();

    // need to update vertices buffer?
    if(myProgressPx != int(myProgress * GLfloat((getRectPx().width() - 2)))) {
        stglUpdateVertices();
    }

    aCtx.core20fwd->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    aCtx.core20fwd->glEnable(GL_BLEND);
    myProgram->use(aCtx, GLfloat(opacityValue), getRoot()->getScreenDispX());

    myVertices.bindVertexAttrib(aCtx, myProgram->getVVertexLoc());
    myColors  .bindVertexAttrib(aCtx, myProgram->getVColorLoc());

    aCtx.core20fwd->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    aCtx.core20fwd->glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

    if(myProgressPx >= 1) {
        aCtx.core20fwd->glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    }

    myColors  .unBindVertexAttrib(aCtx, myProgram->getVColorLoc());
    myVertices.unBindVertexAttrib(aCtx, myProgram->getVVertexLoc());

    myProgram->unuse(aCtx);
    aCtx.core20fwd->glDisable(GL_BLEND);
}

void StSeekBar::doMouseUnclick(const int mouseBtn) {
    signals.onSeekClick(mouseBtn, getPointIn(getRoot()->getCursorZo()).x());
}
