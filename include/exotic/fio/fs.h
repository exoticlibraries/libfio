
/**
    \copyright MIT License Copyright (c) 2020, Adewale Azeez 
    \author Adewale Azeez <azeezadewale98@gmail.com>
    \date 07 April 2020
    \file xconf.h
*/

#ifndef EXOTIC_XCOMMON_H
#define EXOTIC_XCOMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/**
    Define bool to be unsigned
*/
#ifndef bool 
#define bool unsigned
#endif
#ifndef FALSE
    #define FALSE 0
#endif
#ifndef TRUE
    #define TRUE !FALSE
#endif
#ifndef false
    #define false 0
#endif
#ifndef true
    #define true !false
#endif

/**
    The status codes for operation in the x-types 
    library.
*/
enum fio_stat {
    FIO_OK = 0,                       /**< the operation successful */
    FIO_ALLOC_ERR,                    /**< unable to allocate memory for a pointer */
};

/**
    
*/
struct fio_path_s {
    const char *root_name;
    const char *root_path;
    const char *parent_path;
    const char *fullname;
    const char *nameonly;
    const char *extension;
};

#ifdef __cplusplus
}
#endif

#endif