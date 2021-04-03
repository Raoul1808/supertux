//  SuperTux
//  Copyright (C) 2021 Raoul1808 <raoulthegeek@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "supertux/menu/options_graphics_menu.hpp"

#include "gui/item_goto.hpp"
#include "gui/item_stringselect.hpp"
#include "gui/item_toggle.hpp"
#include "gui/menu_item.hpp"
#include "gui/menu_manager.hpp"
#include "supertux/gameconfig.hpp"
#include "supertux/game_session.hpp"
#include "supertux/globals.hpp"
#include "supertux/menu/menu_storage.hpp"
#include "util/gettext.hpp"
#include "util/log.hpp"
#include "video/renderer.hpp"

OptionsGraphicsMenu::OptionsGraphicsMenu() :
  next_magnification(0),
  next_aspect_ratio(0),
  next_window_resolution(0),
  next_resolution(0),
  next_vsync(0),
  magnifications(),
  aspect_ratios(),
  window_resolutions(),
  resolutions(),
  vsyncs()
{
  add_label(_("Video Settings"));
  add_hl();

  magnifications.clear();
  // These values go from screen:640/projection:1600 to
  // screen:1600/projection:640 (i.e. 640, 800, 1024, 1280, 1600)
  magnifications.push_back(_("auto"));
#ifndef ENABLE_TOUCHSCREEN_SUPPORT
  magnifications.push_back("40%");
  magnifications.push_back("50%");
  magnifications.push_back("62.5%");
  magnifications.push_back("80%");
#endif
  magnifications.push_back("100%");
  magnifications.push_back("125%");
  magnifications.push_back("160%");
  magnifications.push_back("200%");
  magnifications.push_back("250%");
  // Gets the actual magnification:
  if (g_config->magnification != 0.0f) //auto
  {
    std::ostringstream out;
    out << (g_config->magnification*100) << "%";
    std::string magn = out.str();
    int count = 0;
    for (const auto& magnification : magnifications)
    {
      if (magnification == magn)
      {
        next_magnification = count;
        magn.clear();
        break;
      }

      ++count;
    }
    if (!magn.empty()) //magnification not in our list but accept anyway
    {
      next_magnification = static_cast<int>(magnifications.size());
      magnifications.push_back(magn);
    }
  }

  aspect_ratios.clear();
  aspect_ratios.push_back(_("auto"));
  aspect_ratios.push_back("5:4");
  aspect_ratios.push_back("4:3");
  aspect_ratios.push_back("16:10");
  aspect_ratios.push_back("16:9");
  aspect_ratios.push_back("1368:768");
  // Gets the actual aspect ratio:
  if (g_config->aspect_size != Size(0, 0)) //auto
  {
    std::ostringstream out;
    out << g_config->aspect_size.width << ":" << g_config->aspect_size.height;
    std::string aspect_ratio = out.str();
    int cnt_ = 0;
    for (const auto& ratio : aspect_ratios)
    {
      if (ratio == aspect_ratio)
      {
        aspect_ratio.clear();
        next_aspect_ratio = cnt_;
        break;
      }
      ++cnt_;
    }

    if (!aspect_ratio.empty())
    {
      next_aspect_ratio = static_cast<int>(aspect_ratios.size());
      aspect_ratios.push_back(aspect_ratio);
    }
  }

  {
    window_resolutions = { "640x480", "854x480", "800x600", "1280x720", "1280x800",
                           "1440x900", "1920x1080", "1920x1200", "2560x1440" };
    next_window_resolution = -1;
    Size window_size = VideoSystem::current()->get_window_size();
    std::ostringstream out;
    out << window_size.width << "x" << window_size.height;
    std::string window_size_text = out.str();
    for (size_t i = 0; i < window_resolutions.size(); ++i)
    {
      if (window_resolutions[i] == window_size_text)
      {
        next_window_resolution = static_cast<int>(i);
        break;
      }
    }
    if (next_window_resolution == -1)
    {
      window_resolutions.insert(window_resolutions.begin(), window_size_text);
      next_window_resolution = 0;
    }
  }

  resolutions.clear();
  int display_mode_count = SDL_GetNumDisplayModes(0);
  std::string last_display_mode;
  for (int i = 0; i < display_mode_count; ++i)
  {
    SDL_DisplayMode mode;
    int ret = SDL_GetDisplayMode(0, i, &mode);
    if (ret != 0)
    {
      log_warning << "failed to get display mode: " << SDL_GetError() << std::endl;
    }
    else
    {
      std::ostringstream out;
      out << mode.w << "x" << mode.h;
      if (mode.refresh_rate)
        out << "@" << mode.refresh_rate;
      if (last_display_mode == out.str())
        continue;
      last_display_mode = out.str();
      resolutions.insert(resolutions.begin(), out.str());
    }
  }
  resolutions.push_back("Desktop");

  std::string fullscreen_size_str = _("Desktop");
  {
    std::ostringstream out;
    if (g_config->fullscreen_size != Size(0, 0))
    {
      out << g_config->fullscreen_size.width << "x" << g_config->fullscreen_size.height;
      if (g_config->fullscreen_refresh_rate)
         out << "@" << g_config->fullscreen_refresh_rate;
      fullscreen_size_str = out.str();
    }
  }

  int cnt = 0;
  for (const auto& res : resolutions)
  {
    if (res == fullscreen_size_str)
    {
      fullscreen_size_str.clear();
      next_resolution = cnt;
      break;
    }
    ++cnt;
  }
  if (!fullscreen_size_str.empty())
  {
    next_resolution = static_cast<int>(resolutions.size());
    resolutions.push_back(fullscreen_size_str);
  }

  { // vsync
    vsyncs.push_back(_("on"));
    vsyncs.push_back(_("off"));
    vsyncs.push_back(_("adaptive"));
    int mode = VideoSystem::current()->get_vsync();

    switch (mode)
    {
      case -1:
        next_vsync = 2;
        break;

      case 0:
        next_vsync = 1;
        break;

      case 1:
        next_vsync = 0;
        break;

      default:
        log_warning << "Unknown swap mode: " << mode << std::endl;
        next_vsync = 0;
    }
  }

  #ifndef ENABLE_TOUCHSCREEN_SUPPORT
  add_toggle(MNID_FULLSCREEN,_("Window Resizable"), &g_config->window_resizable)
    .set_help(_("Allow window resizing, might require a restart to take effect"));

  MenuItem& window_res = add_string_select(MNID_WINDOW_RESOLUTION, _("Window Resolution"), &next_window_resolution, window_resolutions);
  window_res.set_help(_("Resize the window to the given size"));

  add_toggle(MNID_FULLSCREEN,_("Fullscreen"), &g_config->use_fullscreen)
    .set_help(_("Fill the entire screen"));

  MenuItem& fullscreen_res = add_string_select(MNID_FULLSCREEN_RESOLUTION, _("Fullscreen Resolution"), &next_resolution, resolutions);
  fullscreen_res.set_help(_("Determine the resolution used in fullscreen mode (you must toggle fullscreen to complete the change)"));
#endif

  MenuItem& magnification = add_string_select(MNID_MAGNIFICATION, _("Magnification"), &next_magnification, magnifications);
  magnification.set_help(_("Change the magnification of the game area"));

  MenuItem& vsync = add_string_select(MNID_VSYNC, _("VSync"), &next_vsync, vsyncs);
  vsync.set_help(_("Set the VSync mode"));

#ifndef ENABLE_TOUCHSCREEN_SUPPORT
  MenuItem& aspect = add_string_select(MNID_ASPECTRATIO, _("Aspect Ratio"), &next_aspect_ratio, aspect_ratios);
  aspect.set_help(_("Adjust the aspect ratio"));
#endif

  add_hl();
  add_back(_("Back"));
}

