#ifndef MATH3D_H
#define MATH3D_H

typedef struct {
    float x, y, z;
} Vec3;

// Subtraction of two vectors
Vec3 substract_vect(Vec3 V1, Vec3 V2);

// Cross product or vector produc
Vec3 cross_vec3(Vec3 V1, Vec3 V2);

// Calculates the outward-facing normal of a triangle.
// The input vertices must be specified in counter-clockwise order
// as seen from the exterior side of the surface.  
Vec3 get_norm(Vec3 V1, Vec3 V2, Vec3 V3);

Vec3 normalize_vec3(Vec3 V1);

#endif 