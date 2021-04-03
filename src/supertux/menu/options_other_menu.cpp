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

#include "supertux/menu/options_other_menu.hpp"

#include "gui/item_toggle.hpp"
#include "gui/menu_item.hpp"
#include "supertux/gameconfig.hpp"
#include "supertux/globals.hpp"
#include "util/gettext.hpp"

OptionsOtherMenu::OptionsOtherMenu()
{
  add_label(_("Gameplay Settings"));
  add_hl();

  MenuItem& enable_transitions = add_toggle(MNID_TRANSITIONS, _("Enable transitions"), &g_config->transitions_enabled);
  enable_transitions.set_help(_("Enable screen transitions and smooth menu animation"));

  if (g_config->developer_mode)
  {
    add_toggle(MNID_DEVELOPER_MODE, _("Developer Mode"), &g_config->developer_mode);
  }

  if (g_config->is_christmas() || g_config->christmas_mode)
  {
    add_toggle(MNID_CHRISTMAS_MODE, _("Christmas Mode"), &g_config->christmas_mode);
  }

  add_toggle(MNID_CONFIRMATION_DIALOG, _("Confirmation Dialog"), &g_config->confirmation_dialog).set_help(_("Confirm aborting level"));
  add_toggle(MNID_PAUSE_ON_FOCUSLOSS, _("Pause on focus loss"), &g_config->pause_on_focusloss)
    .set_help("Automatically pause the game when the window loses focus");
  add_toggle(MNID_CUSTOM_CURSOR, _("Use custom mouse cursor"), &g_config->custom_mouse_cursor).set_help(_("Whether the game renders its own cursor or uses the system's cursor"));

  add_hl();
  add_back(_("Back"));
}

void
OptionsOtherMenu::menu_action(MenuItem& item)
{
  switch (item.get_id())
  {
    case MNID_CUSTOM_CURSOR:
      SDL_ShowCursor(g_config->custom_mouse_cursor ? 0 : 1);
      break;

    default:
      break;
  }
}

/* EOF */
