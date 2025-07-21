#ifndef PACKETS_HPP
#define PACKETS_HPP

#include "../../sound/sound_types/sound_types.hpp"
#include "../packet_data/packet_data.hpp"

#include <iostream>

struct InputUpdate {
  bool activate;
};

struct SoundUpdate {
  SoundType sound_to_play;
};

struct InputUpdatePacket {
  PacketHeader header;
  InputUpdate input_update;
};

struct SoundUpdatePacket {
  PacketHeader header;
  SoundUpdate sound_update;
};

#endif // PACKETS_HPP
