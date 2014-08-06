#include "rtos-kochab.h"
#include "debug.h"

#define SIGNAL_DEMO_NUM_IDS 8
#define SIGNAL_DEMO_SET_ALL ((1u << SIGNAL_DEMO_NUM_IDS) - 1)

static RtosSignalId
demo_sig_id[SIGNAL_DEMO_NUM_IDS] = {
    RTOS_SIGNAL_SET_DUMMY0,
    RTOS_SIGNAL_SET_DUMMY1,
    RTOS_SIGNAL_SET_DUMMY2,
    RTOS_SIGNAL_SET_DUMMY3,
    RTOS_SIGNAL_SET_DUMMY4,
    RTOS_SIGNAL_SET_UNBLOCK,
    RTOS_SIGNAL_SET_DUMMY6,
    RTOS_SIGNAL_SET_DUMMY7,
};

void
peek_poll_expect_fail(RtosSignalId id)
{
    if (rtos_signal_peek(id)) {
        debug_println("a: signal peek unexpectedly succeeded!");
    }
    if (rtos_signal_poll(id)) {
        debug_println("a: signal poll unexpectedly succeeded!");
    }
}

void
peek_poll_all_expect_fail(void)
{
    debug_println("a: checking for stray signals");

    if (rtos_signal_peek(RTOS_SIGNAL_SET_ALL)) {
        debug_println("a: signal peek unexpectedly succeeded!");
    }

    if (rtos_signal_poll(RTOS_SIGNAL_SET_ALL)) {
        debug_println("a: signal poll unexpectedly succeeded!");
    }
}

void
gather_stray_signals(void)
{
    RtosSignalId i;

    debug_println("a: gathering up all other signals b sent (should not block)");

    for (i = 0; i < SIGNAL_DEMO_NUM_IDS; i++) {
        if (demo_sig_id[i] == RTOS_SIGNAL_SET_UNBLOCK) {
            continue;
        }
        if (rtos_signal_wait_set(demo_sig_id[i]) != demo_sig_id[i]) {
            debug_println("a: signal wait got unexpected id!");
        }
    }
}

