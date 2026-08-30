# Settings Menu: Tab Architecture

A recipe for standing up a tabbed settings menu (Game / Video / Audio / Controls) from `YT_Utility` + `YT_WidgetUtility`. RGBox proved this three-layer split first; this project has since moved the UI layer (`UYT_SettingsContainer`, `UYT_SettingWidgetBase` and its subclasses) out of `YT_Utility` and into `YT_WidgetUtility`, and rebased it on `UCommonActivatableWidget` to match this project's CommonUI-based menu screens (`UPPBWidget_Pause`, `UYTWidget_MainMenu`). Treat `YT_WidgetUtility` as the current home for this pattern going forward, not `YT_Utility`.

## When to use this

Standing up a new project's settings screen: a `GameUserSettings` subclass, a set of `SettingDefinition` data assets, and a tab host that switches between per-category containers.

## Design Principles

- **Three layers, no shortcuts.** Storage (`UYT_GameUserSettings` — Active vs. Pending state per `FGameplayTag`) → Logic (`UYT_SettingDefinition` data assets — rules, routing, `Apply()`) → UI (`UYT_SettingWidgetBase` subclasses — dumb, know only their assigned Data Asset). Don't let a widget reach past its `SettingDefinition` to touch `GameUserSettings` directly, and don't let a `SettingDefinition` hold UI state.
- **Widgets are presentation only.** Per the parent CLAUDE.md UI rule, gameplay/settings logic lives in the definition, not the widget. `UYT_SettingWidgetBase` already enforces this — a new setting type is a new `UYT_SettingDefinition` subclass plus a thin widget subclass, never new logic bolted onto an existing widget.
- **Follow this project's CommonUI idiom, not RGBox's bespoke tab manager.** RGBox wrote its own `URGBoxTabManager` / `URGBoxWidget_TabButton` (plain `UObject` + custom button class) because it predates this project's menu convention. This project's menu screens (`UPPBWidget_Pause`, `UYTWidget_MainMenu`) are already `UCommonActivatableWidget`, and `UYT_SettingsContainer` was migrated onto the same base for exactly this reason. Build the tab host the same way: a `UCommonTabListWidgetBase` (or a simple button row) driving a `UWidgetSwitcher` whose pages are `UYT_SettingsContainer` subclasses — don't reintroduce a second, parallel tab-switching mechanism.
- **One GameUserSettings subclass, one Registry asset, per project.** `UPPB_GameUserSettings : public UYT_GameUserSettings`, and one `UYT_SettingsRegistry` config asset listing every `SettingDefinition` the game should initialize on boot.
- **Save is the only chokepoint that persists to disk.** Individual setting widgets only ever move Pending values (`SetPendingValue`/`SetPendingIndex`) — they never call `Apply()` or `SaveSettings()` themselves. Only the top-level Settings screen's Save action commits. This is what lets Video/Gameplay changes "happen all at once" instead of mid-edit, and it's the single place [[settings_realtime_vs_batched]] carves an exception out of.

## Steps

1. Create `UPPB_GameUserSettings : public UYT_GameUserSettings` (mirrors `URGBoxGameUserSettings`) with a `GetPPBGameUserSettings()` static accessor and any project-specific accessors (e.g. a colorblind-mode getter, following `URGBoxGameUserSettings::GetColorBlindMode`).
2. Point the project at it: Project Settings → Engine → General Settings → **Game User Settings Class** (or `GameUserSettingsClassName` in `DefaultEngine.ini`).
3. Create one `UYT_SettingDefinition` data asset per setting (`UYT_SettingDef_Numeric`/`Categorical`/`Bool`/`AudioBus`/`Volume`), each with a unique `SettingTag`, following the `DA_YT_*` naming already vendored under `Plugins/YT_Utility/Content/Settings`.
4. Add every one of those data assets to a `UYT_SettingsRegistry` config asset (Project Settings → Yellow Tree → Settings Registry) so `InitializeCustomSettings` picks them up on boot.
5. Build one `UYT_SettingsContainer` Blueprint per tab (`WBP_PPB_GameSettings`, `WBP_PPB_VideoSettings`, `WBP_PPB_AudioSettings`, `WBP_PPB_ControlsSettings`), each populated with `UYT_SettingWidget_*` instances bound to a `SettingDefinition` via the `ExposeOnSpawn` property.
6. Build the tab host widget (`WBP_PPB_Settings`) as a `UCommonActivatableWidget`: a tab button row driving a `UWidgetSwitcher` whose pages are the four containers above, plus a Save button.
7. Wire Save to a chokepoint method on the tab host (mirrors `URGBoxWidget_Settings::ApplyAllTabSettings`): iterate every container and call `ApplySettings()` on each — not just the active tab — then call `GameUserSettings->SaveSettings()` once. Applying every tab regardless of which is active is what commits a Data Collection toggle on the Game tab even if the player never left the Video tab.

## Reference implementation

- RGBox: `Settings.md`, `Source/RGBox/UI/Settings/RGBoxWidget_Settings.{h,cpp}`, `Source/RGBox/Settings/RGBoxGameUserSettings.{h,cpp}` — the proven flow, pre-CommonUI.
- ProjectPiggyBank: `Plugins/YT_WidgetUtility/Source/YT_WidgetUtility/Public/Settings/YT_SettingsContainer.h`, `Source/ProjectPiggyBank/Settings/PPBWidget_SettingsContainer.h` — the container piece already exists; the tab host itself (step 6-7) is the missing piece this doc describes.
