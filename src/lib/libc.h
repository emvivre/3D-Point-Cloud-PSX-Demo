#ifndef _LIBC_H_
#define _LIBC_H_

#include <stdarg.h>
#include <string.h>
#include <ctype.h>

namespace LibC
{
    inline void sscanf(const char* buffer, const char* pattern, ...) {
        int l = strlen(buffer);
        int pl  = strlen(buffer);
        va_list va;
        va_start(va, pattern);
        int pn_i = 0; 
        bool in_decimal_token = false;
        int decimal_token_len_left = 0;
        int current_value = 0;
        for ( int i = 0; i < l && pn_i < pl; ) {
            if ( pattern[pn_i] != '%' ) { // is token ?	
                if ( in_decimal_token == false ) {
                    if ( buffer[i] != pattern[pn_i] ) return ;
                    pn_i++;
                    i++;
                    continue;
                }		
                if ( isdigit(buffer[i]) == false ) return ;
                current_value = current_value * 10 + buffer[i] - '0';
                decimal_token_len_left--;
                if ( decimal_token_len_left == 0 ) {
                    in_decimal_token = false;
                    int* v = va_arg(va, int*);
                    *v = current_value;
                }
                i++;
                continue;
            }
            // get token
            pn_i++; if ( pn_i >= pl ) return ;
            if ( pattern[pn_i] != '0' ) return ;
            pn_i++; if ( pn_i >= pl ) return ;
            decimal_token_len_left = pattern[pn_i] - '0';
            pn_i++; if ( pn_i >= pl ) return ;
            if ( pattern[pn_i] != 'd' ) return ;
            in_decimal_token = true;	
            current_value = 0;	       
            pn_i++;
        }
    }
}

#endif