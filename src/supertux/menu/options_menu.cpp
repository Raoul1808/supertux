//  SuperTux
//  Copyright (C) 2004 Tobas Glaesser <tobi.web@gmx.de>
//  Copyright (C) 2006 Matthias Braun <matze@braunis.de>
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

#include "supertux/menu/options_menu.hpp"

#include "audio/sound_manager.hpp"
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


enum OptionsMenuIDs {
#ifdef ENABLE_TOUCHSCREEN_SUPPORT
  MNID_MOBILE_CONTROLS
#endif
};

OptionsMenu::OptionsMenu(bool complete)
{
  add_label(_("Options"));
  add_hl();

  if (complete)
  {
    // Language and profile changes are only be possible in the
    // main menu, since elsewhere it might not always work fully
    add_submenu(_("Select Language"), MenuStorage::LANGUAGE_MENU)
      .set_help(_("Select a different language to display text in"));

    add_submenu(_("Language Packs"), MenuStorage::LANGPACK_MENU)
      .set_help(_("Language packs contain up-to-date translations"));

    add_submenu(_("Select Profile"), MenuStorage::PROFILE_MENU)
      .set_help(_("Select a profile to play with"));
  }

  add_submenu(_("Video Settings"), MenuStorage::OPTIONS_GRAPHICS_MENU)
      .set_help(_("Manage how the game window should look like"));
  if (SoundManager::current()->is_audio_enabled())
  {
    add_submenu(_("Audio Settings"), MenuStorage::OPTIONS_AUDIO_MENU)
        .set_help(_("Enable/Disable sound and music, or change their volumes"));
  }
  else
  {
    add_inactive(_("Audio (disabled)"));
  }

  add_submenu(_("Gameplay Settings"), MenuStorage::OPTIONS_OTHER_MENU);

  add_submenu(_("Setup Keyboard"), MenuStorage::KEYBOARD_MENU)
    .set_help(_("Configure key-action mappings"));

#ifndef UBUNTU_TOUCH
  add_submenu(_("Setup Joystick"), MenuStorage::JOYSTICK_MENU)
    .set_help(_("Configure joystick control-action mappings"));
#endif

#ifdef ENABLE_TOUCHSCREEN_SUPPORT
  add_toggle(MNID_MOBILE_CONTROLS, _("On-screen controls"), &g_config->mobile_controls)
      .set_help(_("Toggle on-screen controls for mobile devices"));
#endif
  
  add_submenu(_("Integrations and presence"), MenuStorage::INTEGRATIONS_MENU)
      .set_help(_("Manage whether SuperTux should display the levels you play on your social media profiles (Discord)"));

  add_hl();
  add_back(_("Back"));
}

OptionsMenu::~OptionsMenu()
{
}

void
OptionsMenu::menu_action(MenuItem& item)
{
  // I'm leaving this just in case
  // if someone wants to add something later.
  // -Raoul1808 (Mew)
  switch (item.get_id()) {
    default:
      break;
  }
}

/* EOF */
