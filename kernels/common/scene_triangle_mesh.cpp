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

#include "scene_triangle_mesh.h"
#include "scene.h"

namespace embree
{
  TriangleMesh::TriangleMesh (Scene* parent, RTCGeometryFlags flags, size_t numTriangles, size_t numVertices, size_t numTimeSteps)
    : Geometry(parent,TRIANGLE_MESH,numTriangles,numTimeSteps,flags), mask(-1)
  {
    triangles.init(numTriangles,sizeof(Triangle));
    for (size_t i=0; i<numTimeSteps; i++) {
      vertices[i].init(numVertices,sizeof(Vec3fa));
    }
    enabling();
  }
  
  void TriangleMesh::enabling() 
  { 
    if (numTimeSteps == 1) atomic_add(&parent->numTriangles ,triangles.size());
    else                   atomic_add(&parent->numTriangles2,triangles.size());
  }
  
  void TriangleMesh::disabling() 
  { 
    if (numTimeSteps == 1) atomic_add(&parent->numTriangles ,-(ssize_t)triangles.size());
    else                   atomic_add(&parent->numTriangles2,-(ssize_t)triangles.size());
  }

  void TriangleMesh::setMask (unsigned mask) 
  {
    if (parent->isStatic() && parent->isBuild())
      throw_RTCError(RTC_INVALID_OPERATION,"static scenes cannot get modified");

    this->mask = mask; 
  }

  void TriangleMesh::setBuffer(RTCBufferType type, void* ptr, size_t offset, size_t stride) 
  { 
    if (parent->isStatic() && parent->isBuild()) 
      throw_RTCError(RTC_INVALID_OPERATION,"static scenes cannot get modified");

    /* verify that all accesses are 4 bytes aligned */
    if (((size_t(ptr) + offset) & 0x3) || (stride & 0x3)) 
      throw_RTCError(RTC_INVALID_OPERATION,"data must be 4 bytes aligned");

    switch (type) {
    case RTC_INDEX_BUFFER  : 
      triangles.set(ptr,offset,stride); 
      break;
    case RTC_VERTEX_BUFFER0: 
      vertices[0].set(ptr,offset,stride); 
      if (vertices[0].size()) {
        /* test if array is properly padded */
        volatile int w = *((int*)vertices[0].getPtr(vertices[0].size()-1)+3); // FIXME: is failing hard avoidable?
      }
      break;
    case RTC_VERTEX_BUFFER1: 
      vertices[1].set(ptr,offset,stride); 
      if (vertices[1].size()) {
        /* test if array is properly padded */
        volatile int w = *((int*)vertices[1].getPtr(vertices[1].size()-1)+3); // FIXME: is failing hard avoidable?
      }
      break;
    default: 
      throw_RTCError(RTC_INVALID_ARGUMENT,"unknown buffer type");
    }
  }

  void* TriangleMesh::map(RTCBufferType type) 
  {
    if (parent->isStatic() && parent->isBuild())
      throw_RTCError(RTC_INVALID_OPERATION,"static scenes cannot get modified");

    switch (type) {
    case RTC_INDEX_BUFFER  : return triangles  .map(parent->numMappedBuffers);
    case RTC_VERTEX_BUFFER0: return vertices[0].map(parent->numMappedBuffers);
    case RTC_VERTEX_BUFFER1: return vertices[1].map(parent->numMappedBuffers);
    default                : throw_RTCError(RTC_INVALID_ARGUMENT,"unknown buffer type"); return nullptr;
    }
  }

  void TriangleMesh::unmap(RTCBufferType type) 
  {
    if (parent->isStatic() && parent->isBuild())
      throw_RTCError(RTC_INVALID_OPERATION,"static scenes cannot get modified");

    switch (type) {
    case RTC_INDEX_BUFFER  : triangles  .unmap(parent->numMappedBuffers); break;
    case RTC_VERTEX_BUFFER0: vertices[0].unmap(parent->numMappedBuffers); break;
    case RTC_VERTEX_BUFFER1: vertices[1].unmap(parent->numMappedBuffers); break;
    default                : throw_RTCError(RTC_INVALID_ARGUMENT,"unknown buffer type"); break;
    }
  }

  void TriangleMesh::immutable () 
  {
    bool freeTriangles = !parent->needTriangles;
    bool freeVertices  = !parent->needTriangleVertices;
    if (freeTriangles) triangles.free();
    if (freeVertices ) vertices[0].free();
    if (freeVertices ) vertices[1].free();
  }

  bool TriangleMesh::verify () 
  {
    if (numTimeSteps == 2 && vertices[0].size() != vertices[1].size())
        return false;

    for (size_t i=0; i<triangles.size(); i++) {     
      if (triangles[i].v[0] >= numVertices()) return false; 
      if (triangles[i].v[1] >= numVertices()) return false; 
      if (triangles[i].v[2] >= numVertices()) return false; 
    }
    for (size_t j=0; j<numTimeSteps; j++) {
      BufferT<Vec3fa>& verts = vertices[j];
      for (size_t i=0; i<verts.size(); i++) {
	if (!isvalid(verts[i])) 
	  return false;
      }
    }
    return true;
  }

  void TriangleMesh::write(std::ofstream& file)
  {
    int type = TRIANGLE_MESH;
    file.write((char*)&type,sizeof(int));
    file.write((char*)&numTimeSteps,sizeof(int));
    int numVerts = numVertices();
    file.write((char*)&numVerts,sizeof(int));
    int numTriangles = triangles.size();
    file.write((char*)&numTriangles,sizeof(int));

    for (size_t j=0; j<numTimeSteps; j++) {
      while ((file.tellp() % 16) != 0) { char c = 0; file.write(&c,1); }
      for (size_t i=0; i<numVerts; i++) file.write((char*)vertexPtr(i,j),sizeof(Vec3fa));  
    }

    while ((file.tellp() % 16) != 0) { char c = 0; file.write(&c,1); }
    for (size_t i=0; i<numTriangles; i++) file.write((char*)&triangle(i),sizeof(Triangle));  

    while ((file.tellp() % 16) != 0) { char c = 0; file.write(&c,1); }
    for (size_t i=0; i<numTriangles; i++) file.write((char*)&triangle(i),sizeof(Triangle));   // FIXME: why is this written twice?
  }
}
