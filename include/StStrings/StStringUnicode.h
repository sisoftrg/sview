/**
 * Copyright © 2011-2013 Kirill Gavrilov <kirill@sview.ru>
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file license-boost.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __StStringUnicode_h__
#define __StStringUnicode_h__

#include <StStrings/StUtfIterator.h>
#include <StTemplates/StHandle.h>

#include <iostream>

/**
 * This template of POD structure for constant UTF-* string.
 */
template<typename Type>
struct StConstStringUnicode {

    const Type* String; //!< string buffer
    size_t      Size;   //!< buffer size in bytes, excluding NULL-termination symbol
    size_t      Length; //!< length of the string in Unicode symbols (cached value, excluding NULL-termination symbol)

};

typedef StConstStringUnicode<stUtf8_t>    StCStringUtf8;
typedef StConstStringUnicode<stUtf16_t>   StCStringUtf16;
typedef StConstStringUnicode<stUtf32_t>   StCStringUtf32;
typedef StConstStringUnicode<stUtfWide_t> StCStringUtfWide;

/**
 * External constructor for StConstStringUnicode POD structure.
 */
template<typename Type>
inline const StConstStringUnicode<Type> stStringExtConstr(const Type*  theString,
                                                          const size_t theSize,
                                                          const size_t theLength) {
    const StConstStringUnicode<Type> aStr = {theString, theSize, theLength};
    return aStr;
}

/**
 * Initialize constant string without Unicode symbols (only ASCII).
 */
#define stCString(theString) stStringExtConstr(theString, sizeof(theString) - sizeof(*theString), sizeof(theString) / sizeof(*theString) - 1)
//#define stCString(theString) {theString, sizeof(theString) - sizeof(*theString), sizeof(theString) / sizeof(*theString) - 1}

const StCStringUtf8 TheTest = stCString("my test");

static_assert(std::is_pod<StCStringUtf8>::value,
              "StCStringUtf8 is not POD structure!");

/**
 * This template class represents constant UTF-* string.
 * String stored in memory continuously, always NULL-terminated
 * and can be used as standard C-string using toCString() method.
 *
 * Notice that changing the string is not allowed
 * and any modifications should produce new string.
 * Class StText is more efficient for frequently modified string.
 */
template<typename Type>
class StStringUnicode {

        public:

    inline StUtfIterator<Type> iterator() const {
        return StUtfIterator<Type>(myString);
    }

    /**
     * Returns the size of the buffer, excluding NULL-termination symbol
     */
    inline size_t getSize() const {
        return mySize;
    }

    /**
     * Returns the length of the string in Unicode symbols.
     */
    inline size_t getLength() const {
        return myLength;
    }

    /**
     * Retrieve Unicode symbol at specified position.
     * Warning! This is a slow access. Iterator should be used for consecutive parsing.
     * @param theCharIndex the index of the symbol, should be lesser then getLength()
     * @return the Unicode symbol value
     */
    stUtf32_t getChar(const size_t theCharIndex) const;

    /**
     * Retrieve string buffer at specified position.
     * Warning! This is a slow access. Iterator should be used for consecutive parsing.
     * @param theCharIndex the index of the symbol, should be lesser then getLength()
     * @return the pointer to the symbol (position in the string)
     */
    const Type* getCharBuffer(const size_t theCharIndex) const;

    /**
     * Retrieve Unicode symbol at specified position.
     * Warning! This is a slow access. Iterator should be used for consecutive parsing.
     */
    inline stUtf32_t operator[](const size_t theCharIndex) const {
        return getChar(theCharIndex);
    }

    /**
     * Initialize empty string.
     */
    StStringUnicode();

    /**
     * Copy constructor.
     * @param theCopy string to copy
     */
    StStringUnicode(const StStringUnicode& theCopy);

    /**
     * Copy constructor.
     */
    StStringUnicode(const StConstStringUnicode<Type>& theCopy);

    /**
     * Copy constructor from NULL-terminated UTF-8 string.
     * @param theCopy   NULL-terminated UTF-8 string to copy
     * @param theLength the length limit in Unicode symbols (NOT bytes!)
     */
    StStringUnicode(const char*  theCopyUtf8,
                    const size_t theLength = size_t(-1));

