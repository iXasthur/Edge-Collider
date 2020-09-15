#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif

#include <iostream>
#include <windows.h>
#include <gdiplus.h>
#include "ColorFlow/ColorFlow.h"
#include "MovingObject/RainbowRect.h"
#include "MovingObject/SpriteNode.h"


const unsigned int FPS = 60;
const unsigned int MOVEMENT_TIMER_ID = 1;
const unsigned int MOVEMENT_UPDATE_DELAY = 1000/FPS;
const POINTFLOAT MOVEMENT_PER_FRAME = POINTFLOAT {200.0f/FPS, 100.0f/FPS};

const SIZE MIN_WINDOW_SIZE = SIZE {200, 150};
const SIZE FIRST_WINDOW_SIZE = SIZE {400, 300};
const COLORREF BACKGROUND_COLOR = RGB(26, 26, 26);

const SIZE RAINBOW_RECT_DEFAULT_SIZE = SIZE {40, 40};
const POINTFLOAT RAINBOW_RECT_DEFAULT_POSITION = POINTFLOAT {25.0f, 25.0f};
const COLORREF RAINBOW_RECT_DEFAULT_COLOR = RGB(241, 196, 15);

const wchar_t SPRITE_IMAGE_PATH[] = L"D:\\Developer\\Edge-Collider\\apple.png";


static bool runRainbowRectAnimation = true;
static RainbowRect rainbowRect = RainbowRect();

static bool runSpriteNodeAnimation = true;
static SpriteNode spriteNode = SpriteNode();

static MovingObject *selectedObject = nullptr;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


void initMovingObjects() {
    rainbowRect = RainbowRect(ColorFlow(RAINBOW_RECT_DEFAULT_COLOR), RAINBOW_RECT_DEFAULT_POSITION, RAINBOW_RECT_DEFAULT_SIZE);
    rainbowRect.colorFlow.speed = 2;

    selectedObject = &rainbowRect;

    Gdiplus::Image image = Gdiplus::Image(SPRITE_IMAGE_PATH);
    spriteNode = SpriteNode(RAINBOW_RECT_DEFAULT_POSITION, RAINBOW_RECT_DEFAULT_SIZE, image.Clone());
    if (image.GetLastStatus() != Gdiplus::Ok) {
        MessageBoxW(nullptr, L"Error opening image", L"Attention", MB_OK);
        std::exit(0);
    }

    spriteNode.isHidden = true;
    runSpriteNodeAnimation = false;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    initMovingObjects();

    const wchar_t WINDOW_CLASS[] = L"MAIN_WINDOW_CLASS";
    const wchar_t WINDOW_TITLE[] = L"Edge collider ^_^";

    WNDCLASSEXW wc;
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(BACKGROUND_COLOR);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = WINDOW_CLASS;
    wc.hIconSm = LoadIconW(nullptr, IDI_APPLICATION);

    RegisterClassExW(&wc);

    HWND hwnd = CreateWindowExW(
            0x0,  // Optional window styles.
            WINDOW_CLASS,   // Window class
            WINDOW_TITLE,   // Window text
            WS_OVERLAPPEDWINDOW,   // Window style
            // Size and position
            CW_USEDEFAULT, CW_USEDEFAULT, FIRST_WINDOW_SIZE.cx, FIRST_WINDOW_SIZE.cy,
            nullptr,   // Parent window
            nullptr,   // Menu
            hInstance,  // Instance handle
            nullptr   // Additional application data
    );

    if (hwnd == nullptr) {
        MessageBoxW(nullptr, L"Error creating window", L"Attention", MB_OK);
        return 0;
    }

    SetTimer(hwnd, MOVEMENT_TIMER_ID, MOVEMENT_UPDATE_DELAY, nullptr);
    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}

void fixMovingObjectBorderPosition(HWND hwnd, MovingObject *obj) {
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    if (obj->position.x + obj->size.cx > clientRect.right) {
        obj->position.x = clientRect.right - obj->size.cx;
        if (obj->directionModifier.x > 0.0f) {
            obj->directionModifier.x = -1.0f;
        }
    } else if (obj->position.x < clientRect.left) {
        obj->position.x = clientRect.left;
        if (obj->directionModifier.x < 0.0f) {
            obj->directionModifier.x = 1.0f;
        }
    }
    if (obj->position.y + obj->size.cy > clientRect.bottom) {
        obj->position.y = clientRect.bottom - obj->size.cy;
        if (obj->directionModifier.y > 0.0f) {
            obj->directionModifier.y = -1.0f;
        }
    } else if (obj->position.y < clientRect.top) {
        obj->position.y = clientRect.top;
        if (obj->directionModifier.y < 0.0f) {
            obj->directionModifier.y = 1.0f;
        }
    }
}

void updateMovingObject(HWND hwnd, MovingObject *obj) {
    obj->position.x = obj->position.x + MOVEMENT_PER_FRAME.x * obj->directionModifier.x;
    obj->position.y = obj->position.y + MOVEMENT_PER_FRAME.y * obj->directionModifier.y;

    fixMovingObjectBorderPosition(hwnd, obj);
}

