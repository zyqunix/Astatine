#include <windows.h>
#include <cmath>
#include <mmsystem.h>
#include <thread>
#include <math.h>

#define M_PI 3.14159265358979323846
#pragma comment(lib, "winmm.lib")

BOOL WINAPI SetProcessDPIAware(void);

DWORD xs;
VOID SeedXorshift32(DWORD dwSeed) {
    xs = dwSeed;
}

DWORD Xorshift32() {
    xs ^= xs << 13;
    xs ^= xs << 17;
    xs ^= xs << 5;
    return xs;
}

typedef union _RGBQUAD {
    COLORREF rgb;
    struct {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE unused;
    };
} *PRGBQUAD;

int red = 0, green = 0, blue = 0;
bool ifcolorblue = false;

COLORREF Hue(int length) {
    if (red != length) {
        red++;
        if (ifcolorblue) {
            return RGB(red, 0, length);
        }
        else {
            return RGB(red, 0, 0);
        }
    }
    else {
        if (green != length) {
            green++;
            return RGB(length, green, 0);
        }
        else {
            if (blue != length) {
                blue++;
                return RGB(0, length, blue);
            }
            else {
                red = 0;
                green = 0;
                blue = 0;
                ifcolorblue = true;
            }
        }
    }
    return RGB(0, 0, 0);
}

DWORD WINAPI bounce(LPVOID lpParam) {
    int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
    int signX = 1, signY = 1;
    int x = 10, y = 10;
    int increment = 10;

    while (1) {
        HDC hdc = GetDC(NULL);
        if (!hdc) break;

        HBRUSH brush = CreateSolidBrush(Hue(rand() % 256));
        if (brush) {
            SelectObject(hdc, brush);
            Ellipse(hdc, x, y, x + 100, y + 100);
        }

        x += increment * signX;
        y += increment * signY;

        if (x + 100 >= w || x <= 0) signX *= -1;
        if (y + 100 >= h || y <= 0) signY *= -1;

        Sleep(10);

        if (brush) DeleteObject(brush);
        ReleaseDC(NULL, hdc);
    }

    return 0;
}

DWORD WINAPI triangle(LPVOID lpParam) {
    int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
    int signX = 1, signY = 1;
    int x = 10, y = 10;
    int increment = 10;
    float angle = 0.0f;

    while (1) {
        HDC hdc = GetDC(NULL);
        if (!hdc) break;

        POINT center = { x + rand() % 50, y + rand() % 25};
        POINT points[] = { {x + rand() % 20 * 2, y + rand() % 40 * 2}, {x / 2 * rand() % 100 * 2, y + rand() % 100 * 2}, {x + rand() % 80 * 2, y + rand() % 40 * 2} };

        for (int i = 0; i < 3; i++) {
            float newX = cos(angle) * (points[i].x - center.x) - sin(angle) * (points[i].y - center.y) + center.x;
            float newY = sin(angle) * (points[i].x - center.x) + cos(angle) * (points[i].y - center.y) + center.y;
            points[i].x = (LONG)newX;
            points[i].y = (LONG)newY;
        }

        HBRUSH brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        if (brush) {
            SelectObject(hdc, brush);
            Polygon(hdc, points, 3);
        }

        angle += sin(rand() % 3) * (2 / rand() % 256) + cos(M_PI) ;
        if (angle >= 2 * M_PI) angle = 0.0f;

        x += increment * signX;
        y += increment * signY;

        if (x + 100 >= w || x <= 0) signX *= -1;
        if (y + 100 >= h || y <= 0) signY *= -1;

        Sleep(10);

        if (brush) DeleteObject(brush);
        ReleaseDC(NULL, hdc);
    }

    return 0;
}

