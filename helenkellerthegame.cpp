#include <Windows.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

// Global variables
bool g_bFlashing = false;
bool g_bWhiteBackground = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        // Fill the window with either black or white color
        HBRUSH hBrush;
        if (g_bWhiteBackground) {
            hBrush = CreateSolidBrush(RGB(255, 255, 255)); // White color
        } else {
            hBrush = CreateSolidBrush(RGB(0, 0, 0)); // Black color
        }
        FillRect(hdc, &ps.rcPaint, hBrush);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_SYSKEYDOWN: // Handle system key down messages (like ALT)
        if ((wParam == VK_F4) && (lParam & 0x20000000)) { // Check if ALT is pressed along with F4
            return 0; // Ignore ALT+F4
        }
        break;
    case WM_KEYDOWN:
        if (wParam == 'F') { // Check if F key is pressed
            PostQuitMessage(0); // Close the game
        }
        if (wParam == VK_SPACE) { // Check if spacebar is pressed
            g_bFlashing = true;
            SetTimer(hwnd, 1, 100, NULL); // Start timer to toggle background color every 100 milliseconds
            SetTimer(hwnd, 2, 30000, NULL); // Timer to stop flashing after 30 seconds
        }
        break;
    case WM_TIMER:
        switch (wParam) {
        case 1:
            g_bWhiteBackground = !g_bWhiteBackground; // Toggle background color
            InvalidateRect(hwnd, NULL, TRUE); // Redraw the window to reflect the color change
            break;
        case 2:
            g_bFlashing = false;
            KillTimer(hwnd, 1); // Stop the flashing timer
            KillTimer(hwnd, 2); // Stop the timer to stop flashing after 30 seconds
            break;
        }
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int main() {
    // Register window class
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = TEXT("FullscreenWindowClass");

    RegisterClass(&wc);

    // Get the dimensions of the primary monitor
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Create a fullscreen window
    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST,
        TEXT("FullscreenWindowClass"),
        TEXT("Helen Keller: The Game"),
        WS_POPUP | WS_VISIBLE,
        0,
        0,
        screenWidth,
        screenHeight,
        NULL,
        NULL,
        NULL,
        NULL
    );

    // Hide the cursor
    ShowCursor(FALSE);

    // Main message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}