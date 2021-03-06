
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

#include <stdio.h>
#include <exotic/xtd/xcommon.h>
#include <exotic/xtd/xstring.h>
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

#define FIO_UNIX_FILE_SEPEATOR    '/'    /**<   */
#define FIO_WIN32_FILE_SEPEATOR   '\\'    /**<   */
#define FIO_UNIX_FILE_SEPEATOR_STR    "/"    /**<   */
#define FIO_WIN32_FILE_SEPEATOR_STR   "\\"    /**<   */

/**

*/
#ifdef _WIN32
#define FIO_FILE_SEPERATOR FIO_WIN32_FILE_SEPEATOR
#define FIO_FILE_SEPERATOR_STR FIO_WIN32_FILE_SEPEATOR_STR
#else
#define FIO_FILE_SEPERATOR FIO_UNIX_FILE_SEPEATOR
#define FIO_FILE_SEPERATOR_STR FIO_UNIX_FILE_SEPEATOR_STR
#endif

/*!
    
*/
static enum x_stat fio_get_current_dir(XAllocator allocator, char **out) {
    char *value;
    char current_dir[256];

    if (out == XTD_NULL) return XTD_PARAM_NULL_ERR;
    #ifdef _WIN32
        if (_getcwd(current_dir, 256) == XTD_NULL) {
    #else
        if (getcwd(current_dir, sizeof(current_dir)) == XTD_NULL) {
    #endif
            return XTD_ERR;
        }
    value = xstring_cstr_concat_cstr(allocator, XTD_NULL, current_dir);
    *out = value;

    return XTD_OK;
}

/*!
    
*/
static enum x_stat fio_absolute_path_name(char *file_name, char *out) {
    if (file_name == XTD_NULL || out == XTD_NULL) {
        return XTD_PARAM_NULL_ERR;
    }
    #ifdef _WIN32
        if (GetFullPathName(file_name, MAX_PATH, out, XTD_NULL) == 0) {
    #elif defined(__STDC_VERSION__)
        if (realpath(file_name, out)) {
    #else
        if (TRUE) {
    #endif
            return XTD_ERR;
        }
    return XTD_OK;
}

/*!

*/
static enum x_stat fio_normalize_path(XAllocator allocator, char *rough_path, char seperator, char **out) {
    char *normalized_path;
    size_t index, next_index;
    size_t rough_path_length;

    if (rough_path == XTD_NULL || out == XTD_NULL) return XTD_PARAM_NULL_ERR;
    rough_path_length = xstring_cstr_length(rough_path);
    normalized_path = xstring_cstr_concat_cstr(allocator, XTD_NULL, "");
    for (index = 0; index < rough_path_length; index++) {
        switch (rough_path[index]) {
            case '\\':
            case '/':
                normalized_path = xstring_cstr_concat_char_free_old(allocator, normalized_path, seperator);
                next_index = index + 1;
                while (next_index < rough_path_length && (rough_path[next_index] == '\\' || rough_path[next_index] == '/')) {
                    next_index++;
                    index++;
                }
                break;
            default:
                normalized_path = xstring_cstr_concat_char_free_old(allocator, normalized_path, rough_path[index]);
        }
    }
    *out = normalized_path;
    
    return XTD_OK;
}

/**
    
*/
static enum x_stat fio_relative_path_name(XAllocator allocator, char *parent_path, char *child_path, char seperator, char **out) {
    char *relative_path;
    char *normalized_parent_path;
    char *normalized_child_path;
    size_t normalized_child_path_length;
    size_t normalized_parent_path_length;

    if (parent_path == XTD_NULL || child_path == XTD_NULL || out == XTD_NULL) return XTD_PARAM_NULL_ERR;
    if (fio_normalize_path(allocator, parent_path, seperator, &normalized_parent_path) != XTD_OK) return XTD_CRITICAL_ERR;
    if (fio_normalize_path(allocator, child_path, seperator, &normalized_child_path) != XTD_OK) return XTD_CRITICAL_ERR;
    if (!xstring_cstr_starts_with(normalized_child_path, normalized_parent_path))  {
        allocator.memory_free(normalized_parent_path);
        allocator.memory_free(normalized_child_path);
        return XTD_NO_OP;
    }
    normalized_child_path_length = xstring_cstr_length(normalized_child_path);
    normalized_parent_path_length = xstring_cstr_length(normalized_parent_path);
    relative_path = xstring_cstr_concat_cstr(allocator, XTD_NULL, "");
    for (; normalized_parent_path_length < normalized_child_path_length; normalized_parent_path_length++) {
        relative_path = xstring_cstr_concat_char_free_old(allocator, relative_path, normalized_child_path[normalized_parent_path_length]);
    }
    *out = relative_path;

    return XTD_OK;
}

/**
    
*/
static enum x_stat fio_file_name_from_path(char *path, char *out) {
    int index_unix;
    int index_win32;
    if (path == XTD_NULL || out == XTD_NULL) {
        return XTD_PARAM_NULL_ERR;
    }
    index_unix = xstring_cstr_last_index_of(path, FIO_UNIX_FILE_SEPEATOR_STR);
    index_win32 = xstring_cstr_last_index_of(path, FIO_WIN32_FILE_SEPEATOR_STR);
    index_unix = (index_unix > index_win32) ? index_unix : index_win32;
    return xstring_cstr_sub_string(path, ++index_unix, out);
}

/**
    
*/
static enum x_stat fio_file_name_only(char *path, char *out) {
    int index;
    if (path == XTD_NULL || out == XTD_NULL) {
        return XTD_PARAM_NULL_ERR;
    }
    index = xstring_cstr_last_index_of(path, ".");
    if (index == -1) {
        index = xstring_cstr_length(path);
    }
    return xstring_cstr_sub_string_in_range(path, 0, index, out);
}

/**
    
*/
static enum x_stat fio_file_extension(char *path, char *out) {
    int index;
    if (path == XTD_NULL || out == XTD_NULL) {
        return XTD_PARAM_NULL_ERR;
    }
    index = xstring_cstr_last_index_of(path, ".");
    if (index == -1) {
        index = xstring_cstr_length(path);
    }
    return xstring_cstr_sub_string(path, index, out);
}

/**
   Create a new file.
   This technique works by creating the specified file
   and file type.

   \param file_name - Non-nullable array of characters.
 */
static bool fio_create_file(char file_name[]) {
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
static bool fio_delete_file(char file_name[]) {
    return remove(file_name) == 0;
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
static bool fio_file_exists(char file_name[]) {
    FILE * file = fopen(file_name, "r");
    if (file) {
        fclose(file);
        return TRUE;
    }
    return FALSE;
}

/*
    
*/
typedef bool (*fio_func_ptr_report_char)(char);

/*
    Read file.
*/
static enum x_stat fio_read_file_chars_cb(FILE * file, fio_func_ptr_report_char callback) {
    if (file == XTD_NULL || callback == XTD_NULL) {
        return XTD_PARAM_NULL_ERR;
    }
    while (TRUE) {
        char c = fgetc(file);
        if (feof(file)) {
            if (!callback('\0')) return XTD_TERMINATED_ERR;
            break;
        }
        if (!callback(c)) return XTD_TERMINATED_ERR;
    }
    return XTD_OK;
}

/* 
    Read file from path.
*/
static enum x_stat fio_read_file_chars_cb_from_path(char *fileName, fio_func_ptr_report_char callback) {
    FILE * file = fopen(fileName, "r");
    enum x_stat status = fio_read_file_chars_cb(file, callback);
    if (status == XTD_OK) {
        fclose(file);
    }
    return status;
}

/*
    
*/
typedef bool (*fio_func_ptr_report_char2)(void *, char);

/*
    Read file. 2
*/
static enum x_stat fio_read_file_chars_cb2(FILE * file, fio_func_ptr_report_char2 callback, void *param) {
    if (file == XTD_NULL || callback == XTD_NULL) {
        return XTD_PARAM_NULL_ERR;
    }
    while (TRUE) {
        char c = fgetc(file);
        if (feof(file)) {
            if (!callback(param, '\0')) return XTD_TERMINATED_ERR;
            break;
        }
        if (!callback(param, c)) return XTD_TERMINATED_ERR;
    }
    return XTD_OK;
}

/* 
    Read file from path. 2
*/
static enum x_stat fio_read_file_chars_cb_from_path2(char *fileName, fio_func_ptr_report_char2 callback, void *param) {
    FILE * file = fopen(fileName, "r");
    enum x_stat status = fio_read_file_chars_cb2(file, callback, param);
    if (status == XTD_OK) {
        fclose(file);
    }
    return status;
}

/*
    Read all file string.
*/
static enum x_stat fio_read_all_file_content(FILE * file, char *out) {
    size_t index = 0;
    if (file == XTD_NULL || out == XTD_NULL) {
        return XTD_PARAM_NULL_ERR;
    }
    while (TRUE) {
        char c = fgetc(file);
        if (feof(file)) {
            out[index] = '\0';
            break;
        }
        out[index++] = c;
    }
    return XTD_OK;
}

/* 
    Read all file string from path.
*/
static enum x_stat fio_read_all_file_content_from_path(char *fileName, char *out) {
    FILE * file = fopen(fileName, "r");
    enum x_stat status = fio_read_all_file_content(file, out);
    if (status == XTD_OK) {
        fclose(file);
    }
    return status;
}


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

/*static char *__fio_path_get_within_internal__(const char *path, size_t delim_last_index) {
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
}*/

/**
    Use mem allocator struct from linxtd, new from config
*/
/*enum x_stat init_fio_path(const char *path, struct fio_path_s **out) {
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
}*/

/**
    
*/
/*void destroy_fio_path(struct fio_path_s *fio_path) {
    if (fio_path->extension != "") {
        fio_path->memory_free(fio_path->extension);
    }
    if (fio_path->name != fio_path->name_only) {
        fio_path->memory_free(fio_path->name_only);
    }
    fio_path->memory_free(fio_path->name);
    fio_path->memory_free(fio_path);
}*/


#ifdef __cplusplus
}
#endif

#endif