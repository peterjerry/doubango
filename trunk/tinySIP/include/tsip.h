/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
*	
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*	
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip.h
 * @brief SIP (RFC 3261) and 3GPP IMS (TS 24.229) implementation.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_TSIP_H
#define TINYSIP_TSIP_H

#include "tinysip_config.h"

#include "tinysip/tsip_operation.h"
#include "tinysip/tsip_timers.h"

#include "tnet_socket.h"

#include "tsk_runnable.h"

TSIP_BEGIN_DECLS

#define TSIP_IARI_GSMAIS				"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-is"
#define TSIP_IARI_QUOTED_GSMAIS			"\""TSIP_IARI_GSMAIS"\""
#define TSIP_ICSI_MMTEL_PSVOICE			"urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel"
#define TSIP_ICSI_QUOTED_MMTEL_PSVOICE	"\""TSIP_ICSI_MMTEL_PSVOICE"\""

typedef uint8_t amf_t[2];
typedef uint8_t operator_id_t[16];
typedef void tsip_stack_handle_t;

#define TSIP_STACK(self)		((tsip_stack_t*)(self))

typedef enum tsip_stack_param_type_e
{
	/* Identity */
	pname_display_name,
	pname_public_identity,
	pname_preferred_identity,
	pname_private_identity,
	pname_password,
#define TSIP_STACK_SET_DISPLAY_NAME(STR)				pname_display_name, (const char*)STR
#define TSIP_STACK_SET_PUBLIC_IDENTITY(URI_STR)			pname_public_identity, (const char*)URI_STR
#define TSIP_STACK_SET_PREFERRED_IDENTITY(URI_STR)		pname_preferred_identity, (const char*)URI_STR
#define TSIP_STACK_SET_PRIVATE_IDENTITY(STR)			pname_private_identity, (const char*)STR
#define TSIP_STACK_SET_PASSWORD(STR)					pname_password, (const char*)STR

	/* Network */
	pname_local_ip,
	pname_local_port,
	pname_privacy,
	pname_operator_id,
	pname_amf,
	pname_netinfo,
	pname_realm,
	pname_proxy_cscf,
	pname_proxy_cscf_port,
	pname_device_id,
	pname_mobility,
	pname_sec_agree_mech,
#define TSIP_STACK_SET_LOCAL_IP(STR)											pname_local_ip, (const char*)STR
#define TSIP_STACK_SET_LOCAL_PORT(INT)											pname_local_port, (int)INT
#define TSIP_STACK_SET_PRIVACY(STR)												pname_privacy, (const char*)STR
#define TSIP_STACK_SET_POPERATOR_ID(OPERATOR_ID)								pname_privacy, (operator_id_t)OPERATOR_ID
#define TSIP_STACK_SET_AMF(AMF)													pname_amf, (amf_t)AMF
#define TSIP_STACK_SET_NETINFO(STR)												pname_netinfo, (const char*)STR
#define TSIP_STACK_SET_REALM(FQDN_STR)											pname_realm, (const char*)FQDN_STR
#define TSIP_STACK_SET_PROXY_CSCF(FQDN_STR, TRANSPORT_STR, USE_IPV6_INT)		pname_proxy_cscf, (const char*)FQDN_STR, (const char*)TRANSPORT_STR, (int)USE_IPV6_INT
#define TSIP_STACK_SET_PROXY_CSCF_PORT(UINT16)									pname_proxy_cscf_port, (uint16_t)UINT16
#define TSIP_STACK_SET_DEVICE_ID(UUID_STR)										pname_device_id, (const char*)UUID_STR
#define TSIP_STACK_SET_MOBILITY(MOB_STR)										pname_mobility, (const char*)MOB_STR
#define TSIP_STACK_SET_SEC_AGREE_MECH(MECH_STR)									pname_sec_agree_mech, (const char*)MECH_STR
	/* Features */
	pname_enable_100rel,
	pname_enable_gsmais,
	pname_enable_precond,
	pname_enable_3gppsms,
	pname_enable_gsmarcs,
	pname_enable_earlyIMS,
	pname_enable_ofdr,
	pname_enable_aa,
	pname_enable_dnd,
	pname_enable_option,

	/* QoS */

	pname_null,
#define TSIP_STACK_SET_NULL()							pname_null
}
tsip_stack_param_type_t;


