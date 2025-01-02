#ifdef LANG_DE
#ifndef LANG_FILE
#define LANG_FILE

#define TXT_OS_NAME "MiteOS"
#define TXT_DEVICE_NAME "MiteWatch"

#define TXT_ENABLE "Aktivieren"
#define TXT_DISABLE "Deaktivieren"
#define TXT_ACTIVE "Aktiv"

#define TXT_SET_TIME "Zeit einstl."

#define TXT_ONCE "Einmal"
#define TXT_WORKDAY "Woche"
#define TXT_WEEKEND "Wochenende"
#define TXT_EVERY_DAY "Täglich"

#define TXT_SETTINGS "Einstellungen"
#define TXT_ALARM "Wecker"
#define TXT_TIMER "Timer"
#define TXT_BLUETOOTH "Bluetooth"
#define TXT_MEDIA "Medien"
#define TXT_TOTP "TOTP"
#define TXT_CALENDAR "Kalender"
#define TXT_WEATHER "Wetter"
#define TXT_APPS "Apps"
#define TXT_ACTIVITY "Aktivität"

#define TXT_ABOUT "Über"
#define TXT_WATCHFACE "Ziffernblatt"
#define TXT_TIME "Zeit"
#define TXT_NETWORK "Netzwerk"
#define TXT_DISPLAY "Display"
#define TXT_INTERACTION "Interaktion"
#define TXT_STORAGE "Speicher"
#define TXT_TEST "Test"
#define TXT_DEBUG "Debug"
#define TXT_SYNC_NTP "Sync NTP"
#define TXT_HOUR_VIB "Stündl. Vib."
#define TXT_SETUP_WIFI "Setup WiFi"
#define TXT_COLOR_SCHEME "Farbschema"
#define TXT_VIB_MOTOR "Motor test"
#define TXT_SHOW_ACC "Accelerometer"
#define TXT_REBOOT "Neustart"
#define TXT_TIMED "Zeit spez."
#define TXT_STATIC "Statisch"
#define TXT_INTERACT_VIBRATE_ON_BTN "Tasten Vib."
#define TXT_INTERACT_DBL_TAP "Double Tap"
#define TXT_WATCHFACE_TOP_LEFT "Oben Links"
#define TXT_WATCHFACE_TOP_RIGHT "Oben Rechts"

#define TXT_STEPS "Schritte"

#define TXT_BTN_1 "Zurück"
#define TXT_BTN_2 "Vor"
#define TXT_BTN_3 "Hoch"
#define TXT_BTN_4 "Runter"

#define TXT_PRESET "Minuten"
#define TXT_ALARM_CAPS "WECKER"
#define TXT_TIMER_CAPS "TIMER"
#define TXT_ALARM_IN TXT_ALARM " in:"
#define TXT_MIN_CAPS "MIN"

#define TEMP_LANG "de"
#define TXT_CHIP "Chip"

#define TXT_BOND "Handy verb."
#define TXT_UNBOND "Verb. aufheben"
#define TXT_BOND_WAIT "Warten auf Koppeln"

#define TXT_SYNCING "Lade..."
#define TXT_NOTIFICATION "Nachrichten"
#define TXT_NO_NOTIFICATION "Keine Benachrichtigungen"
#define TXT_PLAYBACK "Wiedergabe"
#define TXT_NO_PLAYBACK "Keine "TXT_PLAYBACK

#define TXT_HASS "HA"
#define TXT_WAIT "Warte..."
#define TXT_SUCCESS "Erfolgreich"
#define TXT_FAILED "Fehlgeschlagen"

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
#define TXT_FURLY "Wooly"
#define TXT_WARMTH "Wärme"
#define TXT_HUNGER "Hunger"
#define TXT_THIRST "Durst"
#define TXT_FUN "Spaß"
#define TXT_EDUCATION "Bildung"
#define TXT_SICK "Krank"
#define TXT_LEVEL "Level"
#define TXT_MOOD "Stimmung"
#define TXT_BIRTHDATE "Geburtstag"
#define TXT_AGE "Alter"
#define TXT_RESTART "Neustart"

// Month and Day Stuff
#define TXT_DAYS "Tage"

#define TXT_SUNDAY "Sonntag"
#define TXT_SUNDAY_SHORT "Son"
#define TXT_MONDAY "Montag"
#define TXT_MONDAY_SHORT "Mon"
#define TXT_TUESDAY "Dienstag"
#define TXT_TUESDAY_SHORT "Die"
#define TXT_WEDNESDAY "Mittwoch"
#define TXT_WEDNESDAY_SHORT "Mit"
#define TXT_THURSDAY "Donnerstag"
#define TXT_THURSDAY_SHORT "Don"
#define TXT_FRIDAY "Freitag"
#define TXT_FRIDAY_SHORT "Fre"
#define TXT_SATURDAY "Samstag"
#define TXT_SATURDAY_SHORT "Sam"

