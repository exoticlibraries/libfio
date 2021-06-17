
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
#ifdef _WIN32
#include <windows.h>
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

#define FIO_UNIX_FILE_SEPEATOR    '/'    /**<   */
#define FIO_WIN32_FILE_SEPEATOR   '\\'    /**<   */

/**

*/
#ifdef _WIN32
#define FIO_FILE_SEPERATOR FIO_WIN32_FILE_SEPEATOR
#else
#define FIO_FILE_SEPERATOR FIO_UNIX_FILE_SEPEATOR
#endif

#ifndef XTD_PARAM_NULL_ERR
#define XTD_PARAM_NULL_ERR 10
#endif 

/**
    
*/
struct fio_path_s {
    const char *value;
    const char *root_name;
    const char *root_path;
    const char *parent_path;
    /*const char *relative_path;
    const char *absolute_path;*/
    const char *name;
    const char *name_only;
    const char *extension;
    void *(*memory_calloc) (size_t blocks, size_t size);    /**<  memory allocator used to allocate the array and it buffer. calloc */
    void  (*memory_free)   (void *block);                   /**<  the free funtion to release the memory of the array and it buffer */
};

/**
    
*/
typedef struct fio_path_s FioPath;

/**
    
*/
enum x_stat fio_absolute_path_name(char *file_name, char *out) {
    #ifdef _WIN32
        if (GetFullPathName(file_name, MAX_PATH, out, XTD_NULL) == 0) {
    #else

    #endif
            return XTD_ERR;
        }
    return XTD_OK;
}


/* TODO RESOLVE BELOW */

/* use unicode conscious code */
/* move to exotic/xtd/xstring */
size_t xnative_string_index_of_with_length(const size_t str_len, const char *str, const char delim) {
    size_t index = 0;
    if (str == XTD_NULL) {
        return -1;
    }
    for (; index < str_len; index++) {
        if (str[index] == delim) {
            return index;
        }
    }
    return -1;
}

size_t xnative_string_index_of(const char *str, const char delim) {
    return xnative_string_index_of_with_length(strlen(str), str, delim);
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
    size_t index;
    if (begin_index < 0 || begin_index > str_len || end_index >= str_len) {
        return XTD_OUT_OF_RANGE_ERR;
    }
    for (index = begin_index; index <= end_index; index++) {
        out[str_index++] = str[index];
    }
    return XTD_OK;
}

enum x_stat xnative_string_sub_string(const char *str, size_t begin_index, size_t end_index, char *out) {
    return xnative_string_sub_string_with_length(strlen(str), str, begin_index, end_index, out);
}

enum x_stat xnative_string_sub_string_from(const char *str, size_t begin_index, char *out) {
    return xnative_string_sub_string(str, begin_index, strlen(str)-1, out);
}

static char *__fio_path_get_within_internal__(const char *path, size_t delim_last_index) {
    enum x_stat status;
    size_t new_str_len = (strlen(path)-delim_last_index+1);
    char *name_only = calloc(1, sizeof(char) * new_str_len);
    if (name_only == XTD_NULL) {
        return XTD_NULL;
    }
    status = xnative_string_sub_string_from(path, delim_last_index, name_only);
    name_only[new_str_len] = '\0';
    if (status != XTD_OK) {
        return XTD_NULL;
    }
    return name_only;
}