    /**
     * Copy constructor from NULL-terminated UTF-16 string.
     * @param theCopy   NULL-terminated UTF-16 string to copy
     * @param theLength the length limit in Unicode symbols (NOT bytes!)
     */
    StStringUnicode(const stUtf16_t* theCopyUtf16,
                    const size_t     theLength = size_t(-1));

    /**
     * Copy constructor from NULL-terminated UTF-32 string.
     * @param theCopy   NULL-terminated UTF-32 string to copy
     * @param theLength the length limit in Unicode symbols (NOT bytes!)
     */
    StStringUnicode(const stUtf32_t* theCopyUtf32,
                    const size_t     theLength = size_t(-1));

    /**
     * Copy constructor from NULL-terminated wide UTF string.
     * @param theCopy   NULL-terminated wide UTF string to copy
     * @param theLength the length limit in Unicode symbols (NOT bytes!)
     */
    StStringUnicode(const stUtfWide_t* theCopyUtfWide,
                    const size_t       theLength = size_t(-1));

    /**
     * Copy from NULL-terminated Unicode string.
     * @param theStringUtf NULL-terminated Unicode string
     * @param theLength    the length limit in Unicode symbols
     */
    template <typename TypeFrom>
    void fromUnicode(const TypeFrom* theStringUtf,
                     const size_t    theLength = size_t(-1));

    /**
     * Copy from NULL-terminated multibyte string in system locale.
     * You should avoid this function unless extreme necessity.
     * @param theStringUtf NULL-terminated multibyte string
     * @param theLength    the length limit in Unicode symbols
     */
    void fromLocale(const char*  theString,
                    const size_t theLength = size_t(-1));

    /**
     * Convert URL string (with %38%20 codes) to normal Unicode string.
     * @param theUrl the URL
     */
    inline void fromUrl(const StStringUnicode& theUrl) {
        size_t aSizeUtf8, aLengthUtf8;
        StStringUnicode<stUtf8_t>::strGetAdvance(theUrl.myString, theUrl.myLength, aSizeUtf8, aLengthUtf8);
        stUtf8_t* aBuffer = StStringUnicode<stUtf8_t>::stStrAlloc(aSizeUtf8);
        urlDecode(theUrl.myString, aBuffer);
        fromUnicode(aBuffer, size_t(-1));
        StStringUnicode<stUtf8_t>::stStrFree(aBuffer);
    }

    /**
     * Constructor from one symbol.
     * @param theChar character to copy
     */
    StStringUnicode(const char theChar);

    /**
     * Create string from integer.
     * @param theInt32 integer value
     */
    StStringUnicode(const int32_t  theInt32);

    /**
     * Create string from integer.
     * @param theUInt32 integer value
     */
    StStringUnicode(const uint32_t theUInt32);

    /**
     * Create string from integer.
     * @param theInt64 integer value
     */
    StStringUnicode(const int64_t  theInt64);

    /**
     * Create string from integer.
     * @param theUInt64 integer value
     */
    StStringUnicode(const uint64_t theUInt64);

    /**
     * Create string from double.
     * @param theFloat float number
     */
    StStringUnicode(const double   theFloat);

#ifdef ST_HAS_INT64_EXT
    StStringUnicode(const stInt64ext_t  theInt64);
    StStringUnicode(const stUInt64ext_t theUInt64);
#endif

    /**
     * Destructor.
     */
    ~StStringUnicode();

    /**
     * Compares this string with another one.
     */
    bool isEquals(const StStringUnicode& theCompare) const;

    /**
     * Compares this String to another String, ignoring case considerations.
     */
    bool isEqualsIgnoreCase(const StStringUnicode& theCompare) const;

    /**
     * Returns the substring.
     * @param theStart start index (inclusive) of subString
     * @param theEnd   end index (exclusive) of subString
     * @return the substring
     */
    StStringUnicode subString(const size_t theStart,
                              const size_t theEnd) const;

