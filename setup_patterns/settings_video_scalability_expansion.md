# Video Tab: Per-Group Scalability Settings

A recipe for expanding the Video tab beyond RGBox's single aggregate "Graphics Quality" dropdown into individual controls per scalability group, matching the granularity of Unreal's own Engine Scalability screen.

## When to use this

The Video tab needs separate controls for View Distance, Shadows, Global Illumination, Reflections, Anti-Aliasing, Textures, Effects, Post-Processing, Foliage, Shading, and/or Landscape — not just one overall preset.

## What already exists

`UGameFramework::GameUserSettings` (engine base class, verified against UE 5.8) already exposes a paired setter/getter for every group:

| Group | Native accessor |
|---|---|
| View Distance | `SetViewDistanceQuality` / `GetViewDistanceQuality` |
| Shadows | `SetShadowQuality` / `GetShadowQuality` |
| Global Illumination | `SetGlobalIlluminationQuality` / `GetGlobalIlluminationQuality` |
| Reflections | `SetReflectionQuality` / `GetReflectionQuality` |
| Anti-Aliasing | `SetAntiAliasingQuality` / `GetAntiAliasingQuality` |
| Textures | `SetTextureQuality` / `GetTextureQuality` |
| Effects | `SetVisualEffectQuality` / `GetVisualEffectQuality` |
| Post-Processing | `SetPostProcessingQuality` / `GetPostProcessingQuality` |
| Foliage | `SetFoliageQuality` / `GetFoliageQuality` |
| Shading | `SetShadingQuality` / `GetShadingQuality` |
| Landscape | `SetLandscapeQuality` / `GetLandscapeQuality` |
| Overall (aggregate) | `SetOverallScalabilityLevel` / `GetOverallScalabilityLevel` |

Only the aggregate is wrapped today, by `UYT_SettingDef_GraphicsQuality`. None of the eleven individual groups have a `SettingDefinition` yet.

## Pattern: one generic class, not eleven near-identical ones

Per the parent CLAUDE.md's DRY rule, don't write `UYT_SettingDef_ViewDistance`, `UYT_SettingDef_Shadows`, `UYT_SettingDef_AntiAliasing`, etc. as eleven separate `UCLASS`es that differ only in which native accessor they call. Instead:

1. Add one `UYT_SettingDef_ScalabilityGroup : public UYT_SettingDef_Categorical` to `YT_Utility`, with an `EScalabilityGroup` enum property (`ViewDistance`, `Shadow`, `GlobalIllumination`, `Reflection`, `AntiAliasing`, `Texture`, `VisualEffect`, `PostProcessing`, `Foliage`, `Shading`, `Landscape`).
2. `GetActiveIndex_Implementation` / `GetPendingIndex_Implementation` switch on the enum to call the matching `Get*Quality()`, falling back to it exactly the way `UYT_SettingDef_GraphicsQuality` falls back to a native getter when no override is pending — but see the correction below.
3. `Apply_Implementation` switches on the enum to call the matching `Set*Quality(SelectedIndex)`, then `Settings->ApplyPendingSettings()` (same global-flush behavior as [[settings_realtime_vs_batched]] describes — these stay firmly batched, never realtime).
4. One data asset per group (`DA_YT_CategoricalSetting_ViewDistance`, `...Shadows`, `...GlobalIllumination`, `...Reflections`, `...AntiAliasing`, `...Textures`, `...Effects`, `...PostProcessing`, `...Foliage`, `...Shading`, `...Landscape`), each set to the matching enum value and tagged `Settings.Video.<Group>`.
5. Register each in the project's `UYT_SettingsRegistry` asset, and add one `UYT_SettingWidget_Dropdown` per group to the Video tab's container.

**Correction to propagate:** `UYT_SettingDef_GraphicsQuality::GetActiveIndex_Implementation` falls back to `Settings->GetShadingQuality()` when nothing is pending — that reads as a copy-paste leftover (Shading is one group among eleven, not a stand-in for the aggregate). Don't copy that into the new class; each group's fallback should call *its own* accessor, and the aggregate's own fallback (if you touch it) should call `GetOverallScalabilityLevel()`.

## Open design question: preset vs. custom

Unreal's own scalability screen flips the aggregate dropdown to "Custom" the instant any individual group diverges from what the preset implies, and re-picking a preset snaps every group back in sync. Whether PPB wants that fidelity is a real UX decision, not a default to build speculatively:

- **If yes:** `UYT_SettingDef_GraphicsQuality` needs to compare all eleven group values against the current index's expected preset and return a sentinel "Custom" entry when they diverge. This is a one-directional dependency (aggregate reads groups; groups stay ignorant of the aggregate) — keep it that way rather than having the leaf classes know about the preset concept.
- **If no / not yet:** leave the aggregate dropdown as-is, but be aware it will silently go stale relative to per-group edits — it won't reflect "Custom" and won't show what's actually active. Decide (or ask) before shipping both controls side by side, rather than defaulting into the mismatch.

## Reference implementation

- `Engine/Source/Runtime/Engine/Classes/GameFramework/GameUserSettings.h` — native accessor names (verified against the UE 5.8 install this project targets).
- `Plugins/YT_Utility/Source/YT_Utility/{Public,Private}/GameUserSettings/Prebuilt/YT_SettingDef_GraphicsQuality.{h,cpp}` — the pattern to generalize from.
