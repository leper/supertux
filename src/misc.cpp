//  SuperTux
//  Copyright (C) 2004 Tobas Glaesser <tobi.web@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <config.h>

#include "misc.h"
#include "app/globals.h"

void MyConfig::customload(const lisp::Lisp& reader)
{
  reader.get("keyboard-up", keymap.up);
  reader.get("keyboard-down", keymap.down);
  reader.get("keyboard-left", keymap.left);
  reader.get("keyboard-right", keymap.right);
  reader.get("keyboard-jump", keymap.jump);
  reader.get("keyboard-power", keymap.power);
}

void MyConfig::customsave(FILE * config)
{
  fprintf(config, "\t(keyboard-up  %d)\n", keymap.up);
  fprintf(config, "\t(keyboard-down  %d)\n", keymap.down);
  fprintf(config, "\t(keyboard-left  %d)\n", keymap.left);
  fprintf(config, "\t(keyboard-right %d)\n", keymap.right);
  fprintf(config, "\t(keyboard-jump  %d)\n", keymap.jump);
  fprintf(config, "\t(keyboard-power  %d)\n", keymap.power);
}

void process_options_menu(void)
{
  switch (options_menu->check())
    {
    case MNID_OPENGL:
#ifndef NOOPENGL
      if(use_gl != options_menu->isToggled(MNID_OPENGL))
        {
          use_gl = !use_gl;
          Setup::video(screen->w,screen->h);
        }
#else
      options_menu->get_item_by_id(MNID_OPENGL).toggled = false;
#endif
      break;
    case MNID_FULLSCREEN:
      if(use_fullscreen != options_menu->isToggled(MNID_FULLSCREEN))
        {
          use_fullscreen = !use_fullscreen;
          Setup::video(screen->w,screen->h);
        }
      break;
    case MNID_SOUND:
      if(SoundManager::get()->sound_enabled() != options_menu->isToggled(MNID_SOUND))
        SoundManager::get()->enable_sound(!SoundManager::get()->sound_enabled());
      break;
    case MNID_MUSIC:
      if(SoundManager::get()->music_enabled() != options_menu->isToggled(MNID_MUSIC))
        {
          SoundManager::get()->enable_music(!SoundManager::get()->music_enabled());
        }
      break;
    case MNID_SHOWFPS:
      if(show_fps != options_menu->isToggled(MNID_SHOWFPS))
        show_fps = !show_fps;
      break;
    }
}

