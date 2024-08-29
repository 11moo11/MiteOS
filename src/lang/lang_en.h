#ifdef LANG_EN
#ifndef LANG_FILE
#define LANG_FILE

#define TXT_OS_NAME "MiteOS"
#define TXT_DEVICE_NAME "MiteWatch"

#define TXT_ENABLE "Enable"
#define TXT_DISABLE "Disable"
#define TXT_ACTIVE "Active"

#define TXT_SET_TIME "Set Time"

#define TXT_ONCE "Once"
#define TXT_WORKDAY "Workday"
#define TXT_WEEKEND "Weekend"
#define TXT_EVERY_DAY "Every Day"

#define TXT_SETTINGS "Settings"
#define TXT_ALARM "Alarm"
#define TXT_TIMER "Timer"
#define TXT_BLUETOOTH "Bluetooth"
#define TXT_MEDIA "Media"

#define TXT_ABOUT "About"
#define TXT_TIME "Time"
#define TXT_NETWORK "Network"
#define TXT_DISPLAY "Display"
#define TXT_INTERACTION "Interaction"
#define TXT_STORAGE "Storage"
#define TXT_TEST "Test"
#define TXT_SYNC_NTP "Sync NTP"
#define TXT_HOUR_VIB "Hour Vib."
#define TXT_SETUP_WIFI "Setup WiFi"
#define TXT_COLOR_SCHEME "Color Scheme"
#define TXT_VIB_MOTOR "Vibrate Motor"
#define TXT_SHOW_ACC "Accelerometer"
#define TXT_TIMED "Timed"
#define TXT_STATIC "Static"
#define TXT_INTERACT_VIBRATE_ON_BTN "Button Vib."
#define TXT_INTERACT_DBL_TAP "Double Tap"

#define TXT_BTN_1 "Back"
#define TXT_BTN_2 "Forward"
#define TXT_BTN_3 "Up"
#define TXT_BTN_4 "Down"

#define TXT_PRESET "Preset"
#define TXT_ALARM_CAPS "ALARM"
#define TXT_TIMER_CAPS "TIMER"
#define TXT_ALARM_IN TXT_ALARM " in:"
#define TXT_MIN_CAPS "MIN"

#define TXT_CHIP "Chip"

#define TXT_BOND "Bond Phone"
#define TXT_UNBOND "Unbond Phone"
#define TXT_BOND_WAIT "Waiting to bond"

#define TXT_SYNCING "Syncing..."
#define TXT_NOTIFICATION "Notifications"
#define TXT_NO_NOTIFICATION "No Notifications"
#define TXT_NO_PLAYBACK "No Playback"

#define TXT_HASS "HASS"
#define TXT_WAIT "Waiting..."
#define TXT_SUCCESS "Success"
#define TXT_FAILED "Failed"

#define TXT_DARKMODE "Darkmode"
#define TXT_LIGHTMODE "Lightmode"

#define TXT_OPTION_LEFT "<"
#define TXT_OPTION_RIGHT ">"

#define TXT_CHECKBOX_ON "[X]"
#define TXT_CHECKBOX_OFF "[ ]" 

#define TXT_ENABLE_ON TXT_CHECKBOX_ON " " TXT_ACTIVE
#define TXT_ENABLE_OFF TXT_CHECKBOX_OFF " " TXT_ACTIVE

#define TXT_ERR "Err"

// Tamagotchi App
#define TXT_FURLY "Furly"
#define TXT_WARMTH "Warmth"
#define TXT_HUNGER "Hunger"
#define TXT_THIRST "Thirst"
#define TXT_FUN "Fun"
#define TXT_EDUCATION "Education"
#define TXT_SICK "Sick"
#define TXT_LEVEL "Level"
#define TXT_MOOD "Mood"
#define TXT_BIRTHDATE "Birthdate"
#define TXT_AGE "Age"
#define TXT_RESTART "Restart"

// Month and Day Stuff
#define TXT_DAYS "Days"

#define TXT_SUNDAY "Sunday"
#define TXT_SUNDAY_SHORT "Sun"
#define TXT_MONDAY "Monday"
#define TXT_MONDAY_SHORT "Mon"
#define TXT_TUESDAY "Tuesday"
#define TXT_TUESDAY_SHORT "Tue"
#define TXT_WEDNESDAY "Wednesday"
#define TXT_WEDNESDAY_SHORT "Wed"
#define TXT_THURSDAY "Thursday"
#define TXT_THURSDAY_SHORT "Thu"
#define TXT_FRIDAY "Friday"
#define TXT_FRIDAY_SHORT "Fri"
#define TXT_SATURDAY "Saturday"
#define TXT_SATURDAY_SHORT "Sat"

