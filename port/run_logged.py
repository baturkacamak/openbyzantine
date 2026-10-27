#!/usr/bin/env python3
"""Capture one launcher/player run, preserving terminal output and exit status."""
import collections
import datetime
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import shlex
import signal
import subprocess
import sys
import tempfile
import time


def git_output(root, *args):
    result = subprocess.run(["git", "-C", str(root), *args],
                            capture_output=True, text=True, timeout=5)
    return result.stdout.strip() if result.returncode == 0 else "unavailable"


def main():
    command = sys.argv[1:]
    if command[:1] == ["--"]:
        command = command[1:]
    if not command:
        print("usage: run_logged.py -- <command> [arguments...]", file=sys.stderr)
        return 2
    root = Path(__file__).resolve().parents[1]
    directory = Path(os.environ.get("BYZ_LOG_DIR", root / "port/output/logs")).resolve()
    directory.mkdir(parents=True, exist_ok=True)
    now = datetime.datetime.now(datetime.timezone.utc)
    fd, filename = tempfile.mkstemp(prefix=now.strftime("run-%Y%m%dT%H%M%S.%fZ-"),
                                    suffix=".log", dir=directory)
    log_path = Path(filename)
    trace_path = log_path.with_suffix(".trace.log")
    summary_path = log_path.with_suffix(".summary.json")
    trace_path.touch(exist_ok=False)
    # Atomic pointer: simultaneous launches retain distinct logs.
    pointer = directory / (log_path.stem + ".latest.tmp")
    pointer.write_text(str(log_path) + "\n")
    pointer.replace(directory / "LATEST")
    binary = root / "port/byzantine-play"
    metadata = {
        "started_utc": now.isoformat(), "cwd": str(Path.cwd()), "argv": command,
        "command": shlex.join(command), "platform": platform.platform(),
        "git_head": git_output(root, "rev-parse", "HEAD"),
        "git_status": git_output(root, "status", "--short"),
        "player_sha256": hashlib.sha256(binary.read_bytes()).hexdigest() if binary.exists() else None,
        "environment": {k: os.environ[k] for k in
                        ("BYZ_DATA", "SDL_VIDEODRIVER", "SDL_AUDIODRIVER", "DISPLAY", "WAYLAND_DISPLAY")
                        if k in os.environ},
        "log": str(log_path), "trace_log": str(trace_path),
        "status": "running", "exit_code": None,
    }
    summary_path.write_text(json.dumps(metadata, ensure_ascii=False, indent=2) + "\n")
    print(f"Deneme logu: {log_path}\nKısa özet: {summary_path}", file=sys.stderr, flush=True)
    started = time.monotonic()
    tail = collections.deque(maxlen=60)
    important = collections.deque(maxlen=100)
    actions = collections.deque(maxlen=20)
    suspected_stalls = collections.deque(maxlen=20)
    counts = collections.Counter()
    env = dict(os.environ, BYZ_LOG_ACTIVE="1", BYZ_TRACE_LOG=str(trace_path))
    process = None
    received = []

    def forward(signum, _frame):
        received.append(signum)
        if process is not None and process.poll() is None:
            try:
                if os.name == "posix":
                    os.killpg(process.pid, signum)
                else:
                    process.send_signal(signum)
            except ProcessLookupError:
                pass

    with os.fdopen(fd, "w", encoding="utf-8", buffering=1) as log:
        def record(text):
            log.write(f"[+{time.monotonic() - started:.3f}s] {text}\n")

        record("session_start " + json.dumps(metadata, ensure_ascii=False))
        try:
            process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                       env=env, start_new_session=(os.name == "posix"))
            for sig in (signal.SIGINT, signal.SIGTERM):
                signal.signal(sig, forward)
            assert process.stdout is not None
            for raw in iter(process.stdout.readline, b""):
                text = raw.decode("utf-8", errors="replace").rstrip("\r\n")
                record(text)
                tail.append(text)
                if "click_result" in text or re.search(r"click(?: at|_at| slot).*slot \d+ action", text):
                    actions.append(text)
                if "active_slots=0" in text or "unsupported" in text.lower():
                    suspected_stalls.append(text)
                for tag in ("travel ", "click_result", "conversation_end", "audio_failures=",
                            "missing", "ERROR", "stopped:", "not committed", "click_not_committed"):
                    if tag in text:
                        counts[tag.strip()] += 1
                if re.search(r"travel |conversation_end|missing|ERROR|stopped:|not.committed|error=", text):
                    important.append(text)
                try:
                    sys.stdout.buffer.write(raw)
                    sys.stdout.buffer.flush()
                except BrokenPipeError:
                    # Continue preserving the run even if its terminal closes.
                    pass
            code = process.wait()
        except OSError as error:
            record("launcher ERROR " + str(error))
            important.append(str(error))
            code = 127
        record(f"session_end exit_code={code} signals={received} elapsed_s={time.monotonic()-started:.3f}")
    trace_counts = collections.Counter()
    with trace_path.open(errors="replace") as trace:
        for line in trace:
            parts = line.split(maxsplit=3)
            if len(parts) >= 3:
                trace_counts[parts[2]] += 1
    metadata.update(status="finished", exit_code=code, signals=received,
                    elapsed_s=round(time.monotonic() - started, 3),
                    output_counts=dict(counts), trace_counts=dict(trace_counts),
                    last_actions=list(actions), possible_stalls=list(suspected_stalls),
                    important_lines=list(important), last_output_lines=list(tail))
    summary_path.write_text(json.dumps(metadata, ensure_ascii=False, indent=2) + "\n")
    return code if code >= 0 else 128 - code


if __name__ == "__main__":
    sys.exit(main())
