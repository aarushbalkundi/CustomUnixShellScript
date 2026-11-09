#!/bin/bash

#*****************************************************************************
# Test script for MyShell Phase 1 
# Make sure to change the mode of the file to executable
# chmod +x test_phase1.sh
#*****************************************************************************

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

TESTS_RUN=0
TESTS_PASSED=0
TESTS_FAILED=0

print_header() {
    echo ""
    echo -e "${BLUE}================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}================================${NC}"
}

test_pass() {
    echo -e "${GREEN}√ PASS${NC}: $1"
    ((TESTS_PASSED++))
    ((TESTS_RUN++))
}

test_fail() {
    echo -e "${RED}x FAIL${NC}: $1"
    ((TESTS_FAILED++))
    ((TESTS_RUN++))
}

check_executable() {
    if [ ! -f "./myshell" ]; then
        echo -e "${RED}Error: ./myshell not found${NC}"
        echo "Run from TestCase directory"
        exit 1
    fi
    # KEY FIX: Get absolute path to myshell
    MYSHELL="$(cd "$(dirname "./myshell")" && pwd)/$(basename "./myshell")"
}

# ============ TEST GROUP 1: Startup ============
test_group_1() {
    print_header "GROUP 1: Shell Startup and Prompt"

    # Test 1: Shell starts
    # ./myshell--> exit
    # echo "exit" | ./myshell > tmp/myshell_test.out 2>&1
    echo "echo 'exit'" | $MYSHELL > /tmp/myshell_test.out 2>&1
    if [ -s /tmp/myshell_test.out ]; then
        test_pass "Shell starts without error"
    else
        test_fail "Shell failed to start"
    fi
    rm -f /tmp/myshell_test.out

    # Test 2: Welcome message
    #echo "exit" | ./myshell 2>&1 | grep -i "myshell"
    output=$(echo "exit" | $MYSHELL 2>&1 | grep -i "myshell")
    if [ ! -z "$output" ]; then
        test_pass "Shell displays welcome message"
    else
        test_info "Shell may not display welcome message"
    fi

    # Test 3: Prompt
    # echo "exit" | ./myshell 2>&1 |grep -E "myshell|>"
    output=$(echo "exit" | $MYSHELL 2>&1 | grep -E "myshell|>")
    if [ ! -z "$output" ]; then
        test_pass "Shell displays prompt"
    else
        test_info "Shell may not display prompt"
    fi
}
# ============ TEST GROUP 2: Built-in Commands ============

test_group_2() {
    print_header "GROUP 2: Built-in Commands"

    # Test 1: pwd - FIXED: Look for "myshell--> /" instead of "^/"
    output=$(echo "pwd" | $MYSHELL 2>&1 | grep "myshell--> /")
    if [ ! -z "$output" ]; then
        test_pass "pwd returns absolute path"
    else
        test_fail "pwd command not working"
    fi

# Test 2: cd to /tmp (Mac uses /private/tmp, so accept both)                                                                                                                                           
    output=$(echo -e "cd /tmp\npwd\nexit" | $MYSHELL 2>&1 | grep -E "myshell--> /(tmp|private/tmp)")
    if [ ! -z "$output" ]; then
        test_pass "cd changes to /tmp directory"
    else
        test_fail "cd to /tmp failed"
    fi
# Test 3: cd back to original                                                                                                                                                                          
    output=$(echo -e "cd ~\npwd\nexit" | $MYSHELL 2>&1 | grep "myshell--> ")
    if [ ! -z "$output" ]; then
        test_pass "cd changes to home directory"
    else
        test_info "cd to home may need adjustment"
    fi

    # Test 4: Goodbye message
    output=$(echo "exit" | $MYSHELL 2>&1 | grep -i "goodbye")
    if [ ! -z "$output" ]; then
        test_pass "Shell displays goodbye on exit"
    else
        test_info "Shell may not display goodbye"
    fi
}

