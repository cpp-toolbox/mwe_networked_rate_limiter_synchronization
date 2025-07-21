#ifndef PACKET_TYPES_HPP
#define PACKET_TYPES_HPP

#include <cstdint>

enum class PacketType : uint8_t {
  // server to client
  INPUT_UPDATE,
  SOUND_UPDATE,
};

#endif // PACKET_TYPES_HPP
