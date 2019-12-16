#!/bin/bash

echo "Testing input redirection: "

cd ..
./rshell << EOF > integration_tests/input_redirect_test
cat < input_test.txt
exit
EOF

if [ $? -eq 0 ]
then
    echo "Test passed"
else
    echo "Test failed"
fi

echo "Output of commands located in input_redirect_test"