#define TXT_JANUARY "Januar"
#define TXT_JANUARY_SHORT "Jan"
#define TXT_FEBRUARY "Februar"
#define TXT_FEBRUARY_SHORT "Feb"
#define TXT_MARCH "März"
#define TXT_MARCH_SHORT "Mar" // Umlauts dont work, they mess with the array length
#define TXT_APRIL "April"
#define TXT_APRIL_SHORT "Apr"
#define TXT_MAY "Mai"
#define TXT_MAY_SHORT "Mai"
#define TXT_JUNE "Juni"
#define TXT_JUNE_SHORT "Jun"
#define TXT_JULY "Juli"
#define TXT_JULY_SHORT "Jul"
#define TXT_AUGUST "August"
#define TXT_AUGUST_SHORT "Aug"
#define TXT_SEPTEMBER "September"
#define TXT_SEPTEMBER_SHORT "Sep"
#define TXT_OCTOBER "Oktober"
#define TXT_OCTOBER_SHORT "Okt"
#define TXT_NOVEMBER "November"
#define TXT_NOVEMBER_SHORT "Nov"
#define TXT_DECEMBER "Dezember"
#define TXT_DECEMBER_SHORT "Dez"


// Pokemon Watchface
PROGMEM static char *pokemon_names[151] = {
	"BISASAM", "BISAKNOSP", "BISAFLOR", "GLUMANDA", "GLUTEXO", "GLURAK",
	"SCHIGGY", "SCHILLOK", "TURTOK", "RAUPEY", "SAFCON", "SAFRON",
	"HORNLIU", "KOKUNA", "BIBOR", "TAUBSI", "TAUBOGA", "TAUBOSS",
	"RETTAN", "RATTIKARL", "HABITAK", "IBITAK", "RETTAN", "ARBOK",
	"PIKACHU", "RAICHU", "SANDAN", "SANDAMER", "NIDORAN♀", "NIDORINA",
	"NIDOQUEEN", "NIDORAN♂", "NIDORINO", "NIDOKING", "PIEPI", "PIXI",
	"VULPIX", "VULNONA", "PUMMELUFF", "KNUDDELUFF", "ZUBAT", "GOLBAT",
	"MYRAPLA", "DUFLOR", "GIFLOR", "PARAS", "PARASEK", "BLUZUK", 
	"OMOT", "DIGDA", "DIGDRI", "MAUZI", "SNOBILIKAT", "ENTON",
	"ENTORON", "MENKI", "RASAFF", "FUKANO", "ARKANI", "QUIEKEL", "HYPNO",
	"SIMSALA", "ABRA", "KADABRA", "SIMSALA", "MACHOLLO", "MASCHOCK",
	"MACHOMEI", "KNOFENSA", "ULTRIGARIA", "SARZENIA", "TENTACHA",
	"TENTOXA", "KLEINSTEIN", "GEOROK", "GEOWAZ", "PONITA", "GALLOPA",
	"FLEGMON", "LAHMUS", "MAGNEMIT", "MAGNETON", "PORRIGON", "DODUO",
	"DODRI", "JUROBI", "JUGONG", "SLEIMA", "SLEIMOK", "MUSCHAS",
	"AUSTOS", "NEBULAK", "ALPOLLO", "GENGAR", "ONIX", "TRAUMATO",
	"HYPNO", "KRABBY", "KINGLER", "VOLTROB", "LEKTROBAL", "OWEI",
	"KOKOWEI", "TRAGOSSO", "KNOGGA", "KICKLEE", "NOCKCHAN", "SCHLURP",
	"SMOGON", "SMOGMOG", "RHINO", "RHINOCORN", "CHANEIRA", "TANGELA",
	"KANGAMA", "SEEJUNG", "SEEMON", "GOLKING", "GOLKING", "STERNDÜ",
	"STERNDÜ", "PANTIMOS", "SICHLOR", "ROSSANA", "ELEKTEK", "MAGMAR",
	"KABUTOPS", "TAUROS", "KARPAFDO", "GARADOS", "LAPRAS", "DITTO",
	"EVOLI", "AQUANA", "BLITZA", "FLAMARA", "PORYGON", "AMONITAS",
	"AMOROSO", "KABUTO", "KABUTOPS", "AERODACTYL", "RELAXO", "ARCTOS",
	"ZAPDOS", "LAVADOS", "DRATINI", "DRAGONIR", "DRAGORAN", "MEWTU",
	"MEW"
};

#endif
#endif