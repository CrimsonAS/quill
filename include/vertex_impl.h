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


template <typename Data> Vertex<Data> operator+(Vertex<Data> a, Vertex<Data> b)
{
    return Vertex<Data>(a.x + b.x, a.y + b.y, a.data + b.data);
}

template <typename Data> Vertex<Data> operator+(Vertex<Data> a, float x)
{
    return Vertex<Data>(a.x + x,   a.y + x,   a.data + x);
}

template <typename Data> Vertex<Data> operator+(float x, Vertex<Data> a)
{
    return Vertex<Data>(x + a.x,   x + a.y,   x + a.data);
}

template <typename Data> Vertex<Data> operator-(Vertex<Data> a, Vertex<Data> b)
{
    return Vertex<Data>(a.x - b.x, a.y - b.y, a.data - b.data);
}

template <typename Data> Vertex<Data> operator-(Vertex<Data> a, float x)
{
    return Vertex<Data>(a.x - x,   a.y - x),  a.data - x;
}

template <typename Data> Vertex<Data> operator-(float x, Vertex<Data> a)
{
    return Vertex<Data>(x - a.x,   x - a.y),  x - a.data;
}

template <typename Data> Vertex<Data> operator*(Vertex<Data> a, float x)
{
    return Vertex<Data>(a.x * x,   a.y * x,   a.data * x);
}

template <typename Data> Vertex<Data> operator*(float x, Vertex<Data> a)
{
    return Vertex<Data>(x * a.x,   x * a.y,   x * a.data);
}

template <typename Data> Vertex<Data> operator/(Vertex<Data> a, float x)
{
    return Vertex<Data>(a.x / x,   a.y / x ,  a.data / x);
}

template <typename Data>
std::ostream &operator<<(std::ostream &o, Vertex<Data> v)
{
    o << "[" << v.x << "," << v.y << "," << v.data << "]";
    return o;
}



inline Float2D operator+(Float2D a, Float2D b)
{
    return Float2D(a.x + b.x, a.y + b.y);
}

inline Float2D operator+(Float2D a, float x)
{
    return Float2D(a.x + x,   a.y + x);
}

inline Float2D operator+(float x, Float2D a)
{
    return Float2D(x + a.x,   x + a.y);
}

inline Float2D operator-(Float2D a, Float2D b)
{
    return Float2D(a.x - b.x, a.y - b.y);
}

inline Float2D operator-(Float2D a, float x)
{
    return Float2D(a.x - x,   a.y - x);
}

inline Float2D operator-(float x, Float2D a)
{
    return Float2D(x - a.x,   x - a.y);
}

inline Float2D operator*(Float2D a, float x)
{
    return Float2D(a.x * x,   a.y * x);
}

inline Float2D operator*(float x, Float2D a)
{
    return Float2D(x * a.x,   x * a.y);
}

inline Float2D operator/(Float2D a, float x)
{
    return Float2D(a.x / x,   a.y / x);
}

std::ostream &operator<<(std::ostream &o, Float2D v)
{
    o << "[" << v.x << "," << v.y << "]"; return o;
}



inline NoData operator+(NoData, NoData)
{
    return NoData();
}

inline NoData operator+(NoData, float)
{
    return NoData();
}

inline NoData operator+(float, NoData)
{
    return NoData();
}

inline NoData operator-(NoData, NoData)
{
    return NoData();
}

inline NoData operator-(NoData, float)
{
    return NoData();
}

inline NoData operator-(float, NoData)
{
    return NoData();
}

inline NoData operator*(NoData, float)
{
    return NoData();
}

inline NoData operator*(float, NoData)
{
    return NoData();
}

inline NoData operator/(NoData, float)
{
    return NoData();
}

std::ostream &operator<<(std::ostream &o, NoData)
{
    return o;
}
