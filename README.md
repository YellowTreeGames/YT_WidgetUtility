# YT_WidgetUtility

A library of shared UMG widget patterns for Unreal Engine 5. Depends on `YT_Utility` for the settings definition/registry types.

## Systems

### Settings
Widgets that bind to `YT_Utility`'s `UYT_SettingDefinition` data assets and drive `UYT_GameUserSettings`.
- **Base**: `UYT_SettingWidgetBase` — binds a `SettingDefinition`, listens for pending/applied changes, refreshes its visuals, and exposes `GetFocusTarget()` so a container can route initial gamepad/keyboard focus to its interactive control.
- **Controls**: `UYT_SettingWidget_Checkbox`, `UYT_SettingWidget_Dropdown`, `UYT_SettingWidget_Slider`, `UYT_SettingWidget_Audio` (uses `UAudioSlider` for dB-mapped volume controls). `UCheckBox`/`UComboBoxString`/`USlider`/`UAudioSlider` already handle gamepad input natively via Slate (`Accept` to toggle/open, D-pad/analog to adjust or navigate) — no CommonUI-specific work is needed on the controls themselves.
- **Container**: `UYT_SettingsContainer` is a `UCommonActivatableWidget`. It walks its own widget tree and calls `Apply()` on every `UYT_SettingWidgetBase` found within it — drop one or more setting widgets inside a container instance and call `ApplySettings()` to commit them all at once (e.g. from a Save button). It also overrides `NativeGetDesiredFocusTarget()` to focus the first setting control with a non-null `GetFocusTarget()` when the menu activates.

### Message Queue
A focus-managed queue of transient UI messages.
- **Manager**: `UMessageQueueManager` — owns the pending/in-focus queues and timers, broadcasts `OnMessageInFocus`/`OnMessageOutFocus`.
- **Widgets**: `UYT_MessageQueueWidget` (host, spawns/despawns message widgets into a panel) and `UYT_MessageQueueMessageWidget` (per-message display).

### Carousel
- **Base**: `UYT_BrushCarousel` — cycles a list of `FSlateBrush` slides with wraparound next/previous navigation. Subclass in Blueprint to bind `DisplayImage` and optional nav buttons/index label.
