#include <stdlib.h>
#include <stdio.h>

typedef struct point3D_st
{
    int32_t x;
    int32_t y;
    int32_t z;
} Point3d;

Point3d *Point3d_Allocate(int32_t first, int32_t second, int32_t third);

void Point3d_Scale(Point3d *ptr, int32_t n);

Point3d *Point3d_GetOrigin();