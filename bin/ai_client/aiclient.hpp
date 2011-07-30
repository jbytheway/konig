#ifndef KONIG_AI_CLIENT_AICLIENT_HPP
#define KONIG_AI_CLIENT_AICLIENT_HPP

#include <boost/noncopyable.hpp>

#include <konig/tableposition.hpp>
#include <konig/client/clientinterface.hpp>
#include <konig/client/serverinterface.hpp>
#include <konig/ai/ai.hpp>

namespace konig { namespace ai_client {

class AiClient : public client::ClientInterface, boost::noncopyable {
  public:
    AiClient(konig::ai::Ai&, bool debug, TablePosition position);
    virtual void message(std::string const&);
    virtual void warning(std::string const&);
    virtual void abort();
    virtual Player& player();
    virtual void id_known(ClientId);
    void set_server_interface(client::ServerInterface&);
    void unset_server_interface();
  private:
    ai::Ai& ai_;
    bool debug_;
    TablePosition position_;
    client::ServerInterface* server_interface_;
};

}}

#endif // KONIG_AI_CLIENT_AICLIENT_HPP

