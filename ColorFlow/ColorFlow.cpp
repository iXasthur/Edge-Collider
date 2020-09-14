//
// Created by iXasthur on 14.09.2020.
//

#include "ColorFlow.h"

void ColorFlow::generateNextColor() {
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