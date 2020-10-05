//
// Created by iXasthur on 14.09.2020.
//

#ifndef EDGE_COLLIDER_RAINBOWRECT_H
#define EDGE_COLLIDER_RAINBOWRECT_H

#include "MovingObject.h"
#include "../ColorFlow/ColorFlow.h"

class RainbowRect : public MovingObject {
public:
    ColorFlow colorFlow = ColorFlow();

    RainbowRect() = default;

    RainbowRect(ColorFlow colorFlow, POINTFLOAT position, SIZE size) noexcept {
        this->position = position;
        this->size = size;
        this->colorFlow = colorFlow;
    }

    void draw(HDC hdc) override {
        COLORREF initialDCPenColor = GetDCPenColor(hdc);
        COLORREF initialDCBrushColor = GetDCBrushColor(hdc);

        COLORREF rectColor = colorFlow.getCurrentColor();
        SetDCPenColor(hdc, rectColor);
        SetDCBrushColor(hdc, rectColor);

        RECT movingRect;
        movingRect.left = position.x;
        movingRect.top = position.y;
        movingRect.right = size.cx + position.x;
        movingRect.bottom = size.cy + position.y;

        Rectangle(hdc, movingRect.left, movingRect.top, movingRect.right, movingRect.bottom);

        SetDCPenColor(hdc, initialDCPenColor);
        SetDCBrushColor(hdc, initialDCBrushColor);
    }
};


#endif //EDGE_COLLIDER_RAINBOWRECT_H