void
fn_a(void)
{
    int i;
    RtosSignalId r;

    /* Part 0: Solo */
    debug_println("");
    debug_println("Part 0: Solo");
    debug_println("");

    debug_println("a: setting all signals");
    rtos_signal_send_set(RTOS_TASK_ID_A, SIGNAL_DEMO_SET_ALL);

    debug_println("a: peeking/polling specific signals");
    for (i = 0; i < SIGNAL_DEMO_NUM_IDS; i++) {
        if (!rtos_signal_peek_set(demo_sig_id[i])) {
            debug_println("a: signal peek unexpectedly failed!");
        }
        if (rtos_signal_poll_set(demo_sig_id[i]) != demo_sig_id[i]) {
            debug_println("a: signal poll got unexpected id!");
        }
        peek_poll_expect_fail(demo_sig_id[i]);
    }
    peek_poll_all_expect_fail();

    debug_println("a: re-setting all signals");
    rtos_signal_send_set(RTOS_TASK_ID_A, SIGNAL_DEMO_SET_ALL);

    debug_println("a: peeking/polling specific signals in reverse order");
    for (i = SIGNAL_DEMO_NUM_IDS - 1; i >= 0; i--) {
        if (!rtos_signal_peek_set(demo_sig_id[i])) {
            debug_println("a: signal peek unexpectedly failed!");
        }
        if (rtos_signal_poll_set(demo_sig_id[i]) != demo_sig_id[i]) {
            debug_println("a: signal poll got unexpected id!");
        }
        peek_poll_expect_fail(demo_sig_id[i]);
    }
    peek_poll_all_expect_fail();

    debug_println("a: re-setting all signals");
    rtos_signal_send_set(RTOS_TASK_ID_A, SIGNAL_DEMO_SET_ALL);

    debug_println("a: peeking/polling whole signal set");
    if (!rtos_signal_peek_set(RTOS_SIGNAL_SET_ALL)) {
        debug_println("a: signal peek whole set unexpectedly failed!");
    }
    if (rtos_signal_poll_set(RTOS_SIGNAL_SET_ALL) != SIGNAL_DEMO_SET_ALL) {
        debug_println("a: signal poll whole set got unexpected id!");
    }
    peek_poll_all_expect_fail();

    debug_println("a: re-setting all signals");
    rtos_signal_send_set(RTOS_TASK_ID_A, SIGNAL_DEMO_SET_ALL);

    debug_println("a: waiting on specific signals (should not block)");
    for (i = 0; i < SIGNAL_DEMO_NUM_IDS; i++) {
        if (rtos_signal_wait_set(demo_sig_id[i]) != demo_sig_id[i]) {
            debug_println("a: signal wait got unexpected id!");
        }
        peek_poll_expect_fail(demo_sig_id[i]);
    }
    peek_poll_all_expect_fail();

    debug_println("a: re-setting all signals");
    rtos_signal_send_set(RTOS_TASK_ID_A, SIGNAL_DEMO_SET_ALL);

    debug_println("a: waiting on specific signals in reverse order (should not block)");
    for (i = SIGNAL_DEMO_NUM_IDS - 1; i >= 0; i--) {
        if (rtos_signal_wait_set(demo_sig_id[i]) != demo_sig_id[i]) {
            debug_println("a: signal wait got unexpected id!");
        }
        peek_poll_expect_fail(demo_sig_id[i]);
    }
    peek_poll_all_expect_fail();

    debug_println("a: re-setting all signals");
    rtos_signal_send_set(RTOS_TASK_ID_A, SIGNAL_DEMO_SET_ALL);

    debug_println("a: waiting on signal set (should not block)");
    if (rtos_signal_wait_set(RTOS_SIGNAL_SET_ALL) != SIGNAL_DEMO_SET_ALL) {
        debug_println("a: signal wait whole set got unexpected id!");
    }
    peek_poll_all_expect_fail();

    /* Part 1: B unblocks A */
    debug_println("");
    debug_println("Part 1: B unblocks A");
    debug_println("");

    debug_println("a: waiting on signal set");
    r = rtos_signal_wait_set(RTOS_SIGNAL_SET_ALL);
    if (r != RTOS_SIGNAL_SET_UNBLOCK) {
        debug_println("a: signal wait got unexpected id!");
    }
    peek_poll_all_expect_fail();

    debug_println("a: waiting on specific signal");
    r = rtos_signal_wait_set(RTOS_SIGNAL_SET_UNBLOCK);
    if (r != RTOS_SIGNAL_SET_UNBLOCK) {
        debug_println("a: signal wait got unexpected id!");
    }
    debug_println("a: got specific signal");

    gather_stray_signals();
    peek_poll_all_expect_fail();

    debug_println("a: waiting on specific signal");
    r = rtos_signal_wait_set(RTOS_SIGNAL_SET_UNBLOCK);
    if (r != RTOS_SIGNAL_SET_UNBLOCK) {
        debug_println("a: signal wait got unexpected id!");
    }
    debug_println("a: got specific signal");

    gather_stray_signals();
    peek_poll_all_expect_fail();

    debug_println("");
    debug_println("Done.");
    for (;;)
    {
    }
}

void
fn_b(void)
{
    int i;

    /* Part 1: B unblocks A */
    debug_println("b: sending specific signal to a");
    rtos_signal_send_set(RTOS_TASK_ID_A, RTOS_SIGNAL_SET_UNBLOCK);

    debug_println("b: sending whole signal set to a");
    rtos_signal_send_set(RTOS_TASK_ID_A, SIGNAL_DEMO_SET_ALL);

    debug_println("b: sending signals that shouldn't wake up a");
    for (i = 0; i < SIGNAL_DEMO_NUM_IDS; i++) {
        if (demo_sig_id[i] == RTOS_SIGNAL_SET_UNBLOCK) {
            continue;
        }
        rtos_signal_send_set(RTOS_TASK_ID_A, demo_sig_id[i]);
    }

    debug_println("b: sending signal to wake up a");
    rtos_signal_send_set(RTOS_TASK_ID_A, RTOS_SIGNAL_SET_UNBLOCK);

    debug_println("b: shouldn't be here!");
    for (;;)
    {
    }
}

int
main(void)
{
    rtos_start();
    /* Should never reach here, but if we do, an infinite loop is
       easier to debug than returning somewhere random. */
    for (;;) ;
}