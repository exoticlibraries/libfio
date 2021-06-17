/*!gcc {0} -I. -I../../include/ -I../../libxtd/include/ -I../include/ -I../../../libcester/include -o out; ./out */
/*!g++ -ansi -pedantic-errors {0} -I. -I../../include/ -I../../../libxtd/include/ -I../include/ -I../../../libcester/include -o out; ./out */
/*!gcc {0} -I. -I../../include/ -I../../../libcester/include -I../include/ -o out; ./out */
/*!g++ -std=c++11 {0} -I. -I../../include/ -I../include/ -I../../../libcester/include -o out; ./out */

#include <exotic/cester.h>
#include <exotic/fio/fs.h>

CESTER_TODO_TEST(test_init_destroy_file_path, test_inst, {
    FioPath *path;
    enum x_stat status;
    char full_file_path[260];

    status = fio_absolute_path_name(test_inst->argv[0], full_file_path);
    cester_assert_uint_eq(status, XTD_OK);
    status = init_fio_path(full_file_path, &path);
    cester_assert_uint_eq(status, XTD_OK);
    cester_assert_str_not_equal(path->root_name, XTD_NULL);
    cester_assert_str_not_equal(path->root_path, XTD_NULL);
    cester_assert_str_not_equal(path->parent_path, XTD_NULL);
    cester_assert_str_not_equal(path->value, XTD_NULL);
    cester_assert_str_not_equal(path->name, XTD_NULL);
    cester_assert_str_not_equal(path->name_only, XTD_NULL);
    cester_assert_str_not_equal(path->extension, XTD_NULL);

    destroy_fio_path(path);
})

CESTER_TODO_TEST(test_fio_path_1, test_inst, {
    FioPath *path;
    enum x_stat status;

    status = init_fio_path("..\\..\\include\\exotic\\fio\\fs.h", &path);
    cester_assert_uint_eq(status, XTD_OK);
    cester_assert_str_equal(path->value, "..\\..\\include\\exotic\\fio\\fs.h");
    cester_assert_str_equal(path->name, "fs.h");
    cester_assert_str_equal(path->name_only, "fs");
    cester_assert_str_equal(path->extension, ".h");

    status = init_fio_path("../../include/exotic/fio/fs", &path);
    cester_assert_uint_eq(status, XTD_OK);
    cester_assert_str_equal(path->value, "../../include/exotic/fio/fs");
    cester_assert_str_equal(path->name, "fs");
    cester_assert_str_equal(path->name_only, "fs");
    cester_assert_str_equal(path->name, path->name_only);
    cester_assert_str_equal(path->extension, "");

    destroy_fio_path(path);
})

CESTER_TODO_TEST(test_fio_path_2, test_inst, {
    FioPath *path;
    enum x_stat status;

    status = init_fio_path("/tmp/crdownload", &path);
    cester_assert_uint_eq(status, XTD_OK);
    cester_assert_str_equal(path->value, "/tmp/crdownload");
    cester_assert_str_equal(path->root_name, "");
    cester_assert_str_equal(path->root_path, "/");
    cester_assert_str_equal(path->name, "crdownload");
    cester_assert_str_equal(path->name_only, "crdownload");
    cester_assert_str_equal(path->extension, "");

    status = init_fio_path("/tmp/crdownload.mp4.tmp", &path);
    cester_assert_uint_eq(status, XTD_OK);
    cester_assert_str_equal(path->value, "/tmp/crdownload.mp4.tmp");
    cester_assert_str_equal(path->root_name, "");
    cester_assert_str_equal(path->root_path, "/");
    cester_assert_str_equal(path->name, "crdownload.mp4.tmp");
    cester_assert_str_equal(path->name_only, "crdownload.mp4");
    cester_assert_str_equal(path->extension, ".tmp");

    destroy_fio_path(path);
})

CESTER_OPTIONS(
    CESTER_VERBOSE_LEVEL(3);
)