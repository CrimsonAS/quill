/*
    Copyright (c) 2023, reMarkable AS <technology@remarkable.no>
    Copyright (c) 2023, Gunnar Sletta <gunnar@crimson.no>
    All rights reserved.


    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
    THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

template <typename Rasterizer, typename VaryingGenerator>
Stroker<Rasterizer, VaryingGenerator>::Segment::Segment(SegmentType type_,
                                                        float x_, float y_, float width_, float length_,
                                                        JoinStyle joinStyle_, CapStyle capStyle_,
                                                        Varyings left, Varyings right)
    : x(x_)
    , y(y_)
    , width(width_)
    , length(length_)
    , leftVarying(left)
    , rightVarying(right)
    , type(type_)
    , joinStyle(joinStyle_)
    , capStyle(capStyle_)
{
}



template <typename Rasterizer, typename VaryingGenerator>
Stroker<Rasterizer, VaryingGenerator>::~Stroker()
{
    finish();
}



template <typename Rasterizer, typename VaryingGenerator>
Stroker<Rasterizer, VaryingGenerator>::Stroker()
{
    reset();
}



template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::store(float x, float y, SegmentType type, Varyings left, Varyings right)
{
    m_lastSegment = Segment(type,
                            x, y, width, length,
                            joinStyle,
                            capStyle,
                            left, right);
}



template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::moveTo(float x, float y)
{
    // std::cout << "moveTo(" << x << ", " << y << ")" << std::endl;
    if (m_lastSegment.type == LineToSegment) {
        flushStartCap();
        flushEndCap();
    }

    store(x, y, MoveToSegment, varying.left(length, width / 2), varying.right(length, width / 2));
    m_firstSegment = m_lastSegment;
}



template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::lineTo(float x, float y)
{
    if (m_lastSegment.type == InvalidType) {
        moveTo(x, y);
        return;
    }

    if (m_lastSegment.x == x && m_lastSegment.y == y)
        return;

    if (slant.has_value()) {
        lineToSlanted(x, y);
        return;
    }

    Line line(m_lastSegment.x, m_lastSegment.y, x, y);
    float len = line.length();
    float ndx = (line.y0 - line.y1) / len;
    float ndy = (line.x1 - line.x0) / len;

    float chw = width / 2;
    float lhw = m_lastSegment.width / 2;

    // std::cout << "lineTo(" << x << ", " << y << ")" << " length=" << len
    //           << ", width=" << chw << "(" << lhw << ")"
    //           << ", normal=" << ndx << "," << ndy << ", totalLength=" << length
    //           << std::endl;

    Line right(line.x0 + ndx * lhw,
               line.y0 + ndy * lhw,
               line.x1 + ndx * chw,
               line.y1 + ndy * chw);
    Line left(line.x0 - ndx * lhw,
              line.y0 - ndy * lhw,
              line.x1 - ndx * chw,
              line.y1 - ndy * chw);

    if (m_lastSegment.type == LineToSegment) {
        join(m_lastLeft, m_lastRight, left, right, m_lastSegment.leftVarying, m_lastSegment.rightVarying);
    }

    length += len;
    Varyings leftVarying = varying.left(length, chw);
    Varyings rightVarying = varying.right(length, chw);

    stroke(left, right,
           m_lastSegment.leftVarying, m_lastSegment.rightVarying,
           leftVarying, rightVarying);

    if (m_lastSegment.type == MoveToSegment) {
        m_firstLeft = left;
        m_firstRight = right;
    }

    m_lastLeft = left;
    m_lastRight = right;
    store(x, y, LineToSegment, leftVarying, rightVarying);
}



template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::lineToSlanted(float x, float y)
{
    assert(m_lastSegment.type != InvalidType);
    assert(m_lastSegment.x != x || m_lastSegment.y != y);
    assert(slant.has_value());

    const Line line(m_lastSegment.x, m_lastSegment.y, x, y);
    const float len = line.length();

    // line normal
    const float ndx = (line.y0 - line.y1) / len;
    const float ndy = (line.x1 - line.x0) / len;

    // current and last half-width
    const float chw = width / 2;
    const float lhw = m_lastSegment.width / 2;

    // Figure out how wide the line ends up being. The width stands normal to
    // the current line direction, so we can find that by taking finding the
    // dot product between the slant vector and the line normal.
    const float slantDotNormal = ndx * slant->nx + ndy * slant->ny;
    const float effectiveWidth = std::abs(slantDotNormal) * width;

    Line right(line.x0 + slant->nx * lhw,
               line.y0 + slant->ny * lhw,
               line.x1 + slant->nx * chw,
               line.y1 + slant->ny * chw);
    Line left(line.x0 - slant->nx * lhw,
              line.y0 - slant->ny * lhw,
              line.x1 - slant->nx * chw,
              line.y1 - slant->ny * chw);

    if (effectiveWidth < slant->minimumWidth) {
        const float mw2 = slant->minimumWidth / 2.0f;
        right = Line(line.x0 + ndx * mw2,
                     line.y0 + ndy * mw2,
                     line.x1 + ndx * mw2,
                     line.y1 + ndy * mw2);
        left = Line(line.x0 - ndx * mw2,
                    line.y0 - ndy * mw2,
                    line.x1 - ndx * mw2,
                    line.y1 - ndy * mw2);
    }

    if (m_lastSegment.type == LineToSegment) {
        join(m_lastLeft, m_lastRight, left, right, m_lastSegment.leftVarying, m_lastSegment.rightVarying);
    }

    length += len;
    Varyings leftVarying = varying.left(length, chw);
    Varyings rightVarying = varying.right(length, chw);

    stroke(left, right,
           m_lastSegment.leftVarying, m_lastSegment.rightVarying,
           leftVarying, rightVarying);

    if (m_lastSegment.type == MoveToSegment) {
        m_firstLeft = left;
        m_firstRight = right;
    }

    m_lastLeft = left;
    m_lastRight = right;
    store(x, y, LineToSegment, leftVarying, rightVarying);
}


template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::join(Line lastLeft, Line lastRight, Line left, Line right, Varyings leftVarying, Varyings rightVarying)
{
    if (joinStyle == BevelJoin) {
        stroke(Line(lastLeft.x1, lastLeft.y1, left.x0, left.y0),
               Line(lastRight.x1, lastRight.y1, right.x0, right.y0),
               leftVarying, rightVarying,
               leftVarying, rightVarying,
               true);

    } else if (joinStyle == RoundJoin) {
        float angleLast = std::atan2(lastLeft.y1 - m_lastSegment.y, lastLeft.x1 - m_lastSegment.x);
        float angleNext = std::atan2(left.y0 - m_lastSegment.y, left.x0 - m_lastSegment.x);
        float angleDelta = angleNext - angleLast;
        if (angleDelta < -M_PI) {
            angleDelta += float(M_PI * 2);
        } else if (angleDelta > M_PI) {
            angleDelta -= float(M_PI * 2);
        }

        // Shortcut with a bevel join for 'tiny' angles.
        // ### premature-optimization: Need to verify that this makes sense
        // per-wise and that it at the same time doesn't cause too much of a visual
        // impact..
        if (std::abs(angleDelta) < M_PI / 10) {
            stroke(Line(left.x0, left.y0, lastLeft.x1, lastLeft.y1),
                   Line(right.x0, right.y0, lastRight.x1, lastRight.y1),
                   leftVarying, rightVarying,
                   leftVarying, rightVarying,
                   true);
            return;
        }

        // Decide the radius based on the previous segment. We're joining from
        // it, and if the width is changing, 'width' will already have been set
        // to the new value.
        float radius = m_lastSegment.width / 2;
        float arcLength = radius * angleDelta; // from (angleDelta / (2 * PI)) * (2 * PI * r)

        // Don't really know how long steps we have to take, but lets assume a
        // bit more than 3 gives us good results.. Hey, lets just use PI.
        int steps = std::min(30, int(std::ceil(std::abs(arcLength / M_PI))));
        assert(steps > 0);

        float llx = lastLeft.x1;
        float lly = lastLeft.y1;
        float lrx = lastRight.x1;
        float lry = lastRight.y1;
        float dt = angleDelta / float(steps);
        float t = angleLast + dt;
        for (int i=0; i<steps; ++i) {
            float ct = radius * std::cos(t);
            float st = radius * std::sin(t);
            float lx = m_lastSegment.x + ct;
            float ly = m_lastSegment.y + st;
            float rx = m_lastSegment.x - ct;
            float ry = m_lastSegment.y - st;

            stroke(Line(llx, lly, lx, ly),
                   Line(lrx, lry, rx, ry),
                   leftVarying, rightVarying,
                   leftVarying, rightVarying,
                   true);

            t += dt;
            llx = lx;
            lly = ly;
            lrx = rx;
            lry = ry;
        }

    }
}

template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::cap(Line left, Line right, Segment segment, bool endCap)
{
    // std::cout << " - cap(" << segment.x << "," << segment.y << ", " << (endCap ? "end-cap" : "start-cap")
    //           << ", left=" << left << ", right=" << right << ", width=" << segment.width << std::endl;

    if (segment.width <= 0.0f)
        return;

    if (capStyle == FlatCap) {
        return;
    }

    if (!endCap) {
        Line tmp(left.x1, left.y1, left.x0, left.y0);
        left = Line(right.x1, right.y1, right.x0, right.y0);
        right = tmp;
    }

    if (capStyle == RoundCap) {
        float angle = std::atan2(left.y1 - segment.y, left.x1 - segment.x);

        float radius = segment.width / 2;
        float arcLength = float(radius * M_PI); // half a circle..
        int steps = std::min(30, int(std::ceil(std::abs(arcLength / M_PI))));
        assert(steps > 0);

        float lx = left.x1;
        float ly = left.y1;
        float rx = right.x1;
        float ry = right.y1;

        float dt = float(M_PI / steps);
        float rt = angle + float(M_PI) - dt;
        float lt = angle + dt;

        steps = std::max(1, steps / 2);

        for (int i=0; i<steps; ++i) {
            float nlx = radius * std::cos(lt) + segment.x;
            float nly = radius * std::sin(lt) + segment.y;
            float nrx = radius * std::cos(rt) + segment.x;
            float nry = radius * std::sin(rt) + segment.y;

            stroke(Line(lx, ly, nlx, nly),
                   Line(rx, ry, nrx, nry),
                   segment.leftVarying, segment.rightVarying,
                   segment.leftVarying, segment.rightVarying);

            lx = nlx;
            ly = nly;
            lt += dt;
            rx = nrx;
            ry = nry;
            rt -= dt;
        }
    } else if (capStyle == SquareCap) {
        const float w2 = width / 2.0f;

        const float ldx = left.x1 - left.x0;
        const float ldy = left.y1 - left.y0;
        const float llen = std::sqrt(ldx * ldx + ldy * ldy);
        const float nldx = ldx / llen;
        const float nldy = ldy / llen;
        const float lx = left.x1 + nldx * w2;
        const float ly = left.y1 + nldy * w2;

        const float rdx = right.x1 - right.x0;
        const float rdy = right.y1 - right.y0;
        const float rlen = std::sqrt(rdx * rdx + rdy * rdy);
        const float nrdx = rdx / rlen;
        const float nrdy = rdy / rlen;
        const float rx = right.x1 + nrdx * w2;
        const float ry = right.y1 + nrdy * w2;

        stroke(Line(left.x1, left.y1, lx, ly),
               Line(right.x1, right.y1, rx, ry),
               segment.leftVarying, segment.rightVarying,
               segment.leftVarying, segment.rightVarying);
    }
}



template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::flushStartCap()
{
    if (!m_startCapRendered && length > 0.0f) {
        cap(m_firstLeft, m_firstRight, m_firstSegment, false);
        m_startCapRendered = true;
    }
}



template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::flushEndCap()
{
    if (!m_endCapRendered && length > 0.0f) {
        cap(m_lastLeft, m_lastRight, m_lastSegment, true);
        m_endCapRendered = true;
    }
}



template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::close()
{
    if (m_lastSegment.type == LineToSegment) {
        assert(m_firstSegment.type == MoveToSegment);
        lineTo(m_firstSegment.x, m_firstSegment.y);

        // std::cout << " - lineTo(" << m_firstSegment.x << "," << m_firstSegment.y << ")" << std::endl;
        // std::cout << " - lastLeft:   " << m_lastLeft << std::endl
        //           << " - lastRight:  " << m_lastRight << std::endl
        //           << " - firstLeft:  " << m_firstLeft << std::endl
        //           << " - firstRight: " << m_firstRight << std::endl;

        join(m_lastLeft, m_lastRight, m_firstLeft, m_firstRight,
             m_lastSegment.leftVarying, m_lastSegment.rightVarying);
    }
}



template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::finish()
{
    if (m_lastSegment.type == LineToSegment) {
        if (m_lastSegment.x == m_firstSegment.x && m_lastSegment.y == m_firstSegment.y) {
            close();
        } else {
            flushStartCap();
            flushEndCap();
        }
    }

    reset();
}



template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::reset()
{
    m_firstLeft = Line();
    m_firstRight = Line();
    m_lastSegment = Segment();
    m_firstSegment = Segment();
    triangleCount = 0;
    length = 0;
    m_startCapRendered = false;
    m_endCapRendered = false;
}

template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::stroke(Line left, Line right,
                                                   Varyings vl0, Varyings vr0,
                                                   Varyings vl1, Varyings vr1,
                                                   bool checkDirection)
{
    if (checkDirection) {
        // Catch the case where we've decided to stroke lines that pass in
        // opposite directions. This typically happens whenever we join two line
        // segments as the interior of the curve is wrapped back on itself then.

        // We do this by projecting right onto left and checking the
        // direction. Since we only care about the sign and not the length, we
        // can skip the sqrt() and simplify the function a bit

        if (((right.x1-right.x0) * (left.x1-left.x0)) + ((right.y1-right.y0) * (left.y1-left.y0)) < 0) {
            // If opposite, flip the right line and its varyings..
            right = Line(right.x1, right.y1, right.x0, right.y0);
            std::swap(vr0, vr1);
        }
    }

    raster(Triangle(Vertex(left.x0, left.y0),
                    Vertex(left.x1, left.y1),
                    Vertex(right.x0, right.y0)),
                    vl0, vl1, vr0);

    raster(Triangle(Vertex(right.x0, right.y0),
                    Vertex(left.x1, left.y1),
                    Vertex(right.x1, right.y1)),
                    vr0, vl1, vr1);

    triangleCount += 2;
}
