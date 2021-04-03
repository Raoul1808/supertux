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

#include "supertux/menu/options_audio_menu.hpp"

#include "audio/sound_manager.hpp"
#include "gui/item_stringselect.hpp"
#include "gui/item_toggle.hpp"
#include "gui/menu_item.hpp"
#include "gui/menu_manager.hpp"
#include "supertux/gameconfig.hpp"
#include "supertux/globals.hpp"
#include "util/gettext.hpp"

namespace {

bool less_than_volume(const std::string& lhs, const std::string& rhs) {
  int lhs_i, rhs_i;
  if (sscanf(lhs.c_str(), "%i", &lhs_i) == 1 &&
      sscanf(rhs.c_str(), "%i", &rhs_i) == 1)
  {
    return lhs_i < rhs_i;
  }

  return false;
}

} // namespace

OptionsAudioMenu::OptionsAudioMenu() :
  next_sound_volume(0),
  next_music_volume(0),
  sound_volumes(),
  music_volumes()
{
  add_label(_("Audio Settings"));
  add_hl();

  // Sound Volume
  sound_volumes.clear();
  for (const char* percent : {"0%", "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%", "100%"}) {
    sound_volumes.push_back(percent);
  }

  std::ostringstream sound_vol_stream;
  sound_vol_stream << g_config->sound_volume << "%";
  std::string sound_vol_string = sound_vol_stream.str();

  if (std::find(sound_volumes.begin(),
               sound_volumes.end(), sound_vol_string) == sound_volumes.end())
  {
    sound_volumes.push_back(sound_vol_string);
  }

  std::sort(sound_volumes.begin(), sound_volumes.end(), less_than_volume);

  std::ostringstream out;
  out << g_config->sound_volume << "%";
  std::string sound_volume = out.str();
  int cnt_ = 0;
  for (const auto& volume : sound_volumes)
  {
    if (volume == sound_volume)
    {
      sound_volume.clear();
      next_sound_volume = cnt_;
      break;
    }
    ++cnt_;
  }

  // Music Volume
  music_volumes.clear();
  for (const char* percent : {"0%", "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%", "100%"}) {
    music_volumes.push_back(percent);
  }

  std::ostringstream music_vol_stream;
  music_vol_stream << g_config->music_volume << "%";
  std::string music_vol_string = music_vol_stream.str();

  if (std::find(music_volumes.begin(),
               music_volumes.end(), music_vol_string) == music_volumes.end())
  {
    music_volumes.push_back(music_vol_string);
  }

  std::sort(music_volumes.begin(), music_volumes.end(), less_than_volume);

  out.str("");
  out.clear();
  out << g_config->music_volume << "%";
  std::string music_volume = out.str();
  cnt_ = 0;
  for (const auto& volume : music_volumes)
  {
    if (volume == music_volume)
    {
      music_volume.clear();
      next_music_volume = cnt_;
      break;
    }
    ++cnt_;
  }

  add_toggle(MNID_SOUND, _("Sound"), &g_config->sound_enabled)
    .set_help(_("Disable all sound effects"));
  add_toggle(MNID_MUSIC, _("Music"), &g_config->music_enabled)
    .set_help(_("Disable all music"));

  MenuItem& sound_volume_select = add_string_select(MNID_SOUND_VOLUME, _("Sound Volume"), &next_sound_volume, sound_volumes);
  sound_volume_select.set_help(_("Adjust sound volume"));

  MenuItem& music_volume_select = add_string_select(MNID_MUSIC_VOLUME, _("Music Volume"), &next_music_volume, music_volumes);
  music_volume_select.set_help(_("Adjust music volume"));
  
  add_hl();
  add_back(_("Back"));
}

void
OptionsAudioMenu::menu_action(MenuItem& item)
{
  switch (item.get_id())
  {
    case MNID_SOUND:
      SoundManager::current()->enable_sound(g_config->sound_enabled);
      g_config->save();
      break;

    case MNID_SOUND_VOLUME:
      if (sscanf(sound_volumes[next_sound_volume].c_str(), "%i", &g_config->sound_volume) == 1)
      {
        bool sound_enabled = g_config->sound_volume > 0 ? true : false;
        SoundManager::current()->enable_sound(sound_enabled);
        SoundManager::current()->set_sound_volume(g_config->sound_volume);
        g_config->save();
      }
      break;

    case MNID_MUSIC:
      SoundManager::current()->enable_music(g_config->music_enabled);
      g_config->save();
      break;

    case MNID_MUSIC_VOLUME:
      if (sscanf(music_volumes[next_music_volume].c_str(), "%i", &g_config->music_volume) == 1)
      {
        bool music_enabled = g_config->music_volume > 0 ? true : false;
        SoundManager::current()->enable_music(music_enabled);
        SoundManager::current()->set_music_volume(g_config->music_volume);
        g_config->save();
      }
      break;
  }
}

/* EOF */