DWORD WINAPI setpixel(LPVOID lpParam) {
    int y = GetSystemMetrics(SM_CYSCREEN);
    int x = GetSystemMetrics(SM_CXSCREEN);
    int yfull = GetSystemMetrics(SM_CYFULLSCREEN);
    int xfull = GetSystemMetrics(SM_CXFULLSCREEN);
    int rainbow = x - rand() % x - (x / 150 - 112) % 149;
    int inc = round(x / 100);
    round(y / 1);
    round(yfull / 100);
    round(xfull / 10);
    while (1) {
        HDC hdc = GetDC(0);
        for (int yp = 0; yp < y; ++yp) {
            for (int xp = 0; xp < x; ++xp) {
                int xa = inc * xp;
                SetPixel(hdc, xp, yp, RGB(xa, xa, xa));
                SetPixel(hdc, yp, rainbow, RGB(yfull, xa, xfull));
                SetPixel(hdc, rainbow, rainbow, RGB(rainbow, rainbow, rainbow));
                SetPixel(hdc, rainbow, yp, RGB(xfull, yfull, xfull));
            }
        }
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI textout(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int signX = 1;
    int signY = 1;
    int incrementor = 10;
    int x = 10;
    int y = 10;
    LPCWSTR lpText = L"Astatine.exe";

    while (1) {
        HDC hdc = GetDC(0);
        x += incrementor * signX;
        y += incrementor * signY;

        if (x + wcslen(lpText) * 8 >= GetSystemMetrics(SM_CXSCREEN)) {
            signX = -1;
        }

        if (y + 16 >= GetSystemMetrics(SM_CYSCREEN)) {
            signY = -1;
        }

        if (x <= 0) {
            signX = 1;
        }

        if (y <= 0) {
            signY = 1;
        }

        SetTextColor(hdc, RGB(rand() % 256, rand() % 256, rand() % 256));
        SetBkColor(hdc, RGB(rand() % 256, rand() % 256, rand() % 256));
        TextOutW(hdc, x, y, lpText, wcslen(lpText));

        Sleep(10);
        ReleaseDC(0, hdc);
    }
}

VOID WINAPI ci(int x, int y, int w, int h)
{
    HDC hdc = GetDC(0);
    HRGN hrgn = CreateEllipticRgn(x, y, w + x, h + y);
    SelectClipRgn(hdc, hrgn);
    BitBlt(hdc, x, y, w, h, hdc, x, y, NOTSRCCOPY);
    DeleteObject(hrgn);
    ReleaseDC(NULL, hdc);
}

DWORD WINAPI wef(LPVOID lpParam) {
    RECT rect;
    GetWindowRect(GetDesktopWindow(), &rect);
    int w = rect.right - rect.left - 500, h = rect.bottom - rect.top - 500;

    for (int t = 0;; t++)
    {
        const int size = 1000;
        int x = rand() % (w + size) - size / 2, y = rand() % (h + size) - size / 2;

        for (int i = 0; i < size; i += 23)
        {
            ci(x - i / 2, y - i / 2, i, i);
            Sleep(10);
        }
    }
}

DWORD WINAPI pacmans(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        HBRUSH brush = CreateSolidBrush(Hue(239));
        SelectObject(hdc, brush);
        Pie(hdc, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h, rand() % w, rand() % h);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI sines(LPVOID lpParam) {
    HDC desk = GetDC(0); HWND wnd = GetDesktopWindow();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    double angle = 0;
    while (1) {
        desk = GetDC(0);
        for (float i = 0; i < sw + sh; i += 0.99f) {
            int a = sin(angle) * 10;
            BitBlt(desk, 0, i, sw, 1, desk, a, i, SRCCOPY);
            BitBlt(desk, i, 0, 1, sh, desk, i, a, SRCCOPY);
            angle += M_PI / 80;
            DeleteObject(&i); DeleteObject(&a);
        }
        ReleaseDC(wnd, desk);
        DeleteDC(desk); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&angle);
    }
}

DWORD WINAPI patblt(LPVOID lpParam)
{
    while (1) {
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN),
            h = GetSystemMetrics(SM_CYSCREEN);

        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        PatBlt(hdc, 0, 0, w, h, PATINVERT);
        DeleteObject(brush);
        ReleaseDC(NULL, hdc);
    }
}

DWORD WINAPI shader1(LPVOID lpParam) {
    int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(nullptr, w * h * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!data) return 1;

    while (1) {
        HDC desk = GetDC(NULL);
        if (!desk) break;

        HDC memDC = CreateCompatibleDC(desk);
        if (!memDC) {
            ReleaseDC(NULL, desk);
            break;
        }

        HBITMAP hbm = CreateCompatibleBitmap(desk, w, h);
        if (!hbm) {
            DeleteDC(memDC);
            ReleaseDC(NULL, desk);
            break;
        }

        SelectObject(memDC, hbm);
        BitBlt(memDC, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * sizeof(_RGBQUAD), data);

        for (int i = 0; i < w * h; ++i) {
            int x = i % w, y = i / w, t = y ^ y | x;
            data[i].rgb -= t;
        }

        SetBitmapBits(hbm, w * h * sizeof(_RGBQUAD), data);
        BitBlt(desk, 0, 0, w, h, memDC, 0, 0, SRCCOPY);

        DeleteObject(hbm);
        DeleteDC(memDC);
        ReleaseDC(NULL, desk);
        Sleep(10);
    }

    VirtualFree(data, 0, MEM_RELEASE);
    return 0;
}

DWORD WINAPI shader2(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / h;
            data[i].rgb -= x & y;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI shader3(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    if (w <= 0) w = rand() % 1920;
    if (h <= 0) h = rand() % 1080;

    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; i < w * h; i++) {
            int randPixel = Xorshift32() % w;
            int tempB = GetBValue(data[i].rgb);
            data[i].rgb = RGB(GetBValue(data[randPixel].rgb), GetBValue(data[randPixel].rgb), GetBValue(data[randPixel].rgb));
            data[randPixel].rgb = RGB(tempB, tempB, tempB);
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI shader4(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / h;
            data[i].rgb -= (data[i].rgb & rand()) ^ x & y;
        }
        SetBitmapBits(hbm, (h - w) * h * 6 * -1, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI shader5(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 4) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, rand() % 33 + 5, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; i < w * h + M_PI * 10; i++) {
            int x = i % w, y = i / h;
            data[i].rgb -= (data[i].rgb & rand()) ^ x & y;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI shader6(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, w * h * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!data) return 1;

    while (true) {
        HDC desk = GetDC(NULL);
        HDC memDC = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateCompatibleBitmap(desk, w, h);
        SelectObject(memDC, hbm);
        BitBlt(memDC, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * sizeof(_RGBQUAD), data);

        double angle = GetTickCount() / 1000.0;
        double cosA = cos(angle), sinA = sin(angle);
        int cx = w / 2, cy = h / 2;

        _RGBQUAD* temp = (_RGBQUAD*)VirtualAlloc(0, w * h * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (!temp) break;

        for (int y = 0; y < h; y++) {
            int offset = (int)(10 * sin(y / 15.0 + GetTickCount() / 10.0));
            for (int x = 0; x < w; x++) {
                int srcX = x - offset;
                int dx = srcX - cx;
                int dy = y - cy;
                int rx = (int)(cosA * dx - sinA * dy + cx);
                int ry = (int)(sinA * dx + cosA * dy + cy);

                if (rx >= 0 && rx < w && ry >= 0 && ry < h)
                    temp[y * w + x] = data[ry * w + rx];
                else
                    temp[y * w + x] = { 0 };
            }
        }

        SetBitmapBits(hbm, w * h * sizeof(_RGBQUAD), temp);
        BitBlt(desk, 0, 0, w, h, memDC, 0, 0, SRCCOPY);

        VirtualFree(temp, 0, MEM_RELEASE);
        DeleteObject(hbm);
        DeleteDC(memDC);
        ReleaseDC(NULL, desk);
    }

    VirtualFree(data, 0, MEM_RELEASE);
    return 0;
}

DWORD WINAPI shader7(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, w * h * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!data) return 1;

    while (true) {
        HDC desk = GetDC(NULL);
        HDC memDC = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateCompatibleBitmap(desk, w, h);
        SelectObject(memDC, hbm);
        BitBlt(memDC, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * sizeof(_RGBQUAD), data);

        _RGBQUAD* temp = (_RGBQUAD*)VirtualAlloc(0, w * h * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (!temp) break;

        int cx = w / 2, cy = h / 2;
        double time = GetTickCount() / 200.0;

        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                int dx = x - cx, dy = y - cy;
                double dist = sqrt(dx * dx + dy * dy);
                int offset = (int)(10 * sin(dist / 20.0 - time));
                int srcX = x + (dx * offset / (dist + 1));
                int srcY = y + (dy * offset / (dist + 1));

                if (srcX >= 0 && srcX < w && srcY >= 0 && srcY < h)
                    temp[y * w + x] = data[srcY * w + srcX];
                else
                    temp[y * w + x] = { 0 };
            }
        }

        SetBitmapBits(hbm, w * h * sizeof(_RGBQUAD), temp);
        BitBlt(desk, 0, 0, w, h, memDC, 0, 0, SRCCOPY);

        VirtualFree(temp, 0, MEM_RELEASE);
        DeleteObject(hbm);
        DeleteDC(memDC);
        ReleaseDC(NULL, desk);
        Sleep(10);
    }

    VirtualFree(data, 0, MEM_RELEASE);
    return 0;
}

DWORD WINAPI shader8(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, w * h * 4, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!data) return 1;

    while (true) {
        HDC desk = GetDC(NULL), memDC = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateCompatibleBitmap(desk, w, h);
        SelectObject(memDC, hbm);
        BitBlt(memDC, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);

        double t = GetTickCount() / 300.0;
        for (int i = 0; i < w * h; i++) {
            data[i].r = (BYTE)(data[i].r * fabs(sin(t)));
            data[i].g = (BYTE)(data[i].g * fabs(sin(t + 1)));
            data[i].b = (BYTE)(data[i].b * fabs(sin(t + 2)));
        }

        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, memDC, 0, 0, SRCCOPY);
        DeleteObject(hbm); DeleteDC(memDC); ReleaseDC(NULL, desk);
        Sleep(16);
    }

    VirtualFree(data, 0, MEM_RELEASE);
    return 0;
}

