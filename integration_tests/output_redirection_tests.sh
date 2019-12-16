#!/bin/bash

echo "Testing output redirection: "

cd ..
./rshell << EOF > integration_tests/output_test
echo hello > output_test
exit
EOF

if [ $? -eq 0 ]
then
    echo "Test passed"
else
    echo "Test failed"
fi

echo "Output of commands located in output_test"
