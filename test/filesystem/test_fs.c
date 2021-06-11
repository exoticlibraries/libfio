/*!gcc {0} -I. -I../../include/ -I../../libxtd/include/ -I../include/ -I../../../libcester/include -o out; ./out */
/*!g++ -ansi -pedantic-errors {0} -I. -I../../include/ -I../../../libxtd/include/ -I../include/ -I../../../libcester/include -o out; ./out */
/*!gcc {0} -I. -I../../include/ -I../../../libcester/include -I../include/ -o out; ./out */
/*!g++ -std=c++11 {0} -I. -I../../include/ -I../include/ -I../../../libcester/include -o out; ./out */

#include <exotic/cester.h>
#include <exotic/fio/fs.h>

CESTER_TEST(test_init_destroy_file_path, _, {
    FioPath *path;
    enum x_stat status;

    status = init_fio_path("C:/Path/test_fc.txt", &path);
    cester_assert_uint_eq(status, XTD_OK);
    printf("Root Name: %s\n", path->root_name);
    printf("Root Path: %s\n", path->root_path);
    printf("Parent Path: %s\n", path->parent_path);
    printf("Full Path: %s\n", path->full_path);
    printf("Name: %s\n", path->name);
    printf("Name Only: %s\n", path->name_only);
    printf("File Extension: %s\n", path->extension);

    destroy_fio_path(path);
})

