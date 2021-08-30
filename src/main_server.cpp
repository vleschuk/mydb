#include <iostream>
#include <string>
#include "server.h"

int main(int argc, char **argv)
{
  try
  {
    if (argc != 4)
    {
      std::cerr << "Usage: dbserver <address> <port> <dbfile>\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    receiver 0.0.0.0 4321 .\n";
      std::cerr << "  For IPv6, try:\n";
      std::cerr << "    receiver 0::0 4321 .\n";
      return 1;
    }

    my::DBServer s(argv[1], argv[2], argv[3]);

    s.run();
  }
  catch (const std::exception &e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }
}
