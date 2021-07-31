#include "imgui/imgui.h"
#include "appWindow.hpp"
#include "appGui.hpp"

using namespace Giffi;

SDL_Renderer* appWindow::mRenderer = nullptr;
SDL_Window*   appWindow::mWindow = nullptr;
std::shared_ptr<gbGameBoy> appWindow::mGameBoy = nullptr;

bool appWindow::Init()
{
    const int width  = 1280;
    const int height = 720;

    // Init SDL2
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        printf("Unable to init sdl2: %s\n", SDL_GetError()); 
        return false;
    }
    mWindow  = SDL_CreateWindow("GeimBoy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
    if (mWindow == NULL)
    {
        printf("Unable to create window: %s\n", SDL_GetError());
        return false;
    }
    
    mRenderer= SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if (mRenderer == NULL)
    {
        printf("Unable to create renderer: %s\n", SDL_GetError());
        return false;
    }

    // Init GameBoy
    mGameBoy = std::make_shared<gbGameBoy>();
    mGameBoy->Reset();
    mGameBoy->SetPalette(0x9bbc0f, 0x8bac0f, 0x306230, 0x0f380f);
    
    return appGui::Init(mRenderer, mGameBoy, width, height);
}

void appWindow::Run()
{
    // Create Surface out of raw pixel data.
    int depth = 24;
    int pitch = 3 * 160;
    Uint32 rmask = 0x000000FF;
    Uint32 gmask = 0x0000FF00;
    Uint32 bmask = 0x00FF0000;
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(&mGameBoy->mPpu.front_buffer, 160, 144, depth, pitch, rmask, gmask, bmask, NULL);

    while (!ShouldWindowClose())
    {
        int start_ticks = SDL_GetTicks();
        // Update
        {
            // Emulator gets updated in the gui, because of "pause"
            appGui::Update();
        }

        // Drawing
        {
            // Clear Background
            SDL_SetRenderDrawColor(mRenderer, 114, 144, 154, 255);
            SDL_RenderClear(mRenderer);

            // Render Emulator
            SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
            SDL_Rect dst = { 0, 19, 0, 0 }; // Take in count window size, and top bar which takes 19px.
            SDL_GetWindowSize(mWindow, &dst.w, &dst.h);
            dst.h -= dst.y;
            SDL_RenderCopy(mRenderer, texture, NULL, &dst);

            // Draw Gui
            appGui::Draw();

            // Render
            SDL_RenderPresent(mRenderer);
            SDL_DestroyTexture(texture);
        }

        // Using delay to target 60fps
        int end_ticks = SDL_GetTicks();
        if (16 - (end_ticks - start_ticks) > 0)
            SDL_Delay(16 - (end_ticks - start_ticks));
    }

    SDL_FreeSurface(surface);
}

void appWindow::CleanUp()
{
    SDL_Quit();
}

// Does Window events aswell
bool appWindow::ShouldWindowClose()
{
    static bool should_close = false;
    ImGuiIO& io = ImGui::GetIO();
    int mouseX, mouseY, mouseWheel = 0;
    const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
    SDL_Event events;
    while (SDL_PollEvent(&events))
    {
        switch (events.type)
        {
            case SDL_QUIT:
            {
                should_close = true;
                break;
            }

            // Controls currently hardcoded but will be added into a array or something
            case SDL_KEYDOWN:
            {
                uint16_t pressed = events.key.keysym.scancode;
                if (pressed == SDL_SCANCODE_ESCAPE)    { should_close = true; break; }
                if (pressed == SDL_SCANCODE_D)         { mGameBoy->PressButton(gbButton::RIGHT);  break; }
                if (pressed == SDL_SCANCODE_A)         { mGameBoy->PressButton(gbButton::LEFT);   break; }
                if (pressed == SDL_SCANCODE_W)         { mGameBoy->PressButton(gbButton::UP);     break; }
                if (pressed == SDL_SCANCODE_S)         { mGameBoy->PressButton(gbButton::DOWN);   break; }
                if (pressed == SDL_SCANCODE_K)         { mGameBoy->PressButton(gbButton::A);      break; }
                if (pressed == SDL_SCANCODE_J)         { mGameBoy->PressButton(gbButton::B);      break; }
                if (pressed == SDL_SCANCODE_RETURN)    { mGameBoy->PressButton(gbButton::SELECT); break; }
                if (pressed == SDL_SCANCODE_BACKSLASH) { mGameBoy->PressButton(gbButton::START);  break; }
                break;
            }
            case SDL_KEYUP:
            {
                uint16_t released = events.key.keysym.scancode;
                if (released == SDL_SCANCODE_D)         { mGameBoy->ReleaseButton(gbButton::RIGHT);  break; }
                if (released == SDL_SCANCODE_A)         { mGameBoy->ReleaseButton(gbButton::LEFT);   break; }
                if (released == SDL_SCANCODE_W)         { mGameBoy->ReleaseButton(gbButton::UP);     break; }
                if (released == SDL_SCANCODE_S)         { mGameBoy->ReleaseButton(gbButton::DOWN);   break; }
                if (released == SDL_SCANCODE_K)         { mGameBoy->ReleaseButton(gbButton::A);      break; }
                if (released == SDL_SCANCODE_J)         { mGameBoy->ReleaseButton(gbButton::B);      break; }
                if (released == SDL_SCANCODE_RETURN)    { mGameBoy->ReleaseButton(gbButton::SELECT); break; }
                if (released == SDL_SCANCODE_BACKSLASH) { mGameBoy->ReleaseButton(gbButton::START);  break; }
                break;
            }
            case SDL_DROPFILE: // File gets dropped into the program
            {
                std::string file_path = std::string(events.drop.file);
                if (file_path.find(".gb") > 0) // works with .gb .gbc .gbc etc
                {
                    mGameBoy->Reset();
                    mGameBoy->LoadRom(file_path);
                }
                break;
            }
            case SDL_MOUSEWHEEL:
            {
                mouseWheel = events.wheel.y;
                break;
            }
            case SDL_WINDOWEVENT:
            {
                if (events.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    io.DisplaySize.x = (float)(events.window.data1);
                    io.DisplaySize.y = (float)(events.window.data2);
                }
                break;
            }

        }
    }

    // Imgui stuff
    io.DeltaTime = 1.0f / 60.0f;
    io.MousePos = ImVec2((float)mouseX, (float)mouseY);
    io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
    io.MouseWheel = (float)(mouseWheel);

    return should_close;
}