typedef enum tsip_event_type_e
{
	tsip_event_ack,
	tsip_event_bye,
	tsip_event_cancel,
	tsip_event_invite,
	tsip_event_message,
	tsip_event_notify,
	tsip_event_options,
	tsip_event_prack,
	tsip_event_publish,
	tsip_event_refer,
	tsip_event_register,
	tsip_event_subscribe,
	tsip_event_update,

	tsip_event_unpublish,
	tsip_event_unregister,
	tsip_event_unsubscribe,

}
tsip_event_type_t;

typedef struct tsip_event_s
{
	TSK_DECLARE_OBJECT;

	const tsip_stack_handle_t * stack;
	tsip_operation_id_t opid;

	short status_code;
	char *reason_phrase;

	unsigned incoming:1;
	tsip_event_type_t type;
}
tsip_event_t;
TINYSIP_GEXTERN const void *tsip_event_def_t;


typedef int (*tsip_stack_callback)(const tsip_event_t *sipevent);



typedef struct tsip_stack_s
{
	TSK_DECLARE_RUNNABLE;

	tsip_stack_callback callback;

	/* Identity */
	char* display_name;
	struct tsip_uri_s *public_identity;
	struct tsip_uri_s *preferred_identity;
	char *private_identity;
	char *password;

	/* Network */
	char *local_ip;
	uint16_t local_port;
	char *privacy;
	operator_id_t operator_id;
	amf_t amf;
	char *netinfo;
	struct tsip_uri_s *realm;
	char *proxy_cscf;
	int proxy_cscf_port;
	tnet_socket_type_t proxy_cscf_type;
	char* device_id;
	char* mobility;
	char* sec_agree_mech;

	/* features */
	unsigned enable_100rel:1;
	unsigned enable_gsmais:1;
	unsigned enable_precond:1;
	unsigned enable_3gppsms:1;
	unsigned enable_gsmarcs:1;
	unsigned enable_earlyIMS:1;
	unsigned enable_ofdr:1;
	unsigned enable_aa:1;
	unsigned enable_dnd:1;
	unsigned enable_option:1;

	/* QoS */

	/* Internals. */
	tsk_timer_manager_handle_t* timer_mgr;
	tsip_timers_t timers;
	tsip_operations_L_t *operations;

	/* Layers */
	struct tsip_dialog_layer_s *layer_dialog;
	struct tsip_transac_layer_s *layer_transac;
	struct tsip_transport_layer_s *layer_transport;
}
tsip_stack_t;







TINYSIP_API int tsip_global_init();
TINYSIP_API int tsip_global_deinit();

TINYSIP_API tsip_stack_handle_t *tsip_stack_create(tsip_stack_callback callback, ...);
TINYSIP_API int tsip_stack_start(tsip_stack_handle_t *self);
TINYSIP_API int tsip_stack_set(tsip_stack_handle_t *self, ...);
int tsip_stack_alert(const tsip_stack_handle_t *self, tsip_operation_id_t opid, short status_code, char *reason_phrase, int incoming, tsip_event_type_t type);
TINYSIP_API int tsip_stack_stop(tsip_stack_handle_t *self);
TINYSIP_API int tsip_stack_destroy(tsip_stack_handle_t *self);

const tsk_timer_manager_handle_t* tsip_stack_get_timer_mgr(const tsip_stack_handle_t *self);
struct tsip_dialog_layer_s* tsip_stack_get_dialog_layer(const tsip_stack_handle_t *self);
struct tsip_transac_layer_s* tsip_stack_get_transac_layer(const tsip_stack_handle_t *self);
struct tsip_transport_layer_s* tsip_stack_get_transport_layer(const tsip_stack_handle_t *self);

TINYSIP_API int tsip_stack_register(tsip_stack_handle_t *self, const tsip_operation_handle_t *operation);
int tsip_stack_unregister(tsip_stack_handle_t *self, const tsip_operation_handle_t *operation);

#define TSIP_STACK_EVENT_RAISE(stack, status_code, reason_phrase, incoming, type) \
	TSK_RUNNABLE_ENQUEUE(TSK_RUNNABLE(stack), (const tsip_stack_handle_t*)stack, (short)status_code, (const char*)reason_phrase, (unsigned)incoming, (tsip_event_type_t)type);

TSIP_END_DECLS

#endif /* TINYSIP_TSIP_H */