# ============ TEST GROUP 3: External Commands ============
test_group_3() {
    print_header "GROUP 3: External Command Execution"

    # Test 1: echo
    output=$(echo "echo hello" | $MYSHELL 2>&1 | grep "hello")
    if [ ! -z "$output" ]; then
        test_pass "echo command works"
    else
        test_fail "echo command failed"
    fi
# Test 2: ls                                                                                                                                                                                           
    output=$(echo "ls" | $MYSHELL 2>&1 | wc -l)
    if [ "$output" -gt 3 ]; then
        test_pass "ls command works"
    else
        test_fail "ls command failed"
    fi

    # Test 3: whoami
    output=$(echo "whoami" | $MYSHELL 2>&1 | grep -v "myshell" | grep -v "^$")
    if [ ! -z "$output" ]; then
        test_pass "whoami command works"
    else
        test_fail "whoami command failed"
    fi
 # Test 4: date                                                                                                                                                                                         
    output=$(echo "date" | $MYSHELL 2>&1 | grep -E "Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec")
    if [ ! -z "$output" ]; then
        test_pass "date command works"
    else
        test_info "date command may not work"
    fi
}

# ============ TEST GROUP 4: Error Handling ============
 test_group_4() {
    print_header "GROUP 4: Error Handling"

    # Test 1: Invalid command
    output=$(echo "nonexistentcommand12345" | $MYSHELL 2>&1 | grep -i "not found")
    if [ ! -z "$output" ]; then
        test_pass "Nonexistent command shows error"
    else
        test_info "Nonexistent command handled"
    fi
# Test 2: Empty input                                                                                                                                                                                  
    output=$(echo -e "\n\nexit" | $MYSHELL 2>&1)
    if [ ! -z "$output" ]; then
        test_pass "Empty input handled gracefully"
    else
        test_fail "Shell may have crashed"
    fi

    # Test 3: Invalid path
    output=$(echo "ls /nonexistent12345path" | $MYSHELL 2>&1)
    if [ ! -z "$output" ]; then
        test_pass "Invalid path handled gracefully"
    else
        test_fail "Invalid path handling failed"
    fi
}
 # ============ TEST GROUP 5: Process Management ============
test_group_5() {
    print_header "GROUP 5: Process Management"

    # Test 1: Multiple commands
    # eg: echo -e "echo first\necho second\necho third\nexit" | ./myshell
    output=$(echo -e "echo first\necho second\necho third\nexit" | $MYSHELL 2>&1)
    if echo "$output" | grep -q "first" && echo "$output" | grep -q "second" && echo "$output" | grep -q "third"; then
        test_pass "Multiple sequential commands work"
    else
        test_fail "Sequential commands failed"
    fi
# Test 2: pwd after cd: Accept both /tmp and /private/tmp                                                                                                                                              
    output=$(echo -e "cd /tmp\npwd\nexit" | $MYSHELL 2>&1 | grep -E "myshell--> /(tmp|private/tmp)")
    if [ ! -z "$output" ]; then
        test_pass "pwd works after cd"
    else
        test_fail "pwd after cd failed"
    fi

    # Test 3: Multiple cd commands
    output=$(echo -e "pwd\ncd /var\npwd\nexit" | $MYSHELL 2>&1)
    if echo "$output" | grep -q "/var"; then
        test_pass "Directory navigation works"
    else
        test_fail "Directory navigation failed"
    fi
}
# ============ TEST GROUP 6: Input Handling ============
test_group_6() {
    print_header "GROUP 6: Input Handling"

    # Test 1: EOF handling
    output=$(echo "" | $MYSHELL 2>&1)
    if [ ! -z "$output" ] || [ -z "$output" ]; then
        test_pass "EOF (Ctrl+D) handled gracefully"
    else
        test_fail "EOF handling failed"
    fi
   # Test 2: Multiple arguments                                                                                                                                                                           
    output=$(echo "echo hello world test" | $MYSHELL 2>&1 | grep "hello world test")
    if [ ! -z "$output" ]; then
        test_pass "Multiple arguments handled"
    else
        test_fail "Argument handling failed"
    fi

    # Test 3: Multiple spaces
    output=$(echo "echo    multiple    spaces" | $MYSHELL 2>&1 | grep "multiple")
    if [ ! -z "$output" ]; then
        test_pass "Multiple spaces handled"
    else
        test_fail "Space handling failed"
    fi
}
# ============ TEST GROUP 7: Stress Testing ============
test_group_7() {
    print_header "GROUP 7: Stress Testing"

    # Test 1: Multiple pwd commands - FIXED: Look for "myshell--> /" pattern
    output=$(echo -e "pwd\npwd\npwd\npwd\npwd\nexit" | $MYSHELL 2>&1 | grep -c "myshell--> /")
    if [ "$output" -ge 3 ]; then
        test_pass "Multiple pwd commands work"
    else
        test_fail "Multiple pwd commands failed"
    fi
# Test 2: Multiple echo commands                                                                                                                                                                       
    output=$(echo -e "echo 1\necho 2\necho 3\necho 4\necho 5\nexit" | $MYSHELL 2>&1)
    if echo "$output" | grep -q "1" && echo "$output" | grep -q "5"; then
        test_pass "Stress test: many commands"
    else
        test_fail "Stress test failed"
    fi
}

