#ifndef __STUN_H_INCLUDED__
#define __STUN_H_INCLUDED__

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    BindingRequest  = 0x0001,
    BindingResponse = 0x0101,
} StunMessageType;

typedef struct {
    unsigned short type;
    unsigned short message_len;
    unsigned long transaction_1;
    unsigned long transaction_2;
} StunMessageHeader;


typedef struct {
    StunMessageHeader header;
} StunMessage;

bool stun_parse_message(StunMessage *message, const char *data,
                        size_t data_len);

#endif

#ifndef __STUND_H_IMP__
#define __STUND_H_IMP__

#define consume(out, data, data_len)                                           \
    do {                                                                       \
        if (data_len < sizeof(out))                                            \
            return false;                                                      \
        memcpy(&(out), (data), sizeof(out));                                   \
        data += sizeof(out);                                                   \
        data_len -= sizeof(out);                                               \
    } while (0)

// https://datatracker.ietf.org/doc/html/rfc5389#section-6
bool stun_parse_message(StunMessage *message, const char *data, size_t data_len) {
    if (!data || data_len <= 0) return false;
    consume(message->header.type, data, data_len);
    consume(message->header.message_len, data, data_len);
    
    consume(message->header.transaction_1, data, data_len);
    consume(message->header.transaction_2, data, data_len);

    unsigned int magic = 0;
    memcpy(&magic, &message->header.transaction_1, sizeof(magic));
    if (magic != 0x2112A442) {
        return false;
    }
    return true;
}

#endif