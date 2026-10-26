#!/bin/sh
set -eu

project_dir="$(CDPATH= cd "$(dirname "$0")" && pwd)"
node_dir="$HOME/.nvm/versions/node/v24.22.1/bin"

PATH="$node_dir:$PATH"
export PATH

export GHIDRA_INSTALL_DIR="$HOME/.local/opt/rea-deps/ghidra_12.1.4_PUBLIC"
export JAVA_HOME="$HOME/.local/opt/rea-deps/jdk-21.0.13+9"

if [ "$#" -eq 0 ]; then
  set -- analyze "$project_dir/analysis/rea/BETRAYAL.EXE" --provider ghidra
fi

exec "$node_dir/rea" "$@"
