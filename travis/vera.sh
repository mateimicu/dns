#!/usr/bin/env bash
find . -name "*.cpp" -print0 | xargs -0 -n1 vera++ -e 
find . -name "*.h" -print0 | xargs -0 -n1 vera++ -e 
