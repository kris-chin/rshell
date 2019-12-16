#!/bin/bash

echo "Testing for precedence: "

cd ..
./rshell << EOF > integration_tests/precedence_test
(echo A && echo B) || (echo C && echo D)
((echo A) && (echo B)) || (echo C && echo D)
((echo A && echo B) && (echo C && echo D)) || echo E
exit
EOF

if [ $? -eq 0 ]
then
    echo "Test passed"
else
    echo "Test failed"
fi

echo "Output of commands located in precedence_test"
