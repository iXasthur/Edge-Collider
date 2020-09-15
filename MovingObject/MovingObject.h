//
// Created by iXasthur on 16.09.2020.
//

#ifndef EDGE_COLLIDER_MOVINGOBJECT_H
#define EDGE_COLLIDER_MOVINGOBJECT_H

#include <windows.h>

class MovingObject {
public:
    POINTFLOAT position = POINTFLOAT {0, 0};
    POINTFLOAT directionModifier = POINTFLOAT {1.0f, 1.0f};
    SIZE size = SIZE {0, 0};
    bool isHidden = false;
};


#endif //EDGE_COLLIDER_MOVINGOBJECT_H
