#include <iostream>

#include "networking/server_packet_network/server_packet_network.hpp"
#include "networking/packets/packets.hpp"

#include "utility/fixed_frequency_loop/fixed_frequency_loop.hpp"
#include "utility/logger/logger.hpp"
#include "utility/rate_limiter/rate_limiter.hpp"

int main() {
    ServerPacketNetwork server_packet_network;
    server_packet_network.network.logger.disable_level(LogLevel::Info);
    FixedFrequencyLoop ffl;
    RateLimiter activate_rate_limiter(1);

    std::vector<InputUpdate> input_updates_since_last_tick;
    std::function<void(const void *)> input_update_handler = [&](const void *data) {
        const InputUpdatePacket *packet = reinterpret_cast<const InputUpdatePacket *>(data);
        InputUpdate just_received_input_update = packet->input_update;
        input_updates_since_last_tick.push_back(just_received_input_update);
    };

    server_packet_network.packet_handler.register_handler(PacketType::INPUT_UPDATE, input_update_handler);

    auto term = []() { return false; };
    auto tick = [&](double dt) {
        server_packet_network.tick();

        for (const auto &iu : input_updates_since_last_tick) {
            if (iu.activate) {
                if (activate_rate_limiter.attempt_to_run()) {
                    SoundUpdate su(SoundType::SERVER_ACTIVATED);
                    SoundUpdatePacket sup;
                    sup.header.type = PacketType::SOUND_UPDATE;
                    sup.header.size_of_data_without_header = sizeof(SoundUpdate);
                    sup.sound_update = su;
                    server_packet_network.network.reliable_broadcast(&sup, sizeof(SoundUpdatePacket));
                    std::cout << "server activate" << std::endl;
                }
            }
            input_updates_since_last_tick.clear();
        }
    };

    ffl.start(tick, term);

    return 0;
}
