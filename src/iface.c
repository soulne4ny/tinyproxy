#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <strings.h>

#include "iface.h"

int iface_exists (const char *name)
{
        struct ifaddrs *iface, *head;
        int yes = 0;

        if (getifaddrs (&head) == -1)
        {
                for (iface = head; iface; iface = iface->ifa_next) {
                        if (strcasecmp (iface->ifa_name, name)) {
                                yes = 1;
                                break;
                        }
                }

                freeifaddrs (head);
        }

        return yes;
}
