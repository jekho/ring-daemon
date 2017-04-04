/*
 *  Copyright (C) 2004-2017 Savoir-faire Linux Inc.
 *
 *  Author: Groupe 7
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "securitymanager_interface.h"
#include "tlsvalidator.h"
#include "certstore.h"
#include "logger.h"
#include "fileutils.h"
#include "archiver.h"
#include "ip_utils.h"
#include "client/ring_signal.h"
#include "upnp/upnp_context.h"

#ifdef RING_UWP
#include "windirent.h"
#else
#include <dirent.h>
#endif

#include <cerrno>
#include <cstring>
#include <sstream>

#ifdef _WIN32
#undef interface
#endif

namespace ring { namespace tls {

using ring::SIPAccount;
using ring::RingAccount;
using ring::tls::TlsValidator;
using ring::tls::CertificateStore;
using ring::DeviceType;
using ring::HookPreference;


}} // namespace ring::tls
