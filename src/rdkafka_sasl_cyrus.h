/*
 * librdkafka - The Apache Kafka C/C++ library
 *
 * Copyright (c) 2025, Confluent Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _RDKAFKA_SASL_CYRUS_H_
#define _RDKAFKA_SASL_CYRUS_H_

#include <dlfcn.h>

#if defined(__FreeBSD__) || defined(__OpenBSD__)
#include <sys/wait.h> /* For WIF.. */
#endif

#ifdef __APPLE__
/* Apple has deprecated most of the SASL API for unknown reason,
 * silence those warnings. */
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <sasl/sasl.h>


#define RESOLVE_SYM(handle, sym)                                               \
        do {                                                                   \
                sym##_p = dlsym((handle), #sym);                               \
                if (!(sym##_p)) {                                              \
                        fprintf(stderr,                                        \
                                "librdkafka: dlsym(\"%s\") failed: %s\n",      \
                                #sym, dlerror());                              \
                        dlclose(handle);                                       \
                        (handle) = NULL;                                       \
                        return -1;                                             \
                }                                                              \
        } while (0)


#define TRY_DLOPEN_LIST(var, names)                                              \
        do {                                                                     \
                size_t count = sizeof(names) / sizeof(names[0]);                 \
                var          = rd_kafka_sasl_cyrus_try_dlopen_any(names, count); \
                if (!var) {                                                      \
                        fprintf(stderr,                                          \
                                "librdkafka: dlopen() failed for the "           \
                                "following libraries: ");                        \
                        for (size_t i = 0; i < count; i++)                       \
                                fprintf(stderr, "%s%s", names[i],                \
                                        i == count - 1 ? "\n" : ", ");           \
                        return -1;                                               \
                }                                                                \
        } while (0)


static void *rd_kafka_sasl_cyrus_library_handle = NULL;

static int (*sasl_client_init_p)(const sasl_callback_t *)    = NULL;
static int (*sasl_client_new_p)(const char *service,
                                const char *serverFQDN,
                                const char *iplocalport,
                                const char *ipremoteport,
                                const sasl_callback_t *prompt_supp,
                                unsigned flags,
                                sasl_conn_t **pconn)         = NULL;
static int (*sasl_client_start_p)(sasl_conn_t *conn,
                                  const char *mechlist,
                                  sasl_interact_t **prompt_need,
                                  const char **clientout,
                                  unsigned *clientoutlen,
                                  const char **mech)         = NULL;
static int (*sasl_client_step_p)(sasl_conn_t *conn,
                                 const char *serverin,
                                 unsigned serverinlen,
                                 sasl_interact_t **prompt_need,
                                 const char **clientout,
                                 unsigned *clientoutlen)     = NULL;
static void (*sasl_dispose_p)(sasl_conn_t **pconn)           = NULL;
static const char *(*sasl_errdetail_p)(sasl_conn_t *conn)    = NULL;
static const char *(*sasl_errstring_p)(int saslerr,
                                       const char *langlist,
                                       const char **outlang) = NULL;
static int (*sasl_getprop_p)(sasl_conn_t *conn,
                             int propnum,
                             const void **pvalue)            = NULL;
static int (*sasl_listmech_p)(sasl_conn_t *conn,
                              const char *user,
                              const char *prefix,
                              const char *sep,
                              const char *suffix,
                              const char **result,
                              unsigned *plen,
                              int *pcount)                   = NULL;

#endif /* _RDKAFKA_SASL_CYRUS_H_ */