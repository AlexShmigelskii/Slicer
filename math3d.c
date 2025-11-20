#include "math3d.h"
#include <math.h>

Vec3 substract_vec3(Vec3 V1, Vec3 V2) {
    Vec3 V = { V1.x - V2.x, V1.y - V2.y, V1.z - V2.z,};
    return V;
}

Vec3 cross_vec3(Vec3 V1, Vec3 V2) {
    Vec3 V = {
        V1.y * V2.z - V1.z * V2.y,
        V1.z * V2.x - V1.x * V2.z,
        V1.x * V2.y - V1.y * V2.x
    };
    return V;
}

Vec3 get_norm(Vec3 V1, Vec3 V2, Vec3 V3) {
    Vec3 A = substract_vec3(V2, V1);
    Vec3 B = substract_vec3(V3, V1);
    Vec3 N = cross_vec3(A, B);
    return N;
}

Vec3 normalize_vec3(Vec3 V) {
    float L = sqrtf(V.x * V.x + V.y * V.y + V.z * V.z);
    return (Vec3){V.x / L, V.y / L, V.z / L};
}