#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#include "rosalia/test.h"

#ifdef __cplusplus
extern "C" {
#endif

bool rosa_tester_create(rosa_tester* t, size_t max_tests, size_t max_cases)
{
    t->max_tests = max_tests;
    t->tests = mmap(NULL, sizeof(rosa_tester_test) * max_tests, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    t->test_count = 0;
    t->max_cases = max_cases;
    t->cases = mmap(NULL, sizeof(rosa_tester_test_case) * max_cases, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    t->case_count = 0;
    return true;
}

void rosa__tester_add_impl(rosa_tester* t, rosa_test_t* test_func, const char* test_name, const char* file, int line)
{
    if (t->test_count >= t->max_tests) {
        exit(EXIT_FAILURE); //TODO error
    }
    t->tests[t->test_count++] = (rosa_tester_test){
        .file = file,
        .line = line,
        .name = test_name,
        .func = test_func,
        .cases_base = 0,
        .case_count = 0,
        .current_case_ctr = 0,
    };
}

size_t rosa_tester_run(rosa_tester* t)
{
    //TODO print test only if it or any case fails
    //TODO only print failing cases
    //TODO print nr of tests and cases stats, both success and failing
    size_t failed_tests = 0;
    size_t failed_cases = 0;
    for (size_t test_idx = 0; test_idx < t->test_count; test_idx++) {
        rosa_tester_test* current_test = t->tests + test_idx;
        printf("TEST: \"%s\"", current_test->name);
        fflush(stdout);
        bool test_failing = false;
        // run test to gather cases and / or put setup / single case behaviour through test
        pid_t test_pid = fork();
        if (test_pid < 0) {
            // fork failed
            exit(EXIT_FAILURE); //TODO
        }
        if (test_pid == 0) {
            // child
            current_test->func(t, test_idx, test_case_none);
            exit(EXIT_SUCCESS);
        } else if (test_pid > 0) {
            // parent
            int status;
            waitpid(test_pid, &status, 0);
            printf(" (%zu cases)\n", current_test->case_count);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                //TODO was pass
            } else {
                //TODO was crash or assert fail
                test_failing = true;
                failed_cases++;
                printf(" -> FAIL\n");
            }
        }
        // run all cases
        for (size_t case_idx = 0; case_idx < current_test->case_count; case_idx++) {
            rosa_tester_test_case* current_case = &t->cases[current_test->cases_base + case_idx];
            printf("\tCASE: \"%s\"", current_case->name);
            fflush(stdout);
            pid_t case_pid = fork();
            if (case_pid < 0) {
                // fork failed
                exit(EXIT_FAILURE); //TODO
            }
            if (case_pid == 0) {
                // child
                current_test->current_case_ctr = 0;
                current_test->func(t, test_idx, case_idx);
                exit(EXIT_SUCCESS);
            } else {
                // parent
                int case_status;
                waitpid(case_pid, &case_status, 0);
                if (WIFEXITED(case_status) && WEXITSTATUS(case_status) == 0) {
                    //TODO was pass
                    printf(" -> PASS\n");
                } else {
                    //TODO was fail
                    test_failing = true;
                    failed_cases++;
                    printf(" -> FAIL (%s:%i)\n", current_case->file, current_case->line);
                }
            }
        }
        if (test_failing) {
            failed_tests++;
        }
    }
    return failed_cases;
}

void rosa_tester_destroy(rosa_tester* t)
{
    munmap(t->cases, sizeof(rosa_tester_test_case));
    munmap(t->tests, sizeof(rosa_tester_test));
}

bool rosa__tester_test_case(rosa_tester* t, size_t test_nr, size_t test_case, const char* test_case_name, const char* file, int line)
{
    rosa_tester_test* current_test = &t->tests[test_nr];
    if (test_case == test_case_none) {
        // register new case
        if (t->case_count >= t->max_cases) {
            exit(EXIT_FAILURE); //TODO error
        }
        if (current_test->cases_base == test_case_base_none) {
            // get test case base
            current_test->cases_base = t->case_count;
        }
        t->case_count++;
        t->cases[current_test->cases_base + current_test->case_count++] = (rosa_tester_test_case){
            .name = test_case_name, //TODO this is not deduplicated within all the tests cases
            .file = file,
            .line = line,
        };
        return false;
    }
    return (current_test->current_case_ctr++ == test_case);
}

#ifdef __cplusplus
}
#endif
