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

#ifndef HEADER_SUPERTUX_SUPERTUX_MENU_OPTIONS_GRAPHICS_MENU_HPP
#define HEADER_SUPERTUX_SUPERTUX_MENU_OPTIONS_GRAPHICS_MENU_HPP

#include "gui/menu.hpp"

class OptionsGraphicsMenu final : public Menu
{
public:
  OptionsGraphicsMenu();

  void menu_action(MenuItem& item) override;

private:
  enum MenuIDs {
    MNID_WINDOW_RESIZABLE,
    MNID_WINDOW_RESOLUTION,
    MNID_FULLSCREEN,
    MNID_FULLSCREEN_RESOLUTION,
    MNID_MAGNIFICATION,
    MNID_ASPECTRATIO,
    MNID_VSYNC
  };

  int next_magnification;
  int next_aspect_ratio;
  int next_window_resolution;
  int next_resolution;
  int next_vsync;

  std::vector<std::string> magnifications;
  std::vector<std::string> aspect_ratios;
  std::vector<std::string> window_resolutions;
  std::vector<std::string> resolutions;
  std::vector<std::string> vsyncs;

private:
  OptionsGraphicsMenu(const OptionsGraphicsMenu&) = delete;
  OptionsGraphicsMenu& operator=(const OptionsGraphicsMenu&) = delete;
};

#endif

/* EOF */
