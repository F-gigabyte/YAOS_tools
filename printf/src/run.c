// License: GPL-2.0
// Please see https://www.gnu.org/licenses/old-licenses/gpl-2.0.html#SEC1
//
// Unless required by applicable law or agreed to in writing, this software
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.

#include <printf.h>
#ifdef TEST
#include <munit.h>
#endif

#ifdef TEST
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

void testFloat()
{
    int buffer[19];
    int testBuffer[19];
    const char* res = "0.10000000149011612";
    put_str_in_int_buffer(res, buffer, 19);
    set_buffer(testBuffer, 19);
    my_printf("%f", 0.1);
    munit_assert_memory_equal(19 * sizeof(int), buffer, testBuffer);
}
#endif

int main()
{
#ifdef TEST
    testFloat();
#else
    printf("%f\n", 0.1f);
#endif
    return 0;
}
