#!/bin/sh

SCCPREFIX=${SCCPREFIX:-@PREFIX@}
${SCCPREFIX}/bin/scc -E $@
