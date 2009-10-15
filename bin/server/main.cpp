#include <iostream>

#include <boost/filesystem/operations.hpp>

#include <messaging/server.hpp>

#include "server.hpp"

int main(int const /*argc*/, char const* const* const argv)
{
  boost::filesystem::path program(argv[0]);
  auto dir = program.parent_path();
  auto ai_exe = dir / "ai_client" / "konig-ai_client";
  while (!boost::filesystem::exists(ai_exe)) {
    dir = dir.parent_path();
    if (dir.empty()) {
      std::cerr << "Warning: couldn't find AI executable anywhere\n";
      ai_exe = boost::filesystem::path();
      break;
    }
    ai_exe = dir / "ai_client" / "konig-ai_client";
  }
  boost::asio::io_service io;
  konig::server::Server server(io, std::cout, ai_exe);
  io.run();
}

