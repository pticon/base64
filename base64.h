#ifndef __BASE64_H__

#define __BASE64_H__

#define __BASE64_VERSION	1


#define IS_UPPER(x)	( ((x) >= 'A' && (x) <= 'Z') )
#define IS_LOWER(x)	( ((x) >= 'a' && (x) <= 'z') )
#define IS_ALPHA(x)	( (IS_UPPER(x) || IS_LOWER(x)) )
#define IS_DIGIT(x)	( ((x) >= '0' && (x) <= '9') )
#define IS_BASE64(x)	( (IS_ALPHA(x) || IS_DIGIT(x) || (x) == '+' || (x) == '/' || (x) == '=') )


/*!
 * @brief Encode a string helping with base64
 *
 * @param [in] in the input buffer to encode
 * @param [in] input_len the input buffer length
 * @param [out] output_len the encoded buffer length
 *
 * @return a pointer to the malloced encoded buffer on success
 * or NULL otherwise
 */
uint8_t * base64_encode(const uint8_t * in, size_t input_len, size_t *output_len);


/*!
 * @brief Check if the buffer is base64 encoded
 *
 * @param [in] in the input buffer to check
 * @param [in] input_len the input buffer length
 *
 * @return 1 if the buffer is base64 coded or 0 otherwise
 */
int base64_decode_check(const uint8_t * in, size_t input_len);


/*!
 * @brief Decode characters
 *
 * @param [in] in the input buffer to decode
 * @param [in] input_len the input buffer length
 * @param [out] output_len the decoded buffer length
 *
 * @return a pointer to the malloced decoded buffer on success
 * or NULL otherwise
 */
uint8_t * base64_decode(const uint8_t * in, size_t input_len, size_t * output_len);


#endif /* __BASE64_H__ */
