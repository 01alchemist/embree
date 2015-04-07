// ======================================================================== //
// Copyright 2009-2015 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "sys/platform.h"
#include "sys/sysinfo.h"
#include "sys/thread.h"
#include "sys/alloc.h"
#include "sys/ref.h"
#include "sys/intrinsics.h"
#include "sys/atomic.h"
#include "sys/mutex.h"
#include "sys/vector.h"
#include "sys/array.h"
#include "sys/string.h"
#include "sys/regression.h"

#include "math/math.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/bbox.h"
#include "math/obbox.h"
#include "math/affinespace.h"
#include "simd/simd.h"

#if defined(TASKING_LOCKSTEP)
#include "tasking/taskscheduler_mic.h"
#else // if defined(TASKING_TBB_INTERNAL) // FIXME
#include "tasking/taskscheduler_tbb.h"
#endif

#include "config.h"
#include "isa.h"
#include "stat.h"
#include "profile.h"
#include "rtcore.h"

#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <array>

namespace embree
{
#if defined (__SSE__) // || defined (__MIC__)
  typedef Vec2<sseb> sse2b;
  typedef Vec3<sseb> sse3b;
  typedef Vec2<ssei> sse2i;
  typedef Vec3<ssei> sse3i;
  typedef Vec2<ssef> sse2f;
  typedef Vec3<ssef> sse3f;
  typedef Vec4<ssef> sse4f;
  typedef LinearSpace3<sse3f> LinearSpaceSSE3f;
  typedef AffineSpaceT<LinearSpace3<sse3f > > AffineSpaceSSE3f;
  typedef BBox<sse3f > BBoxSSE3f;
#endif

#if defined (__AVX__)
  typedef Vec2<avxb> avx2b;
  typedef Vec3<avxb> avx3b;
  typedef Vec2<avxi> avx2i; 
  typedef Vec3<avxi> avx3i;
  typedef Vec2<avxf> avx2f;
  typedef Vec3<avxf> avx3f;
  typedef Vec4<avxf> avx4f;
#endif

#if defined (__MIC__)
  typedef Vec2<mic_m> mic2b;
  typedef Vec3<mic_m> mic3b;
  typedef Vec2<mic_i> mic2i;
  typedef Vec3<mic_i> mic3i;
  typedef Vec2<mic_f> mic2f;
  typedef Vec3<mic_f> mic3f;
  typedef Vec4<mic_f> mic4f;
  typedef Vec4<mic_i> mic4i;
#endif
}
