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



template <typename Vertex>
inline Triangle<Vertex> Triangle<Vertex>::create(float ax, float ay, float bx, float by, float cx, float cy)
{
    Triangle<Vertex> t;
    t.a.x = ax;
    t.a.y = ay;
    t.b.x = bx;
    t.b.y = by;
    t.c.x = cx;
    t.c.y = cy;
    return t;
}



template <typename Vertex>
inline void Triangle<Vertex>::sort()
{
    if (b.y < a.y) {
        swap(a, b);
    }
    if (c.y < a.y) {
        swap(a, c);
    }
    if (c.y < b.y) {
        swap(c, b);
    }
}



template <typename Vertex>
inline std::ostream &operator<<(std::ostream &o, Triangle<Vertex> t)
{
    o << "triangle(" << t.a << ", " << t.b << ", " << t.c << ")";
    return o;
}

