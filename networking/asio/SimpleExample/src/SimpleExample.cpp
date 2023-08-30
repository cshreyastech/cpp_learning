#include <iostream>

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

int main()
{
  asio::error_code ec;

  // Create a "context" - essentially the platform specific interface
  asio::io_context context;

  // Get the address of somewhere we wish to connect to
  asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);

  return 0;
}