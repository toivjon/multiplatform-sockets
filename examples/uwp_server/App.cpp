#include "mps.h"

using namespace Platform;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::System;
using namespace Windows::UI::Core;

using namespace mps;

constexpr auto SocketAddressFamily = AddressFamily::IPv4;
constexpr auto SocketPort = 56789;

// a utility to help to print out formatted message to debug output.
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
  // ==========================================================================
  
  virtual void Initialize(CoreApplicationView^ applicationView) {}
  virtual void SetWindow(CoreWindow^ window) {}
  virtual void Load(String^ entryPoint) {}
  virtual void Uninitialize() {}

  // ==========================================================================

  virtual void Run() {
    try {
      // build and bind a new UDP socket and print out the socket info.
      UDPSocket socket(SocketAddressFamily, SocketPort);
      log("Bound a new UDP socket with following details:\n");
      log("    local-ip: %s\n", socket.getLocalIP().c_str());
      log("  local-port: %d\n", socket.getLocalPort());
      log("    blocking: %d\n", socket.isBlocking());
      log("     routing: %d\n", socket.isRouting());
      log("broadcasting: %d\n", socket.isBroadcasting());
      log(" recvBufSize: %d\n", socket.getReceiveBufferSize());
      log(" sendBufSize: %d\n", socket.getSendBufferSize());

      // wait for incoming data by blocking and then print datagram info.
      log("Waiting for an incoming message...\n");
      auto packet = socket.receive();
      log("Received a UDP datagram with following details:\n");
      log("  remote-ip: %s\n", packet.address.getIP().c_str());
      log("remote-port: %d\n", packet.address.getPort());
      log("       data: %s\n", &packet.data[0]);

      // echo the received data back to caller to inform that we did receive it.
      socket.send(packet);
    } catch (const AddressException& e) {
      log("Error: %s\n", e.what());
    } catch (const SocketException& e) {
      log("Error: %s\n", e.what());
    }
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
