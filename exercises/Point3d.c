#include <stdio.h>
#include "Point3d.h"

Point3d *Point3d_Allocate(int32_t first, int32_t second, int32_t third)
{
    Point3d *point = (Point3d *)malloc(sizeof(Point3d));
    point->x = first;
    point->y = second;
    point->z = third;
    return point;
}

void Point3d_Scale(Point3d *ptr, int32_t n)
{
    (*ptr).x = n;
    (*ptr).y = n;
    (*ptr).z = n;
}

Point3d *Point3d_GetOrigin()
{
    Point3d *point = (Point3d *)malloc(sizeof(Point3d));
    Point3d p = {0, 0, 0};
    point = &p;
    return point;
}