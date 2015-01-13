/*
THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.
COPYRIGHT 1993-1999 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/

/*
 *
 * Functions to load & save player's settings (*.plr file)
 *
 */

#ifdef HAVE_CONFIG_H
#include <conf.h>
#endif

#ifdef WINDOWS
#include "desw.h"
#include <mmsystem.h>
#endif

#include <stdio.h>
#include <string.h>
#if !defined(_MSC_VER) && !defined(macintosh)
#include <unistd.h>
#endif
#ifndef _WIN32_WCE
#include <errno.h>
#endif

#include <physfs.h>

#include "error.h"

#include "strutil.h"
#include "game.h"
#include "gameseq.h"
#include "player.h"
#include "playsave.h"
#include "joy.h"
#include "kconfig.h"
#include "digi.h"
#include "newmenu.h"
#include "joydefs.h"
#include "palette.h"
#include "multi.h"
#include "menu.h"
#include "config.h"
#include "text.h"
#include "mono.h"
#include "state.h"
#include "gauges.h"
#include "screens.h"
#include "powerup.h"
#include "makesig.h"
#include "byteswap.h"
#include "escort.h"

#include "physfsx.h"

#define SAVE_FILE_ID			MAKE_SIG('D','P','L','R')

#ifdef MACINTOSH
	#include <Files.h>
	#include <Errors.h>			// mac doesn't have "normal" error numbers -- must use mac equivs
	#ifndef ENOENT
		#define ENOENT fnfErr
	#endif
	#include "isp.h"
#elif defined(_WIN32_WCE)
# define errno -1
# define ENOENT -1
# define strerror(x) "Unknown Error"
#endif


#define PLAYER_FILE_VERSION	25 //increment this every time the player file changes

//version 5  ->  6: added new highest level information
//version 6  ->  7: stripped out the old saved_game array.
//version 7  ->  8: added reticle flag, & window size
//version 8  ->  9: removed player_struct_version
//version 9  -> 10: added default display mode
//version 10 -> 11: added all toggles in toggle menu
//version 11 -> 12: added weapon ordering
//version 12 -> 13: added more keys
//version 13 -> 14: took out marker key
//version 14 -> 15: added guided in big window
//version 15 -> 16: added small windows in cockpit
//version 16 -> 17: ??
//version 17 -> 18: save guidebot name
//version 18 -> 19: added automap-highres flag
//version 19 -> 20: added kconfig data for windows joysticks
//version 20 -> 21: save seperate config types for DOS & Windows
//version 21 -> 22: save lifetime netstats 
//version 22 -> 23: ??
//version 23 -> 24: add name of joystick for windows version.
//version 24 -> 25: removed kconfig data, joy name, guidebot name, joy sensitivity, cockpit views, netstats, taunt macros, highest levels

#define COMPATIBLE_PLAYER_FILE_VERSION          17


int Default_leveling_on=1;


int new_player_config()
{
	Player_default_difficulty = 1;
	Auto_leveling_on = Default_leveling_on = 1;

	return 1;
}

extern int Guided_in_big_window,Automap_always_hires;

uint32_t legacy_display_mode[] = { SM(320,200), SM(640,480), SM(320,400), SM(640,400), SM(800,600), SM(1024,768), SM(1280,1024) };

//read in the player's saved games.  returns errno (0 == no error)
int read_player_file()
{
	#ifdef MACINTOSH
	char filename[FILENAME_LEN+15];
	#else
	char filename[FILENAME_LEN];
	#endif
	PHYSFS_file *file;
	int id, i;
	short player_file_version;
	int rewrite_it=0;
	int swap = 0;

	Assert(Player_num>=0 && Player_num<MAX_PLAYERS);

	sprintf(filename, PLAYER_DIR "%.8s.plr", Players[Player_num].callsign);

	if (!PHYSFS_exists(filename))
		return ENOENT;

	file = PHYSFSX_openReadBuffered(filename);

#if 0
#ifndef MACINTOSH
	//check filename
	if (file && isatty(fileno(file))) {
		//if the callsign is the name of a tty device, prepend a char
		PHYSFS_close(file);
		sprintf(filename, PLAYER_DIR "$%.7s.plr",Players[Player_num].callsign);
		file = PHYSFSX_openReadBuffered(filename);
	}
#endif
#endif

	if (!file)
		goto read_player_file_failed;

	PHYSFS_readSLE32(file, &id);

        // SWAPINT added here because old versions of d2x
        // used the wrong byte order.
	if (id!=SAVE_FILE_ID && id!=SWAPINT(SAVE_FILE_ID)) {
		nm_messagebox(TXT_ERROR, 1, TXT_OK, "Invalid player file");
		PHYSFS_close(file);
		return -1;
	}

	player_file_version = cfile_read_short(file);

	if (player_file_version > 255) // bigendian file?
		swap = 1;

	if (swap)
		player_file_version = SWAPSHORT(player_file_version);

	if (player_file_version<COMPATIBLE_PLAYER_FILE_VERSION) {
		nm_messagebox(TXT_ERROR, 1, TXT_OK, TXT_ERROR_PLR_VERSION);
		PHYSFS_close(file);
		return -1;
	}

	Game_window_w = cfile_read_short(file);
	Game_window_h = cfile_read_short(file);

	if (swap) {
		Game_window_w = SWAPSHORT(Game_window_w);
		Game_window_h = SWAPSHORT(Game_window_h);
	}

	Player_default_difficulty = cfile_read_byte(file);
	Default_leveling_on       = cfile_read_byte(file);
	Reticle_on                = cfile_read_byte(file);
	Cockpit_mode              = cfile_read_byte(file);

	i                         = cfile_read_byte(file);
	Default_display_mode = legacy_display_mode[i];

	Missile_view_enabled      = cfile_read_byte(file);
	Headlight_active_default  = cfile_read_byte(file);
	Guided_in_big_window      = cfile_read_byte(file);
		
	if (player_file_version >= 19)
		Automap_always_hires = cfile_read_byte(file);

	Auto_leveling_on = Default_leveling_on;

	if (!PHYSFS_close(file))
		goto read_player_file_failed;

	if (rewrite_it)
		write_player_file();

	return EZERO;

 read_player_file_failed:
	nm_messagebox(TXT_ERROR, 1, TXT_OK, "%s\n\n%s", "Error reading PLR file", PHYSFS_getLastError());
	if (file)
		PHYSFS_close(file);

	return -1;
}


