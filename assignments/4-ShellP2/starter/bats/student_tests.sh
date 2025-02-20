#!/usr/bin/env bats

# File: student_tests.sh
# Create your unit tests suit in this file

@test "check if ls runs fine" {
    run ./dsh <<EOF
ls
EOF

    [ "$status" -eq 0 ]
}


@test "running ls with some extra spaces" {
    run ./dsh <<EOF
    ls     -l    /badfile
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == *"No such file or directory"* ]] || [[ "$output" == *"cannot find"* ]]
}


@test "using pipes to sort unique words" {
    run ./dsh <<EOF
echo "banana apple apple orange" | tr ' ' '\n' | sort | uniq
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == *"apple"* ]] && [[ "$output" == *"banana"* ]] && [[ "$output" == *"orange"* ]]
}


@test "running sleep in the background" {
    run ./dsh <<EOF
sleep 1 &
EOF

    [ "$status" -eq 0 ]
}

@test "echoing a simple message" {
    run ./dsh <<EOF
echo "This is a test."
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == *"This is a test."* ]]
}


@test "running multiple commands with a semicolon" {
    run ./dsh <<EOF
echo "First Command"; echo "Second Command"
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == *"First Command"* ]] && [[ "$output" == *"Second Command"* ]]
}
