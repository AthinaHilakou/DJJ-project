#include "../headers/acutest.h"
#include "../headers/data.h"
#include <math.h>

void test_import(void) {
    int lines;
    Data points = import_data("datasets/given/00000020.bin", &lines);
    TEST_CHECK(points != NULL);

    // Check that the first point is correct
    TEST_CHECK(isfinite(points[0].data_array[0]));
    TEST_CHECK(isfinite(points[0].data_array[0]));

    // Check that the last point is correct
    TEST_CHECK(isfinite(points[lines -1].data_array[0]));
    TEST_CHECK(isfinite(points[lines -1].data_array[0]));

    free(points);
}

TEST_LIST = {
    { "test_import", test_import },
    { NULL, NULL }
};
