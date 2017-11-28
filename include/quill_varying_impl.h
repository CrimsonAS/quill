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


inline Varying2D operator+(Varying2D a, Varying2D b) { return Varying2D(a.x + b.x, a.y + b.y); }
inline Varying2D operator+(Varying2D a, float u)     { return Varying2D(a.x + u,   a.y + u);   }
inline Varying2D operator+(float u, Varying2D a)     { return Varying2D(u + a.x,   u + a.y);   }
inline Varying2D operator-(Varying2D a, Varying2D b) { return Varying2D(a.x - b.x, a.y - b.y); }
inline Varying2D operator-(Varying2D a, float u)     { return Varying2D(a.x - u,   a.y - u);   }
inline Varying2D operator-(float u, Varying2D a)     { return Varying2D(u - a.x,   u - a.y);   }
inline Varying2D operator*(Varying2D a, float u)     { return Varying2D(a.x * u,   a.y * u);   }
inline Varying2D operator*(float u, Varying2D a)     { return Varying2D(u * a.x,   u * a.y);   }
inline Varying2D operator/(Varying2D a, float u)     { return Varying2D(a.x / u,   a.y / u);   }

inline std::ostream &operator<<(std::ostream &o, Varying2D v)
{
    o << "[" << v.x << "," << v.y << "]"; return o;
}



inline Varying3D operator+(Varying3D a, Varying3D b) { return Varying3D(a.x + b.x, a.y + b.y, a.z + b.z); }
inline Varying3D operator+(Varying3D a, float u)     { return Varying3D(a.x + u,   a.y + u,   a.z + u);   }
inline Varying3D operator+(float u, Varying3D a)     { return Varying3D(u + a.x,   u + a.y,   u + a.z);   }
inline Varying3D operator-(Varying3D a, Varying3D b) { return Varying3D(a.x - b.x, a.y - b.y, a.z - b.z); }
inline Varying3D operator-(Varying3D a, float u)     { return Varying3D(a.x - u,   a.y - u,   a.z - u);   }
inline Varying3D operator-(float u, Varying3D a)     { return Varying3D(u - a.x,   u - a.y,   u - a.z);   }
inline Varying3D operator*(Varying3D a, float u)     { return Varying3D(a.x * u,   a.y * u,   a.z * u);   }
inline Varying3D operator*(float u, Varying3D a)     { return Varying3D(u * a.x,   u * a.y,   u * a.z);   }
inline Varying3D operator/(Varying3D a, float u)     { return Varying3D(a.x / u,   a.y / u,   a.z / u);   }

inline std::ostream &operator<<(std::ostream &o, Varying3D v)
{
    o << "[" << v.x << "," << v.y << "," << v.z << "]"; return o;
}



inline Varying4D operator+(Varying4D a, Varying4D b) { return Varying4D(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
inline Varying4D operator+(Varying4D a, float u)     { return Varying4D(a.x + u,   a.y + u,   a.z + u,   a.w + u);   }
inline Varying4D operator+(float u, Varying4D a)     { return Varying4D(u + a.x,   u + a.y,   u + a.z,   u + a.w);   }
inline Varying4D operator-(Varying4D a, Varying4D b) { return Varying4D(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
inline Varying4D operator-(Varying4D a, float u)     { return Varying4D(a.x - u,   a.y - u,   a.z - u,   a.w - u);   }
inline Varying4D operator-(float u, Varying4D a)     { return Varying4D(u - a.x,   u - a.y,   u - a.z,   u - a.w);   }
inline Varying4D operator*(Varying4D a, float u)     { return Varying4D(a.x * u,   a.y * u,   a.z * u,   a.w * u);   }
inline Varying4D operator*(float u, Varying4D a)     { return Varying4D(u * a.x,   u * a.y,   u * a.z,   u * a.w);   }
inline Varying4D operator/(Varying4D a, float u)     { return Varying4D(a.x / u,   a.y / u,   a.z / u,   a.w / u);   }

inline std::ostream &operator<<(std::ostream &o, Varying4D v)
{
    o << "[" << v.x << "," << v.y << "," << v.z << "," << v.w << "]"; return o;
}



inline VaryingNoop operator+(VaryingNoop, VaryingNoop) { return VaryingNoop(); }
inline VaryingNoop operator+(VaryingNoop, float) { return VaryingNoop(); }
inline VaryingNoop operator+(float, VaryingNoop) { return VaryingNoop(); }
inline VaryingNoop operator-(VaryingNoop, VaryingNoop) { return VaryingNoop(); }
inline VaryingNoop operator-(VaryingNoop, float) { return VaryingNoop(); }
inline VaryingNoop operator-(float, VaryingNoop) { return VaryingNoop(); }
inline VaryingNoop operator*(VaryingNoop, float) { return VaryingNoop(); }
inline VaryingNoop operator*(float, VaryingNoop) { return VaryingNoop(); }
inline VaryingNoop operator/(VaryingNoop, float) { return VaryingNoop(); }

inline std::ostream &operator<<(std::ostream &o, VaryingNoop)
{
    return o;
}
