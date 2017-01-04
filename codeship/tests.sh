#!/usr/bin/env bash

if [ "$CI_BRANCH" != "gh-pages" ]; then 
    dns/tests
fi
