#### snprintf() function for the OpenVMS 7.3
```
+------------------------------------------------------------------
 int snprintf( char *str, size_t len, char *format, ... )
               char *str    - pointer to the buffer to print for;
               size_t len   - size of buffer to print for;
               char *format - pointer to the format string
                              see printf() manual;
               ...          - other argument like in the
                              printf() function.
 return   - number of printed symbols otherwize EOF or NULL and
            global variable ERRNO is set to indicate the error.
+------------------------------------------------------------------
```
