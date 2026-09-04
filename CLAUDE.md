# YT_WidgetUtility

Shared widget-facing types: settings UI, menu screen push/pop, buttons, confirmation dialogs.

## Settings UI Layer

Third layer of the shared Storage/Logic/UI settings architecture (storage/logic live in `YT_Utility`):

| Type | Role |
|---|---|
| `UYT_SettingsContainer` | One tab's worth of setting widgets. `ApplySettings()` walks its own widget tree and applies every `UYT_SettingWidgetBase` found. |
| `UYT_SettingsScreen` | Hosts any number of `UYT_SettingsContainer` tabs found in its own widget tree. `RequestSave()` applies every container it finds, then saves once — adding a tab is just dropping another container into the tree, no code change. Its `NativeOnHandleBackAction` override means a gamepad Back action closes it the same way the Back button does. |

## UYT_MenuScreenLibrary

`PushSubScreen`/`PopSubScreen` static helpers over a `UCommonActivatableWidgetSwitcher`. Plain `UWidgetSwitcher` is not enough — it doesn't call `Activate()`/`Deactivate()` on its children or restore focus, which a `UCommonActivatableWidget` sub-screen needs for CommonInput.

## Menu Widgets

`UYTWidget_Pause` / `UYTWidget_MainMenu` own the `ScreenSwitcher` property Settings (or any other sub-screen) pushes onto. These, plus `UYT_ButtonBase` and the Confirmation dialog classes, live here rather than in `YT_Helpers` so every shared widget-facing type has one home — `YT_Helpers` depends on `YT_WidgetUtility`, not the other way around.
