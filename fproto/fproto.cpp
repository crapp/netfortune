#include "fproto.hpp"

Fproto::Fproto() : message_length(0) {}
Fproto::~Fproto() {}

void Fproto::init_header(std::array<unsigned char, 2> header_bytes)
{
    this->message_length = static_cast<unsigned short>(
        ((header_bytes.at(0) << 8) | header_bytes.at(1)));
    if (this->message_length > FPROTO_MESSAGE_MAX)
        throw std::runtime_error("Header length of " +
                                 std::to_string(this->message_length) +
                                 " exceeds maximum ");
}
