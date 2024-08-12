#ifdef LANG_FR
#ifndef LANG_FILE
#define LANG_FILE
// DISCLAIMER: I dont speek french, but the pokemon watchface supports french, so i just added this file to keep it, feel free to correct the translations, i dont even know if the special characters work
#define TXT_OS_NAME "MiteOS"
#define TXT_DEVICE_NAME "MiteWatch"

#define TXT_ENABLE "Activer"
#define TXT_DISABLE "Désactiver"
#define TXT_ACTIVE "Actif"

#define TXT_SET_TIME "Régler l'heure"

#define TXT_ONCE "Une fois"
#define TXT_WORKDAY "Semaine"
#define TXT_WEEKEND "Week-end"
#define TXT_EVERY_DAY "Chaque jour"

#define TXT_SETTINGS "Réglages"
#define TXT_ALARM "Réveil"
#define TXT_TIMER "Minuteur"
#define TXT_BLUETOOTH "Bluetooth"
#define TXT_MEDIA "Médias"

#define TXT_ABOUT "Sur"
#define TXT_TIME "Temps"
#define TXT_NETWORK "Réseau"
#define TXT_DISPLAY "Écran"
#define TXT_INTERACTION "Interaction"
#define TXT_STORAGE "Mémoire"
#define TXT_TEST "Test"
#define TXT_SYNC_NTP "Sync NTP"
#define TXT_HOUR_VIB "Vibrer Heure"
#define TXT_SETUP_WIFI "Setup WiFi"
#define TXT_COLOR_SCHEME "Couleurs"
#define TXT_VIB_MOTOR "Essai du moteur"
#define TXT_SHOW_ACC "Accéléromètre"
#define TXT_TIMED "Temps spec."
#define TXT_STATIC "Statique"
#define TXT_INTERACT_VIBRATE_ON_BTN "Boutons Vib."
#define TXT_INTERACT_DBL_TAP "Double tape"

#define TXT_BTN_1 "Retour"
#define TXT_BTN_2 "Avant"
#define TXT_BTN_3 "Haute"
#define TXT_BTN_4 "Descendre"

#define TXT_PRESET "Minutes"
#define TXT_ALARM_CAPS "SONNERIE"
#define TXT_TIMER_CAPS "TIMER"
#define TXT_ALARM_IN TXT_ALARM " dans:"
#define TXT_MIN_CAPS "MIN"

#define TXT_CHIP "Puce"

#define TXT_BOND "Conn. Tele."
#define TXT_UNBOND "Annuler conn."
#define TXT_BOND_WAIT "Attendre les coupoles"

#define TXT_SYNCING "Chargement..."
#define TXT_NOTIFICATION "Nouvelles"
#define TXT_NO_NOTIFICATION "Aucune notification"
#define TXT_NO_PLAYBACK "Pas de lecture"

#define TXT_HASS "HASS"
#define TXT_WAIT "Attends..."
#define TXT_SUCCESS "Réussir"
#define TXT_FAILED "Échec"

#define TXT_DARKMODE "Mode sombre"
#define TXT_LIGHTMODE "Mode lumière"

#define TXT_OPTION_LEFT "<"
#define TXT_OPTION_RIGHT ">"

#define TXT_CHECKBOX_ON "[X]"
#define TXT_CHECKBOX_OFF "[ ]" 

#define TXT_ENABLE_ON TXT_CHECKBOX_ON " " TXT_ACTIVE
#define TXT_ENABLE_OFF TXT_CHECKBOX_OFF " " TXT_ACTIVE

#define TXT_ERR "Err"

// Month and Day Stuff
#define TXT_DAYS "Jours"

#define TXT_SUNDAY "dimanche"
#define TXT_SUNDAY_SHORT "dim"
#define TXT_MONDAY "lundi"
#define TXT_MONDAY_SHORT "lun"
#define TXT_TUESDAY "mardi"
#define TXT_TUESDAY_SHORT "mar"
#define TXT_WEDNESDAY "mercredi"
#define TXT_WEDNESDAY_SHORT "mer"
#define TXT_THURSDAY "jeudi"
#define TXT_THURSDAY_SHORT "jeu"
#define TXT_FRIDAY "vendredi"
#define TXT_FRIDAY_SHORT "ven"
#define TXT_SATURDAY "samedi"
#define TXT_SATURDAY_SHORT "sam"

