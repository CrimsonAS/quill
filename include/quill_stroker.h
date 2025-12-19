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

enum CapStyle : uint8_t {
    FlatCap,
    SquareCap,
    RoundCap
};

enum JoinStyle : uint8_t {
    BevelJoin,
    // MiterJoin, // not implemented, so lets not pretend
    RoundJoin
};

struct Slant
{
    float nx = 0.0f;
    float ny = 0.0f;
    float minimumWidth = 0.0f;

    bool isValid() const {
        const float almostZero = 0.0001f;
        return std::abs(std::sqrt(nx * nx + ny * ny) - 1.0f) < almostZero;
    }
};

template <typename Rasterizer, typename VaryingGenerator = VaryingGeneratorNoop>
struct Stroker
{
    JoinStyle joinStyle = BevelJoin;
    CapStyle capStyle   = FlatCap;
    float width         = 1.0f;

    std::optional<Slant> slant;

    Rasterizer raster;
    VaryingGenerator varying;

    int triangleCount   = 0;
    float length        = 0.0f;

    float minx = std::numeric_limits<float>::lowest();
    float miny = std::numeric_limits<float>::lowest();
    float maxx = std::numeric_limits<float>::max();
    float maxy = std::numeric_limits<float>::max();

    Stroker();
    ~Stroker();

    void moveTo(float x, float y);
    void lineTo(float x, float y);
    void close();
    void finish();

    void reset();

    void flushStartCap();
    void flushEndCap();

    // ********************
    // Internals
    //

    using Varyings = typename Rasterizer::Varyings;

    enum SegmentType : uint8_t {
        InvalidType,
        MoveToSegment,
        LineToSegment
    };

    struct Segment {
        float x;
        float y;
        float width;
        float length;

        Varyings leftVarying;
        Varyings rightVarying;

        SegmentType type;
        JoinStyle joinStyle;
        CapStyle capStyle;

        Segment(SegmentType type = InvalidType,
                float x = 0.0f,
                float y = 0.0f,
                float width = 1.0f,
                float length = 0.0f,
                JoinStyle joinStyle = BevelJoin,
                CapStyle capStyle = FlatCap,
                Varyings left = Varyings(),
                Varyings right = Varyings());
    };

    void lineToSlanted(float x, float y);

    void cap(Line left, Line right, Segment s, bool endCap);

    void store(float x, float y, SegmentType type, Varyings left, Varyings right);

    void join(Line lastLeft, Line lastRight, Line left, Line right, Varyings leftVarying, Varyings rightVarying);
    void stroke(Line left, Line right,
                Varyings lastLeftVarying, Varyings lastRightVarying,
                Varyings leftVarying, Varyings rightVarying,
                bool checkDirection = false);

    Segment m_lastSegment;
    Line m_lastLeft;
    Line m_lastRight;

    Segment m_firstSegment;
    Line m_firstLeft;
    Line m_firstRight;

    bool m_startCapRendered = false;
    bool m_endCapRendered = false;
};
