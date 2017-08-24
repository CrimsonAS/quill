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



template <typename TrapezoidConsumer>
Stroker<TrapezoidConsumer>::Stroker()
{
    reset();
}



template <typename TrapezoidConsumer>
void Stroker<TrapezoidConsumer>::shift(float x, float y, SegmentType type)
{
    m_last = m_current;
    m_current = {
        .x = x,
        .y = y,
        .width = width,
        .type = type,
        .joinStyle = joinStyle,
        .capStyle = capStyle
    };
}



template <typename TrapezoidConsumer>
void Stroker<TrapezoidConsumer>::moveTo(float x, float y)
{
    shift(x, y, MoveToSegment);
}



template <typename TrapezoidConsumer>
void Stroker<TrapezoidConsumer>::lineTo(float x, float y)
{
    shift(x, y, LineToSegment);
}



template <typename TrapezoidConsumer>
void Stroker<TrapezoidConsumer>::close(float x, float y)
{
}



template <typename TrapezoidConsumer>
void Stroker<TrapezoidConsumer>::reset(float x, float y)
{
    width = 1;
    joinStyle = BevelJoin;
    capStyle = FlatCap;
    memset(&m_current, 0, sizeof(Segment));
    memset(&m_last, 0, sizeof(Segment));
}
