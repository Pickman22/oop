#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <stdio.h>
#include "ringbuffer.h"

static void test_ring_buffer_invalid_arguments_returns_error(void **state) {
    (void)state;
    uint8_t tmp[10];
    Ring_buffer_t buffer;
    assert_int_equal(Ring_buffer_init(&buffer, NULL, 0, 0), -1);
    assert_int_equal(Ring_buffer_init(&buffer, tmp, 0, 0), -1);
    assert_int_equal(Ring_buffer_init(&buffer, tmp, BUFFER_SIZE_MAX + 1U, 0),-1);
    /* Test buffer will reject a data size larger than the buffer itself. */
    assert_int_equal(Ring_buffer_init(&buffer, tmp, 12U, 10U), -1);
    /* Test push method returns error for invalid arguments. */
    assert_int_equal(Ring_buffer_push(NULL, NULL), -1);
    /* Teest pop method returns error for invalid arguments. */
    assert_int_equal(Ring_buffer_pop(NULL, NULL), -1);
}

static void test_ring_buffer_valid_arguments_returns_no_error(void **state) {
    (void)state;
    uint8_t tmp[10U];
    Ring_buffer_t buffer;
    assert_false(Ring_buffer_init(&buffer, tmp, sizeof(uint8_t), 10U));
}

static void test_ringbuffer_inits_correctly(void **state) {
    (void)state;
    uint8_t tmp[10];
    Ring_buffer_t buffer;
    assert_false(Ring_buffer_init(&buffer, tmp, sizeof(uint8_t), 10U));
    assert_true(buffer.tail == buffer.head);
    assert_true((uint8_t*)buffer.tail == tmp);
    assert_true((uint8_t*)buffer.first == tmp);
    assert_true((uint8_t*)buffer.last == &tmp[9]);
    assert_true(buffer.datasz == sizeof(uint8_t));
}

static void test_ringbuffer_push_method(void **state) {
    (void)state;
    Ring_buffer_t buf;
    uint8_t mem[10U];
    uint8_t data = 0xff;
    Ring_buffer_init(&buf, mem, sizeof(uint8_t), 10U);
    int i;
    assert_true(Ring_buffer_is_empty(&buf));

    /* Fill buffer. It must not return errors within the following for loop. */
    for(i = 0; i < 10; ++i) {
        assert_true(buf.head == &mem[i]);
        assert_true(buf.tail == mem);
        assert_true(buf.count == i);
        assert_int_equal(0, Ring_buffer_push(&buf, &data));
    }

    /* Check that the pointers updated correctly, and that it's full. */
    assert_true(buf.head == buf.tail);
    assert_true(Ring_buffer_is_full(&buf));
    /* If another element is added to the buffer, the tail pointer must track
    the head pointer. */
    assert_int_equal(0, Ring_buffer_push(&buf, &data));
    assert_true(buf.tail == buf.tail);
}

static void test_ringbuffer_pop_method(void **state) {
    (void)state;
    Ring_buffer_t buf;
    uint8_t mem[10U];
    uint8_t data = 0xff;
    uint8_t tmp;
    Ring_buffer_init(&buf, mem, sizeof(uint8_t), 10U);
    int i;
    assert_true(Ring_buffer_is_empty(&buf));

    /* Test buffer returns error when empty and pop is called. */
    assert_int_equal(-1, Ring_buffer_pop(&buf, &tmp));

    /* Fill buffer. It must not return errors within the following for loop. */
    for(i = 0; i < 10; ++i) {
        assert_true(buf.head == &mem[i]);
        assert_true(buf.tail == mem);
        assert_true(buf.count == i);
        assert_int_equal(0, Ring_buffer_push(&buf, &data));
    }
    assert_true(Ring_buffer_is_full(&buf));

    /* Now let's pop the values and verify correct behaviour. */
    for(i = 0; i < 10; ++i) {
        assert_int_equal(0, Ring_buffer_pop(&buf, &tmp));
        assert_true(tmp == data);
    }

    /* It must be empty now. */
    assert_true(Ring_buffer_is_empty(&buf));
    assert_true(buf.head == buf.tail);



}

