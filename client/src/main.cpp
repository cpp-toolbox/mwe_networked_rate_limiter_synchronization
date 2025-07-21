#include <iostream>

#include "networking/client_packet_network/client_packet_network.hpp"
#include "networking/packets/packets.hpp"

#include "sound/sound_types/sound_types.hpp"

#include "utility/fixed_frequency_loop/fixed_frequency_loop.hpp"
#include "utility/argument_parser/argument_parser.hpp"
#include "utility/logger/logger.hpp"
#include "utility/rate_limiter/rate_limiter.hpp"
#include "utility/timer/timer.hpp"

#include "sound/sound_system/sound_system.hpp"

int main(int argc, char **argv) {
    ArgumentParser parser("client");

    parser.add_argument("ip", "ip address of server we want to connect to", false, "localhost");
    parser.parse(argc, argv);

    ClientPacketNetwork client_packet_network(parser.get("ip"));
    client_packet_network.network.logger.disable_level(LogLevel::Info);

    FixedFrequencyLoop ffl;
    SoundSystem sound_system(100, {{SoundType::CLIENT_ACTIVATED, "assets/client_activate.wav"},
                                   {SoundType::SERVER_ACTIVATED, "assets/server_activate.wav"}});

    RateLimiter fake_input_rate_limiter(0.25);
    Timer fake_input_press_timer(3);

    RateLimiter activate_rate_limiter(1);

    std::function<void(const void *)> sound_update_handler = [&](const void *data) {
        const SoundUpdatePacket *packet = reinterpret_cast<const SoundUpdatePacket *>(data);
        SoundUpdate just_received_sound_update = packet->sound_update;
        sound_system.queue_sound(just_received_sound_update.sound_to_play);
    };

    client_packet_network.packet_handler.register_handler(PacketType::SOUND_UPDATE, sound_update_handler);

    // NOTE: always holding it down to fire as fast as possible.
    bool activate_pressed = true;

    Timer initial_wait_timer(2, true);

    auto tick = [&](double dt) {
        if (initial_wait_timer.time_up()) { // don't do anything until time up

            // NOTE: only starting a fake press at a specific rate
            if (fake_input_rate_limiter.attempt_to_run()) {
                fake_input_press_timer.start();
            }

            // NOTE: simulating a fake input for this whole press duration
            activate_pressed = not fake_input_press_timer.time_up();
            std::cout << "client activated: " << activate_pressed << std::endl;

            if (activate_pressed) {
                if (activate_rate_limiter.attempt_to_run()) {

                    std::cout << "playing client sound" << std::endl;
                    sound_system.queue_sound(SoundType::CLIENT_ACTIVATED);
                }
            }

            InputUpdate iu(activate_pressed);
            InputUpdatePacket iup;
            iup.header.type = PacketType::INPUT_UPDATE;
            iup.header.size_of_data_without_header = sizeof(InputUpdate);
            iup.input_update = iu;
            client_packet_network.network.send_packet(&iup, sizeof(InputUpdatePacket));
        }

        client_packet_network.tick();
        sound_system.play_all_sounds();
    };
    auto term = []() { return false; };

    ffl.start(tick, term);

    return 0;
}
