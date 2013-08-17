/**
 * Copyright © 2010-2013 Kirill Gavrilov <kirill@sview.ru>
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file license-boost.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __StGLSubtitles_h_
#define __StGLSubtitles_h_

#include <StGLWidgets/StGLTextArea.h>
#include <StGLWidgets/StSubQueue.h>

// dummy
template<>
inline void StArray<StHandle <StSubItem> >::sort() {}

/**
 * Subtitles widget.
 */
class StGLSubtitles : public StGLTextArea {

        private:

    /**
     * This class groups active subtitle items (with interleaved show time).
     * In most cases will contain just one item.
     */
    class StSubShowItems : public StArrayList<StHandle <StSubItem> > {

            public:

        StString myText; //!< active string representation

            public:

        /**
         * Default constructor.
         */
        ST_LOCAL StSubShowItems();

        /**
         * Remove subtitle items with outdated PTS.
         * @param thePTS (const double ) - current presentation timestamp;
         * @return true if active representation was changed (items were removed).
         */
        ST_LOCAL bool pop(const double thePTS);

        /**
         * Append subtitle item.
         */
        ST_LOCAL void add(const StHandle<StSubItem>& theItem);

    };

        public:

    ST_CPPEXPORT StGLSubtitles(StGLWidget*                 theParent,
                               const StHandle<StSubQueue>& theSubQueue);
    ST_CPPEXPORT virtual ~StGLSubtitles();
    ST_CPPEXPORT virtual const StString& getClassName();
    ST_CPPEXPORT virtual void stglUpdate(const StPointD_t& thePointZo);
    ST_CPPEXPORT virtual void stglResize();

    /**
     * Retrieve handle to the queue.
     */
    ST_CPPEXPORT const StHandle<StSubQueue>& getQueue() const;

    /**
     * Update PTS.
     */
    ST_CPPEXPORT void setPTS(const double thePTS);

        private:

    StHandle<StSubQueue> myQueue;     //!< thread-safe subtitles queue
    StSubShowItems       myShowItems; //!< active (shown) subtitle items
    double               myPTS;       //!< active PTS

};

#endif //__StGLSubtitles_h_
