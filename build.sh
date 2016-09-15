#!/usr/bin/env bash

mkdir js_build
cd js_build
cmake .. -G "Unix Makefiles" -DEMBREE_ISPC_SUPPORT=OFF
emmake make

emcc libembree.so libembree.so.2 libembree.so.2.11.0 \
libimage.a liblexers.a liblights.a libnoise.a libscenegraph.a \
libsimd.a libsys.a libtexture.a libtutorial.a libtutorial_device.a \
libembree_avx.a libembree_avx2.a libembree_sse42.a \
././wrapper/wrapper.cpp -o embree.js -s TOTAL_MEMORY=167772160 -s USE_PTHREADS=1 -s EXPORTED_FUNCTIONS="['_init','_createMesh','_intersectMesh']"
