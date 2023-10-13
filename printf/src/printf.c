// License GPL-2.0
// Please see https://www.gnu.org/licenses/old-licenses/gpl-2.0.html#SEC1
//
// Unless required by applicable law or agreed to in writing, this software
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.

#include <printf.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <alloca.h>
#include <assert.h>
#include <ryu/ryu.h>

#define FLOAT_MANTISSA_BITS 52
#define FLOAT_MANTISSA_MASK 0xfffffffffffffl
#define FLOAT_EXP_BITS 11
#define FLOAT_EXP_MASK 0x7ff
#define FLOAT_MAX_MAN 100000

int* buffer = NULL;
int buffer_size = 0;
int buffer_index = 0;

#ifdef TEST
void set_buffer(int* stdout_buffer, int size)
{
    buffer = stdout_buffer;
    buffer_size = size;
    buffer_index = 0;
}
#endif

/*
 * prints a char to the screen
 * uses stdlib as an example but this would be implementation dependent in reality
*/
void put_char(int c)
{
    if(buffer != NULL && buffer_size > 0)
    {
        if(buffer_index < buffer_size)
        {
            buffer[buffer_index] = c;
            buffer_index++;
        }
    }
    else
    {
        putc(c, stdout);
    }
}

/*
 * Internal function
 * Prints internal ascii char buffer of known length
 * Returns number of characters printed
*/
int _print_buffer(const char* buffer, int len)
{
    int n = 0;
    for(int i = 0; i < len; i++)
    {
        put_char(buffer[i]);
        n++;
    }
    return n;
}

/*
 * Internal function
 * Parses the magnitude section of an integer and stores the result in buffer
 * Buffer must be big enough to hold the integer or else have undefined behaviour
 * val > 0 or else will put nothing into the buffer
 * end points to the last character in the buffer (array index)
 * Returns the point in the buffer from which characters were put in it
*/
int _parse_int_mag(uint64_t val, char* buffer, int end)
{
    while(val > 0)
    {
        buffer[end] = (val % 10) + '0';
        val /= 10;
        end--;
    }
    end++;
    return end;
}

/*
 * Internal function
 * Like _parse_int_mag except for binary values
*/
int _parse_bin_mag(uint64_t val, char* buffer, int end)
{
    while(val > 0)
    {
        buffer[end] = (val & 1) + '0';
        val >>= 1;
        end--;
    }
    end++;
    return end;
}

/*
 * Internal function
 * Like _parse_int_mag except for octal values
*/
int _parse_oct_mag(uint64_t val, char* buffer, int end)
{
    while(val > 0)
    {
        buffer[end] = (val & 7) + '0';
        val >>= 3;
        end--;
    }
    end++;
    return end;
}

/*
 * Internal function
 * Like _parse_int_mag except for hexadecimal values
*/
int _parse_hex_mag(uint64_t val, char* buffer, int end)
{
    while(val > 0)
    {
        int digit = val & 0xf;
        if(digit < 10)
        {
            buffer[end] = digit + '0';
        }
        else
        {
            buffer[end] = (digit - 10) + 'a';
        }
        val >>= 4;
        end--;
    }
    end++;
    return end;
}

/*
 * Parses a 64 bit signed integer and prints each character and returns the number of characters printed
 * For 32 bit signed integers, can convert to a 64 bit signed integer and use this function
*/
int print_int(int64_t val)
{
    int n = 0;
    if(val < 0)
    {
        put_char('-');
        n++;
        val = -val;
    }
    else if(val == 0)
    {
        put_char('0');
        n++;
        return n;
    }
    char* data = (char*)alloca(19);
    int pos = _parse_int_mag(val, data, 18);
    n += _print_buffer(&(data[pos]), 19 - pos);
    return n;
}

/*
 * Parses a 64 bit unsigned integer and prints each character and returns the number of characters printed
 * For 32 bit unsigned integers, can convert to a 64 bit unsigned integer
*/
int print_unsigned_int(uint64_t val)
{
    int n = 0;
    if(val == 0)
    {
        put_char('0');
        n++;
        return n;
    }
    char* data = (char*)alloca(20);
    int pos = _parse_int_mag(val, data, 19);
    n += _print_buffer(&(data[pos]), 20 - pos);
    return n;
}

/*
 * Parses a 64 bit unsigned integer and prints each character in binary format and returns the number of
 * characters printed
 * For 32 bit unsigned integers, can convert to a 64 bit unsigned integer
*/
int print_bin(uint64_t val)
{
    int n = 0;
    put_char('0');
    n++;
    put_char('b');
    n++;
    if(val == 0)
    {
        put_char('0');
        n++;
        return n;
    }
    char* data = (char*)alloca(64);
    int pos = _parse_bin_mag(val, data, 63);
    n += _print_buffer(&(data[pos]), 64 - pos);
    return n;
}

