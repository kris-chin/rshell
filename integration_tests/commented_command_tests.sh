#!/bin/bash

echo "Testing commented commands: "

cd ..
./rshell << EOF > integration_tests/commented_test
ls # -lR
echo hello # world
# sleep
exit
EOF

if [ $? -eq 0 ]
then
    echo "Test passed"
else
    echo "Test failed"
fi

echo "Output of commented commands located in commented_test"
