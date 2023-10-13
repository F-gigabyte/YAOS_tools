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

void test_float(float f, const char* float_str)
{
    int len = 0;
    for(const char* c = float_str; *c != 0; c++, len++)
    {

    }
    int* res_buffer = (int*)malloc(len * sizeof(int));
    int* test_buffer = (int*)malloc(len * sizeof(int));
    assert(put_str_in_int_buffer(float_str, res_buffer, len) == len);
    set_buffer(test_buffer, len);
    my_printf("%f", f);
    munit_assert_memory_equal(len * sizeof(int), res_buffer, test_buffer);
    printf("Test pass for %f\n", f);
}

/*
 * Tests combinations of NaN, INF and 0
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

void test_float_general()
{
    test_float(0.1, "0.10000");
    test_float(4.59e10, "45900000000");
    test_float(0.5, "0.5");
    test_float(23.789, "23.789");
    test_float(19.27845, "19.278");
    test_float(23e20, "2300000000000000000000");
}

void run_tests()
{
    test_float_special_case();
    test_float_general();
}
#endif

int main()
{
#ifdef TEST
    run_tests();
#else
    int a = 0xffffffff;
    float x = *(float*)&a;
    printf("%f %f %f, %f\n", 0.123475f, x, 0.0000000000000001f, 0.5f);
#endif
    return 0;
}
