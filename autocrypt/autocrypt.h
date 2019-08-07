/**
 * @file
 * Autocrypt feature
 *
 * @authors
 * Copyright (C) 2019 Kevin J. McCarthy <kevin@8t8.us>
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

#ifndef MUTT_AUTOCRYPT_AUTOCRYPT_H
#define MUTT_AUTOCRYPT_AUTOCRYPT_H

#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>

struct Email;
struct Envelope;
WHERE sqlite3 *AutocryptDB;

/**
 * struct AutocryptAccount - Autocrypt account
 */
struct AutocryptAccount
{
  char *email_addr;
  char *keyid;
  char *keydata;
  int prefer_encrypt;    /* 0 = nopref, 1 = mutual */
  int enabled;
};

/**
 * struct AutocryptPeer - Autocrypt peer
 */
struct AutocryptPeer
{
  char *email_addr;
  sqlite3_int64 last_seen;
  sqlite3_int64 autocrypt_timestamp;
  char *keyid;
  char *keydata;
  int prefer_encrypt;    /* 0 = nopref, 1 = mutual */
  sqlite3_int64 gossip_timestamp;
  char *gossip_keyid;
  char *gossip_keydata;
};

/**
 * struct AutocryptPeerHistory - Autocrypt peer history
 */
struct AutocryptPeerHistory
{
  char *peer_email_addr;
  char *email_msgid;
  sqlite3_int64 timestamp;
  char *keydata;
};

/**
 * struct AutocryptGossipHistory - Autocrypt gossip history
 */
struct AutocryptGossipHistory
{
  char *peer_email_addr;
  char *sender_email_addr;
  char *email_msgid;
  sqlite3_int64 timestamp;
  char *gossip_keydata;
};

/**
 * enum AutocryptRec - Recommendation
 */
enum AutocryptRec
{
  AUTOCRYPT_REC_OFF,        ///< No recommendations
  AUTOCRYPT_REC_NO,         ///< Do no use Autocrypt
  AUTOCRYPT_REC_DISCOURAGE, ///< Prefer not to use Autocrypt
  AUTOCRYPT_REC_AVAILABLE,  ///< Autocrypt is available
  AUTOCRYPT_REC_YES,        ///< Autocrypt should be used
};

int mutt_autocrypt_init (int);
void mutt_autocrypt_cleanup (void);
int mutt_autocrypt_process_autocrypt_header (struct Email *hdr, struct Envelope *env);
int mutt_autocrypt_process_gossip_header (struct Email *hdr, struct Envelope *prot_headers);
enum AutocryptRec mutt_autocrypt_ui_recommendation (struct Email *hdr, char **keylist);
int mutt_autocrypt_set_sign_as_default_key (struct Email *hdr);
int mutt_autocrypt_write_autocrypt_header (struct Envelope *env, FILE *fp);
int mutt_autocrypt_write_gossip_headers (struct Envelope *env, FILE *fp);
int mutt_autocrypt_generate_gossip_list (struct Email *hdr);
void mutt_autocrypt_account_menu (void);

#endif /* MUTT_AUTOCRYPT_AUTOCRYPT_H */
