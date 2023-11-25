/*
    Copyright (c) 2019, reMarkable AS <technology@remarkable.no>
    Copyright (c) 2019, Gunnar Sletta <gunnar@crimson.no>
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

#include <iostream>
#include <cassert>
#include <cmath>
#include <cstring>
#include <optional>

namespace Quill {
    // Various utilities
    #include "quill_global.h"

    // Declarations
    #include "quill_line.h"
    #include "quill_vertex.h"
    #include "quill_triangle.h"
    #include "quill_varying.h"
    #include "quill_varyinggenerator.h"

    #include "quill_clockwiseraster.h"
    #include "quill_clipraster.h"
    #include "quill_lerpraster.h"
    #include "quill_monoraster.h"

    #include "quill_stroker.h"

    // Implementations
    #include "quill_line_impl.h"
    #include "quill_vertex_impl.h"
    #include "quill_triangle_impl.h"
    #include "quill_varying_impl.h"
    #include "quill_varyinggenerator_impl.h"

    #include "quill_clockwiseraster_impl.h"
    #include "quill_clipraster_impl.h"
    #include "quill_lerpraster_impl.h"
    #include "quill_monoraster_impl.h"

    #include "quill_stroker_impl.h"


};
