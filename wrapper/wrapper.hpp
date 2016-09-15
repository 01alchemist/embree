
extern "C" {

void init();
void *createMesh(int numTriangles, float *data);
void intersectMesh(void *mesh, float ox, float oy, float oz, float dx, float dy, float dz, float *t, int *index);

}
