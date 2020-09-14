//
// Created by iXasthur on 14.09.2020.
//

#ifndef EDGE_COLLIDER_RAINBOWRECT_H
#define EDGE_COLLIDER_RAINBOWRECT_H

#include <windows.h>
#include "../ColorFlow/ColorFlow.h"

class RainbowRect {
public:
    POINTFLOAT position = POINTFLOAT {0, 0};
    POINTFLOAT directionModifier = POINTFLOAT {1.0f, 1.0f};

    ColorFlow colorFlow = ColorFlow();

    RainbowRect() = default;

    RainbowRect(ColorFlow colorFlow, POINTFLOAT position) noexcept {
        this->position = position;
        this->colorFlow = colorFlow;
    }
};


#endif //EDGE_COLLIDER_RAINBOWRECT_H
