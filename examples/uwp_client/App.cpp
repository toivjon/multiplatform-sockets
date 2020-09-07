using namespace Platform;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::System;
using namespace Windows::UI::Core;

#include "mps.h"

using namespace mps;

void log(const char* fmt, ...) {
  char s[1024];
  va_list args;
  va_start(args, fmt);
  vsprintf_s(s, fmt, args);
  va_end(args);
  OutputDebugStringA(s);
}

ref class App sealed : public IFrameworkView, IFrameworkViewSource
{
public:
  virtual void Initialize(CoreApplicationView^ applicationView) {
    // ... nothing
  }
  virtual void SetWindow(CoreWindow^ window) {
    // ... nothing 
  }
  virtual void Load(String^ entryPoint) {
    // ... nothing
  }
  virtual void Run() {
    try {
      // TODO from arguments?
      auto addr = Address("xxx.xxx.xxx.xxx", 56789);

      // build and bind a new UDP socket and print out the socket info.
      UDPSocket socket(addr.getFamily());
      log("Bound a new UDP socket with following details:\n");
      log("    local-ip: %s\n", socket.getLocalIP().c_str());
      log("  local-port: %d\n", socket.getLocalPort());
      log("    blocking: %d\n", socket.isBlocking());
      log("     routing: %d\n", socket.isRouting());
      log("broadcasting: %d\n", socket.isBroadcasting());
      log(" recvBufSize: %d\n", socket.getReceiveBufferSize());
      log(" sendBufSize: %d\n", socket.getSendBufferSize());

      // send a simple datagram to the specified remote host IP address and port.
      log("Sending a simple UDP message to remote target:\n");
      log("  remote-ip: %s\n", addr.getIP().c_str());
      log("remote-port: %d\n", addr.getPort());
      log("       data: ping\n");
      socket.send(UDPPacket{ addr, {'p','i','n','g','\0'} });

      // wait for the incoming data by blocking and the print datagram info.
      log("Waiting for the remote target to echo back...\n");
      auto packet = socket.receive();
      log("Received a UDP datagram with following details:\n");
      log("  remote-ip: %s\n", packet.address.getIP().c_str());
      log("remote-port: %d\n", packet.address.getPort());
      log("       data: %s\n", &packet.data[0]);
    } catch (const AddressException& e) {
      log("Error: %s\n", e.what());
    } catch (const SocketException& e) {
      log("Error: %s\n", e.what());
    }
  }
  virtual void Uninitialize() {
    // ... nothing
  }
  virtual IFrameworkView^ CreateView() {
    return ref new App();
  }
};

[MTAThread]
int main(Array<String^>^) {
  CoreApplication::Run(ref new App());
  return 0;
}