/*
 * Parses a 64 bit unsigned integer and prints each character in octal format and returns the number of
 * characters printed
 * For 32 bit unsigned integers, can convert to a 64 bit unsigned integer
*/
int print_oct(uint64_t val)
{
    int n = 0;
    put_char('0');
    n++;
    put_char('o');
    n++;
    if(val == 0)
    {
        put_char('0');
        n++;
        return n;
    }
    char* data = (char*)alloca(22);
    int pos = _parse_oct_mag(val, data, 21);
    n += _print_buffer(&(data[pos]), 22 - pos);
    return n;
}

/*
 * Parses a 64 bit unsigned integer and prints each character in hexadecimal format and returns the number of
 * characters printed
 * For 32 bit unsigned integers, can convert to a 64 bit unsigned integer
*/
int print_hex(uint64_t val)
{
    int n = 0;
    put_char('0');
    n++;
    put_char('x');
    n++;
    if(val == 0)
    {
        put_char('0');
        n++;
        return n;
    }
    char* data = (char*)alloca(16);
    int pos = _parse_hex_mag(val, data, 15);
    n += _print_buffer(&(data[pos]), 16 - pos);
    return n;
}

/*
 * Takes the bits of a float and checks if they are a special case of NaN or INF or 0
 * If they are they are printed
 * Otherwise, the float is decoded and returned as dec
 * If the number is negative its minus sign will also be printed
 * The value returned is whether more characters need printing or not
 * If returned value != 0, dec holds mantissa and exponent and work should be done on printing it in the desired format
 * If returned value == 0, all characters are printed
 * n is a pointer to a variable holding the number of characters printed
*/
int decode_float(double val, int* n, floating_decimal_64* dec)
{
    uint64_t float_bits = *(uint64_t*)&val;
    uint8_t sign = float_bits >> (FLOAT_MANTISSA_BITS + FLOAT_EXP_BITS);
    uint32_t exp = (float_bits >> FLOAT_MANTISSA_BITS) & FLOAT_EXP_MASK;
    uint64_t man = float_bits & FLOAT_MANTISSA_MASK;
    
    if(sign)
    {
        put_char('-');
        (*n)++;
    }

    // NaN -> exp == 0xff and man != 0
    if(exp == FLOAT_EXP_MASK && man != 0)
    {
        put_char('N');
        (*n)++;
        put_char('a');
        (*n)++;
        put_char('N');
        (*n)++;
        return 0;
    }

    // INF -> exp == 0xff and man == 0
    if(exp == FLOAT_EXP_MASK)
    {
        //man == 0 or else is NaN
        put_char('I');
        (*n)++;
        put_char('N');
        (*n)++;
        put_char('F');
        (*n)++;
        return 0;
    }
    
    // handle case number is 0. Also do sign so can see if it's +/- 0
    if(man == 0 && exp == 0)
    {
        put_char('0');
        (*n)++;
        return 0;
    }
    
    *dec = d2d(man, exp);

    return 1;
}



/*
 * Rounds a 64 bit floating decimal number so that it's below FLOAT_MAX_MAN
 * FLOAT_MAX_MAN is used to determine the number of sig figs the floating point number is rounded to
 * It should be 1 more than the largest integer which has that many sig figs
*/
void round_float(floating_decimal_64* dec)
{
    uint64_t frac = 0;
    while(dec->mantissa >= FLOAT_MAX_MAN * 10)
    {
        uint64_t rem = dec->mantissa % 10;
        frac = frac * 10 + rem;
        dec->mantissa /= 10;
        dec->exponent += 1;
    }
    if(dec->mantissa >= FLOAT_MAX_MAN)
    {
        uint64_t rem = dec->mantissa % 10;
        dec->mantissa /= 10;
        dec->exponent += 1;
        if(rem > 5 || (rem == 5 && (dec->mantissa & 1) == 1)) // round to nearest even if have digit 5
        {
            dec->mantissa++;
        }
    }
}

