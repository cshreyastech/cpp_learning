#ifndef NET_CONNECTION_H
#define NET_CONNECTION_H

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"

namespace olc
{
  namespace net
  {
    template<typename T>
    // Create shared pointer within this object
    class connection : public std::enable_shared_from_this<connection<T>>
    {
    public:
      connection()
      {}
      virtual ~connection()
      {}

    public:
      bool ConnectToServer();
      bool Disconnect();
      bool IsConnected() const;

    public:
      bool (const message<T>& msg);

    protected:
      // Each connection has a unique socket to a remote
      asio::ip::tcp::socket m_socket;

      // This context is shared with the whole aiso instance
      // server can have multiple connection but we want it to have single context
      asio::io_context& m_asioContext;

      // This queue hold all messages to be sent to the remote side
      tsqueue<message<T>> m_qMessagesOut;

      // This queue hold all messages that have been recieved from 
      // the remote side of this connection. Note it is a reference
      // as the owner of this connection is expected to provide a queue
      tsqueue<owned_message>& m_qMessagesIn;
    };

  }
}

#endif