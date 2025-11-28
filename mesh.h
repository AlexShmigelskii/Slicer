#include <stdio.h>
#include <assert.h>
#include "math3d.h"

typedef struct {
    Vec3 n;   // Normal vector pointing out of the model
    Vec3 V1;
    Vec3 V2;
    Vec3 V3;
} Triangle;

// Force the compiler to pack the structure with 1-byte alignment,
// so there are no padding bytes between fields.
// STL binary format requires exactly 50 bytes per triangle.
#pragma pack(push, 1)
typedef struct {
    float n[3];        // normal 12 bytes   (3 floats = 12 bytes)
    float V1[3];       // Vertex1 12 bytes
    float V2[3];       // Vertex2 12 bytes
    float V3[3];       // Vertex3 12 bytes
    uint16_t attr;     // Attribute byte count 02 bytes
} Triangle_binary;        // TOTAL = 12 + 12 + 12 + 12 + 2 = 50 bytes
#pragma pack(pop)
// Restore previous packing alignment settings

static_assert(sizeof(Triangle_binary) == 50, "Triangle_binary must be a size of 50 bytes");

typedef struct {
    Triangle* triangles;
    unsigned int nb_triangles;
} Mesh;

typedef enum {
    MESH_OK = 0,
    MESH_ERROR_OPENING_FILE,
    MESH_ERROR_NULL_POINTER,
    MESH_ERROR_HEADER_READ,
    MESH_ERROR_TRIANGLE_COUNT_READ,
    MESH_ERROR_MEMORY_TRIANGLES_ALLOCATION_FAILED,
    MESH_ERROR_TRIANGLES_READ,
} Mesh_Error;

// Initialising Mesh to be sure that structure
// is ready for use 
void init_mesh(Mesh *m);

// Freeing up memory
void free_mesh(Mesh *m);

// Write Mesh m to a file f (open for writing "w")
void write_mesh_ascii(FILE *f, Mesh *m);

// Write Mesh m to a file f (open for writing "wb")
void write_mesh_binary(FILE *f, Mesh *m);

// Read Mesh from binary .STL file to Mesh m_out 
// (open for reading "rb")
Mesh_Error read_mesh_binary(FILE *f, Mesh *m_out);

// Write stats of Mesh m to the console
void write_stats(Mesh *m);
