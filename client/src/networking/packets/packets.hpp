#ifndef PACKETS_HPP
#define PACKETS_HPP

#include "../../sound/sound_types/sound_types.hpp"
#include "../packet_data/packet_data.hpp"

#include <iostream>

struct MouseUpdate {
  unsigned int mouse_pos_update_number;
  unsigned int last_applied_game_update_number;
  double x_pos;
  double y_pos;
  bool fire_pressed;
  double sensitivity;

  friend std::ostream &operator<<(std::ostream &os, const MouseUpdate &mu) {
    os << "MouseUpdate { "
       << "update_number: " << mu.mouse_pos_update_number << ", "
       << "x_pos: " << mu.x_pos << ", "
       << "y_pos: " << mu.y_pos << ", "
       << "fire_pressed: " << std::boolalpha << mu.fire_pressed << ", "
       << "sensitivity: " << mu.sensitivity << " }";
    return os;
  }
};

struct GameUpdate {
  unsigned int last_processed_mouse_pos_update_number;
  unsigned int update_number;
  double yaw;
  double pitch;
  double target_x_pos;
  double target_y_pos;
  double target_z_pos;

  friend std::ostream &operator<<(std::ostream &os, const GameUpdate &gu) {
    os << "GameUpdate { "
       << "last_update_number: " << gu.last_processed_mouse_pos_update_number
       << "update_number: " << gu.update_number << ", "
       << "yaw: " << gu.yaw << ", "
       << "pitch: " << gu.pitch << ", "
       << "target_x: " << gu.target_x_pos << ", "
       << "target_y: " << gu.target_y_pos << ", "
       << "target_z: " << gu.target_z_pos << " }";
    return os;
  }
};

struct SoundUpdate {
  SoundType sound_to_play;
  double x;
  double y;
  double z;
};

struct MouseUpdatePacket {
  PacketHeader header;
  MouseUpdate mouse_update;
};

struct GameUpdatePacket {
  PacketHeader header;
  GameUpdate game_update;
};

struct SoundUpdatePacket {
  PacketHeader header;
  SoundUpdate sound_update;
};

#endif // PACKETS_HPP
