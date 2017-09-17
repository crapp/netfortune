// netfortune session
// Copyright © 2017 Christian Rapp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "fsession.hpp"

namespace bas = boost::asio;

FSession::FSession(bas::ip::tcp::socket socket)
	: socket(std::move(socket)), nfprot(nullptr)
{
	this->logger = spdlog::get("multi_logger");
	this->logger->debug("FSession created");
}

void FSession::start()
{
	this->logger->debug("Session started");
	this->do_read();
}

void FSession::do_read()
{
	// https://stackoverflow.com/a/22291720
	// https://stackoverflow.com/a/25687309/1127601
	this->data_buf.clear();
	this->data_buf.resize(FPROTO_HEADER_SIZE);
	auto self = shared_from_this();
	this->logger->debug("FSession::do_read");
	// TODO: Simplify this code. Don't like these nested calls
	bas::async_read(
		this->socket, bas::buffer(this->data_buf, this->data_buf.size()),
		[this, self](boost::system::error_code ec, size_t bytes) {
			if (!ec) {
				this->nfprot = std::make_unique<Fproto>();
				try {
					this->nfprot->init_header(
						{{this->data_buf.at(0), this->data_buf.at(1)}});
					this->data_buf.clear();
					this->data_buf.resize(this->nfprot->get_message_length());
					bas::async_read(
						this->socket,
						bas::buffer(this->data_buf, this->data_buf.size()),
						[this, self](boost::system::error_code ec,
									 size_t bytes) {
							if (!ec) {
								try {
									this->nfprot->read_message(this->data_buf);
								} catch (const std::runtime_error &err) {
									this->logger->error(err.what());
								}
							}
						});
				} catch (const std::out_of_range &ex) {
					this->logger->error("CAN NOT PARSE HEADER BYTES");
					return;
				} catch (const std::runtime_error &err) {
					this->logger->error(err.what());
					return;
				}
			} else {
				this->logger->debug("No data can be read in this session");
			}
		});
}

void FSession::do_write() {}
