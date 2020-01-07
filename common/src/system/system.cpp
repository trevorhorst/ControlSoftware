#include "common/system/system.h"

/**
 * @brief Constructor
 */
System::System()
{
}

/**
 * @brief Gets the hostname of the system
 * @return char* Hostname
 */
const char *System::getHostname()
{
    gethostname( mHostname, SYSTEM_HOSTNAME_SIZE_MAX );
    return mHostname;
}

uint32_t System::getInterfaces()
{
    uint32_t error = Error::Code::NONE;

    struct ifaddrs *ifaddr = nullptr;
    struct ifaddrs *ifa = nullptr;

    if( getifaddrs( &ifaddr ) < 0 ) {
         LOG_WARN( "failed to get ifaddrs - %s", strerror( errno ) );
         error = Error::Code::PARAM_ACCESS_DENIED;
    }

    if( error == Error::Code::NONE ) {
        for( auto it = ifaddr; it != nullptr; it = it->ifa_next ) {
            if( it->ifa_addr != nullptr ) {

                int32_t family = it->ifa_addr->sa_family;

                printf("%-8s %s (%d)\n",
                       it->ifa_name,
                       (family == AF_PACKET) ? "AF_PACKET" :
                       (family == AF_INET) ? "AF_INET" :
                       (family == AF_INET6) ? "AF_INET6" : "???",
                       family);
                char host[ SYSTEM_HOSTNAME_SIZE_MAX ];

                if (family == AF_INET || family == AF_INET6) {
                    int32_t s = getnameinfo(ifa->ifa_addr,
                            (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                                  sizeof(struct sockaddr_in6),
                            host, NI_MAXHOST,
                            nullptr, 0, NI_NUMERICHOST);
                    if (s != 0) {
                        printf("getnameinfo() failed: %s\n", gai_strerror(s));
                        exit(EXIT_FAILURE);
                    }

                    printf("\t\taddress: <%s>\n", host);

                } else if (family == AF_PACKET && ifa->ifa_data != nullptr) {
                    struct rtnl_link_stats *stats = reinterpret_cast< rtnl_link_stats* >( ifa->ifa_data );

                    printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
                           "\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
                           stats->tx_packets, stats->rx_packets,
                           stats->tx_bytes, stats->rx_bytes);
                }
            }

        }
    }

    return error;
}

/**
 * @brief Sets the hostname of the system
 * @param hostname Desired hostname
 * @return Error code
 */
uint32_t System::setHostname(const char *hostname)
{
    uint32_t error = Error::Code::NONE;

    if( sethostname(hostname, strlen(hostname)) < 0 ) {
        LOG_WARN( "failed to set hostname - %s", strerror( errno ) );
        error = Error::Code::PARAM_ACCESS_DENIED;
    }

    return error;
}