#define TXT_JANUARY "janvier"
#define TXT_JANUARY_SHORT "jan"
#define TXT_FEBRUARY "février"
#define TXT_FEBRUARY_SHORT "fév"
#define TXT_MARCH "mars"
#define TXT_MARCH_SHORT "mar"
#define TXT_APRIL "avril"
#define TXT_APRIL_SHORT "avr"
#define TXT_MAY "mai"
#define TXT_MAY_SHORT "mai"
#define TXT_JUNE "juin"
#define TXT_JUNE_SHORT "jun"
#define TXT_JULY "juillet"
#define TXT_JULY_SHORT "jul"
#define TXT_AUGUST "août"
#define TXT_AUGUST_SHORT "aot"
#define TXT_SEPTEMBER "septembre"
#define TXT_SEPTEMBER_SHORT "sep"
#define TXT_OCTOBER "octobre"
#define TXT_OCTOBER_SHORT "oct"
#define TXT_NOVEMBER "november"
#define TXT_NOVEMBER_SHORT "nov"
#define TXT_DECEMBER "décembre"
#define TXT_DECEMBER_SHORT "déc"


// Pokemon Watchface
PROGMEM static char *pokemon_names[151] = {
	"BULBIZARRE","HERBIZARRE","FLORIZARRE","SALAMECHE","REPTINCEL","DRACAUFEU",
	"CARAPUCE","CARABAFFE","TORTANK","CHENIPAN","CHRYSACIER","PAPILUSION",
	"ASPICOT","COCONFORT","DARDARGNAN","ROUCOOL","ROUCOUPS","ROUCARNAGE",
	"RATTATA","RATTATAC","PIAFABEC","RAPASDEPIC","ABO","ARBOK",
	"PIKACHU","RAICHU","SABELETTE","SABLAIREAU","NIDORAN","NIDORINA",
	"NIDOQUEEN","NIDORAN","NIDORINO","NIDOKING","MELOFEE","MELODELFE",
	"GOUPIX","FEUNARD","RONDOUDOU","GRODOUDOU","NOSFERAPTI","NOSFERALTO",
	"MYSTHERBE","ORTIDE","RAFFLESIA","PARAS","PARASECT","MIMITOSS",
	"AEROMITE","TAUPIQUEUR","TRIOPIKEUR","MIAOUSS","PERSIAN","PSYKOKWAK",
	"AKWAKWAK","FEROSINGE","COLOSSINGE","CANINOS","ARCANIN","PTITARD",
	"TETARTE","TARTARD","ABRA","KADABRA","ALAKAZAM","MACHOC",
	"MACHOPEUR","MACKOGNEUR","CHETIFLOR","BOUSTIFLOR","EMPIFLOR","TENTACOOL",
	"TENTACRUEL","RACAILLOU","GRAVALANCH","GROLEM","PONYTA","GALOPA",
	"RAMOLOSS","FLAGADOSS","MAGNETI","MAGNETON","CANARTICHO","DODUO",
	"DODRIO","OTARIA","LAMANTINE","TADMORV","GROTADMORV","KOKIYAS",
	"CRUSTABRI","FANTOMINUS","SPECTRUM","ECTOPLASMA","ONIX","SOPORIFIK",
	"HYPNOMADE","KRABBY","KRABBOSS","VOLTORBE","ELECTRODE","NOEUNOEUF",
	"NOADKOKO","OSSELAIT","OSSATUEUR","KICKLEE","TYGNON","EXCELANGUE",
	"SMOGO","SMOGOGO","RHINOCORNE","RHINOFEROS","LEVEINARD","SAQUEDENEU",
	"KANGOUREX","HYPOTREMPE","HYPOCEAN","POISSIRENE","POISSOROY","STARI",
	"STAROSS","M. MIME","INSECATEUR","LIPPOUTOU","ELEKTEK","MAGMAR",
	"SCARABRUTE","TAUROS","MAGICARPE","LEVIATOR","LOKHLASS","METAMORPH",
	"EVOLI","AQUALI","VOLTALI","PYROLI","PORYGON","AMONITA",
	"AMONISTAR","KABUTO","KABUTOPS","PTERA","RONFLEX","ARTIKODIN",
	"ELECTHOR","SULFURA","MINIDRACO","DRACO","DRACOLOSSE","MEWTWO",
	"MEW"
};

#endif
#endif