extern int Cockpit_mode_save;


//write out player's saved games.  returns errno (0 == no error)
int write_player_file()
{
	char filename[FILENAME_LEN+15];
	PHYSFS_file *file;
	int i;

//	#ifdef APPLE_DEMO		// no saving of player files in Apple OEM version
//	return 0;
//	#endif

	WriteConfigFile();

	sprintf(filename, PLAYER_DIR "%s.plr", Players[Player_num].callsign);
	file = PHYSFSX_openWriteBuffered(filename);

#if 0 //ndef MACINTOSH
	//check filename
	if (file && isatty(fileno(file))) {

		//if the callsign is the name of a tty device, prepend a char

		PHYSFS_close(file);
		sprintf(filename, PLAYER_DIR "$%.7s.plr", Players[Player_num].callsign);
		file = PHYSFSX_openWriteBuffered(filename);
	}
#endif

	if (!file)
		return -1;

	//Write out player's info
	PHYSFS_writeULE32(file, SAVE_FILE_ID);
	PHYSFS_writeULE16(file, PLAYER_FILE_VERSION);

	PHYSFS_writeULE16(file, Game_window_w);
	PHYSFS_writeULE16(file, Game_window_h);

	PHYSFSX_writeU8(file, Player_default_difficulty);
	PHYSFSX_writeU8(file, Auto_leveling_on);
	PHYSFSX_writeU8(file, Reticle_on);
	PHYSFSX_writeU8(file, (Cockpit_mode_save!=-1)?Cockpit_mode_save:Cockpit_mode);	//if have saved mode, write it instead of letterbox/rear view

	for (i = 0; i < (sizeof(legacy_display_mode) / sizeof(uint32_t)); i++) {
		if (legacy_display_mode[i] == Current_display_mode)
			break;
	}
	PHYSFSX_writeU8(file, i);

	PHYSFSX_writeU8(file, Missile_view_enabled);
	PHYSFSX_writeU8(file, Headlight_active_default);
	PHYSFSX_writeU8(file, Guided_in_big_window);
	PHYSFSX_writeU8(file, Automap_always_hires);

	if (!PHYSFS_close(file))
		goto write_player_file_failed;

	#ifdef MACINTOSH		// set filetype and creator for playerfile
	{
		FInfo finfo;
		Str255 pfilename;
		OSErr err;

		strcpy(pfilename, filename);
		c2pstr(pfilename);
		err = HGetFInfo(0, 0, pfilename, &finfo);
		finfo.fdType = 'PLYR';
		finfo.fdCreator = 'DCT2';
		err = HSetFInfo(0, 0, pfilename, &finfo);
	}
	#endif

	return EZERO;

 write_player_file_failed:
	nm_messagebox(TXT_ERROR, 1, TXT_OK, "%s\n\n%s", TXT_ERROR_WRITING_PLR, PHYSFS_getLastError());
	if (file)
	{
		PHYSFS_close(file);
		PHYSFS_delete(filename);        //delete bogus file
	}

	return -1;
}

//update the player's highest level.  returns errno (0 == no error)
int update_player_file()
{
	int ret;

	if ((ret=read_player_file()) != EZERO)
		if (ret != ENOENT)		//if file doesn't exist, that's ok
			return ret;

	return write_player_file();
}

int get_lifetime_checksum (int a,int b)
 {
  int num;

  // confusing enough to beat amateur disassemblers? Lets hope so

  num=(a<<8 ^ b);
  num^=(a | b);
  num*=num>>2;
  return (num);
 }
  

