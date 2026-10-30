#!/usr/bin/env bash
# Full test matrix: behavior + mixed logic + smaller real-media profiles.
set -euo pipefail
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
make -C "$here" -j2 all
exec python3 -u "$here/explore_campaign.py" "$@" --publish-latest
