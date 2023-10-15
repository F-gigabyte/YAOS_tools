// License: GPL-2.0
// Please see https://www.gnu.org/licenses/old-licenses/gpl-2.0.html#SEC1
//
// Unless required by applicable law or agreed to in writing, this software
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.

#include <printf.h>
#ifdef TEST
#include <munit.h>
#include <malloc.h>
#include <assert.h>
#endif

#ifdef TEST

#define BUFFER_LENGTH 0x400

int put_str_in_int_buffer(const char* str, int* buffer, int length)
{
    int pos = 0;
    while(*str != 0 && pos < length)
    {
        buffer[pos] = *str;
        pos++;
        str++;
    }
    return pos;
}

/*
 * Sets up buffer in form 0.000... for when floating point numbers are very small
*/
void setup_float_buffer(char* buffer, int buffer_len)
{
    memset(buffer, '0', buffer_len - 1);
    buffer[buffer_len - 1] = 0;
    buffer[1] = '.';
}

/*
 * Checks if the printed output of the double f is the same as the text in float_str once float_str has been converted
 * to an integer array of unicode characters
 * For a double it will be the shortest encoding of the value that is stored in the double (not that given in source code)
 * For a float, it will first be cast to a double so will likely be a rounded version of the underlying value stored
 * in the float
*/
void test_float(double f, const char* float_str)
{
    printf("Testing %f\n", f);
    int len = 0;
    for(const char* c = float_str; *c != 0; c++, len++)
    {

    }
    int* res_buffer = (int*)malloc(len * sizeof(int));
    int* test_buffer = (int*)malloc(len * sizeof(int));
    assert(put_str_in_int_buffer(float_str, res_buffer, len) == len);
    set_buffer(test_buffer, len);
    my_printf("%f", f);
    printf("Expected ");
    for(int i = 0; i < len; i++)
    {
        putc(res_buffer[i], stdout);
    }
    printf("\n");
    printf("Got ");
    for(int i = 0; i < len; i++)
    {
        putc(test_buffer[i], stdout);
    }
    printf("\n");
    munit_assert_memory_equal(len * sizeof(int), res_buffer, test_buffer);
    printf("Test pass for %f\n", f);
}

/*
 * Tests float combinations of NaN, INF and 0
*/
void test_float_special_case()
{
    // zero
    float x = 0;
    test_float(x, "0");
    int a = 0x80000000;
    x = *(float*)&a;
    test_float(x, "-0");
    // NaN
    a = 0xffffffff;
    x = *(float*)&a;
    test_float(x, "-NaN");
    a = 0x7fffffff;
    x = *(float*)&a;
    test_float(x, "NaN");
    a = 0xff800001;
    x = *(float*)&a;
    test_float(x, "-NaN");
    a = 0xff80f000;
    test_float(x, "-NaN");
    a = 0x7f800001;
    x = *(float*)&a;
    test_float(x, "NaN");
    a = 0x7f800f00;
    x = *(float*)&a;
    test_float(x, "NaN");
    // INF
    a = 0x7f800000;
    x = *(float*)&a;
    test_float(x, "INF");
    a = 0xff800000;
    x = *(float*)&a;
    test_float(x, "-INF");
}

/*
 * Tests floats generally encountered in programs
*/
void test_float_general()
{
    test_float(0.1f, "0.10000");
    test_float(4.59e10f, "45900000000");
    test_float(0.5f, "0.5");
    test_float(23.789f, "23.789");
    test_float(19.27845f, "19.278");
    test_float(23e20f, "2300000000000000000000");
    test_float(11.0f, "11");
    test_float(5.0f, "5");
    test_float(2.34185f, "2.3418");
    test_float(1.0f, "1");
    test_float(10.0829f, "10.083");
    test_float(-0.1f, "-0.10000");
    test_float(-4.59e10f, "-45900000000");
    test_float(-0.5f, "-0.5");
    test_float(-23.789f, "-23.789");
    test_float(-19.27845f, "-19.278");
    test_float(-23e20f, "-2300000000000000000000");
    test_float(-11.0f, "-11");
    test_float(-5.0f, "-5");
    test_float(-2.34185f, "-2.3418");
    test_float(-1.0f, "-1");
    test_float(-10.0829f, "-10.083");
}

/*
 * Tests floats in subnormal range
*/
void test_float_subnormal()
{
    test_float(8e-39f, "0.0000000000000000000000000000000000000080000");
    test_float(1.4e-45f, "0.0000000000000000000000000000000000000000000014013");
    test_float(2.9e-42f, "0.0000000000000000000000000000000000000000029007");
    test_float(2.35437e-44f, "0.000000000000000000000000000000000000000000023822");
    test_float(1.17549e-38f, "0.000000000000000000000000000000000000011755");
    test_float(-8e-39f, "-0.0000000000000000000000000000000000000080000");
    test_float(-1.4e-45f, "-0.0000000000000000000000000000000000000000000014013");
    test_float(-2.9e-42f, "-0.0000000000000000000000000000000000000000029007");
    test_float(-2.35437e-44f, "-0.000000000000000000000000000000000000000000023822");
    test_float(-1.17549e-38f, "-0.000000000000000000000000000000000000011755");
}

