#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mesh.h"


void write_mesh_ascii(FILE *f, Mesh* m) {

    if (f == NULL) {
        printf("Error: error opening file\n");
        exit(1);
    }

    fprintf(f, "solid house\n\n");

    unsigned int n = m->nb_triangles;
    for (int i = 0; i < n; i++) {
        fprintf(f, "facet normal %f %f %f\n", m->triangles[i].n.x, m->triangles[i].n.y, m->triangles[i].n.z);
        fprintf(f, "\touter loop\n");
        fprintf(f, "\t\tvertex %f %f %f\n", m->triangles[i].V1.x, m->triangles[i].V1.y, m->triangles[i].V1.z);
        fprintf(f, "\t\tvertex %f %f %f\n", m->triangles[i].V2.x, m->triangles[i].V2.y, m->triangles[i].V2.z);
        fprintf(f, "\t\tvertex %f %f %f\n", m->triangles[i].V3.x, m->triangles[i].V3.y, m->triangles[i].V3.z);
        fprintf(f, "\tendloop\n");
        fprintf(f, "endfacet\n\n");
    }

    fprintf(f, "endsolid house");
}

void write_mesh_binary(FILE *f, Mesh *m) {

    if (f == NULL) {
        printf("Error: error opening file\n");
        exit(1);
    }

    // header (80 bytes)
    char header[80] = "My stl house in binary";
    fwrite(header, 1, 80, f);

    // number of triangles (4 bytes)
    uint32_t n = m->nb_triangles;
    static_assert(sizeof(n) == 4, "n must be 4 bytes for STL binary format");
    fwrite(&n, 4, 1, f);

    // for each triangle 
    for (int i = 0; i < n; i++) {
        Triangle_binary t;
        t.n[0] = m->triangles[i].n.x;
        t.n[1] = m->triangles[i].n.y;
        t.n[2] = m->triangles[i].n.z;

        t.V1[0] = m->triangles[i].V1.x;
        t.V1[1] = m->triangles[i].V1.y;
        t.V1[2] = m->triangles[i].V1.z;
        
        t.V2[0] = m->triangles[i].V2.x;
        t.V2[1] = m->triangles[i].V2.y;
        t.V2[2] = m->triangles[i].V2.z;

        t.V3[0] = m->triangles[i].V3.x;
        t.V3[1] = m->triangles[i].V3.y;
        t.V3[2] = m->triangles[i].V3.z;

        t.attr = 0;

        fwrite(&t, 50, 1, f);
    }
}
