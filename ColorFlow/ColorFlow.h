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

    void generateNextColor() {
        r += rModifier;
        if (r >= 255) {
            r = 255;
            rModifier = -1;
        } else if (r <= 0) {
            r = 0;
            rModifier = 1;
        }

        g += gModifier;
        if (g >= 255) {
            g = 255;
            gModifier = -1;
        } else if (g <= 0) {
            g = 0;
            gModifier = 1;
        }

        b += bModifier;
        if (b >= 255) {
            b = 255;
            bModifier = -1;
        } else if (b <= 0) {
            b = 0;
            bModifier = 1;
        }
    }

public:
    int r;
    int g;
    int b;

    explicit ColorFlow(COLORREF colorRef) {
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