void drawBackground(HDC hdc, PAINTSTRUCT ps) {
    HBRUSH backgroundBrush = CreateSolidBrush(BACKGROUND_COLOR);
    FillRect(hdc, &ps.rcPaint, backgroundBrush);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            RECT rcClientRect;
            GetClientRect(hwnd, &rcClientRect);

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HDC memDC = CreateCompatibleDC(hdc);

            SelectObject(memDC, GetStockObject(DC_PEN));
            SelectObject(memDC, GetStockObject(DC_BRUSH));

            HBITMAP bmp = CreateCompatibleBitmap(hdc, rcClientRect.right - rcClientRect.left,
                                                 rcClientRect.bottom - rcClientRect.top);
            HBITMAP oldBmp = (HBITMAP) SelectObject(memDC, bmp);

            drawBackground(memDC, ps);
            if (!rainbowRect.isHidden) {
                rainbowRect.draw(memDC);
            }
            if (!spriteNode.isHidden) {
                spriteNode.draw(memDC);
            }

            BitBlt(hdc, 0, 0, rcClientRect.right - rcClientRect.left, rcClientRect.bottom - rcClientRect.top, memDC, 0,
                   0, SRCCOPY);

            SelectObject(memDC, oldBmp);
            DeleteObject(bmp);
            DeleteDC(memDC);

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_TIMER: {
            if (runRainbowRectAnimation) {
                rainbowRect.colorFlow.getNextColor(); // Generates new color
                updateMovingObject(hwnd, &rainbowRect);
            }
            if (runSpriteNodeAnimation) {
                updateMovingObject(hwnd, &spriteNode);
            }
            InvalidateRect(hwnd, nullptr, false);
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        case WM_KEYDOWN: {
            switch (wParam) {
                case VK_F1: {
                    spriteNode.isHidden = true;
                    runSpriteNodeAnimation = false;

                    rainbowRect.isHidden = false;
                    runRainbowRectAnimation = true;

                    selectedObject = &rainbowRect;
                    selectedObject->position = RAINBOW_RECT_DEFAULT_POSITION;
                    selectedObject->directionModifier = POINTFLOAT {1.0f, 1.0f};
                    break;
                }
                case 0x31: { // 1 key
                    if (selectedObject == &rainbowRect) {
                        spriteNode.isHidden = true;
                        runSpriteNodeAnimation = false;

                        selectedObject = &rainbowRect;
                        if (!rainbowRect.isHidden && runRainbowRectAnimation) {
                            runRainbowRectAnimation = false;
                        } else {
                            rainbowRect.isHidden = false;
                            runRainbowRectAnimation = true;
                        }
                    }
                    break;
                }
                case VK_F2: {
                    rainbowRect.isHidden = true;
                    runRainbowRectAnimation = false;

                    spriteNode.isHidden = false;
                    runSpriteNodeAnimation = true;

                    selectedObject = &spriteNode;
                    selectedObject->position = RAINBOW_RECT_DEFAULT_POSITION;
                    selectedObject->directionModifier = POINTFLOAT {1.0f, 1.0f};
                    break;
                }
                case 0x32: { // 2 key
                    if (selectedObject == &spriteNode) {
                        rainbowRect.isHidden = true;
                        runRainbowRectAnimation = false;

                        selectedObject = &spriteNode;
                        if (!spriteNode.isHidden && runSpriteNodeAnimation) {
                            runSpriteNodeAnimation = false;
                        } else {
                            spriteNode.isHidden = false;
                            runSpriteNodeAnimation = true;
                        }
                    }
                    break;
                }
                case VK_LEFT: {
                    runRainbowRectAnimation = false;
                    runSpriteNodeAnimation = false;
                    selectedObject->position.x -= selectedObject->size.cx / 2.0f;
                    fixMovingObjectBorderPosition(hwnd, selectedObject);
                    break;
                }
                case VK_RIGHT: {
                    runRainbowRectAnimation = false;
                    runSpriteNodeAnimation = false;
                    selectedObject->position.x += selectedObject->size.cx / 2.0f;
                    fixMovingObjectBorderPosition(hwnd, selectedObject);
                    break;
                }
                case VK_UP: {
                    runRainbowRectAnimation = false;
                    runSpriteNodeAnimation = false;
                    selectedObject->position.y -= selectedObject->size.cy / 2.0f;
                    fixMovingObjectBorderPosition(hwnd, selectedObject);
                    break;
                }
                case VK_DOWN: {
                    runRainbowRectAnimation = false;
                    runSpriteNodeAnimation = false;
                    selectedObject->position.y += selectedObject->size.cy / 2.0f;
                    fixMovingObjectBorderPosition(hwnd, selectedObject);
                    break;
                }
            }
            break;
        }
        case WM_MOUSEWHEEL: {
            runRainbowRectAnimation = false;
            runSpriteNodeAnimation = false;

            int fwKeys = GET_KEYSTATE_WPARAM(wParam);
            int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

            if (fwKeys == MK_SHIFT) {
                if (zDelta > 0) {
                    selectedObject->position.x += selectedObject->size.cx / 4.0f;
                } else {
                    selectedObject->position.x -= selectedObject->size.cx / 4.0f;
                }
            } else {
                if (zDelta > 0) {
                    selectedObject->position.y -= selectedObject->size.cy / 4.0f;
                } else {
                    selectedObject->position.y += selectedObject->size.cy / 4.0f;
                }
            }

            fixMovingObjectBorderPosition(hwnd, selectedObject);
            break;
        }
        case WM_GETMINMAXINFO: {
            LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
            lpMMI->ptMinTrackSize.x = MIN_WINDOW_SIZE.cx;
            lpMMI->ptMinTrackSize.y = MIN_WINDOW_SIZE.cy;
            break;
        }
        default: {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return 0;
}