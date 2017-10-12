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

template <typename Rasterizer, typename VaryingGenerator = VaryingGeneratorNoop>
struct Stroker
{
    JoinStyle joinStyle = BevelJoin;
    CapStyle capStyle   = FlatCap;
    float width         = 1.0f;

    Rasterizer raster;
    VaryingGenerator varying;

    int triangleCount   = 0;
    float length        = 0.0f;

    Stroker();

    void moveTo(float x, float y);
    void lineTo(float x, float y);
    void close();
    void finish();

    void reset();

    // ********************
    // Internals
    //

    typedef typename Rasterizer::Varyings Varyings;

    enum SegmentType {
        InvalidType,
        MoveToSegment,
        LineToSegment
    };

    struct Segment {
        float x;
        float y;
        float width;
        float length;

        SegmentType type : 2;
        JoinStyle joinStyle : 2;
        CapStyle capStyle : 2;

        unsigned int reserved : 26;

        Segment(SegmentType type = InvalidType,
                float x = 0.0f,
                float y = 0.0f,
                float width = 1.0f,
                float length = 0.0f,
                JoinStyle joinStyle = BevelJoin,
                CapStyle capStyle = FlatCap);
    };

    void store(float x, float y, SegmentType type);

    void cap(Line left, Line right, float cx, float cy, float length, bool endCap);
    void join(Line lastLeft, Line lastRight, Line left, Line right, float length, float width);
    void stroke(Line left, Line right, float length, float startWidth, float endWidth);

    Segment m_lastSegment;
    Line m_lastLeft;
    Line m_lastRight;

    Segment m_firstSegment;
    Line m_firstLeft;
    Line m_firstRight;
};
