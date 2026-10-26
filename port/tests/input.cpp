// Mouse coordinates in an SDL2 renderer with a logical size.
//
// SDL_RenderSetLogicalSize installs SDL_RendererEventWatch, which rewrites
// SDL_MOUSEMOTION/BUTTON coordinates from window points to the logical size.
// SDL_GetMouseState still reports window coordinates; SDL_RenderWindowToLogical
// is its inverse. This test pins that contract for several window sizes so the
// player never has to guess a scale factor and apply it a second time.

#include <SDL.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace {

int failures = 0;

void check(bool condition, const std::string& what) {
    if (!condition) {
        std::fprintf(stderr, "FAIL: %s\n", what.c_str());
        ++failures;
    }
}

// The point a real window click must land on for `logical`, according to SDL's
// own inverse. SDL_RenderLogicalToWindow applies dpi_scale and the viewport.
void logical_to_window(SDL_Renderer* renderer, float lx, float ly, int* wx, int* wy) {
    SDL_RenderLogicalToWindow(renderer, lx, ly, wx, wy);
}

// Delivers the event exactly the way the X11/macOS/Wayland backends do: the
// event starts in window coordinates and SDL_PushEvent runs the renderer's
// event watch. The queued event must arrive in logical coordinates.
bool push_and_read_window_click(SDL_Window* window, int wx, int wy, SDL_Event* out) {
    SDL_Event event{};
    event.type = SDL_MOUSEBUTTONDOWN;
    event.button.windowID = SDL_GetWindowID(window);
    event.button.button = SDL_BUTTON_LEFT;
    event.button.state = SDL_PRESSED;
    event.button.clicks = 1;
    event.button.x = wx;
    event.button.y = wy;
    if (SDL_PushEvent(&event) != 1) return false;
    SDL_Event polled;
    while (SDL_PollEvent(&polled)) {
        if (polled.type == SDL_MOUSEBUTTONDOWN) {
            *out = polled;
            return true;
        }
    }
    // The dummy driver may deliver the queued event on the next pump.
    SDL_PumpEvents();
    while (SDL_PollEvent(&polled)) {
        if (polled.type == SDL_MOUSEBUTTONDOWN) {
            *out = polled;
            return true;
        }
    }
    return false;
}

void check_size(SDL_Window* window, SDL_Renderer* renderer, int width, int height,
                int logical_width, int logical_height) {
    SDL_SetWindowSize(window, width, height);
    // Let the size-changed event reach SDL_RendererEventWatch, which rebuilds
    // the viewport/scale and dpi_scale for the new window size.
    SDL_PumpEvents();
    SDL_Event flush;
    while (SDL_PollEvent(&flush)) {
    }
    if (SDL_RenderSetLogicalSize(renderer, logical_width, logical_height) != 0) {
        check(false, "SDL_RenderSetLogicalSize failed: " + std::string(SDL_GetError()));
        return;
    }
    SDL_PumpEvents();
    while (SDL_PollEvent(&flush)) {
    }

    int output_width = 0;
    int output_height = 0;
    if (SDL_GetRendererOutputSize(renderer, &output_width, &output_height) != 0) {
        check(false, "SDL_GetRendererOutputSize failed");
        return;
    }
    check(output_width == width && output_height == height,
          "output size is " + std::to_string(output_width) + "x" +
              std::to_string(output_height) + " for window " + std::to_string(width) +
              "x" + std::to_string(height));

    // A few logical points inside the playfield and the answer-list block.
    const float points[][2] = {{352, 201}, {300, 300}, {100, 250}, {639, 399},
                               {0, 0},     {5, 5},     {320, 200}};
    for (const auto& point : points) {
        int wx = 0;
        int wy = 0;
        logical_to_window(renderer, point[0], point[1], &wx, &wy);
        SDL_Event event{};
        const bool delivered = push_and_read_window_click(window, wx, wy, &event);
        check(delivered, "mouse event delivered at window " + std::to_string(wx) + "," +
                             std::to_string(wy));
        if (!delivered) continue;
        // SDL truncates to int in the event watch; allow the one-pixel
        // difference that integer truncation can produce at a non-integer scale.
        const bool same = std::abs(event.button.x - static_cast<int>(point[0])) <= 1 &&
                          std::abs(event.button.y - static_cast<int>(point[1])) <= 1;
        check(same, "logical point " + std::to_string(static_cast<int>(point[0])) + "," +
                        std::to_string(static_cast<int>(point[1])) +
                        " round-trips through a " + std::to_string(width) + "x" +
                        std::to_string(height) + " window as " +
                        std::to_string(event.button.x) + "," +
                        std::to_string(event.button.y));
    }

    // The hover path uses SDL_GetMouseState (window points) +
    // SDL_RenderWindowToLogical. Warp to a known window point and check that
    // the inverse produces the intended logical point.
    int wx = 0;
    int wy = 0;
    logical_to_window(renderer, 352, 201, &wx, &wy);
    SDL_WarpMouseInWindow(window, wx, wy);
    SDL_PumpEvents();
    while (SDL_PollEvent(&flush)) {
    }
    int mouse_x = 0;
    int mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    float lx = 0;
    float ly = 0;
    SDL_RenderWindowToLogical(renderer, mouse_x, mouse_y, &lx, &ly);
    check(std::abs(lx - 352.0f) <= 1.0f && std::abs(ly - 201.0f) <= 1.0f,
          "hover window point " + std::to_string(mouse_x) + "," +
              std::to_string(mouse_y) + " -> logical " + std::to_string(lx) + "," +
              std::to_string(ly));
}

}  // namespace

int main() {
    SDL_setenv("SDL_VIDEODRIVER", "dummy", 1);
    SDL_setenv("SDL_AUDIODRIVER", "dummy", 1);
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::fprintf(stderr, "SDL init failed: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("input-test", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, 1280, 960,
                                          SDL_WINDOW_HIDDEN);
    if (window == nullptr) {
        std::fprintf(stderr, "window failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == nullptr) renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) {
        std::fprintf(stderr, "renderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // The player's 640x400 playfield and the 640x480 conversation screen.
    check_size(window, renderer, 1280, 960, 640, 400);
    check_size(window, renderer, 1024, 768, 640, 400);
    check_size(window, renderer, 900, 700, 640, 400);
    check_size(window, renderer, 960, 600, 640, 400);
    check_size(window, renderer, 640, 480, 640, 400);
    check_size(window, renderer, 1280, 960, 640, 480);
    check_size(window, renderer, 1024, 768, 640, 480);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    if (failures != 0) {
        std::fprintf(stderr, "input-test: %d failure(s)\n", failures);
        return 1;
    }
    std::printf("input coordinate tests passed\n");
    return 0;
}
