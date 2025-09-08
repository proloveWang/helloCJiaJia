// HelloCJiaJia.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

// 配置参数 - 开发者可以调整这些值
const int TOTAL_ITERATIONS = 1;      // 总执行次数
const int INTERVAL_SECONDS = 5;      // 执行间隔（秒）
const int ANIMATION_STEPS = 100;     // 动画步数（值越大移动越平滑）
const int ANIMATION_DURATION = 1000; // 动画持续时间（毫秒）
const bool CLOSE_WINDOW = true;      // 是否点击关闭窗口
const int CLICK_DELAY = 500;         // 移动到位置后到点击的延迟（毫秒）

// 获取活动窗口的关闭按钮位置
POINT GetCloseButtonPosition() {
    HWND hwnd = GetForegroundWindow(); // 获取当前活动窗口
    if (!hwnd) {
        std::cout << "无法获取活动窗口" << std::endl;
        return POINT{ 0, 0 };
    }

    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);

    // 假设关闭按钮在窗口右上角，距离右边约15像素，上边约8像素
    POINT closeButtonPos;
    closeButtonPos.x = windowRect.right - 15;
    closeButtonPos.y = windowRect.top + 8;

    return closeButtonPos;
}

// 获取当前鼠标位置
POINT GetCurrentMousePosition() {
    POINT currentPos;
    GetCursorPos(&currentPos);
    return currentPos;
}

// 模拟鼠标点击
void SimulateMouseClick() {
    // 按下左键
    INPUT inputDown = { 0 };
    inputDown.type = INPUT_MOUSE;
    inputDown.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &inputDown, sizeof(INPUT));

    // 短暂延迟模拟真实点击
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // 释放左键
    INPUT inputUp = { 0 };
    inputUp.type = INPUT_MOUSE;
    inputUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &inputUp, sizeof(INPUT));
}

// 平滑移动鼠标到目标位置
void SmoothMoveMouse(POINT startPos, POINT targetPos) {
    // 计算每一步的移动距离
    double dx = (targetPos.x - startPos.x) / static_cast<double>(ANIMATION_STEPS);
    double dy = (targetPos.y - startPos.y) / static_cast<double>(ANIMATION_STEPS);

    // 计算每一步的等待时间
    int stepDelay = ANIMATION_DURATION / ANIMATION_STEPS;

    // 执行平滑移动
    for (int i = 0; i <= ANIMATION_STEPS; i++) {
        // 计算当前步的位置（使用缓动函数使移动更自然）
        double progress = static_cast<double>(i) / ANIMATION_STEPS;
        // 使用二次缓动函数：easeOutQuad
        progress = 1 - (1 - progress) * (1 - progress);

        int currentX = startPos.x + static_cast<int>((targetPos.x - startPos.x) * progress);
        int currentY = startPos.y + static_cast<int>((targetPos.y - startPos.y) * progress);

        // 移动鼠标
        SetCursorPos(currentX, currentY);

        // 等待一段时间
        std::this_thread::sleep_for(std::chrono::milliseconds(stepDelay));
    }

    // 确保最终位置准确
    SetCursorPos(targetPos.x, targetPos.y);
}

int main() {
    std::cout << "鼠标移动程序配置：" << std::endl;
    std::cout << "执行次数: " << TOTAL_ITERATIONS << std::endl;
    std::cout << "执行间隔: " << INTERVAL_SECONDS << " 秒" << std::endl;
    std::cout << "动画步数: " << ANIMATION_STEPS << std::endl;
    std::cout << "动画时长: " << ANIMATION_DURATION << " 毫秒" << std::endl;
    std::cout << "关闭窗口: " << (CLOSE_WINDOW ? "是" : "否") << std::endl;
    std::cout << "点击延迟: " << CLICK_DELAY << " 毫秒" << std::endl;
    std::cout << "程序将在5秒后开始..." << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(5));

    for (int i = 0; i < TOTAL_ITERATIONS; i++) {
        std::cout << "\n第 " << i + 1 << " 次执行..." << std::endl;

        // 获取起始位置和目标位置
        POINT startPos = GetCurrentMousePosition();
        POINT targetPos = GetCloseButtonPosition();

        std::cout << "从 (" << startPos.x << ", " << startPos.y
            << ") 移动到 (" << targetPos.x << ", " << targetPos.y << ")" << std::endl;

        // 平滑移动鼠标
        SmoothMoveMouse(startPos, targetPos);

        // 如果需要关闭窗口
        if (CLOSE_WINDOW) {
            std::cout << "等待 " << CLICK_DELAY << " 毫秒后点击关闭窗口..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(CLICK_DELAY));

            // 模拟鼠标点击
            SimulateMouseClick();
            std::cout << "已尝试关闭窗口" << std::endl;
        }

        // 如果不是最后一次，等待指定间隔
        if (i < TOTAL_ITERATIONS - 1) {
            std::cout << "等待 " << INTERVAL_SECONDS << " 秒后进行下一次移动..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(INTERVAL_SECONDS));
        }
    }

    std::cout << "\n操作完成，程序结束" << std::endl;
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
