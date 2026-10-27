#!/usr/bin/env python3
"""Check per-run isolation, stderr capture, partial lines and failure status."""
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile

runner = Path(__file__).resolve().parents[1] / "run_logged.py"
with tempfile.TemporaryDirectory(prefix="byz-log-test-") as directory:
    env = dict(os.environ, BYZ_LOG_DIR=directory)
    command = [sys.executable, str(runner), "--", sys.executable, "-c",
               "import sys; print('click_result test active_slots=0'); "
               "print('ERROR test stderr', file=sys.stderr); "
               "sys.stdout.write('partial'); sys.exit(3)"]
    children = [subprocess.Popen(command, env=env, stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE) for _ in range(2)]
    for child in children:
        out, _ = child.communicate(timeout=15)
        assert child.returncode == 3 and b"click_result test" in out
    summaries = list(Path(directory).glob("*.summary.json"))
    assert len(summaries) == 2
    for path in summaries:
        summary = json.loads(path.read_text())
        assert summary["exit_code"] == 3 and summary["status"] == "finished"
        assert summary["output_counts"]["ERROR"] == 1
        assert summary["last_actions"] == ["click_result test active_slots=0"]
        assert summary["possible_stalls"] == summary["last_actions"]
        log = Path(summary["log"]).read_text()
        assert "partial" in log and "ERROR test stderr" in log and "session_end" in log
        assert Path(summary["trace_log"]).exists()
    assert Path(Path(directory, "LATEST").read_text().strip()).exists()
    failed = subprocess.run([sys.executable, str(runner), "--", "/missing/byz-test-command"],
                            env=env, capture_output=True, timeout=15)
    assert failed.returncode == 127
    latest = Path(Path(directory, "LATEST").read_text().strip())
    assert "launcher ERROR" in latest.read_text()
    waiting = subprocess.Popen([sys.executable, str(runner), "--", sys.executable,
                                "-u", "-c", "import time; print('ready'); time.sleep(60)"],
                               env=env, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    try:
        assert waiting.stdout.readline() == b"ready\n"
        waiting.terminate()
        waiting.communicate(timeout=10)
        assert waiting.returncode == 143
        latest = Path(Path(directory, "LATEST").read_text().strip())
        summary = json.loads(latest.with_suffix(".summary.json").read_text())
        assert summary["exit_code"] == -15 and summary["signals"] == [15]
    finally:
        if waiting.poll() is None:
            waiting.kill()
            waiting.wait()
print("Per-run logs: isolation, stderr, partial output, failures PASS")
