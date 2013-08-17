/**
 * Copyright © 2009-2013 Kirill Gavrilov <kirill@sview.ru>
 *
 * StCore library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * StCore library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __StMonitor_h_
#define __StMonitor_h_

#include <StStrings/StString.h>
#include <StTemplates/StRect.h>
#include <StCore/StEDIDParser.h>

/**
 * Class represents monitor connected to videocard.
 */
class StMonitor {

        public:

    /**
     * Empty constructor.
     */
    ST_CPPEXPORT StMonitor();

    /**
     * Copy constructor.
     */
    ST_CPPEXPORT StMonitor(const StMonitor& theCopy);

    ST_CPPEXPORT bool isValid() const;

    ST_LOCAL int getId() const {
        return mySysId;
    }

    ST_LOCAL void setId(const int theMonId) {
        mySysId = theMonId;
    }

    /**
     * @return model PnP identifier assigned by Microsoft
     */
    ST_LOCAL const StString& getPnPId() const {
        return myPnpId;
    }

    /**
     * @param thePnpId model PnP identifier
     * Notice that this method do not affect EDID data.
     */
    ST_LOCAL void setPnPId(const StString& thePnpId) {
        myPnpId = thePnpId;
    }

    /**
     * @return model name
     */
    ST_LOCAL const StString& getName() const {
        return myName;
    }

    /**
     * @param theName model name
     */
    ST_LOCAL void setName(const StString& theName) {
        myName = theName;
    }

    /**
     * @return virtual space (rectangle), how this monitor is arranged with another displays
     */
    ST_LOCAL const StRectI_t& getVRect() const {
        return myRect;
    }

    /**
     * @return virtual space (rectangle)
     */
    ST_LOCAL StRectI_t& changeVRect() {
        return myRect;
    }

    /**
     * @param theRect virtual space (rectangle)
     */
    ST_LOCAL void setVRect(const StRectI_t& theRect) {
        myRect = theRect;
    }

    /**
     * @return pixels density scale factor
     */
    ST_LOCAL float getScale() const {
        return myScale;
    }

    /**
     * @param theScale pixels density scale factor
     */
    ST_LOCAL void setScale(const float theScale) {
        myScale = theScale;
    }

    /**
     * @return current vertical refresh rate
     */
    ST_LOCAL int getFreq() const {
        return myFreq;
    }

    /**
     * @param theFrequency current vertical refresh rate
     */
    ST_LOCAL void setFreq(const int theFrequency) {
        myFreq = theFrequency;
    }

    /**
     * @return maximal vertical refresh rate
     */
    ST_LOCAL int getFreqMax() const {
        return myFreqMax;
    }

    /**
     * @param theFrequencyMax maximal vertical refresh rate
     */
    ST_LOCAL void setFreqMax(const int theFrequencyMax) {
        myFreqMax = theFrequencyMax;
    }

    /**
     * @return GPU to which monitor is connected to
     */
    ST_LOCAL const StString& getGpuName() const {
        return myGpuName;
    }

    /**
     * @param theGpuName GPU to which monitor is connected to
     */
    ST_LOCAL void setGpuName(const StString& theGpuName) {
        myGpuName = theGpuName;
    }

    /**
     * @return associated EDID data
     */
    ST_LOCAL const StEDIDParser& getEdid() const {
         return myEdid;
    }

    /**
     * @return associated EDID data
     */
    ST_LOCAL StEDIDParser& changeEdid() {
         return myEdid;
    }

    /**
     * @return human-readable string with monitor description
     */
    ST_CPPEXPORT StString toString() const;

    ST_LOCAL bool operator==(const StMonitor& compare) const {
        if(&compare == this) {
            return true;
        }
        return compare.mySysId == mySysId
            && compare.myPnpId == myPnpId
            && compare.myRect  == myRect;
    }

    ST_LOCAL bool operator!=(const StMonitor& compare) const {
        return !this->operator==(compare);
    }

    ST_LOCAL bool operator>(const StMonitor& compare) const {
        return mySysId > compare.mySysId;
    }

    ST_LOCAL bool operator<(const StMonitor& compare) const {
        return mySysId < compare.mySysId;
    }

    ST_LOCAL bool operator>=(const StMonitor& compare) const {
        return mySysId >= compare.mySysId;
    }

    ST_LOCAL bool operator<=(const StMonitor& compare) const {
        return mySysId <= compare.mySysId;
    }

        private:

    StString     myPnpId;   //!< PnPId identifier assigned by Microsoft
    StString     myName;    //!< human-readable name for this display
    StString     myGpuName; //!< attached to this GPU
    StEDIDParser myEdid;    //!< EDID data block if available
    StRectI_t    myRect;    //!< virtual space (rectangle)
    int          mySysId;   //!< monitor id
    int          myFreq;    //!< frequency in Hertz
    int          myFreqMax; //!< maximum frequency in Hertz
    float        myScale;   //!< hight pixel density scale factor

};

#endif // __StMonitor_h_
