#include <string.h>

char *dirname(char *path)
{
    char *p;

    if( path == NULL || *path == '\0')
        return ".";

    p = path + strlen(path) - 1;
    while( *p == '/' ) {
        if( p == path )
            return path;
        *p-- = '\0';
    }
    while ( p >= path && *p != '/' )
        p--;
    
    if( p < path )
        return ".";
    else if ( p == path )
        return "/";
    else {
        *p = '\0';
        return path;
    }
}
