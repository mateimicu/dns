#!/usr/bin/env bash

if [ "$CI_BRANCH" != "gh-pages" ]; then 
    make tests
    chmod +x dns/tests
fi
