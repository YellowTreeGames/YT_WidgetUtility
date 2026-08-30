# Settings: Realtime vs. Batched Values

A recipe for letting one setting (e.g. volume) take effect immediately while the player interacts with it, without breaking the batch Apply-on-Save flow that every other setting in [[settings_menu_tab_architecture]] relies on.

## When to use this

A setting needs audible/visible feedback while the player is still dragging the control — volume is the obvious case, a brightness/gamma preview is another — while everything else on that tab (and every other tab) still only commits when the player clicks Save.

## The landmine this works around

Every `SettingDefinition::Apply_Implementation()` in `YT_Utility` — `Numeric`, `Categorical`, `Bool`, `Volume`, `AudioBus`, all of them — calls `Settings->ApplyPendingSettings()`. That call is **global**: it flushes every pending float/int/bool across every tag to Active, not just the one `SettingTag` being applied. Today that's safe, because the only thing that ever calls `Apply()` is a tab host's Save button (per [[settings_menu_tab_architecture]]), where flushing everything is exactly the intended behavior — by the time Save is clicked, every tab's edits are meant to commit together anyway.

That safety breaks the moment a widget calls `Apply()` outside the Save flow. A volume slider that calls `SettingDefinition->Apply()` on every `OnValueChanged` tick (the obvious way to get real-time audio feedback by reusing the existing method) will also silently commit whatever the player left half-edited on another tab. Concretely: the player drags Anti-Aliasing on the Video tab, doesn't hit Save, switches to Audio, and nudges Master volume — if the volume widget calls `Apply()` per-tick, the untouched AA change is now sitting in Active, live, without the player ever confirming it.

## The pattern

Keep "realtime" entirely inside the widget layer. **Never call `SettingDefinition::Apply()` or `ApplyPendingSettings()` from a realtime widget's interaction handler.**

1. On value change, still call `SetPendingValue()` / `SetPendingIndex()` as normal — this only touches the Pending maps and is always safe, realtime or not.
2. Additionally, push the value straight to the live runtime system yourself, from the widget, bypassing the `SettingDefinition::Apply()` path entirely:
   - Volume: `UGameplayStatics::SetSoundMixClassOverride(...)`, the same call `UYT_SettingDef_Volume::Apply_Implementation` makes — just invoked directly from the widget instead of through `Apply()`.
   - A control-bus setting: `UAudioModulationStatics::UpdateMix(...)`, mirroring `UYT_SettingDef_AudioBus::Apply_Implementation`.
   - A non-audio realtime value (brightness/gamma): push to whatever the live subsystem is (console variable, post-process volume) the same way.
3. Leave `Apply()` to the tab host's Save button, same as every other setting. At Save time, `Apply_Implementation` runs once, moves Pending to Active, and reapplies the live override — redundant with step 2's value but harmless, since it's the same number already in effect.
4. Concretely: subclass `UYT_SettingWidget_Audio` (or a numeric equivalent) and override `OnSliderValueChanged` to add the direct-push call alongside the existing `SetPendingValue`. Don't add a new "realtime" flag or code path to `YT_Utility`'s `SettingDefinition` classes — the fix lives entirely in the widget.

## Why not fix `ApplyPendingSettings()` to be per-tag instead

That's the more "correct" long-term shape — each `Apply_Implementation` should only commit its own tag. It was considered and deliberately deferred: it's a shared-plugin change that every project depends on, and would need RGBox's whole Save flow re-verified against it. Revisit if the global-flush behavior causes a second symptom beyond this one.

## When *not* to use this

Anything that can visibly lock up or flicker mid-drag — resolution, fullscreen mode, overall graphics quality, any [[settings_video_scalability_expansion]] group. These are exactly why the batch Apply-on-Save pattern exists; don't make them realtime just because volume is.

## Reference implementation

- `Plugins/YT_WidgetUtility/Source/YT_WidgetUtility/Public/Settings/YT_SettingWidget_Audio.h` — subclass this for the realtime volume widget.
- `Plugins/YT_Utility/Source/YT_Utility/Private/GameUserSettings/Prebuilt/YT_SettingDef_Volume.cpp` — the `Apply_Implementation` that still runs exactly once, at Save.
