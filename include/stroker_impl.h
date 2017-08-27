/*
    Copyright (c) 2017, Gunnar Sletta <gunnar@crimson.no>
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


template <typename Rasterizer>
Stroker<Rasterizer>::Stroker()
{
    reset();
}



template <typename Rasterizer>
void Stroker<Rasterizer>::store(float x, float y, SegmentType type)
{
    m_lastSegment = {
        .x = x,
        .y = y,
        .width = width,
        .type = type,
        .joinStyle = joinStyle,
        .capStyle = capStyle
    };
}



template <typename Rasterizer>
void Stroker<Rasterizer>::moveTo(float x, float y)
{
    // std::cout << "moveTo(" << x << ", " << y << ")" << std::endl;

    store(x, y, MoveToSegment);
    m_firstSegment = m_lastSegment;
}



template <typename Rasterizer>
void Stroker<Rasterizer>::lineTo(float x, float y)
{
    assert(m_lastSegment.type != InvalidType);

    if (m_lastSegment.x == x && m_lastSegment.y == y)
        return;

    Line line(m_lastSegment.x, m_lastSegment.y, x, y);
    float length = line.length();
    float ndx = (line.y0 - line.y1) / length;
    float ndy = (line.x1 - line.x0) / length;

    // std::cout << "lineTo(" << x << ", " << y << ")" << " length=" << length << ", normal=" << ndx << "," << ndy << std::endl;

    float w2 = width / 2;
    float cw2 = m_lastSegment.width / 2;

    Line right(line.x0 + ndx * cw2,
               line.y0 + ndy * cw2,
               line.x1 + ndx * w2,
               line.y1 + ndy * w2);
    Line left(line.x0 - ndx * cw2,
              line.y0 - ndy * cw2,
              line.x1 - ndx * w2,
              line.y1 - ndy * w2);

    if (m_lastSegment.type == LineToSegment) {
        join(m_lastLeft, m_lastRight, left, right);
    }

    emit(left, right);

    if (m_lastSegment.type == MoveToSegment) {
        m_firstLeft = left;
        m_firstRight = right;
    }

    store(x, y, LineToSegment);
    m_lastLeft = left;
    m_lastRight = right;

}



template <typename Rasterizer>
void Stroker<Rasterizer>::join(Line lastLeft, Line lastRight, Line left, Line right)
{
    if (joinStyle == BevelJoin) {
        emit(Line(lastLeft.x1, lastLeft.y1, left.x0, left.y0),
             Line(lastRight.x1, lastRight.y1, right.x0, right.y0));
    }
}



template <typename Rasterizer>
void Stroker<Rasterizer>::close()
{
    if (m_lastSegment.type == LineToSegment) {
        assert(m_firstSegment.type == MoveToSegment);
        lineTo(m_firstSegment.x, m_firstSegment.y);

        // std::cout << " - lineTo(" << m_firstSegment.x << "," << m_firstSegment.y << ")" << std::endl;
        // std::cout << " - lastLeft:   " << m_lastLeft << std::endl
        //           << " - lastRight:  " << m_lastRight << std::endl
        //           << " - firstLeft:  " << m_firstLeft << std::endl
        //           << " - firstRight: " << m_firstRight << std::endl;

        join(m_lastLeft, m_lastRight, m_firstLeft, m_firstRight);
    }
}



template <typename Rasterizer>
void Stroker<Rasterizer>::finish()
{
}



template <typename Rasterizer>
void Stroker<Rasterizer>::reset()
{
    width = 1;
    joinStyle = BevelJoin;
    capStyle = FlatCap;
    std::memset(&m_lastSegment, 0, sizeof(Segment));
    std::memset(&m_firstSegment, 0, sizeof(Segment));
}

template <typename Rasterizer>
void Stroker<Rasterizer>::emit(Line left, Line right)
{
    rasterizer(Triangle(right.x0, right.y0, right.x1, right.y1, left.x0, left.y0));
    rasterizer(Triangle(left.x0, left.y0, right.x1, right.y1, left.x1, left.y1));
}