static char *__fio_path_get_within_internal_front__(const char *path, size_t end_index) {
    enum x_stat status;
    char *name_only = calloc(1, sizeof(char) * (end_index + 1));
    if (name_only == XTD_NULL) {
        return XTD_NULL;
    }
    status = xnative_string_sub_string(path, 0, end_index, name_only);
    name_only[end_index+1] = '\0';
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
    char *root_name = XTD_NULL;
    char *root_path = XTD_NULL;
    char *parent_path = XTD_NULL;
    char *name = XTD_NULL;
    char *name_only = XTD_NULL;
    char *extension = "";
    char seperator = FIO_UNIX_FILE_SEPEATOR;
    if (path == XTD_NULL || out == XTD_NULL) {
        return XTD_PARAM_NULL_ERR;
    }
    fio_path = calloc(1, sizeof(struct fio_path_s));
    if (fio_path == XTD_NULL) {
        return XTD_ALLOC_ERR;
    }
    delim_last_index = xnative_string_index_of(path, seperator);
    if (delim_last_index == -1) {
        seperator = FIO_WIN32_FILE_SEPEATOR;
        delim_last_index = xnative_string_index_of(path, seperator);
    }
    if (delim_last_index > -1) {
        root_path = __fio_path_get_within_internal_front__(path, delim_last_index);
         if (root_path == XTD_NULL) {
            free(fio_path);
            return XTD_ALLOC_ERR;
        }
        if (delim_last_index > 0) {
            new_str_len = strlen(root_path)-1;
            root_name = calloc(1, sizeof(char) * new_str_len);
            status = xnative_string_sub_string(path, 0, new_str_len-1, root_name);
            if (status != XTD_OK) {
                free(root_path);
                free(fio_path);
                return XTD_ALLOC_ERR;
            }
        } else {
            root_name = calloc(1, sizeof(char));
            root_name[0] = '\0';
            if (root_name == XTD_NULL) {
                free(root_path);
                free(fio_path);
                return XTD_ALLOC_ERR;
            }
        }
    }
    delim_last_index = xnative_string_last_index_of(path, seperator);
    if (delim_last_index > -1) {
        parent_path = __fio_path_get_within_internal_front__(path, delim_last_index);
        name = __fio_path_get_within_internal__(path, delim_last_index+1);
        if (name == XTD_NULL) {
            free(root_name);
            free(root_path);
            free(fio_path);
            return XTD_ALLOC_ERR;
        }
        delim_last_index = xnative_string_last_index_of(name, '.');
        if (delim_last_index > -1) {
            name_only = __fio_path_get_within_internal_front__(name, delim_last_index-1);
            if (root_path == XTD_NULL) {
                free(name);
                free(fio_path);
                return XTD_ALLOC_ERR;
            }
            extension = __fio_path_get_within_internal__(name, delim_last_index);
            if (extension == XTD_NULL) {
                free(name);
                free(name_only);
                free(fio_path);
                return XTD_ALLOC_ERR;
            }
        } else {
            name_only = name;
        }
    }
    
    fio_path->value = path;
    fio_path->root_name = root_name;
    fio_path->parent_path = parent_path;
    fio_path->root_path = root_path;
    fio_path->name = name;
    fio_path->name_only = name_only;
    fio_path->extension = extension;
    fio_path->memory_free = free;
    *out = fio_path;
    return XTD_OK;
} 

/**
    
*/
void destroy_fio_path(struct fio_path_s *fio_path) {
    if (fio_path->extension != "") {
        fio_path->memory_free(fio_path->extension);
    }
    if (fio_path->name != fio_path->name_only) {
        fio_path->memory_free(fio_path->name_only);
    }
    fio_path->memory_free(fio_path->name);
    fio_path->memory_free(fio_path);
}

/**
   Create a new file.
   This technique works by creating the specified file
   and file type.

   \param file_name - Non-nullable array of characters.
 */
bool fio_create_file(char file_name[]) {
    FILE * file = fopen(file_name, "w");
    if(file == NULL) {
        return FALSE;
    }
    fclose(file);
    return TRUE;
}

/**
   Delete a file.
 */
bool fio_delete_file(char file_name[]) {
    return remove(file_name) == 0;
}

/*
    
*/
typedef void (*fio_func_ptr_report_char)(char);

/*
    Read file.
*/
enum x_stat fio_read_file_chars_cb(FILE * file, fio_func_ptr_report_char callback) {
    if (file == XTD_NULL || callback == XTD_NULL) {
        return XTD_PARAM_NULL_ERR;
    }
    while (TRUE) {
        char c = fgetc(file);
        if (feof(file)) {
            break;
        }
        callback(c);
    }
    return XTD_OK;
}

/* 
    Read file from path.
*/
enum x_stat fio_read_file_chars_cb_from_path(char *fileName, fio_func_ptr_report_char callback) {
    FILE * file = fopen(fileName, "r");
    enum x_stat status = fio_read_file_chars_cb(file, callback);
    if (status == XTD_OK) {
        fclose(file);
    }
    return status;
}

/**
    Check if a file exists. This tehcnique works by trying 
    to read the specified file. The file exists if it is
    read successfully. However, it is unable to read a file
    if there is no permission to the directory where the 
    specified file belongs.

    \param file_name -  Non-nullale array of characters

    \return TRUE if the exists, else FALSE
*/
bool fio_file_exists(char file_name[]) {
    FILE * file = fopen(file_name, "r");
    if (file) {
        fclose(file);
        return TRUE;
    }
    return FALSE;
}


#ifdef __cplusplus
}
#endif

#endif