# OpenByzantine README and GitHub identity (29 October 2026)

## Request and reference review

The user requested a rich README, review of sibling projects, GitHub description
and publication with gh. Read LocalTube, Eksi Arti, CuraFlux, Spotifast and
WordPress Media Integrity README examples. Applied their useful presentation
patterns: centred introduction, badges, feature tables, real screenshots,
copyable setup commands, controls, file locations and focused documentation links.

## Documentation

- Expanded root README in Turkish with an English project summary.
- Added three existing player captures under docs/images, with provenance.
  They are copies of saved output, not fresh window/speaker validation.
- Documented persistent BYZ_DATA to avoid putting progress in temporary storage.
- Distinguished the default MAP save entry from a complete new-game walkthrough.
- Documented actual platform/language validation limits and the ambient-chain
  restore approximation. Did not claim a root source licence or completed game.
- Corrected the port README's obsolete claim that CMake had never been run.
- No new binary behavior was derived and no engine logic was edited in this step.
  Previously pending settings/language/session changes were validated and
  committed separately as c351c4d to keep documentation and published code aligned.

## GitHub operations

Initial origin: baturkacamak/byzantine-betrayal-reconstruction (PRIVATE).
Fetched main and verified it is an ancestor of local HEAD, with no remote-only
commits. Used gh repo rename to select openbyzantine and updated local origin.

Target: https://github.com/baturkacamak/openbyzantine
Visibility: PRIVATE (preserved).
Description:

> Portable C++17 reconstruction of Byzantine: The Betrayal with SDL2, FFmpeg, original game data, and documented reverse-engineering evidence.

Topics: adventure-game, cplusplus, ffmpeg, game-engine, game-preservation,
reverse-engineering, sdl2. Issues are enabled. Publication targets the existing
main branch, without a force push. No game ISO/ZIP/EXE is tracked.

## Validation

- make -C port -j2 all test: exit 0.
- Clean CMake configure/build in /tmp/openbyzantine-readme-build: exit 0.
- CTest: 14/14 passed.
- GitHub gh api markdown rendered the README with eight tables, seven images
  and two details blocks. Six navigation targets correspond to headings;
  all 16 local file/directory/image references exist.
- git diff --check passes.

Evidence retained under port/output/github-readme/: build-test.log,
cmake-configure.log, cmake-build.log, ctest.log, README.rendered.html and
readme-check.log. GitHub metadata and final publication verification are also
saved there. Existing runtime run run-20261028T224030.673870Z-rlru45kf remains
under port/output/logs; historical absolute paths point to the old project root.