#define TXT_JANUARY "January"
#define TXT_JANUARY_SHORT "Jan"
#define TXT_FEBRUARY "February"
#define TXT_FEBRUARY_SHORT "Feb"
#define TXT_MARCH "March"
#define TXT_MARCH_SHORT "Mar"
#define TXT_APRIL "April"
#define TXT_APRIL_SHORT "Apr"
#define TXT_MAY "May"
#define TXT_MAY_SHORT "May"
#define TXT_JUNE "June"
#define TXT_JUNE_SHORT "Jun"
#define TXT_JULY "July"
#define TXT_JULY_SHORT "Jul"
#define TXT_AUGUST "August"
#define TXT_AUGUST_SHORT "Aug"
#define TXT_SEPTEMBER "September"
#define TXT_SEPTEMBER_SHORT "Sep"
#define TXT_OCTOBER "October"
#define TXT_OCTOBER_SHORT "Oct"
#define TXT_NOVEMBER "November"
#define TXT_NOVEMBER_SHORT "Nov"
#define TXT_DECEMBER "December"
#define TXT_DECEMBER_SHORT "Dec"


// Pokemon Watchface
PROGMEM static char *pokemon_names[151] = {
	"BULBASAUR", "IVYSAUR", "VENUSAUR", "CHARMANDER", "CHARMELEON", "CHARIZARD",
	"SQUIRTLE", "WARTORTLE", "BLASTOISE", "CATERPIE", "METAPOD", "BUTTERFREE",
	"WEEDLE", "KAKUNA", "BEEDRILL", "PIDGEY", "PIDGEOTTO", "PIDGEOT",
	"RATTATA", "RATICATE", "SPEAROW", "FEAROW", "EKANS", "ARBOK",
	"PIKACHU", "RAICHU", "SANDSHREW", "SANDSLASH", "NIDORAN", "NIDORINA",
	"NIDOQUEEN", "NIDORAN", "NIDORINO", "NIDOKING", "CLEFAIRY", "CLEFABLE",
	"VULPIX", "NINETALES", "JIGGLYPUFF", "WIGGLYTUFF", "ZUBAT", "GOLBAT",
	"ODDISH", "GLOOM", "VILEPLUME", "PARAS", "PARASECT", "VENONAT",
	"VENOMOTH", "DIGLETT", "DUGTRIO", "MEOWTH", "PERSIAN", "PSYDUCK",
	"GOLDUCK", "MANKEY", "PRIMEAPE", "GROWLITHE", "ARCANINE", "POLIWAG",
	"POLIWHIRL", "POLIWRATH", "ABRA", "KADABRA", "ALAKAZAM", "MACHOP",
	"MACHOKE", "MACHAMP", "BELLSPROUT", "WEEPINBELL", "VICTREEBEL", "TENTACOOL",
	"TENTACRUEL", "GEODUDE", "GRAVELER", "GOLEM", "PONYTA", "RAPIDASH",
	"SLOWPOKE", "SLOWBRO", "MAGNEMITE", "MAGNETON", "FARFETCH'D", "DODUO",
	"DODRIO", "SEEL", "DEWGONG", "GRIMER", "MUK", "SHELLDER",
	"CLOYSTER", "GASTLY", "HAUNTER", "GENGAR", "ONIX", "DROWZEE",
	"HYPNO", "KRABBY", "KINGLER", "VOLTORB", "ELECTRODE", "EXEGGCUTE",
	"EXEGGUTOR", "CUBONE", "MAROWAK", "HITMONLEE", "HITMONCHAN", "LICKITUNG",
	"KOFFING", "WEEZING", "RHYHORN", "RHYDON", "CHANSEY", "TANGELA",
	"KANGASKHAN", "HORSEA", "SEADRA", "GOLDEEN", "SEAKING", "STARYU",
	"STARMIE", "MR. MIME", "SCYTHER", "JYNX", "ELECTABUZZ", "MAGMAR",
	"PINSIR", "TAUROS", "MAGIKARP", "GYARADOS", "LAPRAS", "DITTO",
	"EEVEE", "VAPOREON", "JOLTEON", "FLAREON", "PORYGON", "OMANYTE",
	"OMASTAR", "KABUTO", "KABUTOPS", "AERODACTYL", "SNORLAX", "ARTICUNO",
	"ZAPDOS", "MOLTRES", "DRATINI", "DRAGONAIR", "DRAGONITE", "MEWTWO",
	"MEW"
};

#endif
#endif