DWORD WINAPI shader9(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, w * h * 4, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!data) return 1;

    while (true) {
        HDC desk = GetDC(NULL), memDC = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateCompatibleBitmap(desk, w, h);
        SelectObject(memDC, hbm);
        BitBlt(memDC, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);

        for (int y = 0; y < h; y++) {
            int offset = (int)(10 * sin(y / 20.0));
            memmove(data + y * w, data + y * w + offset, (w - abs(offset)) * sizeof(_RGBQUAD));
        }

        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, memDC, 0, 0, SRCCOPY);
        DeleteObject(hbm); DeleteDC(memDC); ReleaseDC(NULL, desk);
        Sleep(16);
    }

    VirtualFree(data, 0, MEM_RELEASE);
    return 0;
}

DWORD WINAPI shader10(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, w * h * 4, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!data) return 1;

    while (true) {
        HDC desk = GetDC(NULL), memDC = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateCompatibleBitmap(desk, w, h);
        SelectObject(memDC, hbm);
        BitBlt(memDC, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);

        for (int i = 0; i < w * h; i++) {
            data[i].r = 255 - data[i].r;
            data[i].g = 255 - data[i].g;
            data[i].b = 255 - data[i].b;
        }

        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, memDC, 0, 0, SRCCOPY);
        DeleteObject(hbm); DeleteDC(memDC); ReleaseDC(NULL, desk);
        Sleep(1000);
    }

    VirtualFree(data, 0, MEM_RELEASE);
    return 0;
}