static void test_ringbuffer_functionality(void **state) {
    (void)state;
    /* Let's push some more data without, filling. Afeter that, poping some
    amount, and then pushing some more, and poping some more. This in order
    to test correct behaviour of pointer wrap arounds. */
    Ring_buffer_t buf;
    uint8_t mem[10U];
    uint8_t data = 0xff;
    uint8_t tmp;
    Ring_buffer_init(&buf, mem, sizeof(uint8_t), 10U);
    int i;
    int j;
    assert_true(Ring_buffer_is_empty(&buf));
    for(j = 0; j < 20; ++j) {
        for(i = 0; i < 8; ++i) {
            assert_int_equal(0, Ring_buffer_push(&buf, &data));
        }
        /* Test buffer is not full yet. */
        assert_false(Ring_buffer_is_full(&buf));

        /* Now let's pop the values and verify correct behaviour. */
        for(i = 0; i < 5; ++i) {
            assert_int_equal(0, Ring_buffer_pop(&buf, &tmp));
            assert_true(tmp == data);
        }
        /* Test buffer is neither full nor empty. */
        assert_false(Ring_buffer_is_full(&buf));
        assert_false(Ring_buffer_is_empty(&buf));
        assert_true(buf.count == 3);

        for(i = 0; i < 8; ++i) {
            assert_int_equal(0, Ring_buffer_push(&buf, &data));
        }
        assert_true(buf.count == 10);
        assert_true(Ring_buffer_is_full(&buf));

        /* Now let's pop the values and verify correct behaviour. */
        for(i = 0; i < 10; ++i) {
            assert_int_equal(0, Ring_buffer_pop(&buf, &tmp));
            assert_true(tmp == data);
        }
        assert_true(Ring_buffer_is_empty(&buf));
    }
}

static void test_ringbuffer_functionality_16bits(void **state) {
    (void)state;
    /* Let's push some more data without, filling. Afeter that, poping some
    amount, and then pushing some more, and poping some more. This in order
    to test correct behaviour of pointer wrap arounds. */
    Ring_buffer_t buf;
    uint16_t mem[10U];
    uint16_t data = 0xff00;
    uint16_t tmp;
    Ring_buffer_init(&buf, mem, sizeof(uint16_t), 10U);
    int i;
    int j;
    assert_true(Ring_buffer_is_empty(&buf));
    for(j = 0; j < 20; ++j) {
        for(i = 0; i < 8; ++i) {
            assert_int_equal(0, Ring_buffer_push(&buf, &data));
        }
        /* Test buffer is not full yet. */
        assert_false(Ring_buffer_is_full(&buf));

        /* Now let's pop the values and verify correct behaviour. */
        for(i = 0; i < 5; ++i) {
            assert_int_equal(0, Ring_buffer_pop(&buf, &tmp));
            assert_true(tmp == data);
        }
        /* Test buffer is neither full nor empty. */
        assert_false(Ring_buffer_is_full(&buf));
        assert_false(Ring_buffer_is_empty(&buf));
        assert_true(buf.count == 3);

        for(i = 0; i < 8; ++i) {
            assert_int_equal(0, Ring_buffer_push(&buf, &data));
        }
        assert_true(buf.count == 10);
        assert_true(Ring_buffer_is_full(&buf));

        /* Now let's pop the values and verify correct behaviour. */
        for(i = 0; i < 10; ++i) {
            assert_int_equal(0, Ring_buffer_pop(&buf, &tmp));
            assert_true(tmp == data);
        }
        assert_true(Ring_buffer_is_empty(&buf));
    }
}

