/*
 *  Copyright (C) 2012, 2013 LOTES TM LLC
 *  Author : Andrey Loukhnov <aol.nnov@gmail.com>
 *  Author : Patrick Keroulas <patrick.keroulas@savoirfairelinux.com>
 *  This file is a part of pult5-voip
 *
 *  pult5-voip is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  pult5-voip is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this programm. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Additional permission under GNU GPL version 3 section 7:
 *
 *  If you modify pult5-voip, or any covered work, by linking or
 *  combining it with the OpenSSL project's OpenSSL library (or a
 *  modified version of that library), containing parts covered by the
 *  terms of the OpenSSL or SSLeay licenses, LOTES-TM LLC
 *  grants you additional permission to convey the resulting work.
 *  Corresponding Source for a non-source form of such a combination
 *  shall include the source code for the parts of OpenSSL used as well
 *  as that of the covered work.
 */

#ifndef SIPBUDDY_H
#define SIPBUDDY_H

#include <pjsip-simple/presence.h>
#include <pj/timer.h>
#include <pj/pool.h>
#include <string>

#include <pjsip-simple/evsub.h>
#include <pjsip-simple/evsub_msg.h>
#include <pjsip/sip_endpoint.h>
#include <pjsip/sip_transport.h>
#include "noncopyable.h"

class SIPAccount;

/**
 * Transaction functions of event subscription client side.
 */
static void buddy_evsub_on_state(pjsip_evsub *sub, pjsip_event *event);
static void buddy_evsub_on_tsx_state(pjsip_evsub *sub,
                                     pjsip_transaction *tsx,
                                     pjsip_event *event);
static void buddy_evsub_on_rx_notify(pjsip_evsub *sub,
                                     pjsip_rx_data *rdata,
                                     int *p_st_code,
                                     pj_str_t **p_st_text,
                                     pjsip_hdr *res_hdr,
                                     pjsip_msg_body **p_body);
static void buddy_timer_cb(pj_timer_heap_t *th, pj_timer_entry *entry);


class SIPBuddy {

    public:
        /**
         * Constructor
         * @param uri   SIP uri of remote user that we want to subscribe,
         */
        SIPBuddy(const std::string &uri, SIPAccount *acc);
        /**
         * Destructor.
         * Process the the unsubscription before the destruction.
         */
        ~SIPBuddy();
        /**
         * Compare with another buddy's uris.
         * @param b     Other buddy pointer
         */
        bool match(SIPBuddy *b);
        /**
         * The PBX must approve the subrciption before the buddy is added in the buddy list.
         */
        void accept();
        /**
         * Send a SUBCRIBE to the PXB or directly to a buddy in the IP2IP context.
         */
        bool subscribe();
        /**
         * Send a SUBCRIBE to the PXB or directly to a buddy in the IP2IP context but
         * the 0s timeout make the dialog expire immediatly.
         */
        bool unsubscribe();
        /**
         * Return  the monitor variable.
         */
        bool isSubscribed();
        /**
         * Return the buddy URI
         */
        std::string getURI();


        friend void buddy_evsub_on_state(pjsip_evsub *sub, pjsip_event *event);
        friend void buddy_evsub_on_tsx_state(pjsip_evsub *sub,
                                             pjsip_transaction *tsx,
                                             pjsip_event *event);
        friend void buddy_evsub_on_rx_notify(pjsip_evsub *sub,
                                             pjsip_rx_data *rdata,
                                             int *p_st_code,
                                             pj_str_t **p_st_text,
                                             pjsip_hdr *res_hdr,
                                             pjsip_msg_body **p_body);
        friend void buddy_timer_cb(pj_timer_heap_t *th, pj_timer_entry *entry);
        void incLock() {
            lock_count++;
        }
        void decLock() {
            lock_count--;
        }

    private:

        NON_COPYABLE(SIPBuddy);
        /**
         * Plan a retry or a renew a subscription.
         * @param reschedule    Allow for reschedule.
         * @param msec          Delay value in milliseconds.
         */
        void rescheduleTimer(bool reschedule, unsigned msec);
        /**
         * Callback after a presence notification was received.
         * Tranfert info to the SIP account.
         */
        pj_status_t reportPresence();
        /**
         * Process the un/subscribe request transmission.
         */
        pj_status_t updateSubscription();
        /*
         * Compare the reason of a transaction end with the given string.
         */
        bool isTermReason(std::string);
        /**
         * return the code after a transaction is terminated.
         */
        unsigned getTermCode();

        SIPAccount          *acc;       /**< Associated SIP account pointer */
        pj_str_t         uri;        /**< Buddy URI.            */
        pj_str_t         contact;   /**< Contact learned from subscrp.    */
        pj_str_t         display;   /**< Buddy display name.        */
        pjsip_dialog    *dlg;        /**< The underlying dialog.        */
        pj_bool_t         monitor;   /**< Should we monitor?        */
        pj_str_t         name;        /**< Buddy name.            */
        pj_caching_pool      cp_;
        pj_pool_t        *pool;        /**< Pool for this buddy.        */
        pjsip_pres_status     status;    /**< Buddy presence status.        */
        pjsip_evsub        *sub;        /**< Buddy presence subscription    */
        unsigned         term_code; /**< Subscription termination code    */
        pj_str_t         term_reason;/**< Subscription termination reason */
        pj_timer_entry     timer;        /**< Resubscription timer        */
        void        *user_data; /**< Application data. */
        int lock_count;
};

#endif    /* SIPBUDDY_H */