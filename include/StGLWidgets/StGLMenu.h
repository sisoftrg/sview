/**
 * Copyright © 2009-2014 Kirill Gavrilov <kirill@sview.ru>
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file license-boost.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __StGLMenu_h_
#define __StGLMenu_h_

#include <StGLWidgets/StGLTextArea.h>
#include <StGL/StGLVertexBuffer.h>

// forward declarations
class StAction;
class StBoolParam;
class StInt32Param;
class StFloat32Param;
class StGLMenuItem;
class StGLMenuProgram;

/**
 * Widget represents classical menu object.
 */
class StGLMenu : public StGLWidget {

        public:

    enum {
        MENU_VERTICAL   = 0,
        MENU_HORIZONTAL = 1,
        MENU_ZERO       = 2,
    };

    // recursively delete all submenus and that this menu itself
    // should be used only for dynamic menu recreation
    ST_CPPEXPORT static void DeleteWithSubMenus(StGLMenu* theMenu);

    ST_CPPEXPORT StGLMenu(StGLWidget* theParent,
                          const int   theLeft,
                          const int   theTop,
                          const int   theOrient = MENU_VERTICAL,
                          const bool  theIsRootMenu = false);

    ST_CPPEXPORT virtual ~StGLMenu();

    ST_CPPEXPORT virtual void setVisibility(bool isVisible, bool isForce);
    ST_CPPEXPORT virtual void stglResize();
    ST_CPPEXPORT virtual bool stglInit();
    ST_CPPEXPORT virtual void stglDraw(unsigned int theView);
    ST_CPPEXPORT virtual bool tryUnClick(const StPointD_t& theCursorZo, const int& theMouseBtn, bool& theIsItemUnclicked);

    inline bool isRootMenu() const {
        return myIsRootMenu;
    }

    inline bool isActive() const {
        return myIsActive;
    }

    inline void setActive(const bool isActive) {
        myIsActive = isActive;
    }

    /**
     * Skip mouse unclick event - keep menu in active state.
     */
    inline void setKeepActive() {
        myKeepActive = true;
    }

    inline int getOrient() const {
        return myOrient;
    }

    /**
     * @return height of item of standard size
     */
    inline int getItemHeight() const {
        return myItemHeight;
    }

    /**
     * @return maximum width of item in this menu
     */
    inline int getItemWidth() const {
        return myWidth;
    }

    /**
     * Setup background color of menu.
     */
    inline void setColor(const StGLVec3& theColor) {
        myColorVec.rgb() = theColor;
    }

    /**
     * Draw bounds rectangle (off by default).
     */
    inline void setShowBounds(const bool theToShow) {
        myToDrawBounds = theToShow;
    }

    /**
     * Update all children menus layout.
     */
    ST_CPPEXPORT void stglUpdateSubmenuLayout();

    ST_CPPEXPORT StGLMenuItem* addItem(const StString& theLabel,
                                       const size_t    theUserData = 0);
    ST_CPPEXPORT StGLMenuItem* addItem(const StString& theLabel,
                                       StGLMenu*       theSubMenu);

    /**
     * Append checkbox menu item.
     * @param theLabel        menu item text
     * @param theTrackedValue tracked boolean value
     * @return created menu item widget
     */
    ST_CPPEXPORT StGLMenuItem* addItem(const StString&              theLabel,
                                       const StHandle<StBoolParam>& theTrackedValue);

    /**
     * Append radio button menu item.
     * @param theLabel        menu item text
     * @param theTrackedValue tracked integer (enumeration) value
     * @param theOnValue      associated integer (enumeration) value for this radio button
     * @return created menu item widget
     */
    ST_CPPEXPORT StGLMenuItem* addItem(const StString&               theLabel,
                                       const StHandle<StInt32Param>& theTrackedValue,
                                       const int32_t                 theOnValue);

    /**
     * Append radio button menu item.
     * @param theLabel        menu item text
     * @param theTrackedValue tracked float value
     * @param theOnValue      associated float value for this radio button
     * @return created menu item widget
     */
    ST_CPPEXPORT StGLMenuItem* addItem(const StString&                 theLabel,
                                       const StHandle<StFloat32Param>& theTrackedValue,
                                       const float                     theOnValue);

    /**
     * Append menu item with bound action.
     * @param theLabel  menu item text
     * @param theAction action to invoke on click
     * @return created menu item widget
     */
    ST_CPPEXPORT StGLMenuItem* addItem(const StString&           theLabel,
                                       const StHandle<StAction>& theAction,
                                       StGLMenu*                 theSubMenu = NULL);

        protected: //! @name protected fields

    StGLShare<StGLMenuProgram> myProgram;
    StGLVertexBuffer           myVertexBuf;
    StGLVertexBuffer           myVertexBndBuf;
    StGLVec4                   myColorVec;
    int                        myOrient;
    int                        myItemHeight;
    int                        myWidth;
    bool                       myIsRootMenu;
    bool                       myIsActive;
    bool                       myKeepActive;
    bool                       myIsInitialized;
    bool                       myToDrawBounds;

};

#endif //__StGLMenu_h_
