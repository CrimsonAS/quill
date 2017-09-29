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

struct RasterBuffer
{
    unsigned int width = 0;
    unsigned int height = 0;

    unsigned int *data = nullptr;

    ~RasterBuffer();

    void allocate(unsigned int w, unsigned int h);
    void release();

    unsigned int *scanline(unsigned int y);
    void fill(unsigned int value);
};

inline RasterBuffer::~RasterBuffer()
{
    release();
}

inline void RasterBuffer::allocate(unsigned int width, unsigned int height)
{
    assert(!data);

    this->width = width;
    this->height = height;
    data = new unsigned int[width * height];
}

inline void RasterBuffer::release()
{
    delete [] data;
}

inline unsigned int *RasterBuffer::scanline(unsigned int y)
{
    assert(y < height);
    assert(data);

    return data + y * width;
}

inline void RasterBuffer::fill(unsigned int value)
{
    for (unsigned int y=0; y<height; ++y) {
        unsigned int *line = scanline(y);
        for (unsigned int x=0; x<width; ++x) {
            line[x] = value;
        }
    }
}
