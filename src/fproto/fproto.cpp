#include "fproto.hpp"

namespace
{
using json = nlohmann::json;
}

Fproto::Fproto() : message_length(0)
{
    this->logger = spdlog::get("multi_logger");
}

void Fproto::init_header(
    std::array<unsigned char, FPROTO_HEADER_SIZE> header_bytes)
{
    this->message_length = static_cast<unsigned short>(
        ((header_bytes.at(0) << 8) | header_bytes.at(1)));
    this->logger->debug("Message length " +
                        std::to_string(this->message_length) + " bytes");
    if (this->message_length > FPROTO_MESSAGE_MAX) {
        throw std::runtime_error("Header length of " +
                                 std::to_string(this->message_length) +
                                 " exceeds maximum ");
    }
}

void Fproto::read_message(const std::vector<unsigned char> &message_bytes)
{
    if (!message_bytes.empty()) {
        std::string msg_string(message_bytes.begin(), message_bytes.end());
        this->logger->info("Parsed string \"" + msg_string + "\"");
        try {
            this->request = json::parse(msg_string);
            this->logger->debug("Successfully parsed json");
        } catch (const std::exception &e) {
            this->logger->error("Invalid json encoding");
            this->logger->error(e.what());
        }
    } else {
        this->logger->info("Received message is empty");
    }
}

unsigned short Fproto::get_message_length() const
{
    return this->message_length;
}
