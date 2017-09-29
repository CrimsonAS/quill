/*
    Copyright (c) 2017, Gunnar Sletta <gunnar@crimson.no>
    Copyright (c) 2017, reMarkable AS <technology@remarkable.no>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once



template <typename Rasterizer, typename VaryingGenerator>
Stroker<Rasterizer, VaryingGenerator>::Segment::Segment(SegmentType type, float x, float y, float width, JoinStyle joinStyle, CapStyle capStyle)
    : x(x)
    , y(y)
    , width(width)
    , type(type)
    , joinStyle(joinStyle)
    , capStyle(capStyle)
{
}



template <typename Rasterizer, typename VaryingGenerator>
Stroker<Rasterizer, VaryingGenerator>::Stroker()
{
    reset();
}



template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::store(float x, float y, SegmentType type)
{
    m_lastSegment = Segment(type, x, y, width, joinStyle, capStyle);
}



template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::moveTo(float x, float y)
{
    // std::cout << "moveTo(" << x << ", " << y << ")" << std::endl;

    store(x, y, MoveToSegment);
    m_firstSegment = m_lastSegment;
}



template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::lineTo(float x, float y)
{
    assert(m_lastSegment.type != InvalidType);

    if (m_lastSegment.x == x && m_lastSegment.y == y)
        return;

    Line line(m_lastSegment.x, m_lastSegment.y, x, y);
    float len = line.length();
    float ndx = (line.y0 - line.y1) / len;
    float ndy = (line.x1 - line.x0) / len;


    float w2 = width / 2;
    float cw2 = m_lastSegment.width / 2;

    // std::cout << "lineTo(" << x << ", " << y << ")" << " length=" << len
    //           << ", width=" << w2 << "(" << cw2 << ")"
    //           << ", normal=" << ndx << "," << ndy << ", totalLength=" << length
    //           << std::endl;

    Line right(line.x0 + ndx * cw2,
               line.y0 + ndy * cw2,
               line.x1 + ndx * w2,
               line.y1 + ndy * w2);
    Line left(line.x0 - ndx * cw2,
              line.y0 - ndy * cw2,
              line.x1 - ndx * w2,
              line.y1 - ndy * w2);

    if (m_lastSegment.type == LineToSegment) {
        join(m_lastLeft, m_lastRight, left, right, length, cw2);
    }

    stroke(left, right, length + len, cw2, w2);

    if (m_lastSegment.type == MoveToSegment) {
        m_firstLeft = left;
        m_firstRight = right;
    }

    store(x, y, LineToSegment);
    m_lastLeft = left;
    m_lastRight = right;
    length += len;
}



template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::join(Line lastLeft, Line lastRight, Line left, Line right, float len, float width)
{
    if (joinStyle == BevelJoin) {
        stroke(Line(lastLeft.x1, lastLeft.y1, left.x0, left.y0),
               Line(lastRight.x1, lastRight.y1, right.x0, right.y0),
               len, width, width);

    } else if (joinStyle == RoundJoin) {
        float angleLast = std::atan2(lastLeft.y1 - m_lastSegment.y, lastLeft.x1 - m_lastSegment.x);
        float angleNext = std::atan2(left.y0 - m_lastSegment.y, left.x0 - m_lastSegment.x);
        float angleDelta = angleNext - angleLast;
        if (angleDelta < -M_PI) {
            angleDelta += M_PI * 2;
        } else if (angleDelta > M_PI) {
            angleDelta -= M_PI * 2;
        }
        assert(angleDelta <= M_PI && angleDelta >= -M_PI);

        // Shortcut with a bevel join for 'tiny' angles.
        // ### premature-optimization: Need to verify that this makes sense
        // per-wise and that it at the same time doesn't cause too much of a visual
        // impact..
        if (std::abs(angleDelta) < M_PI / 10) {
            stroke(Line(lastLeft.x1, lastLeft.y1, left.x0, left.y0),
                   Line(lastRight.x1, lastRight.y1, right.x0, right.y0),
                   len, width, width);
            return;
        }

        float radius = width;
        float arcLength = radius * angleDelta; // frin (2 * M_PI * radius) / (angleDelta / (2 * M_PI))

        // Don't really know how long steps we have to take, but lets assume a
        // bit more than 3 gives us good results.. Hey, lets just use PI.
        int steps = std::ceil(std::abs(arcLength / M_PI));
        if (steps > 30)
            steps = 30;
        assert(steps > 0);

        float llx = lastLeft.x1;
        float lly = lastLeft.y1;
        float lrx = lastRight.x1;
        float lry = lastRight.y1;
        float dt = angleDelta / steps;
        float t = angleLast + dt;
        for (int i=0; i<steps; ++i) {
            float ct = radius * cos(t);
            float st = radius * sin(t);
            float lx = m_lastSegment.x + ct;
            float ly = m_lastSegment.y + st;
            float rx = m_lastSegment.x - ct;
            float ry = m_lastSegment.y - st;

            stroke(Line(llx, lly, lx, ly),
                   Line(lrx, lry, rx, ry),
                   len, width, width);

            t += dt;
            llx = lx;
            lly = ly;
            lrx = rx;
            lry = ry;
        }

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

        join(m_lastLeft, m_lastRight, m_firstLeft, m_firstRight, length, m_firstSegment.width);
    }
}



template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::finish()
{
}



template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::reset()
{
    width = 1;
    joinStyle = BevelJoin;
    capStyle = FlatCap;
    m_lastSegment = Segment();
    m_firstSegment = Segment();
    triangleCount = 0;
    length = 0;
}

template <typename Rasterizer, typename VaryingGenerator>
void Stroker<Rasterizer, VaryingGenerator>::stroke(Line left, Line right, float newLength, float startWidth, float endWidth)
{
    rasterizer(Triangle(Vertex(left.x0, left.y0),
                        Vertex(left.x1, left.y1),
                        Vertex(right.x0, right.y0)),
               varying.l0(length, startWidth),
               varying.l1(newLength, endWidth),
               varying.r0(length, startWidth));

    rasterizer(Triangle(Vertex(right.x0, right.y0),
                        Vertex(left.x1, left.y1),
                        Vertex(right.x1, right.y1)),
               varying.r0(length, startWidth),
               varying.l1(newLength, endWidth),
               varying.r1(newLength, endWidth));

    triangleCount += 2;
}
