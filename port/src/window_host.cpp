#include "byzantine/window_host.hpp"
#include <SDL.h>
#if defined(BYZ_HAVE_X11) && defined(SDL_VIDEO_DRIVER_X11)
#include <SDL_syswm.h>
#include <X11/Xutil.h>
#endif
namespace byzantine {
bool set_window_aspect_hint(SDL_Window* window, bool enabled) {
#if defined(BYZ_HAVE_X11) && defined(SDL_VIDEO_DRIVER_X11)
    SDL_SysWMinfo info{};
    SDL_VERSION(&info.version);
    if (SDL_GetWindowWMInfo(window, &info) && info.subsystem == SDL_SYSWM_X11) {
        XSizeHints hints{};
        long supplied = 0;
        XGetWMNormalHints(info.info.x11.display, info.info.x11.window, &hints, &supplied);
        const bool has_aspect = (hints.flags & PAspect) != 0;
        if (enabled && has_aspect && hints.min_aspect.x == 4 && hints.min_aspect.y == 3 &&
            hints.max_aspect.x == 4 && hints.max_aspect.y == 3) return true;
        if (!enabled && !has_aspect) return true;
        if (enabled) {
            hints.flags |= PAspect;
            hints.min_aspect.x = hints.max_aspect.x = 4;
            hints.min_aspect.y = hints.max_aspect.y = 3;
        } else hints.flags &= ~PAspect;
        XSetWMNormalHints(info.info.x11.display, info.info.x11.window, &hints);
        XFlush(info.info.x11.display);
        return true;
    }
#else
    (void)window; (void)enabled;
#endif
    return false;
}
} // namespace byzantine
