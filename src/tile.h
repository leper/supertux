//  $Id$
// 
//  SuperTux
//  Copyright (C) 2004 Tobias Glaesser <tobi.web@gmx.de>
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
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.
#ifndef TILE_H
#define TILE_H

#include <vector>
#include "video/surface.h"
#include "math/rectangle.h"
#include "lisp/lisp.h"

using namespace SuperTux;

/**
Tile Class
*/
class Tile
{
public:
  /// bitset for tile attributes
  enum {
    /** solid tile that is indestructable by Tux */                         
    SOLID     = 0x0001,
    /** uni-directional solid tile */
    UNISOLID  = 0x0002,
    /** a brick that can be destroyed by jumping under it */
    BRICK     = 0x0004,
    /** an ice brick that makes tux sliding more than usual */
    ICE       = 0x0008,
    /** a water tile in which tux starts to swim */                         
    WATER     = 0x0010,
    /** a tile that hurts the player if he touches it */
    SPIKE     = 0x0020,
    /** Bonusbox, content is stored in \a data */
    FULLBOX   = 0x0040,
    /** Tile is a coin */
    COIN      = 0x0080,
    /** the level should be finished when touching a goaltile.
     * if data is 0 then the endsequence should be triggered, if data is 1
     * then we can finish the level instantly.
     */
    GOAL      = 0x0100,
    /** slope tile */
    SLOPE     = 0x0200,
  };

  /// worldmap flags
  enum {
    WORLDMAP_NORTH = 0x0001,
    WORLDMAP_SOUTH = 0x0002,
    WORLDMAP_EAST  = 0x0004,
    WORLDMAP_WEST  = 0x0008,
    
    WORLDMAP_STOP  = 0x0010
  };
  
private:
  unsigned int id;

  struct ImageSpec {
    ImageSpec(const std::string& newfile, const Rectangle& newrect)
      : file(newfile), rect(newrect)
    { }

    std::string file;
    Rectangle rect;
  };
  std::vector<ImageSpec> imagespecs;
  std::vector<Surface*> images;

  std::string editor_imagefile;
  Surface* editor_image;
  
  /** tile attributes */
  Uint32 attributes;
  
  /** General purpose data attached to a tile (content of a box, type of coin)*/
  int data;

  float anim_fps;

public:
  ~Tile();
  
  /** Draw a tile on the screen */
  void draw(DrawingContext& context, const Vector& pos, int layer) const;

  Surface* get_editor_image() const;

  unsigned int getID() const
  { return id; }

  Uint32 getAttributes() const
  { return attributes; }

  int getData() const
  { return data; }

  /// returns the width of the tile in pixels
  int getWidth() const
  { 
    if(!images.size())
      return 0;
    return images[0]->w;
  }

  /// returns the height of the tiles in pixels
  int getHeight() const
  {
    if(!images.size())
      return 0;
    return images[0]->h;
  }

protected:
  friend class TileManager;
  Tile();

  void load_images(const std::string& tilesetpath);

  /// parses the tile and returns it's id number
  void parse(const lisp::Lisp& reader);
  void parse_images(const lisp::Lisp& cur);
};

#endif