void
OptionsGraphicsMenu::menu_action(MenuItem& item)
{
  switch (item.get_id())
  {
      case MNID_ASPECTRATIO:
      {
        if (aspect_ratios[next_aspect_ratio] == _("auto"))
        {
          g_config->aspect_size = Size(0, 0); // Magic values
          VideoSystem::current()->apply_config();
          MenuManager::instance().on_window_resize();
        }
        else if (sscanf(aspect_ratios[next_aspect_ratio].c_str(), "%d:%d",
                        &g_config->aspect_size.width, &g_config->aspect_size.height) == 2)
        {
          VideoSystem::current()->apply_config();
          MenuManager::instance().on_window_resize();
        }
        else
        {
          log_fatal << "Invalid aspect ratio " << aspect_ratios[next_aspect_ratio] << " specified" << std::endl;
          assert(false);
        }
      }
      break;

    case MNID_MAGNIFICATION:
      if (magnifications[next_magnification] == _("auto"))
      {
        g_config->magnification = 0.0f; // Magic value
      }
      else if (sscanf(magnifications[next_magnification].c_str(), "%f", &g_config->magnification) == 1)
      {
        g_config->magnification /= 100.0f;
      }
      VideoSystem::current()->apply_config();
      MenuManager::instance().on_window_resize();
      break;

    case MNID_WINDOW_RESIZABLE:
      if (!g_config->window_resizable) {
        next_window_resolution = 0;
      }
      break;

    case MNID_WINDOW_RESOLUTION:
      {
        int width;
        int height;
        if (sscanf(window_resolutions[next_window_resolution].c_str(), "%dx%d",
                   &width, &height) != 2)
        {
          log_fatal << "can't parse " << window_resolutions[next_window_resolution] << std::endl;
        }
        else
        {
          g_config->window_size = Size(width, height);
          VideoSystem::current()->apply_config();
          MenuManager::instance().on_window_resize();
        }
      }
      break;

    case MNID_FULLSCREEN_RESOLUTION:
      {
        int width;
        int height;
        int refresh_rate;
        if (resolutions[next_resolution] == "Desktop")
        {
          g_config->fullscreen_size.width = 0;
          g_config->fullscreen_size.height = 0;
          g_config->fullscreen_refresh_rate = 0;
        }
        else if (sscanf(resolutions[next_resolution].c_str(), "%dx%d@%d",
                  &width, &height, &refresh_rate) == 3)
        {
          // do nothing, changes are only applied when toggling fullscreen mode
          g_config->fullscreen_size.width = width;
          g_config->fullscreen_size.height = height;
          g_config->fullscreen_refresh_rate = refresh_rate;
        }
        else if (sscanf(resolutions[next_resolution].c_str(), "%dx%d",
                       &width, &height) == 2)
        {
            g_config->fullscreen_size.width = width;
            g_config->fullscreen_size.height = height;
            g_config->fullscreen_refresh_rate = 0;
        }
      }
      break;

    case MNID_VSYNC:
      switch (next_vsync)
      {
        case 2:
          VideoSystem::current()->set_vsync(-1);
          break;

        case 1:
          VideoSystem::current()->set_vsync(0);
          break;

        case 0:
          VideoSystem::current()->set_vsync(1);
          break;

        default:
          assert(false);
          break;
      }
      break;

    case MNID_FULLSCREEN:
      VideoSystem::current()->apply_config();
      MenuManager::instance().on_window_resize();
      g_config->save();
      break;

    default:
      break;
  }
}

/* EOF */
