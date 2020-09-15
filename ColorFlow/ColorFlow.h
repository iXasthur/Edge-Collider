//
// Created by iXasthur on 14.09.2020.
//

#ifndef EDGE_COLLIDER_COLORFLOW_H
#define EDGE_COLLIDER_COLORFLOW_H

#include <windows.h>

class ColorFlow {
private:
    int rModifier = 1;
    int gModifier = 1;
    int bModifier = 1;

    void generateNextColor();

public:
    int r = 255;
    int g = 255;
    int b = 255;

    int speed = 1;

    ColorFlow() = default;

    explicit ColorFlow(COLORREF colorRef) noexcept {
        r = GetRValue(colorRef);
        g = GetGValue(colorRef);
        b = GetBValue(colorRef);
    }

    COLORREF getCurrentColor() {
        return RGB(r, g, b);
    }

    COLORREF getNextColor() {
        generateNextColor();
        return RGB(r, g, b);
    }
};


#endif //EDGE_COLLIDER_COLORFLOW_H
