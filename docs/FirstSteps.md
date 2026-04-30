# First Steps
## Internet Connection
To connect the watch to the internet:
- go into the app menu and select the `Settings` App.  
- Scroll down to `Network` and select it  
- Now select `Setup Wifi`  
- The watch will create a WiFi Network you need to connect to on your phone, there you are able to input the WiFi credentials.


## Phone Connection
To connect the watch to your phone:
- Open the phone app and navigate to `Pair Watch`
- On the watch navigate into the `Bluetooth` App
- Now select `Bond Phone` on the watch
- At the same time press the magnifying glass button (on the bottom right) on your phone
- The Watch should now appear as a pairable device on your phone, select it and complete the bonding process
- After that you can use the `Bluetooth` App to `Sync Settings` and `Sync Calander` (be sure to do that at least once)

## Weather Information
For now im using Open Meteo, it will require you to setup `Latitude` and `Longitude` on the app.  
A API Token and CityID isn't required anymore, so you can skip that. (It will be removed in future versions)

## Calendar
Setup the Calendar sync through the app
- Click on `Manage Calendar Sync`
- Select all the calendars you want to sync to your watchy


## TOTP
Setup the TOTP entries through the app
- Click on `Manage TOTP Entries`
- Now you can either edit existing entries or add new ones

## Home Assistant
Setup the Home Assistant through the app
- Specify a `Home Assistant URL` and `Home Assistant Token`
  - The `Home Assistant URL` needs to be **HTTP** not HTTPS
- To add entries, you can click on `Manage Home Assistant Entities`
- Now you can either edit existing entries or add new ones
  - There is no auto detection at the moment, but i plan to add it in future versions