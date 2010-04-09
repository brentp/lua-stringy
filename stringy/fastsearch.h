/* fastsearch implementation copied from python's stringlib. */

/* fast search/count implementation, based on a mix between boyer-
   moore and horspool, with a few more bells and whistles on the top.
   for some more background, see: http://effbot.org/zone/stringlib.htm */

/* note: fastsearch may access string[n], which isn't a problem when using
   strings without a trailing \0.
   the count mode returns -1
   if there cannot possible be a match in the target string, and 0 if
   it has actually checked for matches, but didn't find any. */

#define FAST_COUNT 0
#define FAST_SEARCH 1
#define FAST_RSEARCH 2

#define BLOOM_ADD(mask, ch) ((mask |= (1 << ((ch) & 0x1F))))
#define BLOOM(mask, ch)     ((mask &  (1 << ((ch) & 0x1F))))

inline size_t fastsearch(const char* string, size_t slen,
           const char* token, size_t tlen,
           size_t maxcount, int mode) {
    // set maxcount = -1 to find all.
    long mask;
    size_t skip, count = 0;
    size_t i, j, tlast; 

    int w = slen - tlen;

    if ((w < 0) || (mode == FAST_COUNT && maxcount == 0)) {
        return -1;
    }

    /* look for special cases */
    if (tlen <= 1) {
        // empty search string.
        if (tlen <= 0) return -1;

        /* use special case for 1-character strings */
        if (mode == FAST_COUNT) {
            for (i = 0; i < slen; i++) {
                if (string[i] == token[0]) {
                    count++;
                    if (count == maxcount) return maxcount;
                }
            }
            return count;
        } else if (mode == FAST_SEARCH) {
            for (i = 0; i < slen; i++) {
                if (string[i] == token[0]) return i;
            }
        } else {    /* FAST_RSEARCH */
            for (i = slen - 1; i > -1; i--) {
                if (string[i] == token[0]) return i;
            }
        }
        return -1;
    }

    tlast = tlen - 1;
    skip = tlast - 1;
    mask = 0;

    if (mode != FAST_RSEARCH) { // FAST_SEARCH | COUNT

        /* create compressed boyer-moore delta 1 table */

        /* process pattern[:-1] */
        for (i = 0; i < tlast; i++) {
            BLOOM_ADD(mask, token[i]);
            if (token[i] == token[tlast]) skip = tlast - i - 1;
        }
        /* process pattern[-1] outside the loop */
        BLOOM_ADD(mask, token[tlast]);

        for (i = 0; i <= w; i++) {
            /* note: using tlast in the skip path slows things down on x86 */
            if (string[i+tlen-1] == token[tlen-1]) {
                /* candidate match */
                for (j = 0; j < tlast; j++) {
                    if (string[i+j] != token[j]) break;
                }
                if (j == tlast) {
                    /* got a match! */
                    if (mode != FAST_COUNT) return i;
                    count++;
                    if (count == maxcount) return maxcount;
                    i = i + tlast;
                    continue;
                }
                /* miss: check if next character is part of pattern */
                if (!BLOOM(mask, string[i+tlen])) i = i + tlen;
                else i = i + skip;
            } else {
                /* skip: check if next character is part of pattern */
                if (!BLOOM(mask, string[i+tlen])) i = i + tlen;
            }
        }
    } else {    /* FAST_RSEARCH */

        /* create compressed boyer-moore delta 1 table */

        /* process pattern[0] outside the loop */
        BLOOM_ADD(mask, token[0]);
        /* process pattern[:0:-1] */
        for (i = tlast; i > 0; i--) {
            BLOOM_ADD(mask, token[i]);
            if (token[i] == token[0]) skip = i - 1;
        }

        for (i = w; i >= 0; i--) {
            if (string[i] == token[0]) {
                /* candidate match */
                for (j = tlast; j > 0; j--) {
                    if (string[i+j] != token[j]) break;
                }
                /* got a match! */
                if (j == 0) return i;
                /* miss: check if previous character is part of pattern */
                if (!BLOOM(mask, string[i-1])) i = i - tlen;
                else i = i - skip;
            } else {
                /* skip: check if previous character is part of pattern */
                if (!BLOOM(mask, string[i-1])) i = i - tlen;
            }
        }
    }

    if (mode != FAST_COUNT)
        return -1;
    return count;
}