static void test_ringbuffer_functionality_32bits(void **state) {
    (void)state;
    /* Let's push some more data without, filling. Afeter that, poping some
    amount, and then pushing some more, and poping some more. This in order
    to test correct behaviour of pointer wrap arounds. */
    Ring_buffer_t buf;
    uint32_t mem[10U];
    uint32_t data = 0xf2200f;
    uint32_t tmp;
    Ring_buffer_init(&buf, mem, sizeof(uint32_t), 10U);
    int i;
    int j;
    assert_true(Ring_buffer_is_empty(&buf));
    for(j = 0; j < 20; ++j) {
        for(i = 0; i < 8; ++i) {
            assert_int_equal(0, Ring_buffer_push(&buf, &data));
        }

        /* Test buffer is not full yet. */
        assert_false(Ring_buffer_is_full(&buf));

        /* Now let's pop the values and verify correct behaviour. */
        for(i = 0; i < 5; ++i) {
            assert_int_equal(0, Ring_buffer_pop(&buf, &tmp));
            assert_true(tmp == data);
        }

        /* Test buffer is neither full nor empty. */
        assert_false(Ring_buffer_is_full(&buf));
        assert_false(Ring_buffer_is_empty(&buf));
        assert_true(buf.count == 3);

        for(i = 0; i < 8; ++i) {
            assert_int_equal(0, Ring_buffer_push(&buf, &data));
        }
        assert_true(buf.count == 10);
        assert_true(Ring_buffer_is_full(&buf));

        /* Now let's pop the values and verify correct behaviour. */
        for(i = 0; i < 10; ++i) {
            assert_int_equal(0, Ring_buffer_pop(&buf, &tmp));
            assert_true(tmp == data);
        }
        assert_true(Ring_buffer_is_empty(&buf));
    }
}

static void test_ringbuffer_functionality_64bits(void **state) {
    (void)state;
    /* Let's push some more data without, filling. Afeter that, poping some
    amount, and then pushing some more, and poping some more. This in order
    to test correct behaviour of pointer wrap arounds. */
    Ring_buffer_t buf;
    uint64_t mem[10U];
    uint64_t data = 0x1234ff;
    uint64_t tmp;
    Ring_buffer_init(&buf, mem, sizeof(uint64_t), 10U);
    int i;
    int j;
    assert_true(Ring_buffer_is_empty(&buf));
    for(j = 0; j < 20; ++j) {
        for(i = 0; i < 8; ++i) {
            assert_int_equal(0, Ring_buffer_push(&buf, &data));
        }

        /* Test buffer is not full yet. */
        assert_false(Ring_buffer_is_full(&buf));

        /* Now let's pop the values and verify correct behaviour. */
        for(i = 0; i < 5; ++i) {
            assert_int_equal(0, Ring_buffer_pop(&buf, &tmp));
            assert_true(tmp == data);
        }

        /* Test buffer is neither full nor empty. */
        assert_false(Ring_buffer_is_full(&buf));
        assert_false(Ring_buffer_is_empty(&buf));
        assert_true(buf.count == 3);

        for(i = 0; i < 8; ++i) {
            assert_int_equal(0, Ring_buffer_push(&buf, &data));
        }
        assert_true(buf.count == 10);
        assert_true(Ring_buffer_is_full(&buf));

        /* Now let's pop the values and verify correct behaviour. */
        for(i = 0; i < 10; ++i) {
            assert_int_equal(0, Ring_buffer_pop(&buf, &tmp));
            assert_true(tmp == data);
        }
        assert_true(Ring_buffer_is_empty(&buf));
    }
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_ring_buffer_invalid_arguments_returns_error),
        cmocka_unit_test(test_ring_buffer_valid_arguments_returns_no_error),
        cmocka_unit_test(test_ringbuffer_inits_correctly),
        cmocka_unit_test(test_ringbuffer_push_method),
        cmocka_unit_test(test_ringbuffer_pop_method),
        cmocka_unit_test(test_ringbuffer_functionality),
        cmocka_unit_test(test_ringbuffer_functionality_32bits),
        cmocka_unit_test(test_ringbuffer_functionality_16bits),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
