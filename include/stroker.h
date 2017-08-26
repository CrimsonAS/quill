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
struct Stroker
{
    enum CapStyle {
        FlatCap,
        SquareCap,
        RoundCap
    };

    enum JoinStyle {
        BevelJoin,
        MiterJoin,
        RoundJoin
    };

    JoinStyle joinStyle = BevelJoin;
    CapStyle capStyle   = FlatCap;
    float width         = 1.0f;

    Rasterizer rasterizer;

    Stroker();

    void moveTo(float x, float y);
    void lineTo(float x, float y);
    void close();
    void finish();

    void reset();

    // ********************
    // Internals
    //

    enum SegmentType {
        InvalidType,
        MoveToSegment,
        LineToSegment
    };

    struct Segment {
        float x;
        float y;
        float width;

        SegmentType type : 2;
        JoinStyle joinStyle : 2;
        CapStyle capStyle : 2;

        unsigned int reserved : 26;
    };

    void store(float x, float y, SegmentType type);

    void join(Line lastLeft, Line lastRight, Line left, Line right);
    void emit(Line left, Line right);

    Segment m_lastSegment;
    Line m_lastLeft;
    Line m_lastRight;

    Segment m_firstSegment;
    Line m_firstLeft;
    Line m_firstRight;
};
