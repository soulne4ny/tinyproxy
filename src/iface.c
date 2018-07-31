#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "iface.h"
#include "heap.h"

/* Return the scope of the given address. */
/*
 * unsigned int Curl_ipv6_scope(const struct sockaddr *sa)
 */
unsigned int ipv6_scope (const struct sockaddr *sa)
{
        if (sa->sa_family == AF_INET6) {
                const struct sockaddr_in6 *sa6 =
                    (const struct sockaddr_in6 *) (const void *) sa;
                const unsigned char *b = sa6->sin6_addr.s6_addr;
                unsigned short w = (unsigned short) ((b[0] << 8) | b[1]);

                if ((b[0] & 0xFE) == 0xFC)      /* Handle ULAs */
                        return IPV6_SCOPE_UNIQUELOCAL;
                switch (w & 0xFFC0) {
                case 0xFE80:
                        return IPV6_SCOPE_LINKLOCAL;
                case 0xFEC0:
                        return IPV6_SCOPE_SITELOCAL;
                case 0x0000:
                        w = b[1] | b[2] | b[3] | b[4] | b[5] | b[6] | b[7] |
                            b[8] | b[9] | b[10] | b[11] | b[12] | b[13] | b[14];
                        if (w || b[15] != 0x01)
                                break;
                        return IPV6_SCOPE_NODELOCAL;
                default:
                        break;
                }
        }

        return IPV6_SCOPE_GLOBAL;
}

/* if2ip_result_t Curl_if2ip(int af, unsigned int remote_scope,
 *                           unsigned int remote_scope_id, const char *interf,
 *                           char *buf, int buf_size)
 */
char *iface_address (const char *name, int af, unsigned int remote_scope)
{
        struct ifaddrs *iface, *head;
        void *addr;
        char ipstr[INET6_ADDRSTRLEN + 32] = "";

        if (getifaddrs (&head) == -1)
                return 0;

        for (iface = head; iface; iface = iface->ifa_next) {
                if (!iface->ifa_addr)
                        continue;
                if (iface->ifa_addr->sa_family != af)
                        continue;
                if (strcasecmp (iface->ifa_name, name))
                        continue;

                if (af == AF_INET6) {
                        if (ipv6_scope (iface->ifa_addr) != remote_scope)
                                continue;

                        addr = &((struct sockaddr_in6 *) (void *)
                                 iface->ifa_addr)->sin6_addr;
                } else
                        addr = &((struct sockaddr_in *) (void *)
                                 iface->ifa_addr)->sin_addr;

                inet_ntop (af, addr, ipstr, sizeof ipstr);
                strncat (ipstr, "%", sizeof ipstr);
                strncat (ipstr, name, sizeof ipstr);
                break;
        }

        freeifaddrs (head);
        return *ipstr ? safestrdup (ipstr) : 0;
}