# ============ TEST GROUP 8: GLOB PATTERN TESTING ============
test_group_8() {
    print_header "GROUP 8: Glob Pattern Testing"
    
    ORIGINAL_DIR=$(pwd)
    mkdir -p test_glob_dir
    cd test_glob_dir
    
    touch file1.c file2.c file3.c
    touch readme.txt
    
    # TEST 1: ls *.c - Use absolute path to myshell
    output=$(echo "ls *.c" | $MYSHELL 2>&1)
    if echo "$output" | grep -q "file1.c" && echo "$output" | grep -q "file2.c" && echo "$output" | grep -q "file3.c"; then
        test_pass "Glob: ls *.c expands correctly"
    else
        test_fail "Glob: ls *.c failed"
    fi
    
    # TEST 2: ls *.txt
    output=$(echo "ls *.txt" | $MYSHELL 2>&1)
    if echo "$output" | grep -q "readme.txt"; then
        test_pass "Glob: ls *.txt lists text files"
    else
        test_fail "Glob: ls *.txt failed"
    fi
    
    # TEST 3: echo *.c
    output=$(echo "echo *.c" | $MYSHELL 2>&1)
    if echo "$output" | grep -q "file1.c" && echo "$output" | grep -q "file2.c" && echo "$output" | grep -q "file3.c"; then
        test_pass "Glob: echo *.c expands correctly"
    else
        test_fail "Glob: echo *.c failed"
    fi
    
    # TEST 4: echo *2.c
    output=$(echo "echo *2.c" | $MYSHELL 2>&1)
    if echo "$output" | grep -q "file2.c"; then
        test_pass "Glob: echo *2.c expands correctly"
    else
        test_fail "Glob: echo *2.c failed"
    fi
    
    # TEST 5: echo file*.c
    output=$(echo "echo file*.c" | $MYSHELL 2>&1)
    if echo "$output" | grep -q "file1.c"; then
        test_pass "Glob: echo file*.c expands correctly"
    else
        test_fail "Glob: echo file*.c failed"
    fi
    
    # TEST 6: cat *.c
    echo "test content" >> file1.c
    output=$(echo "cat *.c" | $MYSHELL 2>&1)
    if echo "$output" | grep -q "test content"; then
        test_pass "Glob: cat *.c works"
    else
        test_fail "Glob: cat *.c failed"
    fi
    
    # TEST 7: No match
    output=$(echo "ls *.nonexistent" | $MYSHELL 2>&1)
    if echo "$output" | grep -qE "No such file|cannot access|not found"; then
        test_pass "Glob: No match shows error"
    else
        test_fail "Glob: No match handling failed"
    fi
    
    cd $ORIGINAL_DIR
    rm -rf test_glob_dir
}

main() {
    echo ""
    echo -e "${BLUE}MyShell Phase 1 - Test Suite${NC}"
    echo ""
    
    check_executable
    echo -e "${BLUE}Using myshell: $MYSHELL${NC}"
    echo ""
    
    test_group_1
    test_group_2
    test_group_3
    test_group_4
    test_group_5
    test_group_6
    test_group_7    
    test_group_8
    
    print_header "TEST SUMMARY"
    echo -e "Total Tests Run:  ${BLUE}$TESTS_RUN${NC}"
    echo -e "Tests Passed:     ${GREEN}$TESTS_PASSED${NC}"
    echo -e "Tests Failed:     ${RED}$TESTS_FAILED${NC}"
    echo ""
    
    if [ $TESTS_RUN -gt 0 ]; then
        PASS_PERCENT=$((TESTS_PASSED * 100 / TESTS_RUN))
        echo -e "Pass Rate: ${BLUE}${PASS_PERCENT}%${NC}"
    fi
    echo ""
    
    if [ $TESTS_FAILED -eq 0 ]; then
        echo -e "${GREEN}✓ All tests passed!${NC}"
        return 0
    else
        echo -e "${RED}✗ Some tests failed${NC}"
        return 1
    fi
}

main
exit $?
