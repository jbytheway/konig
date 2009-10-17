#include "aiclient.hpp"

namespace konig { namespace ai_client {

AiClient::AiClient(konig::ai::Ai& a, bool debug, TablePosition position) :
  ai_(a),
  debug_(debug),
  position_(position),
  server_interface_(NULL)
{
}

void AiClient::message(std::string const& s)
{
  if (debug_)
    std::cout << "ai: " << s << std::endl;
}

void AiClient::warning(std::string const& s)
{
  std::cout << "ai: warning: " << s << std::endl;
}

void AiClient::abort()
{
  message("Network instigated abort");
}

Player& AiClient::player()
{
  return ai_;
}

void AiClient::set_server_interface(client::ServerInterface& si)
{
  assert(!server_interface_);
  server_interface_ = &si;
}

void AiClient::unset_server_interface()
{
  assert(server_interface_);
  server_interface_ = NULL;
}

}}