/*
 * Parses a 64 bit floating point number (using Ryu) and prints it out in long format and returns the number of
 * characters printed
 * For 32 bit floating point numbers, can cast to double
 * The number of digits printed is the shortest length decimal representation of the floating point number
 * It prints the float to FLOAT_SIG_FIG significant figures
 * For the 6th sig fig digit, if > 5 rounds up, < 5 rounds down and = 5, rounds to nearest even
*/
int print_float(double val)
{
    int n = 0;
    floating_decimal_64 dec;
    if(decode_float(val, &n, &dec) == 0)
    {
        return n;
    }

    round_float(&dec);

    if(dec.exponent > 0) // essentially mantissa and enough zeroes to offset everything to correct place
    {
        n += print_unsigned_int(dec.mantissa);
        while(dec.exponent > 0)
        {
            put_char('0');
            n++;
            dec.exponent -= 1;
        }
    }
    else
    {
        char* data = (char*)alloca(20);
        int pos = _parse_int_mag(dec.mantissa, data, 19);
        int length = 20 - pos;
        int index = length + dec.exponent - 1;
        // printing floats with no integer part
        if(index < 0)
        {
            put_char('0');
            n++;
            put_char('.');
            n++;
            for(int i = 0; i < -index - 1; i++)
            {
                put_char('0');
                n++;
            }
            for(int j = 0; j < length; j++)
            {
                put_char(data[pos]);
                pos++;
                n++;
            }
        }
        else
        {
            // put out all digits before decimal point
            int j = 0;
            int first_length = index + 1; // calculate number of digits before decimal point
            for(; j < first_length; j++)
            {
                put_char(data[pos]);
                pos++;
                n++;
            }
            // check if need decimal point and print it and remainder of digits if do
            if(j < length)
            {
                put_char('.');
                n++;
                for(; j < length; j++)
                {
                    put_char(data[pos]);
                    pos++;
                    n++;
                }
            }
        }
    }

    return n;
}

/*
 * Parses a 64 bit floating point number (using Ryu) and prints it out in scientific notation and returns the number of
 * characters printed
 * For 32 bit floating point numbers can cast to double 
 * The number of digits printed is the shortest length scientific representation of the floating point number
 * It prints the float to FLOAT_SIG_FIG significant figures
*/
int print_float_scientific(double val)
{
    int n = 0;
    floating_decimal_64 dec;
    if(decode_float(val, &n, &dec) == 0)
    {
        // correct the printing for val == 0
        if(val == 0)
        {
            put_char('e');
            put_char('0');
        }
        return n;
    }
    round_float(&dec);
    char* data = (char*)alloca(20);
    int pos = _parse_int_mag(dec.mantissa, data, 19);
    int length = 20 - pos;
    dec.exponent += length - 1;

    // shouldn't be 0 but better to be safe
    if(length != 0)
    {
        put_char(data[pos]);
        pos++;
        n++;
        // have more than 1 digit
        if(length > 1)
        {
            put_char('.');
            n++;
        }
    }
    for(; pos < 20; pos++)
    {
        put_char(data[pos]);
        n++;
    }
    put_char('e');
    n++;
    print_int((int64_t)dec.exponent);

    return n;
}

/*
 * Decodes a UTF-8 char from str and returns the number of bytes it holds
 * code is the Unicode character code of the UTF-8 bytes
 * If the char is not in UTF-8 format, returns 0 and *code is 0
*/
int decode_char(const char* str, int* code)
{
    *code = 0;
    int temp_code = 0; // used in process of building char
    // decode utf-8
    if((*str & 0x80) == 0)
    {
        *code = *str;
        return 1;
    }
    else if((*str & 0xe0) == 0xc0)
    {
        temp_code = (*str & 0x1f) << 6;
        str++;
        if((*str & 0xc0) == 0x80)
        {
            temp_code |= *str & 0x3f;
            *code = temp_code;
            return 2;
        }
    }
    else if((*str & 0xf0) == 0xe0)
    {
        temp_code = (*str & 0xf) << 12;
        str++;
        if((*str & 0xc0) == 0x80)
        {
            temp_code |= (*str & 0x3f) << 6;
            str++;
            if((*str & 0xc0) == 0x80)
            {
                temp_code |= *str & 0x3f;
                *code = temp_code;
                return 3;
            }
        }
    }
    else if((*str & 0xf8) == 0xf0)
    {
        temp_code = (*str & 0x7) << 18;
        str++;
        if((*str & 0xc0) == 0x80)
        {
            temp_code |= (*str & 0x3f) << 12;
            str++;
            if((*str & 0xc0) == 0x80)
            {
                temp_code |= (*str & 0x3f) << 6;
                str++;
                if((*str & 0xc0) == 0x80)
                {
                    temp_code |= *str & 0x3f;
                    *code = temp_code;
                    return 4;
                }
            }
        }
    }
    return 0;
}

