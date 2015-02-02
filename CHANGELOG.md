Version History
---------------

### New Features in Embree 2.4

-   Support for Catmull Clark subdivision surfaces (triangle/quad base
    primitives)
-   Support for vector displacements on Catmull Clark subdivision
    surfaces
-   Various bugfixes (e.g. 4-byte alignment of vertex buffers works)

### New Features in Embree 2.3.3

-   BVH builders more robustly handle invalid input data (Intel® Xeon®
    processor family)
-   Motion blur support for hair geometry (Xeon)
-   Improved motion blur performance for triangle geometry (Xeon)
-   Improved robust ray tracing mode (Xeon)
-   Added `rtcCommitThread` API call for easier integration into
    existing tasking systems (Xeon and Intel® Xeon Phi™ coprocessor)
-   Added support for recording and replaying all
    `rtcIntersect`/`rtcOccluded` calls (Xeon and Xeon Phi)

### New Features in Embree 2.3.2

-   Improved mixed AABB/OBB-BVH for hair geometry (Xeon Phi)
-   Reduced amount of pre-allocated memory for BVH builders (Xeon Phi)
-   New 64\ bit Morton code-based BVH builder (Xeon Phi)
-   (Enhanced) Morton code-based BVH builders use now tree rotations to
    improve BVH quality (Xeon Phi)
-   Bug fixes (Xeon and Xeon Phi)

### New Features in Embree 2.3.1

-   High quality BVH mode improves spatial splits which result in up to
    30% performance improvement for some scenes (Xeon).
-   Compile time enabled intersection filter functions do not reduce
    performance if no intersection filter is used in the scene (Xeon and
    Xeon Phi)
-   Improved ray tracing performance for hair geometry by \>20% on Xeon
    Phi. BVH for hair geometry requires 20% less memory
-   BVH8 for AVX/AVX2 targets improves performance for single ray
    tracing on Haswell by up to 12% and by up to 5% for hybrid (Xeon)
-   Memory conservative BVH for Xeon Phi now uses BVH node quantization
    to lower memory footprint (requires half the memory footprint of the
    default BVH)

### New Features in Embree 2.3

-   Support for ray tracing hair geometry (Xeon and Xeon Phi)
-   Catching errors through error callback function
-   Faster hybrid traversal (Xeon and Xeon Phi)
-   New memory conservative BVH for Xeon Phi
-   Faster Morton code-based builder on Xeon
-   Faster binned-SAH builder on Xeon Phi
-   Lots of code cleanups/simplifications/improvements (Xeon and Xeon
    Phi)

### New Features in Embree 2.2

-   Support for motion blur on Xeon Phi
-   Support for intersection filter callback functions
-   Support for buffer sharing with the application
-   Lots of AVX2 optimizations, e.g. \~20% faster 8-wide hybrid
    traversal
-   Experimental support for 8-wide (AVX/AVX2) and 16-wide BVHs (Xeon
    Phi)

### New Features in Embree 2.1

-   New future proof API with a strong focus on supporting dynamic
    scenes
-   Lots of optimizations for 8-wide AVX2 (Haswell architecture)
-   Automatic runtime code selection for SSE, AVX, and AVX2
-   Support for user-defined geometry
-   New and improved BVH builders:
    -   Fast adaptive Morton code-based builder (without SAH-based
        top-level rebuild)
    -   Both the SAH and Morton code-based builders got faster (Xeon
        Phi)
    -   New variant of the SAH-based builder using triangle pre-splits
        (Xeon Phi)

### Example Performance Numbers for Embree 2.1

BVH rebuild performance (including triangle accel generation, excluding
memory allocation) for scenes with 2-12 million triangles:

-   Intel® Core™ i7 (Haswell-based CPU, 4 cores @ 3.0\ GHz)
    -   7-8 million triangles/s for the SAH-based BVH builder
    -   30-36 million triangles/s for the Morton code-based BVH builder
-   Intel® Xeon Phi™ 7120
    -   37-40 million triangles/s for the SAH-based BVH builder
    -   140-160 million triangles/s for the Morton code-based BVH
        builder

Rendering of the Crown model (`crown.ecs`) with 4\ samples per pixel
(`-spp 4`):

-   Intel® Core™ i7 (Haswell-based CPU, 4 cores CPU @ 3.0\ GHz)
    -   1024×1024 resolution: 7.8 million rays per sec
    -   1920×1080 resolution: 9.9 million rays per sec
-   Intel® Xeon Phi™ 7120
    -   1024×1024 resolution: 47.1 million rays per sec
    -   1920×1080 resolution: 61.1 million rays per sec

### New Features in Embree 2.0

-   Support for the Intel® Xeon Phi™ coprocessor platform
-   Support for high-performance "packet" kernels on SSE, AVX, and Xeon
    Phi
-   Integration with the Intel® SPMD Program Compiler (ISPC)
-   Instantiation and fast BVH reconstruction
-   Example photo-realistic rendering engine for both C++ and ISPC
