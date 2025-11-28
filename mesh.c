#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include "mesh.h"


void init_mesh(Mesh *m) {
    if (!m) return;
    m->triangles = NULL;
    m->nb_triangles = 0;
}

void free_mesh(Mesh *m) {
    free(m->triangles);
    m->triangles = NULL;
    m->nb_triangles = 0;
}

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

Mesh_Error read_mesh_binary(FILE *f, Mesh *m_out) {

    if (f == NULL) {
        return MESH_ERROR_OPENING_FILE;
    }

    if (m_out == NULL) {
        return MESH_ERROR_NULL_POINTER;
    }

    // header (80 bytes)
    char header[80];
    if (fread(header, 1, 80, f) != 80) {
        fclose(f);
        return MESH_ERROR_HEADER_READ;
    }

    // number of triangles (4 bytes)
    uint32_t n = 0;
    if (fread(&n, 4, 1, f) != 1) {
        fclose(f);
        return MESH_ERROR_TRIANGLE_COUNT_READ;
    }

    // allocate memory for triangles
    Triangle *triangles = malloc(n * sizeof(Triangle));
    if (triangles == NULL) {
        fclose(f);
        return MESH_ERROR_MEMORY_TRIANGLES_ALLOCATION_FAILED;
    }

    // read triangles
    for (int i = 0; i < n; i++) {
        Triangle_binary triangle_bin;
        if (fread(&triangle_bin, 50, 1, f) != 1) {
            free(triangles);
            fclose(f);
            return MESH_ERROR_TRIANGLES_READ;
        }

        triangles[i].n.x = triangle_bin.n[0];
        triangles[i].n.y = triangle_bin.n[1];
        triangles[i].n.z = triangle_bin.n[2];

        triangles[i].V1.x = triangle_bin.V1[0];
        triangles[i].V1.y = triangle_bin.V1[1];
        triangles[i].V1.z = triangle_bin.V1[2];

        triangles[i].V2.x = triangle_bin.V2[0];
        triangles[i].V2.y = triangle_bin.V2[1];
        triangles[i].V2.z = triangle_bin.V2[2];

        triangles[i].V3.x = triangle_bin.V3[0];
        triangles[i].V3.y = triangle_bin.V3[1];
        triangles[i].V3.z = triangle_bin.V3[2];
    }

    fclose(f);

    // success
    m_out->triangles = triangles;
    m_out->nb_triangles = n;

    return MESH_OK; 
}

void write_stats(Mesh *m) {

    if (m == NULL || m->triangles == NULL) {
        printf("Error: mesh is NULL or triangles are NULL\n");
        return;
    }

    printf("Mesh stats:\n");
    printf("--------------------------------\n");
    printf("Number of triangles: %d\n", m->nb_triangles);

    // bounding box
    float min_x = FLT_MAX, min_y = FLT_MAX, min_z = FLT_MAX;
    float max_x = -FLT_MAX, max_y = -FLT_MAX, max_z = -FLT_MAX;

    for (unsigned int i = 0; i < m->nb_triangles; i++) {
        Triangle t = m->triangles[i];

        Vec3 vs[3] = { t.V1, t.V2, t.V3 };

        for (int k = 0; k < 3; k++) {
            if (vs[k].x < min_x) min_x = vs[k].x;
            if (vs[k].y < min_y) min_y = vs[k].y;
            if (vs[k].z < min_z) min_z = vs[k].z;

            if (vs[k].x > max_x) max_x = vs[k].x;
            if (vs[k].y > max_y) max_y = vs[k].y;
            if (vs[k].z > max_z) max_z = vs[k].z;
        }
    }

    printf("Bounding box:\n");
    printf("\tX: %f .. %f (size = %f)\n", min_x, max_x, max_x - min_x);
    printf("\tY: %f .. %f (size = %f)\n", min_y, max_y, max_y - min_y);
    printf("\tZ: %f .. %f (size = %f)\n\n", min_z, max_z, max_z - min_z);

    printf("Model height (Z size): %f\n", max_z - min_z);

    printf("--------------------------------\n");
}
