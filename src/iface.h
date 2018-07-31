#ifndef TINYPROXY_IFACE_H
#define TINYPROXY_IFACE_H

/* CURL */
/* IPv6 address scopes. */
#define IPV6_SCOPE_GLOBAL       0       /* Global scope. */
#define IPV6_SCOPE_LINKLOCAL    1       /* Link-local scope. */
#define IPV6_SCOPE_SITELOCAL    2       /* Site-local scope (deprecated). */
#define IPV6_SCOPE_UNIQUELOCAL  3       /* Unique local */
#define IPV6_SCOPE_NODELOCAL    4       /* Loopback. */

unsigned int ipv6_scope (const struct sockaddr *sa);
char *iface_address (const char *name, int af, unsigned int remote_scope);

#endif
