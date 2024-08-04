#ifndef CUSTOM_TCP_SOCKET_BASE_H
#define CUSTOM_TCP_SOCKET_BASE_H


#include "ns3/tcp-socket-base.h"

namespace ns3 {

class CustomTcpSocketBase : public TcpSocketBase
{

public:
    CustomTcpSocketBase() {
        std::cout << "CustomTcpSocketBase constructor" << std::endl;
        TcpSocketBase();
    }
    static TypeId GetTypeId(void) {
            static TypeId tid = 
                TypeId("CustomTcpSocketBase")
                .SetParent<TcpSocketBase>()
                .AddConstructor<CustomTcpSocketBase>();

            return tid;
        }

    /**
     * \brief Close the socket.
     *
     * Override the base class Close() method so that no RST is sent
     *
     * \return 0 if the socket was successfully closed, -1 if the socket
     * was not closed.
     */
    
    int Close() override
    {
        // if (m_tcb->m_rxBuffer->Size() != 0)
        // {
        //     NS_LOG_WARN("Socket " << this << " << unread rx data during close.  Sending reset."
        //                         << "This is probably due to a bad sink application; check its code");
        //     // SendRST();
        //     return 0;
        // }

        // if (m_txBuffer->SizeFromSequence(m_tcb->m_nextTxSequence) > 0)
        // { // App close with pending data must wait until all data transmitted
        //     if (m_closeOnEmpty == false)
        //     {
        //         m_closeOnEmpty = true;
        //         NS_LOG_INFO("Socket " << this << " deferring close, state " << TcpStateName[m_state]);
        //     }
        //     return 0;
        // }
        return DoClose();
    }



};

} // namespace ns3

#endif /* CUSTOM_TCP_SOCKET_BASE_H */