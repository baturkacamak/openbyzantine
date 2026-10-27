# Scene entry flash — 27 October 2026

User run: `run-20261027T170234.605392Z-o3mb46kx`, preserved under `port/output/logs/`. MR → MAP → NB: action 410 plays `NBEst1`, then travels to NB. The log shows `NBOKEY.jpg` composed three times during sound gates, followed by `panorama_view load=nodes/nb/NB00.jpg` and the correct panorama. The image is the static fallback, not a load observed in that entry trace. The logs establish the unexpected intervening composition; they do not independently prove what was physically displayed.

Cause in the port: budgeted sound/video replays present an intermediate frame before applying panorama events. A panorama layer with no loaded node falls back to the first static background. Direct `--entry` startup also presented a speculative initial frame before executing entry.

Change: apply newly reached panorama loads before presenting gated frames; retain the previous presented frame if no scene layer is ready. Do not present the speculative startup frame for `--entry`. Sound and video execution remains gated. This is a port-side sequencing fix, not a claim about original transition effects. No new binary behavior was derived.

Validation: `make -C port -j2 all test` passes. With real assets and `57.btb`, SDL dummy video/audio, direct NB entry and `--no-video`, the old build composed NBOKEY initially and at two gates; the new build loads NB00 before both gates and composes only the panorama. This is a headless direct-entry check, not a replay of the exact MR → MAP journey or a physical screen check. Original transitions remain unimplemented.
