/**
 * Copyright © 2010-2014 Kirill Gavrilov <kirill@sview.ru>
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file license-boost.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __StGLImageProgram_h_
#define __StGLImageProgram_h_

#include <StGL/StGLSaturationMatrix.h>
#include <StGL/StGLBrightnessMatrix.h>
#include <StGL/StGLProgramMatrix.h>
#include <StGLMesh/StGLMesh.h>
#include <StImage/StImage.h>
#include <StSettings/StFloat32Param.h>

/**
 * GLSL program for Image Region widget.
 */
class StGLImageProgram : public StGLProgramMatrix<1, 5, StGLMeshProgram> {

        public:

    typedef enum tagTextureFilter {
        FILTER_NEAREST, //!< ugly filter
        FILTER_LINEAR,  //!< linear filter
        FILTER_BLEND,   //!< blend deinterlace filter
    } TextureFilter;

    /**
     * Color conversion options in GLSL Fragment Shader.
     */
    enum FragSection {
        FragSection_Main = 0, //!< section with main() function
        FragSection_GetColor, //!< read color values from textures
        FragSection_ToRgb,    //!< color conversion
        FragSection_Correct,  //!< color correction
        FragSection_Gamma,    //!< gamma correction
        FragSection_NB
    };

    /**
     * Color getter options in GLSL Fragment Shader.
     */
    enum FragGetColor {
        FragGetColor_Normal = 0,
        FragGetColor_Blend,
        FragGetColor_NB
    };

    /**
     * Color conversion options in GLSL Fragment Shader.
     */
    enum FragToRgb {
        FragToRgb_FromRgb = 0,
        FragToRgb_FromRgba,
        FragToRgb_FromGray,
        FragToRgb_FromYuvFull,
        FragToRgb_FromYuvMpeg,
        FragToRgb_FromYuv9Full,
        FragToRgb_FromYuv9Mpeg,
        FragToRgb_FromYuv10Full,
        FragToRgb_FromYuv10Mpeg,
        FragToRgb_NB,
    };

    /**
     * Color correction options in GLSL Fragment Shader.
     */
    enum FragCorrect {
        FragCorrect_Off = 0,
        FragCorrect_On,
        FragCorrect_NB
    };

    /**
     * Gamma correction options in GLSL Fragment Shader.
     */
    enum FragGamma {
        FragGamma_Off = 0,
        FragGamma_On,
        FragGamma_NB
    };

        public:

    ST_CPPEXPORT StGLImageProgram(const StString& theTitle);

    ST_CPPEXPORT virtual ~StGLImageProgram();

    ST_CPPEXPORT void setTextureSizePx(StGLContext&    theCtx,
                                       const StGLVec2& theVec2);

    ST_CPPEXPORT void setTextureMainDataSize(StGLContext&    theCtx,
                                             const StGLVec4& theTexDataVec4);

    ST_CPPEXPORT void setTextureUVDataSize(StGLContext&    theCtx,
                                           const StGLVec4& theTexDataVec4);

    ST_LOCAL void setColorScale(const StGLVec3& theScale) {
        myColorScale = theScale;
    }

    ST_LOCAL void resetColorScale() {
        myColorScale = StGLVec3(1.0f, 1.0f, 1.0f);
    }

    /**
     * Initialize default shaders, nothing more.
     */
    ST_CPPEXPORT virtual bool init(StGLContext&                 theCtx,
                                   const StImage::ImgColorModel theColorModel,
                                   const StImage::ImgColorScale theColorScale,
                                   const FragGetColor           theFilter);

        public: //!< Properties

    struct {

        StHandle<StFloat32Param> gamma;      //!< gamma correction coefficient
        StHandle<StFloat32Param> brightness; //!< brightness level
        StHandle<StFloat32Param> saturation; //!< saturation value

    } params;

        private: //!< callback Slots

    ST_LOCAL void setupCorrection(StGLContext& theCtx);

        protected:

    ST_LOCAL bool hasNoColorScale() const {
        return myColorScale.r() > 0.9f
            && myColorScale.g() > 0.9f
            && myColorScale.b() > 0.9f;
    }

        protected:

    StGLVarLocation uniTexMainDataLoc;
    StGLVarLocation uniTexUVDataLoc;
    StGLVarLocation uniTexSizePxLoc;
    StGLVarLocation uniTexelSizePxLoc;
    StGLVarLocation uniColorProcessingLoc;
    StGLVarLocation uniGammaLoc;

    StGLVec3        myColorScale; //!< scale filter for de-anaglyph processing

};

#endif //__StGLImageProgram_h_
