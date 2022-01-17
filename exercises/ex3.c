/*
 * Copyright ©2022 Phuoc Huynh & Isabella Bunnell.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2022 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdio.h>  // for printf
#include <stdlib.h> // for EXIT_SUCCESS
#include <assert.h>
#include "Point3d.h"

int main(int argc, char *argv[])
{
    Point3d p = *Point3d_Allocate(1, 2, 3);
    assert(p.x == 1);
    Point3d_Scale(&p, 5);
    assert(p.y == 5);
    free(&p);
    Point3d q = *(Point3d_GetOrigin());
    assert(q.z == 0);
    free(&q);
    return EXIT_SUCCESS;
}