    bool isStartsWith(const StStringUnicode& theStartString) const;
    bool isStartsWithIgnoreCase(const StStringUnicode& theStartString) const;

    bool isEndsWith(const StStringUnicode& theEndString) const;
    bool isEndsWithIgnoreCase(const StStringUnicode& theEndString) const;

    /**
     * Returns NULL-terminated Unicode string.
     * Should not be modifed or deleted!
     * @return pointer to the string
     */
    inline const Type* toCString() const {
        return myString;
    }

    /**
     * Cast this string to constant string structure
     * for interconnection with methods taking .
     */
    const StConstStringUnicode<Type>& toConstString() const {
        return *static_cast< StConstStringUnicode<Type>* >(this);
    }

    /**
     * Returns copy in UTF-8 format.
     */
    const StStringUnicode<stUtf8_t> toUtf8() const;

    /**
     * Returns copy in UTF-16 format.
     */
    const StStringUnicode<stUtf16_t> toUtf16() const;

    /**
     * Returns copy in UTF-32 format.
     */
    const StStringUnicode<stUtf32_t> toUtf32() const;

    /**
     * Returns copy in wide format (UTF-16 on Windows and UTF-32 on Linux).
     */
    const StStringUnicode<stUtfWide_t> toUtfWide() const;

    /**
     * Converts the string into multibyte string.
     * You should avoid this function unless extreme necessity.
     * @param theBuffer    output buffer
     * @param theSizeBytes buffer size in bytes
     * @return true on success
     */
    bool toLocale(char*     theBuffer,
                  const int theSizeBytes) const;

    /**
     * Auxiliary method to dump information about the string intended for debugging purposes.
     */
    const StStringUnicode<stUtf8_t> toDump() const;

    /**
     * Returns true if string contains requested Unicode symbol.
     */
    bool isContains(const stUtf32_t theSubChar) const;

    /**
     * Returns true if string contains requested substring.
     */
    bool isContains(const StStringUnicode& theSubString) const;

    /**
     * @return true if string is empty.
     */
    inline bool isEmpty() const {
        return myString[0] == Type(0);
    }

    /**
     * Zero string.
     */
    void clear();

    /**
     * Returns this string without quotes (double quotes "my string" or single quotes 'my string').
     */
    StStringUnicode unquoted() const;

    /**
     * Split the string using delimiter char.
     * @param theDelimeter (const stUtf32_t ) - delimiter char;
     * @param theLimitNb   (const size_t )  - maximum split sections (when reached - trailing part will be in one string without parsing);
     * @return the list of split strings.
     */
    StHandle <StArrayList<StStringUnicode> > split(const stUtf32_t theDelimeter,
                                                   const size_t    theLimitNb = size_t(-1)) const;

    StStringUnicode replace(const StStringUnicode& theSubString,
                            const StStringUnicode& theReplacer) const;

    /**
     * Replace all substring occurrences within given replacement.
     * This is a low-level method and requires that substring and replacement has same size
     * in bytes (not in Unicode symbols!).
     */
    void replaceFast(const StStringUnicode<Type>& theSubString,
                     const StStringUnicode<Type>& theReplacer);

        public: //!< assign operators

    /**
     * Copy from another string.
     */
    const StStringUnicode& operator=(const StStringUnicode& theOther);

    /**
     * Copy from UTF-8 NULL-terminated string.
     */
    const StStringUnicode& operator=(const char* theStringUtf8);

    /**
     * Copy from wchar_t UTF NULL-terminated string.
     */
    const StStringUnicode& operator=(const stUtfWide_t* theStringUtfWide);

    /**
     * Join strings.
     */
    StStringUnicode& operator+=(const StStringUnicode& theAppend);

    /**
     * Join two strings.
     */
    friend StStringUnicode operator+(const StStringUnicode& theLeft,
                                     const StStringUnicode& theRight) {
        StStringUnicode aSumm;
        stStrFree(aSumm.myString);
        aSumm.mySize   = theLeft.mySize   + theRight.mySize;
        aSumm.myLength = theLeft.myLength + theRight.myLength;
        aSumm.myString = stStrAlloc(aSumm.mySize);

        // copy bytes
        stStrCopy((stUByte_t* )aSumm.myString,                  (const stUByte_t* )theLeft.myString,  theLeft.mySize);
        stStrCopy((stUByte_t* )aSumm.myString + theLeft.mySize, (const stUByte_t* )theRight.myString, theRight.mySize);
        return aSumm;
    }