/*
 * Tests double combinations of NaN, INF and 0
*/
void test_double_special_case()
{
    // zero
    double x = 0;
    test_float(x, "0");
    long a = 0x8000000000000000;
    x = *(double*)&a;
    printf("x is %f\n", x);
    test_float(x, "-0");
    // NaN
    a = 0xffffffffffffffff;
    x = *(double*)&a;
    test_float(x, "-NaN");
    a = 0x7fffffffffffffff;
    x = *(double*)&a;
    test_float(x, "NaN");
    a = 0xfff0000000000001;
    x = *(double*)&a;
    test_float(x, "-NaN");
    a = 0xfff0f00000000000;
    test_float(x, "-NaN");
    a = 0x7ff0000000000001;
    x = *(double*)&a;
    test_float(x, "NaN");
    a = 0x7ff00f0000000000;
    x = *(double*)&a;
    test_float(x, "NaN");
    // INF
    a = 0x7ff0000000000000;
    x = *(double*)&a;
    test_float(x, "INF");
    a = 0xfff0000000000000;
    x = *(double*)&a;
    test_float(x, "-INF");
}

/*
 * Tests doubles generally encountered in programs
*/
void test_double_general()
{
    test_float(0.1, "0.1");
    test_float(4.59e10, "45900000000");
    test_float(0.5, "0.5");
    test_float(23.789, "23.789");
    test_float(19.27845, "19.278");
    test_float(23e20, "2300000000000000000000");
    test_float(11.0, "11");
    test_float(5.0, "5");
    test_float(2.34185, "2.3418");
    test_float(1.0, "1");
    test_float(10.0829, "10.083");
    test_float(-0.1, "-0.1");
    test_float(-4.59e10, "-45900000000");
    test_float(-0.5, "-0.5");
    test_float(-23.789, "-23.789");
    test_float(-19.27845, "-19.278");
    test_float(-23e20, "-2300000000000000000000");
    test_float(-11.0, "-11");
    test_float(-5.0, "-5");
    test_float(-2.34185, "-2.3418");
    test_float(-1.0, "-1");
    test_float(-10.0829, "-10.083");
}

/*
 * Tests doubles in subnormal range
*/
void test_double_subnormal()
{
    char small_double[328];
    setup_float_buffer(small_double, 313);
    small_double[311] = '8';
    test_float(8e-310, small_double);
    setup_float_buffer(small_double, 327);
    small_double[325] = '5';
    test_float(5e-324, small_double);
    setup_float_buffer(small_double, 318);
    small_double[315] = '2';
    small_double[316] = '9';
    test_float(2.9e-314, small_double);
    setup_float_buffer(small_double, 326);
    small_double[321] = '2';
    small_double[322] = '3';
    small_double[323] = '5';
    small_double[324] = '4';
    test_float(2.35437e-320, small_double);
    setup_float_buffer(small_double, 315);
    small_double[309] = '2';
    small_double[310] = '2';
    small_double[311] = '2';
    small_double[312] = '5';
    small_double[313] = '1';
    test_float(2.22507e-308, small_double);
    small_double[0] = '-';
    char* neg_small_double = &small_double[1];
    setup_float_buffer(neg_small_double, 313);
    neg_small_double[311] = '8';
    test_float(-8e-310, small_double);
    setup_float_buffer(neg_small_double, 327);
    neg_small_double[325] = '5';
    test_float(-5e-324, small_double);
    setup_float_buffer(neg_small_double, 318);
    neg_small_double[315] = '2';
    neg_small_double[316] = '9';
    test_float(-2.9e-314, small_double);
    setup_float_buffer(neg_small_double, 326);
    neg_small_double[321] = '2';
    neg_small_double[322] = '3';
    neg_small_double[323] = '5';
    neg_small_double[324] = '4';
    test_float(-2.35437e-320, small_double);
    setup_float_buffer(neg_small_double, 315);
    neg_small_double[309] = '2';
    neg_small_double[310] = '2';
    neg_small_double[311] = '2';
    neg_small_double[312] = '5';
    neg_small_double[313] = '1';
    test_float(-2.22507e-308, small_double);
}

void run_tests()
{
    printf("Testing float special case\n");
    test_float_special_case();
    printf("Testing float general\n");
    test_float_general();
    printf("Testing float subnormal\n");
    test_float_subnormal();
    printf("Testing double special case\n");
    test_double_special_case();
    printf("Testing double general\n");
    test_double_general();
    printf("Testing double subnormal\n");
    test_double_subnormal();
}
#endif

int main()
{
#ifdef TEST
    run_tests();
#else
    int a = 0xffffffff;
    float x = *(float*)&a;
    printf("%f %f %f, %f %f\n", 0.123475f, x, 0.0000000000000001f, 0.5f, 1.4e-45f);
#endif
    return 0;
}
