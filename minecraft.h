#ifndef __MINECRAFT_H
#define __MINECRAFT_H

#include "net/pkt.h"
#include "version.h"
#include <sys/types.h>
#include <openssl/evp.h>

enum MC_REQ {
    MC_REQ_HANDSHAKE,
    /* Status (Handshake) */
    MC_REQ_SPL,
    MC_REQ_PING,

    /* Login */
    MC_REQ_LOGIN,
    MC_REQ_ENCRYPTRES,

    /* Play */
    MC_REQ_CHAT,
    MC_REQ_SET_DIFFICULT,
};

enum MC_STATUS {
    MC_STATUS_HANDSHAKE = 1,
    MC_STATUS_LOGIN = 2,
    MC_STATUS_PLAY = 3
};

typedef struct conninfo {
    int sockfd;
    uint32_t proto;
    const char *addr;
    uint16_t port;
    int32_t state;
    uint32_t thresh;
} conninfo_t;

typedef struct encrypt {
    struct buffer *e_id;
    struct buffer *e_pubkey;
    struct buffer *e_verify;
    struct buffer *e_secret;
    EVP_CIPHER_CTX *e_encctx;
    EVP_CIPHER_CTX *e_decctx;
} encrypt_t;

typedef struct serverinfo {
    struct conninfo si_conninfo;
    struct encrypt *si_encinfo;
} serverinfo_t;

struct serverinfo *mc_connect(const char *host, u_int16_t port, u_int32_t proto);
void mc_login(struct serverinfo *si, struct userinfo *ui);
void mc_getinfo(struct serverinfo *si, enum MC_REQ info);
void mc_set_difficult(struct serverinfo *si, int32_t level);
void mc_chat(struct serverinfo *si, const char *msg);
void mc_eventloop(struct serverinfo *si);
void mc_cleanup(void *ptr);

#endif // __MINECRAFT_H