DWORD WINAPI shader11(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, w * h * 4, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!data) return 1;

    while (true) {
        HDC desk = GetDC(NULL), memDC = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateCompatibleBitmap(desk, w, h);
        SelectObject(memDC, hbm);
        BitBlt(memDC, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);

        for (int y = 1; y < h - 1; y++) {
            for (int x = 1; x < w - 1; x++) {
                int i = y * w + x;
                BYTE r = 0, g = 0, b = 0;
                for (int j = -1; j <= 1; j++)
                    for (int k = -1; k <= 1; k++) {
                        int idx = (y + j) * w + (x + k);
                        r += data[idx].r / 9;
                        g += data[idx].g / 9;
                        b += data[idx].b / 9;
                    }
                data[i].r = r; data[i].g = g; data[i].b = b;
            }
        }

        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, memDC, 0, 0, SRCCOPY);
        DeleteObject(hbm); DeleteDC(memDC); ReleaseDC(NULL, desk);
        Sleep(16);
    }

    VirtualFree(data, 0, MEM_RELEASE);
    return 0;
}

DWORD WINAPI shader12(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 8) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / h;
            data[i].rgb = RGB(x % 256, y % 256, (x + y) % 256); ;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI meltX(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1), x;
    while (1) {
        HDC hdc = GetDC(0);
        x = rand() % w;
        BitBlt(hdc, x, 0, 10, h, hdc, x, 1, SRCCOPY);
        ReleaseDC(0, hdc);
    }
}

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t & (t >> 7) - t & t >> 8);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> 5 | (t >> 2) * (t >> 5));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t << 1) ^ ((t << 1) + (t >> 7) & t >> 6)) | t >> (4 - (1 ^ 7 & (t >> 19))) | t >> 7;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(10 * (t & 5 * t | t >> 6 | (t & 32768 ? -6 * t / 7 : (t & 65536 ? -9 * t & 100 : -9 * (t & 100)) / 11)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t & 64 | t >> 5) ^ (t & 33 | t >> 8) ^ (t & 14 | t >> 9 | t & 76) ^ (t | 187) ^ t * (t >> 8 & 838 + t >> 13) & 644;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> 10 | t * 5) & (t >> 8 | t * 4) & (t >> 4 | t * 6);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound8() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 60] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t & 16384 ? 4 : 93) * (19 + (7 & t >> 29)) >> (8 & t >> 2) | t >> 0);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    int w = GetSystemMetrics(SM_CXFULLSCREEN), h = GetSystemMetrics(SM_CYFULLSCREEN);
    SetProcessDPIAware();

    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST,
        L"STATIC",
        L"Fullscreen",
        WS_POPUP,
        0, 0, w, h,
        NULL, NULL, hInstance, NULL
    );

    int r1 = MessageBox(NULL, L"Do you want to run this malware?", L"Astatine.exe", MB_YESNO | MB_ICONWARNING);

    if (r1 == IDNO) {
        exit;
    } else {
        int r2 = MessageBox(NULL, L"Are you REALLY sure?", L"Astatine.exe", MB_YESNO | MB_ICONWARNING);
        if (r2 == IDNO) {
            exit;
        }
        else {
            while (1) {
                HANDLE thread0 = CreateThread(0, 0, bounce, 0, 0, 0);
                sound5();
                Sleep(5000);

                CreateThread(0, 0, textout, 0, 0, 0);
                HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
                sound1();
                Sleep(30000);

                TerminateThread(thread1, 0);
                CloseHandle(thread1);
                InvalidateRect(0, 0, 0);

                HANDLE thread2 = CreateThread(0, 0, pacmans, 0, 0, 0);
                CreateThread(0, 0, shader2, 0, 0, 0);
                sound2();
                Sleep(30000);

                TerminateThread(thread2, 0);
                CloseHandle(thread2);
                InvalidateRect(0, 0, 0);

                HANDLE thread3 = CreateThread(0, 0, shader3, 0, 0, 0);
                CreateThread(0, 0, triangle, 0, 0, 0);
                sound3();
                Sleep(30000);

                TerminateThread(thread3, 0);
                CloseHandle(thread3);
                InvalidateRect(0, 0, 0);

                HANDLE thread4 = CreateThread(0, 0, shader4, 0, 0, 0);
                CreateThread(0, 0, patblt, 0, 0, 0);
                sound4();
                Sleep(30000);

                TerminateThread(thread4, 0);
                CloseHandle(thread4);
                InvalidateRect(0, 0, 0);

                HANDLE thread5 = CreateThread(0, 0, shader5, 0, 0, 0);
                CreateThread(0, 0, meltX, 0, 0, 0);
                sound5();
                Sleep(30000);

                TerminateThread(thread5, 0);
                CloseHandle(thread5);
                InvalidateRect(0, 0, 0);

                HANDLE thread6 = CreateThread(0, 0, shader6, 0, 0, 0);
                CreateThread(0, 0, wef, 0, 0, 0);
                sound7();
                Sleep(30000);

                TerminateThread(thread6, 0);
                CloseHandle(thread6);
                InvalidateRect(0, 0, 0);

                HANDLE thread7 = CreateThread(0, 0, shader7, 0, 0, 0);
                CreateThread(0, 0, setpixel, 0, 0, 0);
                sound8();
                Sleep(30000);

                TerminateThread(thread7, 0);
                CloseHandle(thread7);
                InvalidateRect(0, 0, 0);

                HANDLE thread8 = CreateThread(0, 0, shader8, 0, 0, 0);
                CreateThread(0, 0, shader9, 0, 0, 0);
                sound1();
                Sleep(30000);

                TerminateThread(thread8, 0);
                CloseHandle(thread8);
                InvalidateRect(0, 0, 0);


                HANDLE thread9 = CreateThread(0, 0, shader10, 0, 0, 0);
                CreateThread(0, 0, shader11, 0, 0, 0);
                sound2();
                Sleep(30000);

                TerminateThread(thread9, 0);
                CloseHandle(thread9);
                InvalidateRect(0, 0, 0);

                HANDLE thread10 = CreateThread(0, 0, shader10, 0, 0, 0);
                CreateThread(0, 0, shader11, 0, 0, 0);
                sound2();
                Sleep(30000);

                TerminateThread(thread10, 0);
                CloseHandle(thread10);
                InvalidateRect(0, 0, 0);

                HANDLE thread11 = CreateThread(0, 0, shader12, 0, 0, 0);
                sound3();
                Sleep(30000);

                TerminateThread(thread10, 0);
                CloseHandle(thread10);
                InvalidateRect(0, 0, 0);
            }
        }
    }
}