    /**
     * Set all template concretizations as friends to access private methods.
     */
    template <class OtherType> friend class StStringUnicode;

        public: //!< compare operators

    inline bool operator==(const StStringUnicode& theCompare) const {
        return isEquals(theCompare);
    }
    bool operator!=(const StStringUnicode& theCompare) const;
    bool operator> (const StStringUnicode& theCompare) const;
    bool operator< (const StStringUnicode& theCompare) const;
    bool operator>=(const StStringUnicode& theCompare) const;
    bool operator<=(const StStringUnicode& theCompare) const;

        private: //!< low-level methods

    /**
     * Compute advance for specified string.
     * @param theStringUtf  [in] pointer to the NULL-terminated Unicode string
     * @param theLengthMax  [in] length limit (to cut the string), set to -1 to compute up to NULL-termination symbol
     * @param theSizeBytes [out] advance in bytes
     * @param theLength    [out] string length
     */
    template<typename TypeFrom>
    static void strGetAdvance(const TypeFrom* theStringUtf,
                              const size_t    theLengthMax,
                              size_t&         theSizeBytes,
                              size_t&         theLength);

    /**
     * Allocate NULL-terminated string buffer.
     */
    static inline Type* stStrAlloc(const size_t theSizeBytes) {
        Type* aPtr = stMemAlloc<Type*>(theSizeBytes + sizeof(Type));
        if(aPtr != NULL) {
            // always NULL-terminate the string
            aPtr[theSizeBytes / sizeof(Type)] = Type(0);
        }
        return aPtr;
    }

    /**
     * Release string buffer and nullify the pointer.
     */
    static inline void stStrFree(Type*& thePtr) {
        stMemFree(thePtr);
        thePtr = NULL;
    }

    /**
     * Optimized copy.
     * Provides bytes interface to avoid incorrect pointer arithmetics.
     */
    static inline void stStrCopy(stUByte_t*       theStrDst,
                                 const stUByte_t* theStrSrc,
                                 const size_t     theSizeBytes) {
        stMemCpy(theStrDst, theStrSrc, theSizeBytes);
    }

    /**
     * Compare two Unicode strings per-byte.
     */
    static inline bool stStrAreEqual(const Type*  theString1,
                                     const size_t theSizeBytes1,
                                     const Type*  theString2,
                                     const size_t theSizeBytes2) {
        return (theSizeBytes1 == theSizeBytes2)
            && stAreEqual(theString1, theString2, theSizeBytes1);
    }

    /**
     * Simple parser for '%XX' encoded character.
     */
    static int hexPairValue(const Type* theCode);

    /**
     * Function process conversion URL to UTF-8 string.
     * @param theSrcUrl [IN]  source URL
     * @param theOut    [OUT] buffer for decoded string
     * @return converted string length
     */
    static size_t urlDecode(const Type* theSrcUrl,
                            stUtf8_t*   theOut);

        private: //!< private fields, should start from StConstStringUnicode fields!

    Type*  myString; //!< string buffer
    size_t mySize;   //!< buffer size in bytes, excluding NULL-termination symbol
    size_t myLength; //!< length of the string in Unicode symbols (cached value, excluding NULL-termination symbol)

};

typedef StStringUnicode<stUtf8_t>    StStringUtf8;
typedef StStringUnicode<stUtf16_t>   StStringUtf16;
typedef StStringUnicode<stUtf32_t>   StStringUtf32;
typedef StStringUnicode<stUtfWide_t> StStringUtfWide;

// make sure StStringUnicode could be casted to StConstStringUnicode
//static_assert(std::is_standard_layout<StStringUtf8>::value,
//              "StStringUtf8 is not standard layout class!");

// template implementation (inline methods)
#include <StStrings/StStringUnicode.inl>

#endif //__StStringUnicode_h__
