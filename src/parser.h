/**
 *    ______      ___
 *   / ____/___  /   | _____________  __________
 *  / / __/ __ \/ /| |/ ___/ ___/ _ \/ ___/ ___/
 * / /_/ / /_/ / ___ / /__/ /__/  __(__  |__  )
 * \____/\____/_/  |_\___/\___/\___/____/____/
 *
 * The MIT License (MIT)
 * Copyright (c) 2009-2016 Gerardo Orellana <hello @ goaccess.io>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#define LINE_BUFFER 	  4096
#define KEY_FOUND       1
#define KEY_NOT_FOUND  -1
#define REF_SITE_LEN    512
#define NUM_TESTS       20

#include "commons.h"

/* Log properties. Note: This is per line parsed */
typedef struct GLogItem_
{
  char *agent;
  char *browser;
  char *browser_type;
  char *continent;
  char *country;
  char *date;
  char *host;
  char *keyphrase;
  char *method;
  char *os;
  char *os_type;
  char *protocol;
  char *qstr;
  char *ref;
  char *req;
  char *req_key;
  char *status;
  char *time;
  char *uniq_key;
  char *vhost;

  char site[REF_SITE_LEN];

  uint64_t resp_size;
  uint64_t serve_time;

  int type_ip;
  int is_404;
  int is_static;
  int uniq_nkey;
  int agent_nkey;
} GLogItem;

/* Overall parsed log properties */
typedef struct GLog_
{
  unsigned int excluded_ip;
  unsigned int invalid;
  unsigned int offset;
  unsigned int processed;
  unsigned int valid;
  unsigned long long resp_size;
  unsigned short load_from_disk_only;
  unsigned short piping;
  GLogItem *items;
} GLog;

/* Raw Data extracted from table stores */
typedef struct GRawDataItem_
{
  int key;
  int value;
} GRawDataItem;

/* Raw Data per module */
typedef struct GRawData_
{
  GRawDataItem *items;          /* data */
  GModule module;               /* current module */
  int idx;                      /* first level index */
  int size;                     /* total num of items on ht */
} GRawData;

/* Each record contains a data value, i.e., Windows XP, and it may contain a
 * root value, i.e., Windows, and a unique key which is the combination of
 * date, IP and user agent */
typedef struct GKeyData_
{
  void *data;
  void *data_key;
  int data_nkey;

  void *root;
  void *root_key;
  int root_nkey;

  void *uniq_key;
  int uniq_nkey;
} GKeyData;

typedef struct GParse_
{
  GModule module;
  int (*key_data) (GKeyData * kdata, GLogItem * glog);

  /* data field */
  void (*datamap) (int data_nkey, const char *data, GModule module);
  void (*rootmap) (int root_nkey, const char *root, GModule module);

  /* metrics */
  void (*hits) (int data_nkey, GModule module);
  void (*visitor) (int uniq_nkey, GModule module);
  void (*bw) (int data_nkey, uint64_t size, GModule module);
  void (*cumts) (int data_nkey, uint64_t ts, GModule module);
  void (*maxts) (int data_nkey, uint64_t ts, GModule module);
  void (*method) (int data_nkey, const char *method, GModule module);
  void (*protocol) (int data_nkey, const char *proto, GModule module);
  void (*agent) (int data_nkey, int agent_nkey, GModule module);
} GParse;

GLog *init_log (void);
GLogItem *init_log_item (GLog * logger);
GRawDataItem *new_grawdata_item (unsigned int size);
GRawData *new_grawdata (void);
int parse_log (GLog ** logger, char *tail, int n);
int test_format (GLog * logger);
void free_raw_data (GRawData * raw_data);
void reset_struct (GLog * logger);
void verify_formats (void);

#endif
