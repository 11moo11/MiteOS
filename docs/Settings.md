# Settings
The following settings are available to the user.  
While the "On Compile" Settings might not be for everyone, i tried to expose every possible setting to be changed on a later date without recompiling.  

**Legend**  
❌ Not possible | ✅ Possible |  📋 Planned  

| Setting | Description  | Default |  On Compile (Configuration.h)  | On Watch | In Companion App  |
|---|---|---|---|---|---|
| NTP_SERVER  | Configures the server that is used for syncing the network time  | pool.ntp.org | ✅ | ❌ | ❌ |
| GMT_OFFSET_MIN | Configures the GMT Offset  | 0 (gets changed by the phone app) | ✅ | ❌ | ✅ |
| Darkmode | Toggles darkmode or sets it to the below times | timed | ❌ | ✅ | 📋 |
| INVERSE_DARKMODE  | Defines whether the timespan will make it darkmode or lightmode | no  | ✅  | ❌ | 📋 |
| DARKMODE_START_H  | Darkmode Start Time (Hour)  | 23  | ✅  | ❌ | 📋 |
| DARKMODE_START_M  | Darkmode Start Time (Minute)  | 0  | ✅  | ❌ | 📋 |
| DARKMODE_END_H  | Darkmode End Time (Hour)  | 6  | ✅  | ❌ | 📋 |
| DARKMODE_END_M  | Darkmode End Time (Minute)  | 30  | ✅  | ❌ | 📋 |
| Language | Change the Language (LANG_EN / LANG_DE / LANG_FR) | EN | ✅ | ❌ | ❌ |
| Alarm Count | Change the amount of alarms being available | 6 | ✅ | ❌ | ❌ |
| Set Alarms | Setting alarms to specific time and days | - | ❌ | ✅ | ❌ |
| Used Watchface | Configure what watchface is shown | 7SEG | ❌ | ✅ | 📋 |
| Action Button | Configure Top Left and Top Right Action Button | Top Left: Settings / Top Right: Notifications  | ❌ | ✅ | ❌ |
| Time | Set the time manually | Now | ❌ | ✅ | ❌ |
| Hour Vibration | Short vibration when the hour changes | yes | ❌ | ✅ | ❌ |
| Button Press Vibration | Short vibration when a button gets pressed | yes | ❌ | ✅ | ❌ |
| Setup Wifi | Setup a wifi connection (requires a seperate Wifi Able Device) | none | 📋 | ✅ | 📋 |
| Double Tap Screen | Button that gets pressed when double tapping the screen | Back | ❌ | ✅ | ❌ |
| Wakeup Behaviour | Changes the wakeup behaviour | minutely | ❌ | ✅ | ❌ |
| Bond Bluetooth | Bonding a bluetooth device requires a interaction on the watch and in the companion app at the same time | - | ❌ | ✅ | ✅ |
| Home Assistant Entities | Configure the Home Assistant Entities that are shown | - | 📋 | ❌ | ✅ |
| TOTP Entries | Configure TOTP entries that are shown | - | 📋 | ❌ | ✅ |
| Open Weather Map | API Token, City, Unit (Metric / Imperial) | - | 📋 | ❌ | ✅ |
| Calendars to sync | Configure what calendar appointments are getting synced | - | ❌ | ❌ | ✅ |
| Dither Album | Whether to dither the album cover art when syncing playback data | yes | ❌ | ❌ | ✅ |


**Legend**  
❌ Not possible | ✅ Possible |  📋 Planned  