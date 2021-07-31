/* snprintf() function for the CC compiler on a OpenVMS 7.3
 +------------------------------------------------------------------
 | int snprintf( char *str, size_t len, char *format, ... )
 |               char *str    - pointer to the buffer to print for;
 |               size_t len   - size of buffer to print for;
 |               char *format - pointer to the format string
 |                              see printf() manual;
 |               ...          - other argument like in the
 |                              printf() function.
 | return   - number of printed symbols otherwize EOF or NULL and
 |            global variable ERRNO is set to indicate the error.
 +------------------------------------------------------------------
 * The base idea is:
 * All xprintf() functions are return number of printed symbols,
 * so if we will use fprintf() function and will print anything
 * then we will know How many symbols have been printed. 
 * If we know How many symbols have been printed then we can
 * allocate a temporary buffer to use it to print sequence of symbol
 * and then simply copy needed number of symbols into the buffer.
 * -------------------------------------------------------
 * Need some information about parsing of ERRORs inside
 * snprintf function in the OpenVMS 8.4 to make compatible
 * function on the OpenVMS 7.3
 * -------------------------------------------------------
 * Inside this realise of function snprintf are
 * 4 places for parsing posibble ERRORS:
 *
 * - error from fopen() function
 * - error from fclose() function
 * - error from vfprintf()
 * - error from malloc() to allocate temporary buffer
 * -------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


int snprintf( char *str, size_t len, char *format, ... )
{
  FILE *sys$null;
  char *null$dev = "NLA0:";                /* The name of NULL OpenVMS device */
  char *tmpbuf;
  int retval;
  va_list ap;

  sys$null = fopen( null$dev, "w" );

  if( sys$null )
    {
      va_start(ap, format);
      retval = vfprintf( sys$null, format, ap  );
      if( !fclose( sys$null ) )
        {
          if( retval > 0 )
            {
              retval++;      /* added for the tailing '\0'  */
              tmpbuf = malloc( retval );
              if( tmpbuf )
                {
                   retval = vsprintf( tmpbuf, format, ap );
                   /* retval will be returned from snprintf() if all is Ok */
                   if( retval > 0 )
                     {
                       retval++;  /* increment for the tailing '\0' */
                       memcpy( str, tmpbuf, ( ( retval < len )? retval : len ) );
                       retval--;  /* return How many symbols has been printed into the buffer */
                     }       /* end of if( retval )    */
                   free( tmpbuf );
                }            /* end of if( tmpbuf )     */
              else
                {
                   /* error allocate temporary memory                                */
                   /* printf( "Error allocate memory for the temporary buffer.\n" ); */
                   /* simply return NULL and need to check ERRNO                     */
                   retval = (int) NULL;
                }
            }                /* end of if( retval > 0 ) */
          else
            {
                /* error from vfprintf()                               */
                /* printf( "Something wrong with the vfprintf().\n" ); */
                /* nothing to do, simply return what return vfprintf() */
                ;
            }
        }                  /* end of if( !fclose( sys$null ) ) */

      else  retval =  EOF; /* return error EOF if fclose() return error */

      va_end(ap);

    }                      /* end of if( sys$null )   */
  else
    {
        /* error fopen null device                       */
        /* printf( "Error open SYS$NULL device.\n" );    */
        /* return EOF and need to check ERRNO            */
        retval = EOF;
    }
  return( retval );
}
