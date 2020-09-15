//
// Created by iXasthur on 14.09.2020.
//

#ifndef EDGE_COLLIDER_RAINBOWRECT_H
#define EDGE_COLLIDER_RAINBOWRECT_H

#include "MovingObject.h"
#include "../ColorFlow/ColorFlow.h"

class RainbowRect: public MovingObject {
public:
    ColorFlow colorFlow = ColorFlow();

    RainbowRect() = default;

    RainbowRect(ColorFlow colorFlow, POINTFLOAT position, SIZE size) noexcept {
        this->position = position;
        this->size = size;
        this->colorFlow = colorFlow;
    }
};


#endif //EDGE_COLLIDER_RAINBOWRECT_H
