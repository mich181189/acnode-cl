#include <networking/LWIPTasks.h>
#include <core/utils.h>

#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"

#include "ethernetif.h"

struct netif gnetif;
static ACNodeCore* gCore;

void dhcpTask(void *voidCore)
{
    ACNodeCore* core = static_cast<ACNodeCore*>(voidCore);

    LOG_INFO(core->logger, "Starting DHCP task");

    //for(;;) vTaskDelay(100);

    enum DHCPState
    {
        STATE_INIT,
        STATE_WAIT,
        STATE_IDLE,
    };

    DHCPState state = STATE_INIT;

    TickType_t lastCourseTimer = 0;
    TickType_t lastFineTimer = 0;

    for(;;)
    {
        if((xTaskGetTickCount() - lastCourseTimer) > DHCP_COARSE_TIMER_MSECS)
        {
            dhcp_coarse_tmr();
            lastCourseTimer = xTaskGetTickCount();
            logTaskStats(core->logger);
        }

        if((xTaskGetTickCount() - lastFineTimer) > DHCP_FINE_TIMER_MSECS)
        {
            dhcp_fine_tmr();
            lastFineTimer = xTaskGetTickCount();
        }

        switch(state)
        {
            case STATE_INIT:
            {
                if(netif_is_up(&gnetif))
                {
                    LOG_INFO(core->logger, "Network up, starting DHCP");
                    dhcp_start(&gnetif);
                    state = STATE_WAIT;
                }
            } break;
            case STATE_WAIT:
            {
                if(dhcp_supplied_address(&gnetif))
                {
                    union
                    {
                        struct
                        {
                            uint8_t a;
                            uint8_t b;
                            uint8_t c;
                            uint8_t d;
                        };
                        uint32_t full;
                    } ipaddr;
                    ipaddr.full = gnetif.ip_addr.u_addr.ip4.addr;

                    LOG_INFO(core->logger, "Lease acquired - %u.%u.%u.%u", ipaddr.a, ipaddr.b, ipaddr.c, ipaddr.d);
                    state = STATE_IDLE;
                    logTaskStats(core->logger);
                }
                if(!netif_is_up(&gnetif))
                {
                    LOG_INFO(core->logger, "Network down, stopping DHCP");
                    dhcp_stop(&gnetif);
                    state = STATE_INIT;
                    logTaskStats(core->logger);
                }
            } break;
            case STATE_IDLE:
            {
                if(!netif_is_up(&gnetif))
                {
                    LOG_INFO(core->logger, "Network down, stopping DHCP");
                    dhcp_stop(&gnetif);
                    state = STATE_INIT;
                    logTaskStats(core->logger);
                }
            } break;
        }
        vTaskDelay(500);
    }
}

void ethernet_link_status_updated(struct netif *netif)
{
    LOG_INFO(gCore->logger, "Link status changed");

    if(netif_is_up(netif))
    {
        LOG_INFO(gCore->logger, "Link is up");
    }
    else
    {
        LOG_WARN(gCore->logger, "Link is down");
    }
    logTaskStats(gCore->logger);
}

void initLWIP(ACNodeCore* core)
{
    gCore = core;
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;

    ip_addr_set_zero_ip4(&ipaddr);
    ip_addr_set_zero_ip4(&netmask);
    ip_addr_set_zero_ip4(&gw);

    LOG_INFO(core->logger, "Initialising TCP stack");
    tcpip_init(NULL, NULL);
    LOG_INFO(core->logger, "Initialising interface");
    netif_add(&gnetif, &ipaddr.u_addr.ip4, &netmask.u_addr.ip4, &gw.u_addr.ip4, NULL, &ethernetif_init, &tcpip_input);
    netif_set_default(&gnetif);
    LOG_INFO(core->logger, "Initialising IP6 auto config");
    netif_set_ip6_autoconfig_enabled(&gnetif, true);


    netif_set_link_callback(&gnetif, ethernet_link_status_updated);
    xTaskCreate(ethernet_link_thread, "Link Thread", configMINIMAL_STACK_SIZE*2, &gnetif, tskIDLE_PRIORITY+2, nullptr);
    xTaskCreate(dhcpTask, "DHCP", configMINIMAL_STACK_SIZE*2, core, tskIDLE_PRIORITY + 2, nullptr);
}