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

#include "scene_subdiv_mesh.h"
#include "scene.h"

#if !defined(__MIC__)
#include "subdiv/feature_adaptive_eval.h"
#endif

namespace embree
{
  void SubdivMeshAVX::interpolate(unsigned primID, float u, float v, const float* src_i, size_t byteStride, float* dst, size_t numFloats) 
  {
#if defined(DEBUG) // FIXME: use function pointers and also throw error in release mode
    if ((parent->aflags & RTC_INTERPOLATE) == 0) 
      throw_RTCError(RTC_INVALID_OPERATION,"rtcInterpolate can only get called when RTC_INTERPOLATE is enabled for the scene");
#endif

#if !defined(__MIC__) // FIXME: not working on MIC yet
    const char* src = (const char*) src_i;
    
    for (size_t i=0; i<numFloats; i+=8)
    {
      if (i+4 > numFloats)
      {
        const size_t n = numFloats-i;
        auto load = [&](const SubdivMesh::HalfEdge* p) { 
          const unsigned vtx = p->getStartVertexIndex();
          return ssef::loadu((float*)&src[vtx*byteStride],n); 
        };
        ssef out = feature_adaptive_point_eval(getHalfEdge(primID),load,u,v);
        for (size_t j=i; j<numFloats; j++)
          dst[j] = out[j-i];
      }
      else if (i+8 > numFloats) 
      {
        const size_t n = numFloats-i;
        auto load = [&](const SubdivMesh::HalfEdge* p) { 
          const unsigned vtx = p->getStartVertexIndex();
          return avxf::loadu((float*)&src[vtx*byteStride],n); 
        };
        avxf out = feature_adaptive_point_eval(getHalfEdge(primID),load,u,v);
        for (size_t j=i; j<numFloats; j++)
          dst[j] = out[j-i];
      } 
      else
      {
        auto load = [&](const SubdivMesh::HalfEdge* p) { 
          const unsigned vtx = p->getStartVertexIndex();
          return avxf::loadu((float*)&src[vtx*byteStride]);
        };
        avxf out = feature_adaptive_point_eval(getHalfEdge(primID),load,u,v);
        for (size_t j=i; j<i+8; j++)
          dst[j] = out[j-i];
      }
    }
    AVX_ZERO_UPPER();
#endif
  }
}