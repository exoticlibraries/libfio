
/**
    \copyright MIT License Copyright (c) 2020, Adewale Azeez 
    \author Adewale Azeez <azeezadewale98@gmail.com>
    \date 07 April 2020
    \file xconf.h
*/

#ifndef EXOTIC_FS_H
#define EXOTIC_FS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <exotic/xtd/xcommon.h>
/*#include <exotic/xtd/xstring.h>*/

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
    
*/
#ifdef _WIN32
#define FIO_FILE_SEPERATOR '\\'
#else
#define FIO_FILE_SEPERATOR '/'
#endif

/**
    
*/
struct fio_path_s {
    const char *root_name;
    const char *root_path;
    const char *parent_path;
    const char *full_path;
    const char *name_only;
    const char *extension;
    void *(*memory_calloc) (size_t blocks, size_t size);    /**<  memory allocator used to allocate the array and it buffer. calloc */
    void  (*memory_free)   (void *block);                   /**<  the free funtion to release the memory of the array and it buffer */
};

/**
    
*/
typedef struct fio_path_s FioPath;

/* use unicode conscious code */
/* move to exotic/xtd/xstring */
size_t xnative_string_index_of(const char *str, const char delim) {
    size_t index = 0;
    if (str == XTD_NULL) {
        return -1;
    }
    do {
        const char current_char = (*str);
        if (current_char == '\0' || current_char == XTD_NULL || current_char == delim) {
            return index;
        }
    } while (str++ && ++index);
    return -1;
}

/* provide another that reads from behind */
int xnative_string_last_index_of_with_length(const size_t str_len, const char *str, const char delim) {
    int index = str_len;
    if (str_len == 0 || str == XTD_NULL) {
        return -1;
    }
    while (index-- > 0){
        const char current_char = str[index];
        if (current_char == delim) {
            return index;
        }
    }
    return -1;
}

int xnative_string_last_index_of(const char *str, const char delim) {
    return xnative_string_last_index_of_with_length(strlen(str), str, delim);
}

enum x_stat xnative_string_sub_string_with_length(const size_t str_len, const char *str, size_t begin_index, size_t end_index, char *out) {
    size_t str_index = 0;
    if (begin_index < 0 || begin_index > str_len || end_index >= str_len) {
        return XTD_OUT_OF_RANGE_ERR;
    }
    for (size_t index = begin_index; index <= end_index; index++) {
        out[str_index++] = str[index];
    }
    out[str_index] = '\0';
    return XTD_OK;
}

enum x_stat xnative_string_sub_string(const char *str, size_t begin_index, size_t end_index, char *out) {
    return xnative_string_sub_string_with_length(strlen(str), str, begin_index, end_index, out);
}

enum x_stat xnative_string_sub_string_from(const char *str, size_t begin_index, char *out) {
    return xnative_string_sub_string(str, begin_index, strlen(str)-1, out);
}

static const char *__fio_path_get_within_internal__(const char *path, size_t delim_last_index) {
    enum x_stat status;
    size_t new_str_len = (strlen(path)-delim_last_index+1);
    char *name_only = calloc(1, sizeof(char) * new_str_len);
    if (name_only == XTD_NULL) {
        return XTD_NULL;
    }
    status = xnative_string_sub_string_from(path, delim_last_index+1, name_only);
    if (status != XTD_OK) {
        return XTD_NULL;
    }
    return name_only;
}

/**
    Use mem allocator struct from linxtd, new from config
*/
enum x_stat init_fio_path(const char *path, struct fio_path_s **out) {
    struct fio_path_s *fio_path;
    size_t new_str_len;
    int delim_last_index;
    enum x_stat status;
    char *name_only;
    char *extension;
    if (path == XTD_NULL || out == XTD_NULL) {
        return XTD_PARAM_NULL_ERR;
    }
    fio_path = calloc(1, sizeof(struct fio_path_s));
    if (fio_path == XTD_NULL) {
        return XTD_ALLOC_ERR;
    }
    delim_last_index = xnative_string_last_index_of(path, '/');
    if (delim_last_index > -1) {
        name_only = __fio_path_get_within_internal__(path, delim_last_index);
        if (name_only == XTD_NULL) {
            free(fio_path);
            return XTD_ALLOC_ERR;
        }
        delim_last_index = xnative_string_last_index_of(name_only, '.');
        if (delim_last_index > -1) { 
            extension = __fio_path_get_within_internal__(name_only, delim_last_index-1);
            if (extension == XTD_NULL) {
                free(name_only);
                free(fio_path);
                return XTD_ALLOC_ERR;
            }
        } else {
            extension = calloc(1, sizeof(char));
            extension[0] = '\0';
        }
    }
    
    fio_path->full_path = path;
    fio_path->name_only = name_only;
    fio_path->extension = extension;
    fio_path->memory_free = free;
    *out = fio_path;
    return XTD_OK;
} 

/**
    
*/
void destroy_fio_path(struct fio_path_s *fio_path) {
    fio_path->memory_free(fio_path->name_only);
    fio_path->memory_free(fio_path->extension);
    fio_path->memory_free(fio_path);
}


#ifdef __cplusplus
}
#endif

#endif