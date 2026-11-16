#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

find_catkin_workspace() {
  if [[ -n "${CATKIN_WS:-}" && -d "${CATKIN_WS}/src" ]]; then
    printf '%s\n' "$CATKIN_WS"
    return
  fi
  local current="$SCRIPT_DIR"
  while [[ "$current" != "/" ]]; do
    if [[ "$(basename "$current")" == "src" ]]; then
      dirname "$current"
      return
    fi
    current="$(dirname "$current")"
  done
  if [[ -d "$HOME/catkin_ws/src" ]]; then
    printf '%s\n' "$HOME/catkin_ws"
    return
  fi
  echo "Impossibile trovare il catkin workspace. Imposta CATKIN_WS." >&2
  return 1
}

setup_ros() {
  local workspace
  workspace="$(find_catkin_workspace)"
  if [[ ! -f /opt/ros/noetic/setup.bash ]]; then
    echo "ROS Noetic non trovato in /opt/ros/noetic." >&2
    return 1
  fi
  # shellcheck disable=SC1091
  source /opt/ros/noetic/setup.bash
  if [[ ! -f "$workspace/devel/setup.bash" ]]; then
    echo "Workspace non compilato. Esegui: cd $workspace && catkin_make" >&2
    return 1
  fi
  # shellcheck disable=SC1090
  source "$workspace/devel/setup.bash"
  export CATKIN_WS="$workspace"
}
