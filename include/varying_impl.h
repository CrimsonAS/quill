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



inline VaryingUV operator+(VaryingUV a, VaryingUV b)
{
    return VaryingUV(a.u + b.u, a.v + b.v);
}



inline VaryingUV operator+(VaryingUV a, float u)
{
    return VaryingUV(a.u + u,   a.v + u);
}



inline VaryingUV operator+(float u, VaryingUV a)
{
    return VaryingUV(u + a.u,   u + a.v);
}



inline VaryingUV operator-(VaryingUV a, VaryingUV b)
{
    return VaryingUV(a.u - b.u, a.v - b.v);
}



inline VaryingUV operator-(VaryingUV a, float u)
{
    return VaryingUV(a.u - u,   a.v - u);
}



inline VaryingUV operator-(float u, VaryingUV a)
{
    return VaryingUV(u - a.u,   u - a.v);
}



inline VaryingUV operator*(VaryingUV a, float u)
{
    return VaryingUV(a.u * u,   a.v * u);
}



inline VaryingUV operator*(float u, VaryingUV a)
{
    return VaryingUV(u * a.u,   u * a.v);
}



inline VaryingUV operator/(VaryingUV a, float u)
{
    return VaryingUV(a.u / u,   a.v / u);
}



inline std::ostream &operator<<(std::ostream &o, VaryingUV v)
{
    o << "[" << v.u << "," << v.v << "]"; return o;
}



inline VaryingNoop operator+(VaryingNoop, VaryingNoop)
{
    return VaryingNoop();
}



inline VaryingNoop operator+(VaryingNoop, float)
{
    return VaryingNoop();
}



inline VaryingNoop operator+(float, VaryingNoop)
{
    return VaryingNoop();
}



inline VaryingNoop operator-(VaryingNoop, VaryingNoop)
{
    return VaryingNoop();
}



inline VaryingNoop operator-(VaryingNoop, float)
{
    return VaryingNoop();
}



inline VaryingNoop operator-(float, VaryingNoop)
{
    return VaryingNoop();
}



inline VaryingNoop operator*(VaryingNoop, float)
{
    return VaryingNoop();
}



inline VaryingNoop operator*(float, VaryingNoop)
{
    return VaryingNoop();
}



inline VaryingNoop operator/(VaryingNoop, float)
{
    return VaryingNoop();
}



inline std::ostream &operator<<(std::ostream &o, VaryingNoop)
{
    return o;
}
