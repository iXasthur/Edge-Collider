//
// Created by iXasthur on 15.09.2020.
//

#ifndef EDGE_COLLIDER_SPRITENODE_H
#define EDGE_COLLIDER_SPRITENODE_H

#include <windows.h>
#include <gdiplus.h>
#include "MovingObject.h"

class SpriteNode : public MovingObject {
public:
    Gdiplus::Image *image = nullptr;

    SpriteNode() = default;

    SpriteNode(POINTFLOAT position, SIZE size, Gdiplus::Image *image) noexcept {
        this->position = position;
        this->size = size;
        this->image = image;
    }

    void draw(HDC hdc) override {
        Gdiplus::Graphics(hdc).DrawImage(image, Gdiplus::Rect(position.x, position.y, size.cx, size.cy));
    }
};


#endif //EDGE_COLLIDER_SPRITENODE_H
