#!/bin/bash

echo "Testing single commands: "

cd ..
./rshell << EOF > integration_tests/single_test
date
cal
ls
echo hello
exit
EOF

if [ $? -eq 0 ]
then
    echo "Test passed"
else
    echo "Test failed"
fi

echo "Output of commands located in single_test"