/*
 * Prints the string given to it
 * Also formats the string based on the arguments given
 * Returns number of characters outputted
 *
 * This is a cut down version of the actual printf function designed to print out values stored
 * but not to have all the features provided by the stdlib printf
 *
 * Format:
 * %s -> string
 * %c -> char byte
 * %d -> integer (decimal format)
 * %u -> integer (decimal format, unsigned)
 * %b -> integer (binary format)
 * %o -> integer (octal format)
 * %h -> integer (hex format)
 * %f -> float (decimal format)
 * %e -> float (scientific notation, base 10)
 * %% -> %
 *
 * length specifiers
 * l -> int, float means int type is 64 bit wide and float type is 64 bit wide (double)
 *
 * When you have an invalid length specifier for a certain format or you have the length specifier and no
 * known format (or no format at all), the character '?' is outputted
*/
#ifdef TEST
int my_printf(const char* str, ...)
#else
int printf(const char* str, ...)
#endif
{
    int num = 0; // number of chars printed
    va_list arg_list;
    va_start(arg_list, str);
    while(*str != 0)
    {
        if(*str == '%')
        {
            char l = 0; // have l flag (0 or 1)
            str++;
            if(*str == 'l')
            {
                l = 1;
                str++;
            }
            switch(*str)
            {
                case 's':
                {
                    if(l)
                    {
                        put_char('?');
                        num++;
                    }
                    else
                    {
                        const char* s = va_arg(arg_list, const char*);
                        while(*s)
                        {
                            int code = 0;
                            int bytes = decode_char(s, &code);
                            if(bytes > 0)
                            {
                                put_char(code);
                                s += bytes;
                            }
                            else
                            {
                                put_char('?');
                                s++;
                            }
                            num++;
                        }
                        str++;
                    }
                    break;
                }
                case 'c':
                {
                    if(l)
                    {
                        put_char('?');
                        num++;
                    }
                    else
                    {
                        char c = (char)va_arg(arg_list, int); // char byte promoted to int
                        put_char(c);
                        num++;
                        str++;
                    }
                    break;
                }
                case 'd':
                {
                    if(l)
                    {
                        int64_t d = va_arg(arg_list, int64_t);
                        num += print_int(d);
                    }
                    else
                    {
                        int32_t d = va_arg(arg_list, int32_t);
                        num += print_int((int64_t)d);
                    }
                    str++;
                    break;
                }
                case 'u':
                {
                    if(l)
                    {
                        uint64_t u = va_arg(arg_list, uint64_t);
                        num += print_unsigned_int(u);
                    }
                    else
                    {
                        uint32_t u = va_arg(arg_list, uint32_t);
                        num += print_unsigned_int((uint64_t)u);
                    }
                    str++;
                    break;
                }
                case 'b':
                {
                    if(l)
                    {
                        uint64_t b = va_arg(arg_list, uint64_t);
                        num += print_bin(b);
                    }
                    else
                    {
                        uint32_t b = va_arg(arg_list, uint32_t);
                        num += print_bin((uint64_t)b);
                    }
                    str++;
                    break;
                }
                case 'o':
                {
                    if(l)
                    {
                        uint64_t o = va_arg(arg_list, uint64_t);
                        num += print_oct(o);
                    }
                    else
                    {
                        uint32_t o = va_arg(arg_list, uint32_t);
                        num += print_oct((uint64_t)o);
                    }
                    str++;
                    break;
                }
                case 'h':
                {
                    if(l)
                    {
                        uint64_t h = va_arg(arg_list, uint64_t);
                        num += print_hex(h);
                    }
                    else
                    {
                        uint32_t h = va_arg(arg_list, uint32_t);
                        num += print_hex((uint64_t)h);
                    }
                    str++;
                    break;
                }
                case 'f':
                {
                    if(l)
                    {
                        double f = va_arg(arg_list, double);
                        num += print_float(f);
                    }
                    else
                    {
                        float f = (float)va_arg(arg_list, double);
                        num += print_float((double)f);
                    }
                    str++;
                    break;
                }
                case 'e':
                {
                    if(l)
                    {
                        double e = va_arg(arg_list, double);
                        num += print_float_scientific(e);
                    }
                    else
                    {
                        float e = (float)va_arg(arg_list, double);
                        num += print_float_scientific((double)e);
                    }
                    str++;
                    break;
                }
                case '%':
                {
                    if(l)
                    {
                        put_char('?');
                        num++;
                    }
                    else
                    {
                        put_char('%');
                        num++;
                        str++;
                    }
                    break;
                }
                default:
                {
                    if(l) // unknown format specifier
                    {
                        put_char('?');
                        num++;
                    }
                    else
                    {
                        put_char('%');
                        num++;
                    }
                }
            }
        }
        else
        {
            int code = 0;
            int bytes = decode_char(str, &code);
            if(bytes > 0)
            {
                put_char(code);
                str += bytes;
            }
            else
            {
                put_char('?'); // no idea what char encoded
                str++;
            }
            num++;
        }
    }
    va_end(arg_list);
    return num;
}
