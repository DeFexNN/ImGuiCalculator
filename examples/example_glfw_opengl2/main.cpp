#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <thread>
#include <atomic>
#include <random>
#include <chrono>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>
#include <imgui_internal.h>
using namespace std;


std::atomic<int> random_number(0);
std::atomic<bool> is_running(true);

void StartRandomNumberGenerator()
{
    std::thread([]() {
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(1000000000, 99999999999);

        while (is_running)
        {
            random_number = dist(rng);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        }).detach();
}


#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


bool IsFirstNum = true;
bool IsSecondNum = false;
int NumCount = 1;
double result = 0;
char num[12]{ 'g' };
double firstnum = 0, secondnum = 0;
int mode = 0;
void freenum(char* num, int* numcount);


void Equal() {
    if (secondnum == 0) {
        secondnum = atof(num);
        freenum(num, &NumCount);
        return;
    }
    switch (mode) {
    case 1://+
        result = firstnum + secondnum;
        freenum(num, &NumCount);
        break;

    case 2://-
        result = firstnum - secondnum;
        freenum(num, &NumCount);
        break;
    case 3://*
        result = firstnum * secondnum;
        freenum(num, &NumCount);
        break;
    case 4:// /
        result = firstnum / secondnum;
        freenum(num, &NumCount);
        break;
    case 5://^
        result = pow(firstnum, secondnum);
        freenum(num, &NumCount);
        break;
    case 6://sqrt
        result = sqrt(firstnum);
        freenum(num, &NumCount);
        break;
    case 7://1/x
        break;
    case 8://CE
        break;
    case 9://C
        break;
    case 10://back

        break;
    case 11://negate
        break;
    case 12://%
        result = (firstnum * secondnum) / 100;
        break;
    default:
        result = 1000 - 7;
        break;
    }
    freenum(num, &NumCount);
    firstnum = result;
    secondnum = 0;
    mode = 0;
    result = firstnum;
}








// Main code
int main(int, char**)
{
    for (int i = 0; i < 10; i++) {
        num[i] = ' ';
        if (int i = 9)num[i + 1] = ' ';
    }
    double number = 0;
    double x = 0, y = 0;
    string testik = "hello world";
    bool IsCalc = false;
    float rightPadding = 8.0f;
    bool test = false;
    int prikol = 0;
    StartRandomNumberGenerator();
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Dear ImGui GLFW+OpenGL2 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // Enable vsync
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(5);
            continue;
        }
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Calculator");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 500.0f / io.Framerate, io.Framerate);
            ImGui::Text("This is my first calculator for ImGui :D");
            ImGui::SetNextItemWidth(70);
            ImGui::InputDouble("Enter first num", &x);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(70);
            ImGui::InputDouble("Enter second num", &y);
            if (ImGui::Button("Start Calc", ImVec2(200, 50))) {
                testik = "Calc Started";
                IsCalc = true;
            }
            
            ImGui::Text("%d", prikol);
            ImGui::Text("%s", testik.c_str());
            ImGui::Text("x: %lf", x);
            ImGui::SameLine;
            ImGui::Text("y: %lf", y);
            if (ImGui::Button("Exit")) {
                std::exit(1);
            }
            ImGui::End();
        }


        if (IsCalc) {
            if (IsFirstNum) {
                firstnum = atof(num);
            }
            else
            {
                secondnum = atof(num);
            }
            ImGui::Begin("Calculator Window");
            ImVec2 size = ImGui::GetWindowSize();
            float width = size.x-16;
            float height = size.y;
            float widthh = ImGui::GetWindowWidth();
            float heightt = 120.0f; // Висота прямокутника
            // Лівий вертикальний сепаратор
            if (NumCount >= 10) {NumCount--; num[NumCount] = '0';}
            if (NumCount <= 0)NumCount++;
            if (IsFirstNum) {
                number = atof(num);
            }
            else number = firstnum;
            ImGui::Separator();
            ImGui::SameLine(100.0f);
            ImGui::SetCursorPosY(32.0f);
            ImGui::InputDouble("## ", &number);    
            ImGui::SetCursorPosX(100.0f);
            ImGui::InputDouble(" ", &secondnum);
            ImGui::Separator();
            ImGui::Text("result:%lf mode:%d",result,mode);
            ImGui::Text("xD:%d%d%d%d", random_number.load(),random_number.load(), random_number.load(), random_number.load());

            float windowWidth = ImGui::GetWindowWidth();
            float sidePadding = 8.0f;
            float buttonSpacing = 10.0f;
            float buttonWidth = (windowWidth - 2 * sidePadding - 3 * buttonSpacing) / 4.0f;
            ImGui::Text("string: %s", num);
            ImGui::Text("windowWidth: %.2f", windowWidth);
            if (ImGui::Button("%", ImVec2(buttonWidth, height / 9))) {
                mode = 12;
                Equal();
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("CE", ImVec2(buttonWidth, height / 9))) {
                result = 0;
                secondnum = 0;
                freenum(num, &NumCount);
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("C", ImVec2(buttonWidth, height / 9))) {
                result = 0;
                firstnum = secondnum = 0;
                freenum(num, &NumCount);
                mode = '\0';
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("back", ImVec2(buttonWidth, height / 9))) {
                if (NumCount == 10) {
                    num[10] = 'n';
                    NumCount--;
                }
                else {
                    if (NumCount >= 0 && NumCount <= 10) {

                        if (num[NumCount - 1] == '.')NumCount--;
                        num[NumCount - 1] = 'n';
                        NumCount--;
                    }
                }
            }
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);
            if (ImGui::Button("1/x", ImVec2(buttonWidth, height / 9))) {
                result = 1 / firstnum;
                firstnum = result;
                freenum(num, &NumCount);
                Equal();
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("x^y", ImVec2(buttonWidth, height / 9))) {
                mode = 5;
                Equal();
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("sqrt(x)", ImVec2(buttonWidth, height / 9))) {
                mode = 6;
                Equal();
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("/", ImVec2(buttonWidth, height / 9))) {
                mode = 4;
                Equal();
            }
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);
            if (ImGui::Button("7", ImVec2(buttonWidth, height/9))) {
                num[NumCount] = '7';
                NumCount++;
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("8", ImVec2(buttonWidth, height / 9))) {
                num[NumCount] = '8';
                NumCount++;
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("9", ImVec2(buttonWidth, height / 9))) {
                num[NumCount] = '9';
                NumCount++;
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("*", ImVec2(buttonWidth, height / 9))) {
                mode = 3;
                Equal();
            }
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);
            if (ImGui::Button("4", ImVec2(buttonWidth, height / 9))) {
                 num[NumCount] = '4';
                 NumCount++;
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("5", ImVec2(buttonWidth, height / 9))) {
                num[NumCount] = '5';
                NumCount++;
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("6", ImVec2(buttonWidth, height / 9))) {
                num[NumCount] = '6';
                NumCount++;
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("-", ImVec2(buttonWidth, height / 9))) {
                mode = 2;
                Equal();
            }
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);
            if (ImGui::Button("1", ImVec2(buttonWidth, height / 9))) {
                num[NumCount] = '1';
                NumCount++;
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("2", ImVec2(buttonWidth, height / 9))) {
                num[NumCount] = '2';
                NumCount++;
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("3", ImVec2(buttonWidth, height / 9))) {
                num[NumCount] = '3';
                NumCount++;
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("+", ImVec2(buttonWidth, height / 9))) {
                mode = 1;
                Equal();
            }
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);
            if (ImGui::Button("+/-", ImVec2(buttonWidth, height / 9))) {
                if (secondnum == 0) {
                    firstnum *= -1;
                    if (num[0] != '-')num[0] = '-';
                    else num[0] = ' ';
                }
                else {
                    if (num[0] != '-')num[0] = '-';
                    else num[0] = ' ';
                }

            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("0", ImVec2(buttonWidth, height / 9))) {
                num[NumCount] = '0';
                NumCount++;
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button(".", ImVec2(buttonWidth, height / 9))) {
                num[NumCount] = '.';
                NumCount++;
            }ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("=", ImVec2(buttonWidth, height / 9))) {
                Equal();
            }







            ImGui::End();
        }
        if (test) {
            // Отримуємо розміри вікна
            ImGui::Begin("Window");

            // Отримуємо ширину вікна
            float windowWidth = ImGui::GetWindowWidth();

            // Відступи від лівого та правого краю
            float sidePadding = 8.0f;

            // Відступ між кнопками
            float buttonSpacing = 10.0f;

            // Ширина кнопки
            float buttonWidth = (windowWidth - 2 * sidePadding - 3 * buttonSpacing) / 4.0f;

            // Початковий відступ від лівого краю
            ImGui::SetCursorPosX(sidePadding);

            // Перша кнопка
            if (ImGui::Button("Button 1", ImVec2(buttonWidth, 0))) {
                // Дія для кнопки 1
            }

            // Друга кнопка
            ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("Button 2", ImVec2(buttonWidth, 0))) {
                // Дія для кнопки 2
            }

            // Третя кнопка
            ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("Button 3", ImVec2(buttonWidth, 0))) {
                // Дія для кнопки 3
            }

            // Четверта кнопка
            ImGui::SameLine(0, buttonSpacing);
            if (ImGui::Button("Button 4", ImVec2(buttonWidth, 0))) {
                // Дія для кнопки 4
            }

            ImGui::End();
        }
        

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        // If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
        // you may need to backup/reset/restore other state, e.g. for current shader using the commented lines below.
        //GLint last_program;
        //glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
        //glUseProgram(0);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        //glUseProgram(last_program);

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
void freenum(char* num, int* numcount) {
    for (int i = 0; i < 10; i++) {
        num[i] = ' ';
        if (int i = 9)num[i + 1] = ' ';
    }
    *numcount = 0;

    if (IsFirstNum) {
        IsFirstNum = false;
        IsSecondNum = true;
    }
}
