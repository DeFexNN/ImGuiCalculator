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
#define STB_IMAGE_IMPLEMENTATION
#include "./Cosmetic/stb_image.h"
#define GLEW_STATIC
#include "./GL/glew.h"
#include "./resource.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>
#include <imgui_internal.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wingdi.h>


using namespace std;



////////////////////////////////////OpenGL background////////////////////////////////////
GLuint loadTexture(const char* path) {
    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load image: " << path << std::endl;
        return 0;
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    std::cout << "Texture loaded successfully! ID: " << textureID << ", Size: " << width << "x" << height << ", Channels: " << channels << std::endl;
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error in loadTexture: " << err << std::endl;
    }
    return textureID;
}

void renderBackground(GLuint textureID) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f); // Лівий нижній кут
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);  // Правий нижній кут
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);   // Правий верхній кут
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);  // Лівий верхній кут
    glEnd();
}
////////////////////////////////////OpenGL background////////////////////////////////////


//////////////////////////////////Random Num Generator///////////////////////////////////
std::atomic<int> random_number(0);
std::atomic<bool> is_running(true);
void StartRandomNumberGenerator()
{
    std::thread([]() {
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(100000000, 999999999);

        while (is_running)
        {
            random_number = dist(rng);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        }).detach();
}
//////////////////////////////////Random Num Generator///////////////////////////////////




#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

//////////////////////////////////Global Variables///////////////////////////////////
bool IsFirstNum = true;
bool IsSecondNum = false;
bool SetCenterPos = true;
int NumCount = 1;
double result = 0;
char num[12]{ 'g' };
double firstnum = 0, secondnum = 0;
int mode = 0;
string status="All Works";
//////////////////////////////////Global Variables///////////////////////////////////
//Function Prototypes//
void freenum(char* num, int* numcount);
void Equal();
ImFont* LoadEmbeddedFont(ImGuiIO& io, HINSTANCE hInstance);
void SetupImGuiFonts(ImGuiIO& io, HINSTANCE hInstance);
//Function Prototypes//







// Main code
int main(int, char**)
{
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    // Icon installing
    HWND hwnd = GetConsoleWindow();
    HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    freenum(num, &NumCount);
    double number = 0;
    string status = "Status:hello world";
    bool IsCalc = false;
    int prikol = 0;
    bool vsync = true;
    bool fpsmetter = true;
    bool fpstest = true;
    StartRandomNumberGenerator();
    //////////Initializing window//////////
    glfwSetErrorCallback(glfw_error_callback);
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "ImGui DeFexCalc", monitor, nullptr);
    if (window == nullptr)
        return 1;
    const GLFWvidmode* moded = glfwGetVideoMode(monitor);
    bool fullscreen = false;
    bool f11Pressed = false;
    glfwMakeContextCurrent(window);
    std::cout << "OpenGL context created successfully!" << std::endl;
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    std::cout << "GLEW initialized successfully!" << std::endl;
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << version << std::endl;
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error after initialization: " << err << std::endl;
    }
    glfwSwapInterval(1);
    GLuint textureID = loadTexture("D:/Project/Imgui/Calculator/examples/example_glfw_opengl2/background.png");
    if (!textureID) {
        std::cerr << "Failed to load background texture!" << std::endl;
    }
    else std::cerr << "background texture loaded!" << std::endl;
    GLFWimage icon;
    icon.pixels = stbi_load("icon.ico", &icon.width, &icon.height, 0, 4); // Завантаження іконки
    glfwSetWindowIcon(window, 1, &icon);
    stbi_image_free(icon.pixels); // Очищення пам'яті

    glfwSwapBuffers(window);
    glfwPollEvents();// Enable vsync
    //////////Initializing window//////////
    //////////ImGui Initialization//////////
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //Font installing
    SetupImGuiFonts(io, GetModuleHandle(nullptr));
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();
    ImVec4 clear_color = ImVec4(0, 0, 0, 0);
    ImVec4 colorText = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    //////////ImGui Initialization//////////
    while (!glfwWindowShouldClose(window))
    {
        ////////////OpenGL initialization////////////
        glClearColor(255.0f, 255.0f, 255.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        GLenum err = glGetError();
        renderBackground(textureID);//render background
        glDisable(GL_TEXTURE_2D);
        if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS && !f11Pressed)
        {
            fullscreen = !fullscreen;
            if (fullscreen){glfwSetWindowMonitor(window, monitor, 0, 0, moded->width, moded->height, moded->refreshRate);}
            else{glfwSetWindowMonitor(window, nullptr, 100, 100, 1280, 720, 0);}
            f11Pressed = true;
        }
        else if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_RELEASE){f11Pressed = false;}
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(5);
            continue;
        }
        ////////////OpenGL initialization////////////
        ////////////////////////////////Start of ImGui Part////////////////////////////////
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {

            ImGui::Begin("Calculator");
            ImGui::Text("This is my first calculator for ImGui :D");
            ImGui::SetNextItemWidth(70);
            ImVec2 mainWindow = ImGui::GetWindowSize();
            if (ImGui::Button("Start Calc", ImVec2(mainWindow.x - 16.0f, 50))) {
                status = "Status:Calc Started";
                IsCalc = true;
            }
            if (ImGui::Button("Exit", ImVec2(mainWindow.x - 16.0f, 30))) {
                std::exit(1);
            }
            ImGui::Separator();
            ImGui::Text("Status:");
            ImGui::Text("%s", status.c_str());
            ImGui::Separator();
            ImGui::Text("Settings:");
            ImGui::Checkbox("Enable V-Sync", &vsync);
            if (vsync) {glfwSwapInterval(1);}
            else {glfwSwapInterval(0);}
            ImGui::End();

        }
        if (fpstest) {

            ImGui::Begin("Test", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |ImGuiWindowFlags_NoMove);
            float fpsxsize = 240;
            float fpsysize = 50;
            ImGui::SetWindowPos(ImVec2(5, 0), 0);
            ImGui::SetWindowSize(ImVec2(fpsxsize+20.0f, fpsysize+15.0f),0);
            ImDrawList* draw = ImGui::GetWindowDrawList();
            ImVec2 leftTop = ImVec2(10, 0);
            ImVec2 bottomRight = ImVec2(fpsxsize, fpsysize);
            draw->AddRectFilled(leftTop, bottomRight, IM_COL32(0, 0, 0, 160), 30.0f, ImDrawFlags_RoundCornersAll);
            draw->AddRectFilled(ImVec2(20,5), ImVec2(80,45), IM_COL32(0, 0, 0, 160), 15.0f, ImDrawFlags_RoundCornersAll);
            char fpsText[64];
            std::snprintf(fpsText, sizeof(fpsText), "%.0fFPS", ImGui::GetIO().Framerate);
            draw->AddText(ImVec2(22,14), IM_COL32(255, 255, 255, 255), fpsText);
            ImGui::End();
        }
        if (IsCalc) {
            if (IsFirstNum) {
                firstnum = atof(num);
            }
            else secondnum = atof(num);

            if (secondnum != atof(num) && firstnum != 0)secondnum = atof(num);

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
            ImGui::SetCursorPosY(38.0f);
            ImGui::InputDouble("## ", &number);    
            ImGui::SetCursorPosX(100.0f);
            ImGui::InputDouble(" ", &secondnum);
            ImGui::Separator();
            ImGui::Text("result:%lf                        mode:%d                        %s",result,mode,status.c_str());
            ImGui::Text("Random:%d%d%d%d%d%d%d%d",random_number.load(), random_number.load(), random_number.load(), random_number.load(), random_number.load(), random_number.load(), random_number.load(), random_number.load());

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
        

        // Rendering
        ImGui::Render();

        // Налаштування вікна для ImGui
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        // Рендеринг ImGui
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        // Обмін буферів
        glfwSwapBuffers(window);

        // Обробка подій
        glfwPollEvents();

    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glDeleteTextures(1, &textureID);
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
void Equal() {
    if (firstnum == 0 && secondnum != 0) {
        firstnum = secondnum;
        freenum(num, &NumCount);
        return;
    }
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
        if (secondnum == 0)status = "cant divide by zero";
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
ImFont* LoadEmbeddedFont(ImGuiIO& io, HINSTANCE hInstance) {
    // Завантажуємо шрифт з ресурсів
    HRSRC hRes = FindResource(hInstance, MAKEINTRESOURCE(IDR_FONT1), RT_RCDATA);

    if (!hRes) return nullptr;

    HGLOBAL hMem = LoadResource(hInstance, hRes);
    if (!hMem) return nullptr;

    void* fontData = LockResource(hMem);
    DWORD fontSize = SizeofResource(hInstance, hRes);

    // Додаємо шрифт у ImGui
    return io.Fonts->AddFontFromMemoryTTF(fontData, fontSize, 20.0f);
}
void SetupImGuiFonts(ImGuiIO& io, HINSTANCE hInstance) {
    ImFont* font = LoadEmbeddedFont(io, hInstance);
    if (!font) {
        // Обробка помилки
        MessageBox(nullptr, "Failed to load embedded font!", "Error", MB_ICONERROR);
    }
}
