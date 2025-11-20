#include <stdio.h>
#include <stdlib.h>
#include "math3d.h"
#include "mesh.h"

/*
Общий пайплайн (в терминах структур)
	1.	AppConfig cfg = load_config_from_defaults_or_cli(...);
	2.	Mesh mesh = stl_read_binary(cfg.input_stl_path);
	3.	mesh_normalize_to_build_volume(&mesh, &cfg);
	4.	Расчёт количества слоёв: int layer_count = (int)(cfg.z_max_mm / cfg.layer_height_mm);
	5.	Для каждого слоя i:
        z = i * cfg.layer_height_mm;
        SliceGeometry geom = slice_mesh_at_z(&mesh, z);
        SliceLayer layer; rasterize_slice(&geom, &cfg, &layer);
        (для первых bottom_layer_count слоёв — одна экспозиция, дальше — другая).
	6.	goo_write_file(cfg.output_goo_path, &cfg, layers, layer_count);
	7.	Освободить всё.
*/

int main(void) {
    FILE *f;
    Mesh m;

    Vec3 A = {0.0f, 0.0f, 0.0f};
    Vec3 B = {1.0f, 0.0f, 0.0f};
    Vec3 C = {1.0f, 1.0f, 0.0f};
    Vec3 D = {0.0f, 1.0f, 0.0f};

    Vec3 E = {0.0f, 0.0f, 1.0f};
    Vec3 F = {1.0f, 0.0f, 1.0f};
    Vec3 G = {1.0f, 1.0f, 1.0f};
    Vec3 H = {0.0f, 1.0f, 1.0f};

    Vec3 P = {0.5f, 0.5f, 2.0f};

    m.nb_triangles = 14;
    m.triangles = malloc(sizeof(Triangle) * m.nb_triangles);

    m.triangles[0] = (Triangle){normalize_vec3(get_norm(A, C, B)), A, C, B};
    m.triangles[1] = (Triangle){normalize_vec3(get_norm(A, D, C)), A, D, C};

    m.triangles[2] = (Triangle){normalize_vec3(get_norm(A, B, F)), A, B, F};
    m.triangles[3] = (Triangle){normalize_vec3(get_norm(A, F, E)), A, F, E};

    m.triangles[4] = (Triangle){normalize_vec3(get_norm(B, C, G)), B, C, G};
    m.triangles[5] = (Triangle){normalize_vec3(get_norm(B, G, F)), B, G, F};

    m.triangles[6] = (Triangle){normalize_vec3(get_norm(C, D, H)), C, D, H};
    m.triangles[7] = (Triangle){normalize_vec3(get_norm(C, H, G)), C, H, G};

    m.triangles[8] = (Triangle){normalize_vec3(get_norm(D, A, E)), D, A, E};
    m.triangles[9] = (Triangle){normalize_vec3(get_norm(D, E, H)), D, E, H};

    m.triangles[10] = (Triangle){normalize_vec3(get_norm(E, F, P)), E, F, P};
    m.triangles[11] = (Triangle){normalize_vec3(get_norm(F, G, P)), F, G, P};
    m.triangles[12] = (Triangle){normalize_vec3(get_norm(G, H, P)), G, H, P};
    m.triangles[13] = (Triangle){normalize_vec3(get_norm(H, E, P)), H, E, P};

    // ASCII .STL
    f = fopen("house_ascii.stl", "w");
    write_mesh_ascii(f, &m);
    fclose(f);

    // Binary .STL
    f = fopen("house_binary.stl", "wb");
    write_mesh_binary(f, &m);
    fclose(f);
    
    free(m.triangles);

    return 0;
}