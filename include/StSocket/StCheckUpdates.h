/**
 * Copyright © 2009-2014 Kirill Gavrilov <kirill@sview.ru>
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file license-boost.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __StCheckUpdates_h_
#define __StCheckUpdates_h_

#include <StFile/StRawFile.h>

#include <StThreads/StMutex.h>
#include <StThreads/StThread.h>
#include <StVersion.h>

/**
 * Helper class to check sView for updates.
 */
class StCheckUpdates {

        public:

    StCheckUpdates()
    : myIsInitialized(false),
      myToUpdate(false) {
        //
    }

    ~StCheckUpdates() {
        if(!myThread.isNull()) {
            myThread->wait();
        }
    }

    bool isNeedUpdate() {
        myMutex.lock();
        const bool aResult = myToUpdate;
        myMutex.unlock();
        return aResult;
    }

    void setNeedUpdate(bool theToUpdate) {
        myMutex.lock();
        myToUpdate = theToUpdate;
        myMutex.unlock();
    }

    bool isInitialized() {
        myMutex.lock();
        const bool aResult = myIsInitialized;
        myMutex.unlock();
        return aResult;
    }

    void setInitialized() {
        myMutex.lock();
        myIsInitialized = true;
        myMutex.unlock();
    }

    void init() {
        /// TODO (Kirill Gavrilov#1) instability in browsers???
        myMutex.lock();
        if(!myThread.isNull()) {
            myMutex.unlock();
            return;
        }
        myThread = new StThread(checkUpdatesThread, this, "StCheckUpdates");
        myMutex.unlock();
    }

    static bool checkUpdates() {
        const char     THE_URL_TEMPLATE[] = "http://www.sview.ru/updates/?appl=sView&year=%d&month=%d&rStatus=%d&rSubVer=%d";
        const StString THE_TO_UPDATE      = "<update>yes</update>";

        const StVersionInfo aCurrVer = StVersionInfo::getSDKVersionInfo();
        char anUrl[2048];
        sprintf(anUrl, THE_URL_TEMPLATE, aCurrVer.getYear(), aCurrVer.getMonth(), aCurrVer.getReleaseStatus(), aCurrVer.getSubVersion());

        const StString anAnswer = StRawFile::readTextFile(StString(anUrl));
        if(anAnswer.isEmpty()) {
            return false;
        }
        if(anAnswer.isContains(THE_TO_UPDATE)) {
            ST_DEBUG_LOG("Updated version is available!");
            return true;
        }
        return false;
    }

        private:

    static SV_THREAD_FUNCTION checkUpdatesThread(void* theThis) {
        StCheckUpdates* aThis = (StCheckUpdates* )theThis;
        if(StCheckUpdates::checkUpdates()) {
            aThis->setNeedUpdate(true);
        }
        aThis->setInitialized();
        return SV_THREAD_RETURN 0;
    }

        private:

    StMutex            myMutex;
    StHandle<StThread> myThread;
    bool               myIsInitialized;
    bool               myToUpdate;

};

#endif //__StCheckUpdates_h_
