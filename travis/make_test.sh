#!/usr/bine/env bash
make all

# test if the file is created
if [[ -f "dns/dns" ]]; then
    exit 0;
fi

exit 1;
