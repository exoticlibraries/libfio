/*!gcc {0} -I. -I../../include/ -I../../libxtd/include/ -I../include/ -I../../../libcester/include -o out; ./out */
/*!g++ -ansi -pedantic-errors {0} -I. -I../../include/ -I../../../libxtd/include/ -I../include/ -I../../../libcester/include -o out; ./out */
/*!gcc {0} -I. -I../../include/ -I../../../libcester/include -I../include/ -o out; ./out */
/*!g++ -std=c++11 {0} -I. -I../../include/ -I../include/ -I../../../libcester/include -o out; ./out */

#include <exotic/cester.h>
#include <exotic/fio/fs.h>

CESTER_BODY(

void log_file(char c) {
    printf("%c", c);
}

)

CESTER_TEST(fio_create_file, test_inst, {
    cester_assert_true(fio_create_file("dami.txt"));
    cester_assert_false(fio_create_file("/folder/dami.txt"));
    cester_assert_false(fio_create_file("dami.$#&$*$*($)$)!@#$%%^&^*&*(()"));
})

CESTER_TEST(fio_file_exists, test_inst, {
    cester_assert_false(fio_file_exists("new-file.txt"));
    cester_assert_true(fio_file_exists("dami.txt"));
})

CESTER_TEST(fio_read_file_chars_cb, test_inst, {
    FILE *my_file = fopen("dami.txt", "a");

    cester_assert_int_eq(fio_read_file_chars_cb(my_file, XTD_NULL), XTD_PARAM_NULL_ERR);
    cester_assert_int_eq(fio_read_file_chars_cb(XTD_NULL, log_file), XTD_PARAM_NULL_ERR);
    fprintf(my_file, "Hello World");

    fclose(my_file);
})

CESTER_TEST(fio_read_file_chars_cb_from_path, test_inst, {
    cester_assert_int_eq(fio_read_file_chars_cb_from_path("dami.txt", log_file), XTD_OK);
})

CESTER_TEST(fio_delete_file, test_inst, {
    cester_assert_true(fio_delete_file("dami.txt"));
    cester_assert_false(fio_delete_file("dami.txt"));
})

