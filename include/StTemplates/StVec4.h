/**
 * Copyright © 2010-2013 Kirill Gavrilov <kirill@sview.ru>
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file license-boost.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __StVec4_h_
#define __StVec4_h_

#include <StTemplates/StVec3.h>

/**
 * Generic 4-components vector.
 * To be used as RGBA color vector or XYZW 3D-point with special W-component
 * for operations with projection / model view matrices.
 * Use this class for 3D-points carefully because declared W-component may
 * results in strange results if used without matrices.
 */
template<typename Element_t>
class StVec4 {

        private:

    Element_t v[4];

        public:

    static size_t length() {
        return 4;
    }

    /**
     * Empty constructor. Construct the zero vector.
     */
    StVec4() {
        stMemSet(this, 0, sizeof(StVec4));
    }

    /**
     * Initialize ALL components of vector within specified value.
     */
    explicit StVec4(Element_t xyzw) {
        v[0] = v[1] = v[2] = v[3] = xyzw;
    }

    /**
     * Per-component constructor.
     */
    explicit StVec4(Element_t x, Element_t y, Element_t z, Element_t w) {
        v[0] = x; v[1] = y; v[2] = z; v[3] = w;
    }

    /**
     * Constructor from 2-components vector.
     */
    explicit StVec4(const StVec2<Element_t>& vec2) {
        v[0] = vec2[0]; v[1] = vec2[1]; v[2] = v[3] = 0.0f;
    }

    /**
     * Constructor from 3-components vector.
     */
    explicit StVec4(const StVec3<Element_t>& vec3) {
        stMemCpy(this, &vec3, sizeof(StVec3<Element_t>)); v[3] = 0.0f;
    }

    /**
     * Constructor from 3-components vector + alpha value.
     */
    explicit StVec4(const StVec3<Element_t>& vec3, const Element_t alpha) {
        stMemCpy(this, &vec3, sizeof(StVec3<Element_t>)); v[3] = alpha;
    }

    /**
     * Copy constructor.
     */
    StVec4(const StVec4& vec4) {
        stMemCpy(this, &vec4, sizeof(StVec4));
    }

    /**
     * Assignment operator.
     */
    const StVec4& operator=(const StVec4& vec4) {
        stMemCpy(this, &vec4, sizeof(StVec4));
        return *this;
    }

    Element_t x() const { return v[0]; }
    Element_t r() const { return v[0]; } // Red color

    Element_t y() const { return v[1]; }
    Element_t g() const { return v[1]; } // Green color

    Element_t z() const { return v[2]; }
    Element_t b() const { return v[2]; } // Blue color

    Element_t w() const { return v[3]; }
    Element_t a() const { return v[3]; } // Alpha color

    ST_VEC_COMPONENTS_2D(x, y);
    ST_VEC_COMPONENTS_2D(x, z);
    ST_VEC_COMPONENTS_2D(x, w);
    ST_VEC_COMPONENTS_2D(y, z);
    ST_VEC_COMPONENTS_2D(y, w);
    ST_VEC_COMPONENTS_2D(z, w);

    ST_VEC_COMPONENTS_3D(x, y, z);
    ST_VEC_COMPONENTS_3D(x, y, w);
    ST_VEC_COMPONENTS_3D(x, z, w);
    ST_VEC_COMPONENTS_3D(y, z, w);

    ST_VEC_COMPONENTS_3D(r, g, b);

    Element_t& x() { return v[0]; }
    Element_t& r() { return v[0]; } // Red color

    Element_t& y() { return v[1]; }
    Element_t& g() { return v[1]; } // Green color

    Element_t& z() { return v[2]; }
    Element_t& b() { return v[2]; } // Blue color

    Element_t& w() { return v[3]; }
    Element_t& a() { return v[3]; } // Alpha color

    StVec2<Element_t>& xy()  { return *((StVec2<Element_t>* )&v[0]); }
    StVec2<Element_t>& yz()  { return *((StVec2<Element_t>* )&v[1]); }
    StVec2<Element_t>& zw()  { return *((StVec2<Element_t>* )&v[2]); }
    StVec3<Element_t>& xyz() { return *((StVec3<Element_t>* )&v[0]); }
    StVec3<Element_t>& yzw() { return *((StVec3<Element_t>* )&v[1]); }
    StVec3<Element_t>& rgb() { return *((StVec3<Element_t>* )&v[0]); }

    /**
     * Row access to the data (for OpenGL exchange).
     */
    const Element_t* getData() const { return v; }
    operator const Element_t*() const { return v; }
    operator Element_t*() { return v; }

    /**
     * Compute per-component summary.
     */
    StVec4& operator+=(const StVec4& add) {
        v[0] += add.v[0];
        v[1] += add.v[1];
        v[2] += add.v[2];
        v[3] += add.v[3];
        return *this;
    }

    /**
     * Compute per-component summary.
     */
    friend StVec4 operator+(const StVec4& left, const StVec4& right) {
        StVec4 summ = StVec4(left);
        return summ += right;
    }

    /**
     * Compute per-component subtraction.
     */
    StVec4& operator-=(const StVec4& dec) {
        v[0] -= dec.v[0];
        v[1] -= dec.v[1];
        v[2] -= dec.v[2];
        v[3] -= dec.v[3];
        return *this;
    }

    /**
     * Compute per-component subtraction.
     */
    friend StVec4 operator-(const StVec4& left, const StVec4& right) {
        StVec4 summ = StVec4(left);
        return summ -= right;
    }

    /**
     * Compute per-component multiplication.
     */
    StVec4& operator*=(const StVec4& right) {
        v[0] *= right.v[0];
        v[1] *= right.v[1];
        v[2] *= right.v[2];
        v[3] *= right.v[3];
        return *this;
    }

    /**
     * Compute per-component multiplication.
     */
    friend StVec4 operator*(const StVec4& left, const StVec4& right) {
        StVec4 result = StVec4(left);
        return result *= right;
    }

    /**
     * Compute per-component multiplication.
     */
    void multiply(Element_t factor) {
        v[0] *= factor;
        v[1] *= factor;
        v[2] *= factor;
        v[3] *= factor;
    }

    /**
     * Compute per-component multiplication.
     */
    StVec4& operator*=(Element_t factor) {
        multiply(factor);
        return *this;
    }

    /**
     * Compute per-component multiplication.
     */
    StVec4 operator*(Element_t factor) const {
        return multiplied(factor);
    }

    /**
     * Compute per-component multiplication.
     */
    StVec4 multiplied(Element_t factor) const {
        StVec4 copyVec4(*this);
        copyVec4 *= factor;
        return copyVec4;
    }

    /**
     * Compute per-component division by scale factor.
     */
    StVec4& operator/=(Element_t invFactor) {
        v[0] /= invFactor;
        v[1] /= invFactor;
        v[2] /= invFactor;
        v[3] /= invFactor;
        return *this;
    }

    /**
     * Compute per-component division by scale factor.
     */
    StVec4 operator/(Element_t invFactor) {
        StVec4 result(*this);
        return result /= invFactor;
    }

    StString toString() const {
        return StString('(') + x() + "; " + y() + "; " + z() + "; " + w() + ')';
    }

};

/**
 * Optimized version.
 */
template<>
inline StVec4<float>& StVec4<float>::operator/=(float invFactor) {
    multiply(1.0f / invFactor);
    return *this;
}

template<>
inline StVec4<double>& StVec4<double>::operator/=(double invFactor) {
    multiply(1.0 / invFactor);
    return *this;
}

#endif //__StVec4_h_
