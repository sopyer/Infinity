/** 
 * mjson - modified json parser
 * syntax changes:
 *   - no {} needed around the whole file
 *   - "=" is allowed instead of ":"
 *   - quotes around the key are optional
 *   - commas after values are optional 
 *   - and c-style comments allowed
 *
 * intermediate storage is based on ideas from BJSON specification: http://bjson.org
 *
 * some code ideas are borrowed from another json parser: https://github.com/megous/sjson
 */

#ifndef __MJSON_H_INCLUDED__
#define __MJSON_H_INCLUDED__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct _mjson_entry_t;

typedef const struct _mjson_entry_t* mjson_element_t;

enum mjson_element_id_t
{
    MJSON_ID_NULL           =  0,
    MJSON_ID_FALSE          =  1,
    MJSON_ID_EMPTY_STRING   =  2,
    MJSON_ID_TRUE           =  3,

    MJSON_ID_UINT32         =  4,
    MJSON_ID_UINT64         =  5,

    MJSON_ID_SINT32         =  6,
    MJSON_ID_SINT64         =  7,

    MJSON_ID_FLOAT32        =  8,
    MJSON_ID_FLOAT64        =  9,

    MJSON_ID_UTF8_KEY32     = 10,
    MJSON_ID_UTF8_KEY64     = 11,

    MJSON_ID_UTF8_STRING32  = 12,
    MJSON_ID_UTF8_STRING64  = 13,

    MJSON_ID_BINARY32       = 14,
    MJSON_ID_BINARY64       = 15,

    MJSON_ID_ARRAY32        = 16,
    MJSON_ID_ARRAY64        = 17,

    MJSON_ID_DICT32         = 18,
    MJSON_ID_DICT64         = 19
};

int mjson_parse(const char *json_data, size_t json_data_size, void* storage_buf, size_t storage_buf_size, mjson_element_t* top_element);

mjson_element_t   mjson_get_top_element(void* storage_buf, size_t storage_buf_size);

mjson_element_t   mjson_get_element_first(mjson_element_t array);
mjson_element_t   mjson_get_element_next (mjson_element_t array, mjson_element_t current_value);
mjson_element_t   mjson_get_element      (mjson_element_t array, int index);

mjson_element_t   mjson_get_member_first(mjson_element_t dictionary, mjson_element_t* value);
mjson_element_t   mjson_get_member_next (mjson_element_t dictionary, mjson_element_t current_key, mjson_element_t* next_value);
mjson_element_t   mjson_get_member      (mjson_element_t dictionary, const char* name);

int mjson_get_type(mjson_element_t element);

const char* mjson_get_string(mjson_element_t element, const char* fallback);
int32_t     mjson_get_int   (mjson_element_t element, int32_t     fallback);
float       mjson_get_float (mjson_element_t element, float       fallback);
int         mjson_get_bool  (mjson_element_t element, int         fallback);
int         mjson_is_null   (mjson_element_t element);

#ifdef __cplusplus
}
#endif

#endif
