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

void run_tests()
{
    test_float(0.1f, "0.10000");
    test_float(0.123475f, "0.12348");
    int a = 0xffffffff;
    float x = *(float*)&a;
    test_float(x, "-NaN");
    a = 0x7fffffff;
    x = *(float*)&a;
    test_float(x, "NaN");
    a = 0x7f800000;
    x = *(float*)&a;
    test_float(x, "INF");
    a = 0xff800000;
    x = *(float*)&a;
    test_float(x, "-INF");
    test_float(0.0000000000000001f, "0.00000000000000010000");
}
#endif

int main()
{
#ifdef TEST
    run_tests();
#else
    int a = 0xffffffff;
    float x = *(float*)&a;
    printf("%f %f %f\n", 0.123475f, x, 0.0000000000000001f);
#endif
    return 0;
}