void st_menu(void)
{
  main_menu      = new Menu();
  options_menu   = new Menu();
  options_keys_menu     = new Menu();
  options_joystick_menu = new Menu();
  load_game_menu = new Menu();
  save_game_menu = new Menu();
  game_menu      = new Menu();
  highscore_menu = new Menu();
  contrib_menu   = new Menu();
  contrib_subset_menu   = new Menu();
  worldmap_menu  = new Menu();

  main_menu->set_pos(screen->w/2, 335);
  main_menu->additem(MN_GOTO, _("Start Game"),0,load_game_menu, MNID_STARTGAME);
  main_menu->additem(MN_GOTO, _("Contrib Levels"),0,contrib_menu, MNID_LEVELS_CONTRIB);
  main_menu->additem(MN_GOTO, _("Options"),0,options_menu, MNID_OPTIONMENU);
  main_menu->additem(MN_ACTION, _("Level Editor"),0,0, MNID_LEVELEDITOR);
  main_menu->additem(MN_ACTION, _("Credits"),0,0, MNID_CREDITS);
  main_menu->additem(MN_ACTION, _("Quit"),0,0, MNID_QUITMAINMENU);

  options_menu->additem(MN_LABEL,_("Options"),0,0);
  options_menu->additem(MN_HL,"",0,0);
#ifndef NOOPENGL
  options_menu->additem(MN_TOGGLE,_("OpenGL    "),use_gl,0, MNID_OPENGL);
#else
  options_menu->additem(MN_DEACTIVE,_("OpenGL (not supported)"),use_gl, 0, MNID_OPENGL);
#endif
  options_menu->additem(MN_TOGGLE,_("Fullscreen"),use_fullscreen,0, MNID_FULLSCREEN);
  if(SoundManager::get()->audio_device_available())
    {
      options_menu->additem(MN_TOGGLE,_("Sound     "), SoundManager::get()->sound_enabled(),0, MNID_SOUND);
      options_menu->additem(MN_TOGGLE,_("Music     "), SoundManager::get()->music_enabled(),0, MNID_MUSIC);
    }
  else
    {
      options_menu->additem(MN_DEACTIVE,_("Sound     "), false,0, MNID_SOUND);
      options_menu->additem(MN_DEACTIVE,_("Music     "), false,0, MNID_MUSIC);
    }
  options_menu->additem(MN_TOGGLE,_("Show FPS  "),show_fps,0, MNID_SHOWFPS);
  options_menu->additem(MN_GOTO,_("Setup Keys"),0,options_keys_menu);

  if(use_joystick)
    options_menu->additem(MN_GOTO,_("Setup Joystick"),0,options_joystick_menu);

  options_menu->additem(MN_HL,"",0,0);
  options_menu->additem(MN_BACK,_("Back"),0,0);
  
  options_keys_menu->additem(MN_LABEL,_("Keyboard Setup"),0,0);
  options_keys_menu->additem(MN_HL,"",0,0);
  options_keys_menu->additem(MN_CONTROLFIELD_KB,_("Left move"), 0,0, 0,&keymap.left);
  options_keys_menu->additem(MN_CONTROLFIELD_KB,_("Right move"), 0,0, 0,&keymap.right);
  options_keys_menu->additem(MN_CONTROLFIELD_KB,_("Up/Activate"), 0,0, 0,&keymap.up);
  options_keys_menu->additem(MN_CONTROLFIELD_KB,_("Down/Duck"), 0,0, 0,&keymap.down);
  options_keys_menu->additem(MN_CONTROLFIELD_KB,_("Jump"), 0,0, 0,&keymap.jump);
  options_keys_menu->additem(MN_CONTROLFIELD_KB,_("Power/Run"), 0,0, 0,&keymap.power);
  options_keys_menu->additem(MN_HL,"",0,0);
  options_keys_menu->additem(MN_BACK,_("Back"),0,0);

  if(use_joystick)
    {
    options_joystick_menu->additem(MN_LABEL,_("Joystick Setup"),0,0);
    options_joystick_menu->additem(MN_HL,"",0,0);
    //options_joystick_menu->additem(MN_CONTROLFIELD_JS,"X axis", 0,0, 0,&joystick_keymap.x_axis);
    //options_joystick_menu->additem(MN_CONTROLFIELD_JS,"Y axis", 0,0, 0,&joystick_keymap.y_axis);
    options_joystick_menu->additem(MN_CONTROLFIELD_JS,_("A button"), 0,0, 0,&joystick_keymap.a_button);
    options_joystick_menu->additem(MN_CONTROLFIELD_JS,_("B button"), 0,0, 0,&joystick_keymap.b_button);
    //options_joystick_menu->additem(MN_CONTROLFIELD_JS,"Start", 0,0, 0,&joystick_keymap.start_button);
    //options_joystick_menu->additem(MN_CONTROLFIELD_JS,"DeadZone", 0,0, 0,&joystick_keymap.dead_zone);
    options_joystick_menu->additem(MN_HL,"",0,0);
    options_joystick_menu->additem(MN_BACK,_("Back"),0,0);
    }
  
  load_game_menu->additem(MN_LABEL,_("Start Game"),0,0);
  load_game_menu->additem(MN_HL,"",0,0);
  load_game_menu->additem(MN_DEACTIVE,"Slot 1",0,0, 1);
  load_game_menu->additem(MN_DEACTIVE,"Slot 2",0,0, 2);
  load_game_menu->additem(MN_DEACTIVE,"Slot 3",0,0, 3);
  load_game_menu->additem(MN_DEACTIVE,"Slot 4",0,0, 4);
  load_game_menu->additem(MN_DEACTIVE,"Slot 5",0,0, 5);
  load_game_menu->additem(MN_HL,"",0,0);
  load_game_menu->additem(MN_BACK,_("Back"),0,0);

  save_game_menu->additem(MN_LABEL,_("Save Game"),0,0);
  save_game_menu->additem(MN_HL,"",0,0);
  save_game_menu->additem(MN_DEACTIVE,"Slot 1",0,0, 1);
  save_game_menu->additem(MN_DEACTIVE,"Slot 2",0,0, 2);
  save_game_menu->additem(MN_DEACTIVE,"Slot 3",0,0, 3);
  save_game_menu->additem(MN_DEACTIVE,"Slot 4",0,0, 4);
  save_game_menu->additem(MN_DEACTIVE,"Slot 5",0,0, 5);
  save_game_menu->additem(MN_HL,"",0,0);
  save_game_menu->additem(MN_BACK,"Back",0,0);

  game_menu->additem(MN_LABEL,_("Pause"),0,0);
  game_menu->additem(MN_HL,"",0,0);
  game_menu->additem(MN_ACTION,_("Continue"),0,0,MNID_CONTINUE);
  game_menu->additem(MN_GOTO,_("Options"),0,options_menu);
  game_menu->additem(MN_HL,"",0,0);
  game_menu->additem(MN_ACTION,_("Abort Level"),0,0,MNID_ABORTLEVEL);

  worldmap_menu->additem(MN_LABEL,_("Pause"),0,0);
  worldmap_menu->additem(MN_HL,"",0,0);
  worldmap_menu->additem(MN_ACTION,_("Continue"),0,0, WorldMapNS::MNID_RETURNWORLDMAP);
  worldmap_menu->additem(MN_GOTO,_("Options"),0, options_menu);
  worldmap_menu->additem(MN_HL,"",0,0);
  worldmap_menu->additem(MN_ACTION,_("Quit Game"),0,0, WorldMapNS::MNID_QUITWORLDMAP);

  highscore_menu->additem(MN_TEXTFIELD,_("Enter your name:"),0,0);
}

/* Free menus */
void st_menu_free()
{
  delete worldmap_menu;
  delete main_menu;
  delete game_menu;
  delete options_menu;
  delete options_keys_menu;
  delete options_joystick_menu;
  delete highscore_menu;
  delete contrib_menu;
  delete contrib_subset_menu;
  delete save_game_menu;
  delete load_game_menu;
}
