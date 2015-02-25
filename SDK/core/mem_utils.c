void mem_set(void *dest, size_t len, uint8_t value)
{
    uint8_t *dp = (uint8_t*)dest;
    size_t tlen = len;

    if (tlen)  do { *dp++ = value; } while(--tlen);
}

void mem_set8(uint8_t *dp, size_t len, uint8_t value)
{
    size_t tlen = len;

    if (tlen)  do { *dp++ = value; } while(--tlen);
}

void mem_set16(uint16_t *dp, size_t len, uint16_t value)
{
    size_t tlen = len;

    if (tlen)  do { *dp++ = value; } while(--tlen);
}


void mem_set32(uint32_t *dp, size_t len, uint32_t value)
{
    size_t tlen = len;

    if (tlen)  do { *dp++ = value; } while(--tlen);
}

void mem_move(void *dest, const void *src, size_t len)
{
    const uint8_t*  sp = (const uint8_t*)src;
    uint8_t*        dp = (uint8_t*)dest;

    uint32_t tlen = len;

    if (tlen == 0) return;
    
    //  Determine if we need to copy forward or backward (overlap)
    if ((uintptr_t)dp < (uintptr_t)sp) {
        //  Copy forward.
       do { *dp++ = *sp++; } while(--tlen);
    } else {
        //  Copy backwards,

        //go to end of the memory to copy
        sp += len;
        dp += len;

        do { *--dp = *--sp; } while (--tlen);
    }
}


void mem_move8(uint8_t *dp, const uint8_t *sp, size_t len)
{
    uint32_t tlen = len;

    if (tlen == 0) return;

    //  Determine if we need to copy forward or backward (overlap)
    if (dp < sp) {
        //  Copy forward.
       do { *dp++ = *sp++; } while(--tlen);
    } else {
        //  Copy backwards,

        //go to end of the memory to copy
        sp += len;
        dp += len;

        do { *--dp = *--sp; } while (--tlen);
    }
}


void mem_move16(uint16_t *dp, const uint16_t *sp, size_t len)
{
    uint32_t tlen = len;

    if (tlen == 0) return;

    //  Determine if we need to copy forward or backward (overlap)
    if (dp < sp) {
        //  Copy forward.
       do { *dp++ = *sp++; } while(--tlen);
    } else {
        //  Copy backwards,

        //go to end of the memory to copy
        sp += len;
        dp += len;

        do { *--dp = *--sp; } while (--tlen);
    }

}


void mem_move32(uint32_t *dp, const uint32_t *sp, size_t len)
{
    uint32_t tlen = len;

    if (tlen == 0) return;

    //  Determine if we need to copy forward or backward (overlap)
    if (dp < sp) {
        //  Copy forward.
       do { *dp++ = *sp++; } while(--tlen);
    } else {
        //  Copy backwards,

        //go to end of the memory to copy
        sp += len;
        dp += len;

        do { *--dp = *--sp; } while (--tlen);
    }

}

void mem_copy(void *dest, const void *src, size_t len)
{
    mem_move(dest, src, len);
}

void mem_copy8(uint8_t *dp, const uint8_t *sp, size_t len)
{
    mem_move8(dp, sp, len);
}

void mem_copy16(uint16_t *dp, const uint16_t *sp, size_t len)
{
    mem_move16(dp, dp, len);
}

void mem_copy32(uint32_t *dp, const uint32_t *sp, size_t len)
{
    mem_move32(dp, sp, len);
}
