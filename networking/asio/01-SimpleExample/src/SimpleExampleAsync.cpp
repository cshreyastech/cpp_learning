#include <iostream>

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <chrono>

std::vector<char> vBuffer(20 * 1024);

void GrabSomeData(asio::ip::tcp::socket& socket)
{
  // Asyncronous read
  // socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);

  // As async nothing will happen right now. It will prime the context 
  // with some work to do when data will be available from that socket
  // to read.
  socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
    [&](std::error_code ec, std::size_t length)
    {

      if(!ec)
      {
        std::cout << "\n\nRead " << length << " bytes\n\n";

        for(std::size_t i = 0; i < length; i++)
          std::cout << vBuffer[i];

        // Not a recursive function. It will wait for data to arive.
        // This would implement the lamda function on arival of data.
        GrabSomeData(socket);
      }
    
    }
  );
}
int main()
{
  asio::error_code ec;

  // Create a "context" - essentially the platform specific interface
  asio::io_context context;

  // Give some fake task to asio so the context doest finish
  asio::io_context::work idleWork(context);

  // context blocks the main thead until it completes executing its instrution
  // So assign it to a seperate thread
  std::thread thrContext = std::thread([&]() { context.run(); });
  // thrContext.join();
  thrContext.detach();

  // Get the address of somewhere we wish to connect to
  //asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);
  // Website deliver more data
  asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", ec), 80);

  // Create a socket, the context will deliver the implementation
  asio::ip::tcp::socket socket(context);
  
  // Tesll socket to try and connect
  socket.connect(endpoint, ec);


  if(!ec)
  {
    std::cout << "Connected!" << std::endl;
  }
  else
  {
    std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
  }

  if(socket.is_open())
  {
    GrabSomeData(socket);

    std::string sRequest =
      "GET /index.html HTTP/1.1\r\n"
      "Host: example.com\r\n"
      "Connection: close\r\n\r\n";

    // Send as much of data as possible to server
    socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

    // Program does something else, while asio handles data transfer in background
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2000ms);

  }

  return 0;
}