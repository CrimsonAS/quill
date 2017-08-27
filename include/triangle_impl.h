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



inline Triangle::Triangle()
{
}



inline Triangle::Triangle(float x0_, float y0_, float x1_, float y1_, float x2_, float y2_)
    : x0(x0_)
    , y0(y0_)
    , x1(x1_)
    , y1(y1_)
    , x2(x2_)
    , y2(y2_)
{
}



inline void Triangle::sort()
{
    if (y1 < y0) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    if (y2 < y0) {
        std::swap(x0, x2);
        std::swap(y0, y2);
    }
    if (y2 < y1) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
}



inline std::ostream &operator<<(std::ostream &o, Triangle t)
{
    o << "triangle(" << t.x0 << "," << t.y0
      << " - " << t.x1 << "," << t.y1
      << " - " << t.x2 << "," << t.y2 << ")";
    return o;
}

