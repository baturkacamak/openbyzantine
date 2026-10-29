# OpenByzantine naming and local directory move (29 October 2026)

User selected OpenByzantine and asked which directory spelling to use.
The display name is **OpenByzantine**; the directory/repository slug is
`openbyzantine`. A hyphen is optional, not required for this compound name.

## Changes

- Renamed `/media/baturkacamak/M.2/PROJECTS/Byzantine - The Betrayal`
  to `/media/baturkacamak/M.2/PROJECTS/openbyzantine` using a directory rename.
  The existing Git directory, uncommitted work and original game media moved
  together; no game media bytes were edited.
- Added the root README and named the port documentation OpenByzantine.
- Updated the two launcher comment examples to `/path/to/openbyzantine`.
- Checked port shell/Python scripts and build definitions: no literal reference
  to the former M.2 project path remains. Existing launcher code resolves its
  own directory at runtime.
- Retained historical evidence/log contents, including their recorded old
  absolute paths. Such paths must be resolved under the new root when reading
  an old run. Pre-test LATEST was
  `run-20261028T224030.673870Z-rlru45kf.log`; its bundle remains in
  `port/output/logs/` under the new root.

## Validation and scope

`make -C port -j2 all test` passed from the renamed directory (exit 0).
Output: `port/output/project-naming/build-test.log`.
No new binary behavior was derived. Existing C++ namespace and executable
names remain compatible with the current scripts.

GitHub has not been changed or pushed in this naming step. The local origin
still points to `https://github.com/baturkacamak/byzantine-betrayal-reconstruction.git`.
Remote existence and publication status were not verified in this step.
