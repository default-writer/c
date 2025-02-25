#!/bin/bash

# Find all .c and .h files and append \r at the end of each file
find /path/to/directory -type f \( -name "*.c" -o -name "*.h" \) -print0 | xargs -0 -I {} sh -c 'echo -ne "\r" >> "{}"'
