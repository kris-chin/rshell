#!/bin/bash

echo "Testing exit command: exit"

cd ..
./rshell << EOF > integration_tests/exit_test
exit
EOF

if [ $? -eq 0 ]
then
    echo "Test passed"
else
    echo "Test failed"
fi

echo "Output of exit command located in exit_test"
