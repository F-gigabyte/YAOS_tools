// License GPL-2.0
// Please see https://www.gnu.org/licenses/old-licenses/gpl-2.0.html#SEC1
//
// Unless required by applicable law or agreed to in writing, this software
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.

#ifndef PRINTF_H
#define PRINTF_H

#ifdef TEST
int my_printf(const char* str, ...);
#else
int printf(const char* str, ...);
#endif
void set_buffer(int* stdout_buffer, int size);

#endif
