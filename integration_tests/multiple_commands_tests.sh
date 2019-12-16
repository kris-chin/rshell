#!/bin/bash

echo "Testing multiple commands: "

cd ..
./rshell << EOF > integration_tests/mult_test
ls -a; echo hello && date || echo world; git status; 
date; echo hello && echo world; 
cal; mkdir test && ls -a; git status; 
exit 
EOF

if [ $? -eq 0 ]
then
    echo "Test passed"
else
    echo "Test failed"
fi

echo "Output of multiple commands located in mult_test"
