/*!gcc {0} -I. -I../../include/ -I../../../libcester/include -I../include/ -o out; ./out */

#include <stdio.h>
#include <exotic/fio/fs.h>

#define METAREF_USING_ELSE(body, alt_body) \
    if (_status == 0) {\
        body\
    } else {\
        alt_body\
    }\

#define METAREF_USING(signature, type, variable, body) \
    {\
        type variable;\
        unsigned _status = init_##signature(&variable);\
        body\
        if (_status == 0) {\
            destroy_##signature(variable);\
        }\
    }

int main() {
    METAREF_USING(fio_path, FioPath *, path, 
    {
        
    })
    METAREF_USING(fio_path, FioPath *, path, 
    METAREF_USING_ELSE({
        printf("It passed\n");
    }, 
    {
        printf("It failed\n");
    })
    )
}