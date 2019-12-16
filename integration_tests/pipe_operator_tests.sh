#!/bin/bash

echo "Testing pipe operator: "

cd ..
./rshell << EOF > integration_tests/pipe_op_test
ls | grep rshell
exit
EOF

if [ $? -eq 0 ]
then
    echo "Test passed"
else
    echo "Test failed"
fi

echo "Output of commands located in pipe_op_test"
