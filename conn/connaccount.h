/**
 * @file
 * Connection Credentials
 *
 * @authors
 * Copyright (C) 2000-2005,2008 Brendan Cully <brendan@kublai.com>
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MUTT_CONN_ACCOUNT_H
#define MUTT_CONN_ACCOUNT_H

#include <stdint.h>

struct Account;

/**
 * enum ConnAccountField - Login credentials
 */
enum ConnAccountField
{
  MUTT_CA_HOST = 1,  ///< Server name
  MUTT_CA_LOGIN,     ///< Login name
  MUTT_CA_USER,      ///< User name
  MUTT_CA_PASS,      ///< Password
  MUTT_CA_OAUTH_CMD, ///< OAuth refresh command
};

/**
 * typedef ca_get_field_t - Prototype for a function to get some login credentials
 * @param field Field to get, e.g. #MUTT_CA_PASS
 * @retval ptr Requested string
 */
typedef const char *(*ca_get_field_t)(enum ConnAccountField field);

typedef uint8_t MuttAccountFlags;     ///< Flags, Which ConnAccount fields are initialised, e.g. #MUTT_ACCT_PORT
#define MUTT_ACCT_NO_FLAGS        0   ///< No flags are set
#define MUTT_ACCT_PORT      (1 << 0)  ///< Port field has been set
#define MUTT_ACCT_USER      (1 << 1)  ///< User field has been set
#define MUTT_ACCT_LOGIN     (1 << 2)  ///< Login field has been set
#define MUTT_ACCT_PASS      (1 << 3)  ///< Password field has been set
#define MUTT_ACCT_SSL       (1 << 4)  ///< Account uses SSL/TLS

/**
 * struct ConnAccount - Login details for a remote server
 */
struct ConnAccount
{
  char user[128];
  char login[128];
  char pass[256];
  char host[128];
  unsigned short port;
  unsigned char type;     ///< Connection type, e.g. #MUTT_ACCT_TYPE_IMAP
  MuttAccountFlags flags; ///< Which fields are initialised, e.g. #MUTT_ACCT_USER
  const char *service;    ///< Name of the service, e.g. "imap"
  struct Account *account; ///< Parent Account for this Connection

  ca_get_field_t get_field; ///< Function to get some data
};

int   mutt_account_getlogin      (struct ConnAccount *account);
char *mutt_account_getoauthbearer(struct ConnAccount *account);
int   mutt_account_getpass       (struct ConnAccount *account);
int   mutt_account_getuser       (struct ConnAccount *account);
void  mutt_account_unsetpass     (struct ConnAccount *account);

#endif /* MUTT_CONN_ACCOUNT_H */
