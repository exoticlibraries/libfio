/*!gcc {0} -I. -I../../include/ -I../../libxtd/include/ -I../include/ -I../../../libcester/include -o out; ./out */
/*!g++ -ansi -pedantic-errors {0} -I. -I../../include/ -I../../../libxtd/include/ -I../include/ -I../../../libcester/include -o out; ./out */
/*!gcc {0} -I. -I../../include/ -I../../../libcester/include -I../include/ -o out; ./out */
/*!g++ -std=c++11 {0} -I. -I../../include/ -I../include/ -I../../../libcester/include -o out; ./out */

#include <exotic/cester.h>
#include <exotic/fio/fs.h>

CESTER_TEST(fio_absolute_path_name, test_inst, {
    char full_file_path[260];
    enum x_stat status;

    status = fio_absolute_path_name(test_inst->argv[0], full_file_path);
    cester_assert_uint_eq(status, XTD_OK);
    cester_assert_str_not_equal(full_file_path, XTD_NULL);

    status = fio_absolute_path_name("C:../../.././../../../.././//.../../../.././../../", full_file_path);
    cester_assert_uint_eq(status, XTD_OK);
    cester_assert_str_not_equal(full_file_path, XTD_NULL);
#ifdef _WIN32
    cester_assert_str_equal(full_file_path, "C:\\");
#endif

    status = fio_absolute_path_name("Y:..:/Users/thecarisma", full_file_path);
    cester_assert_uint_eq(status, XTD_OK);
    cester_assert_str_not_equal(full_file_path, XTD_NULL);
#ifdef _WIN32
    cester_assert_str_equal(full_file_path, "Y:\\..:\\Users\\thecarisma");
#endif
})

CESTER_TODO_TEST(fio_relative_path_name, test_inst, {
    char full_file_path[260];
    enum x_stat status;

    status = fio_relative_path_name(test_inst->argv[0], full_file_path);
    cester_assert_uint_eq(status, XTD_OK);
    cester_assert_str_not_equal(full_file_path, XTD_NULL);
})

CESTER_TEST(fio_file_name_from_path, test_inst, {
    char *file_name1 = "./../../test_fs.c";
    char *file_name2 = "C:\\Users\\name\\Documents\\home.blade.php";
    char *file_name3 = "/tmp\\noextension";
    char *file_name4 = "nofilename";
    char file_name_only[20];

    cester_assert_uint_eq(fio_file_name_from_path(XTD_NULL, file_name_only), XTD_PARAM_NULL_ERR);
    cester_assert_uint_eq(fio_file_name_from_path(file_name1, XTD_NULL), XTD_PARAM_NULL_ERR);
    cester_assert_uint_eq(fio_file_name_from_path(XTD_NULL, XTD_NULL), XTD_PARAM_NULL_ERR);
    cester_assert_uint_eq(fio_file_name_from_path(file_name1, file_name_only), XTD_OK);
    cester_assert_str_equal(file_name_only, "test_fs.c");
    cester_assert_uint_eq(fio_file_name_from_path(file_name2, file_name_only), XTD_OK);
    cester_assert_str_equal(file_name_only, "home.blade.php");
    cester_assert_uint_eq(fio_file_name_from_path(file_name3, file_name_only), XTD_OK);
    cester_assert_str_equal(file_name_only, "noextension");
    cester_assert_uint_eq(fio_file_name_from_path(file_name4, file_name_only), XTD_OK);
    cester_assert_str_equal(file_name_only, "nofilename");
})

CESTER_TEST(fio_file_name_only, test_inst, {
    char *file_name1 = "test_fs.c";
    char *file_name2 = "home.blade.php";
    char *file_name3 = "noextension";
    char file_name_only[11];

    cester_assert_uint_eq(fio_file_name_only(XTD_NULL, file_name_only), XTD_PARAM_NULL_ERR);
    cester_assert_uint_eq(fio_file_name_only(file_name1, XTD_NULL), XTD_PARAM_NULL_ERR);
    cester_assert_uint_eq(fio_file_name_only(XTD_NULL, XTD_NULL), XTD_PARAM_NULL_ERR);
    cester_assert_uint_eq(fio_file_name_only(file_name1, file_name_only), XTD_OK);
    cester_assert_str_equal(file_name_only, "test_fs");
    cester_assert_uint_eq(fio_file_name_only(file_name2, file_name_only), XTD_OK);
    cester_assert_str_equal(file_name_only, "home.blade");
    cester_assert_uint_eq(fio_file_name_only(file_name3, file_name_only), XTD_OK);
    cester_assert_str_equal(file_name_only, "noextension");
})

CESTER_TEST(fio_file_extension, test_inst, {
    char *file_name1 = "test_fs.c";
    char *file_name2 = "home.blade.php";
    char *file_name3 = "noextension";
    char file_name_only[11];

    cester_assert_uint_eq(fio_file_extension(XTD_NULL, file_name_only), XTD_PARAM_NULL_ERR);
    cester_assert_uint_eq(fio_file_extension(file_name1, XTD_NULL), XTD_PARAM_NULL_ERR);
    cester_assert_uint_eq(fio_file_extension(XTD_NULL, XTD_NULL), XTD_PARAM_NULL_ERR);
    cester_assert_uint_eq(fio_file_extension(file_name1, file_name_only), XTD_OK);
    cester_assert_str_equal(file_name_only, ".c");
    cester_assert_uint_eq(fio_file_extension(file_name2, file_name_only), XTD_OK);
    cester_assert_str_equal(file_name_only, ".php");
    cester_assert_uint_eq(fio_file_extension(file_name3, file_name_only), XTD_OK);
    cester_assert_str_equal(file_name_only, "");
})

CESTER_OPTIONS(
    CESTER_VERBOSE_LEVEL(3);
)
