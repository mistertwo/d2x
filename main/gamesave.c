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
 * Save game information
 *
 */

#ifdef HAVE_CONFIG_H
#include <conf.h>
#endif

#include <stdio.h>
#include <string.h>

#include "strutil.h"
#include "mono.h"
#include "key.h"
#include "gr.h"
#include "inferno.h"
#ifdef EDITOR
#include "editor/editor.h"
#endif
#include "error.h"
#include "cfile.h"
#include "byteswap.h"
#include "makesig.h"


char Gamesave_current_filename[PATH_MAX];

int Gamesave_current_version;

#define GAME_VERSION            32
#define GAME_COMPATIBLE_VERSION 22

//version 28->29  add delta light support
//version 27->28  controlcen id now is reactor number, not model number
//version 28->29  ??
//version 29->30  changed trigger structure
//version 30->31  changed trigger structure some more
//version 31->32  change segment structure, make it 512 bytes w/o editor, add Segment2s array.

#define MENU_CURSOR_X_MIN       MENU_X
#define MENU_CURSOR_X_MAX       MENU_X+6

#ifdef EDITOR
struct {
	ushort  fileinfo_signature;
	ushort  fileinfo_version;
	int     fileinfo_sizeof;
} game_top_fileinfo;    // Should be same as first two fields below...

struct {
	ushort  fileinfo_signature;
	ushort  fileinfo_version;
	int     fileinfo_sizeof;
	char    mine_filename[15];
	int     level;
	int     player_offset;              // Player info
	int     player_sizeof;
	int     object_offset;              // Object info
	int     object_howmany;
	int     object_sizeof;
	int     walls_offset;
	int     walls_howmany;
	int     walls_sizeof;
	int     doors_offset;
	int     doors_howmany;
	int     doors_sizeof;
	int     triggers_offset;
	int     triggers_howmany;
	int     triggers_sizeof;
	int     links_offset;
	int     links_howmany;
	int     links_sizeof;
	int     control_offset;
	int     control_howmany;
	int     control_sizeof;
	int     matcen_offset;
	int     matcen_howmany;
	int     matcen_sizeof;
	int     dl_indices_offset;
	int     dl_indices_howmany;
	int     dl_indices_sizeof;
	int     delta_light_offset;
	int     delta_light_howmany;
	int     delta_light_sizeof;
} game_fileinfo;
#endif // EDITOR

//  LINT: adding function prototypes
void read_object(object *obj, CFILE *f, int version);
#ifdef EDITOR
void write_object(object *obj, short version, PHYSFS_file *f);
void do_load_save_levels(int save);
#endif
#ifndef NDEBUG
void dump_mine_info(void);
#endif

extern char MaxPowerupsAllowed[MAX_POWERUP_TYPES];
extern char PowerupsInMine[MAX_POWERUP_TYPES];

#ifdef EDITOR
extern char mine_filename[];
extern int save_mine_data_compiled(PHYSFS_file *SaveFile);
//--unused-- #else
//--unused-- char mine_filename[128];
#endif

int Gamesave_num_org_robots = 0;
//--unused-- grs_bitmap * Gamesave_saved_bitmap = NULL;

#ifdef EDITOR
// Return true if this level has a name of the form "level??"
// Note that a pathspec can appear at the beginning of the filename.
int is_real_level(char *filename)
{
	int len = (int)strlen(filename);

	if (len < 6)
		return 0;

	//mprintf((0, "String = [%s]\n", &filename[len-11]));
	return !strnicmp(&filename[len-11], "level", 5);

}
#endif

//--unused-- vms_angvec zero_angles={0,0,0};

#define vm_angvec_zero(v) do {(v)->p=(v)->b=(v)->h=0;} while (0)

int Gamesave_num_players=0;

int N_save_pof_names;
char Save_pof_names[MAX_POLYGON_MODELS][FILENAME_LEN];

void check_and_fix_matrix(vms_matrix *m);

void verify_object( object * obj )	{

	obj->lifeleft = IMMORTAL_TIME;		//all loaded object are immortal, for now

	if ( obj->type == OBJ_ROBOT )	{
		Gamesave_num_org_robots++;

		// Make sure valid id...
		if ( obj->id >= N_robot_types )
			obj->id = obj->id % N_robot_types;

		// Make sure model number & size are correct...
		if ( obj->render_type == RT_POLYOBJ ) {
			Assert(Robot_info[obj->id].model_num != -1);
				//if you fail this assert, it means that a robot in this level
				//hasn't been loaded, possibly because he's marked as
				//non-shareware.  To see what robot number, print obj->id.

			Assert(Robot_info[obj->id].always_0xabcd == 0xabcd);
				//if you fail this assert, it means that the robot_ai for
				//a robot in this level hasn't been loaded, possibly because
				//it's marked as non-shareware.  To see what robot number,
				//print obj->id.

			obj->rtype.pobj_info.model_num = Robot_info[obj->id].model_num;
			obj->size = Polygon_models[obj->rtype.pobj_info.model_num].rad;

			//@@Took out this ugly hack 1/12/96, because Mike has added code
			//@@that should fix it in a better way.
			//@@//this is a super-ugly hack.  Since the baby stripe robots have
			//@@//their firing point on their bounding sphere, the firing points
			//@@//can poke through a wall if the robots are very close to it. So
			//@@//we make their radii bigger so the guns can't get too close to 
			//@@//the walls
			//@@if (Robot_info[obj->id].flags & RIF_BIG_RADIUS)
			//@@	obj->size = (obj->size*3)/2;

			//@@if (obj->control_type==CT_AI && Robot_info[obj->id].attack_type)
			//@@	obj->size = obj->size*3/4;
		}

		if (obj->id == 65)						//special "reactor" robots
			obj->movement_type = MT_NONE;

		if (obj->movement_type == MT_PHYSICS) {
			obj->mtype.phys_info.mass = Robot_info[obj->id].mass;
			obj->mtype.phys_info.drag = Robot_info[obj->id].drag;
		}
	}
	else {		//Robots taken care of above

		if ( obj->render_type == RT_POLYOBJ ) {
			int i;
			char *name = Save_pof_names[obj->rtype.pobj_info.model_num];

			for (i=0;i<N_polygon_models;i++)
				if (!stricmp(Pof_names[i],name)) {		//found it!	
					// mprintf((0,"Mapping <%s> to %d (was %d)\n",name,i,obj->rtype.pobj_info.model_num));
					obj->rtype.pobj_info.model_num = i;
					break;
				}
		}
	}

	if ( obj->type == OBJ_POWERUP ) {
		if ( obj->id >= N_powerup_types )	{
			obj->id = 0;
			Assert( obj->render_type != RT_POLYOBJ );
		}
		obj->control_type = CT_POWERUP;
		obj->size = Powerup_info[obj->id].size;
		obj->ctype.powerup_info.creation_time = 0;

#ifdef NETWORK
		if (Game_mode & GM_NETWORK)
			{
			  if (multi_powerup_is_4pack(obj->id))
				{
				 PowerupsInMine[obj->id-1]+=4;
			 	 MaxPowerupsAllowed[obj->id-1]+=4;
				}
			  PowerupsInMine[obj->id]++;
		     MaxPowerupsAllowed[obj->id]++;
			  mprintf ((0,"PowerupLimiter: ID=%d\n",obj->id));
			  if (obj->id>MAX_POWERUP_TYPES)
				mprintf ((1,"POWERUP: Overwriting array bounds!! Get JL!\n"));
		 	}
#endif

	}

	if ( obj->type == OBJ_WEAPON )	{
		if ( obj->id >= N_weapon_types )	{
			obj->id = 0;
			Assert( obj->render_type != RT_POLYOBJ );
		}

		if (obj->id == PMINE_ID) {		//make sure pmines have correct values

			obj->mtype.phys_info.mass = Weapon_info[obj->id].mass;
			obj->mtype.phys_info.drag = Weapon_info[obj->id].drag;
			obj->mtype.phys_info.flags |= PF_FREE_SPINNING;

			// Make sure model number & size are correct...		
			Assert( obj->render_type == RT_POLYOBJ );

			obj->rtype.pobj_info.model_num = Weapon_info[obj->id].model_num;
			obj->size = Polygon_models[obj->rtype.pobj_info.model_num].rad;
		}
	}

	if ( obj->type == OBJ_CNTRLCEN ) {

		obj->render_type = RT_POLYOBJ;
		obj->control_type = CT_CNTRLCEN;

		if (Gamesave_current_version <= 1) { // descent 1 reactor
			obj->id = 0;                         // used to be only one kind of reactor
			obj->rtype.pobj_info.model_num = Reactors[0].model_num;// descent 1 reactor
		}

		// Make sure model number is correct...
		//obj->rtype.pobj_info.model_num = Reactors[obj->id].model_num;
	}

	if ( obj->type == OBJ_PLAYER )	{
		//int i;

		//Assert(obj == Player);

		if ( obj == ConsoleObject )		
			init_player_object();
		else
			if (obj->render_type == RT_POLYOBJ)	//recover from Matt's pof file matchup bug
				obj->rtype.pobj_info.model_num = Player_ship->model_num;

		//Make sure orient matrix is orthogonal
		check_and_fix_matrix(&obj->orient);

		obj->id = Gamesave_num_players++;
	}

	if (obj->type == OBJ_HOSTAGE) {

		//@@if (obj->id > N_hostage_types)
		//@@	obj->id = 0;

		obj->render_type = RT_HOSTAGE;
		obj->control_type = CT_POWERUP;
	}

}

//static gs_skip(int len,CFILE *file)
//{
//
//	cfseek(file,len,SEEK_CUR);
//}


extern int multi_powerup_is_4pack(int);
//reads one object of the given version from the given file
void read_object(object *obj,CFILE *f,int version)
{

	obj->type           = cfile_read_byte(f);
	obj->id             = cfile_read_byte(f);

	obj->control_type   = cfile_read_byte(f);
	obj->movement_type  = cfile_read_byte(f);
	obj->render_type    = cfile_read_byte(f);
	obj->flags          = cfile_read_byte(f);

	obj->segnum         = cfile_read_short(f);
	obj->attached_obj   = -1;

	cfile_read_vector(&obj->pos,f);
	cfile_read_matrix(&obj->orient,f);

	obj->size           = cfile_read_fix(f);
	obj->shields        = cfile_read_fix(f);

	cfile_read_vector(&obj->last_pos,f);

	obj->contains_type  = cfile_read_byte(f);
	obj->contains_id    = cfile_read_byte(f);
	obj->contains_count = cfile_read_byte(f);

	switch (obj->movement_type) {

		case MT_PHYSICS:

			cfile_read_vector(&obj->mtype.phys_info.velocity,f);
			cfile_read_vector(&obj->mtype.phys_info.thrust,f);

			obj->mtype.phys_info.mass		= cfile_read_fix(f);
			obj->mtype.phys_info.drag		= cfile_read_fix(f);
			obj->mtype.phys_info.brakes	= cfile_read_fix(f);

			cfile_read_vector(&obj->mtype.phys_info.rotvel,f);
			cfile_read_vector(&obj->mtype.phys_info.rotthrust,f);

			obj->mtype.phys_info.turnroll	= cfile_read_fixang(f);
			obj->mtype.phys_info.flags		= cfile_read_short(f);

			break;

		case MT_SPINNING:

			cfile_read_vector(&obj->mtype.spin_rate,f);
			break;

		case MT_NONE:
			break;

		default:
			Int3();
	}

	switch (obj->control_type) {

		case CT_AI: {
			int i;

			obj->ctype.ai_info.behavior				= cfile_read_byte(f);

			for (i=0;i<MAX_AI_FLAGS;i++)
				obj->ctype.ai_info.flags[i]			= cfile_read_byte(f);

			obj->ctype.ai_info.hide_segment			= cfile_read_short(f);
			obj->ctype.ai_info.hide_index			= cfile_read_short(f);
			obj->ctype.ai_info.path_length			= cfile_read_short(f);
			obj->ctype.ai_info.cur_path_index		= cfile_read_short(f);

			if (version <= 25) {
				cfile_read_short(f);	//				obj->ctype.ai_info.follow_path_start_seg	= 
				cfile_read_short(f);	//				obj->ctype.ai_info.follow_path_end_seg		= 
			}

			break;
		}

		case CT_EXPLOSION:

			obj->ctype.expl_info.spawn_time		= cfile_read_fix(f);
			obj->ctype.expl_info.delete_time		= cfile_read_fix(f);
			obj->ctype.expl_info.delete_objnum	= cfile_read_short(f);
			obj->ctype.expl_info.next_attach = obj->ctype.expl_info.prev_attach = obj->ctype.expl_info.attach_parent = -1;

			break;

		case CT_WEAPON:

			//do I really need to read these?  Are they even saved to disk?

			obj->ctype.laser_info.parent_type		= cfile_read_short(f);
			obj->ctype.laser_info.parent_num		= cfile_read_short(f);
			obj->ctype.laser_info.parent_signature	= cfile_read_int(f);

			break;

		case CT_LIGHT:

			obj->ctype.light_info.intensity = cfile_read_fix(f);
			break;

		case CT_POWERUP:

			if (version >= 25)
				obj->ctype.powerup_info.count = cfile_read_int(f);
			else
				obj->ctype.powerup_info.count = 1;

			if (obj->id == POW_VULCAN_WEAPON)
					obj->ctype.powerup_info.count = VULCAN_WEAPON_AMMO_AMOUNT;

			if (obj->id == POW_GAUSS_WEAPON)
					obj->ctype.powerup_info.count = VULCAN_WEAPON_AMMO_AMOUNT;

			if (obj->id == POW_OMEGA_WEAPON)
					obj->ctype.powerup_info.count = MAX_OMEGA_CHARGE;

			break;


		case CT_NONE:
		case CT_FLYING:
		case CT_DEBRIS:
			break;

		case CT_SLEW:		//the player is generally saved as slew
			break;

		case CT_CNTRLCEN:
			break;

		case CT_MORPH:
		case CT_FLYTHROUGH:
		case CT_REPAIRCEN:
		default:
			Int3();
	
	}

	switch (obj->render_type) {

		case RT_NONE:
			break;

		case RT_MORPH:
		case RT_POLYOBJ: {
			int i,tmo;

			obj->rtype.pobj_info.model_num		= cfile_read_int(f);

			for (i=0;i<MAX_SUBMODELS;i++)
				cfile_read_angvec(&obj->rtype.pobj_info.anim_angles[i],f);

			obj->rtype.pobj_info.subobj_flags	= cfile_read_int(f);

			tmo = cfile_read_int(f);

			#ifndef EDITOR
			obj->rtype.pobj_info.tmap_override	= tmo;
			#else
			if (tmo==-1)
				obj->rtype.pobj_info.tmap_override	= -1;
			else {
				int xlated_tmo = tmap_xlate_table[tmo];
				if (xlated_tmo < 0)	{
					mprintf( (0, "Couldn't find texture for demo object, model_num = %d\n", obj->rtype.pobj_info.model_num));
					Int3();
					xlated_tmo = 0;
				}
				obj->rtype.pobj_info.tmap_override	= xlated_tmo;
			}
			#endif

			obj->rtype.pobj_info.alt_textures	= 0;

			break;
		}

		case RT_WEAPON_VCLIP:
		case RT_HOSTAGE:
		case RT_POWERUP:
		case RT_FIREBALL:

			obj->rtype.vclip_info.vclip_num	= cfile_read_int(f);
			obj->rtype.vclip_info.frametime	= cfile_read_fix(f);
			obj->rtype.vclip_info.framenum	= cfile_read_byte(f);

			break;

		case RT_LASER:
			break;

		default:
			Int3();

	}

}

#ifdef EDITOR

//writes one object to the given file
void write_object(object *obj, short version, PHYSFS_file *f)
{
	PHYSFSX_writeU8(f, obj->type);
	PHYSFSX_writeU8(f, obj->id);

	PHYSFSX_writeU8(f, obj->control_type);
	PHYSFSX_writeU8(f, obj->movement_type);
	PHYSFSX_writeU8(f, obj->render_type);
	PHYSFSX_writeU8(f, obj->flags);

	PHYSFS_writeSLE16(f, obj->segnum);

	PHYSFSX_writeVector(f, &obj->pos);
	PHYSFSX_writeMatrix(f, &obj->orient);

	PHYSFSX_writeFix(f, obj->size);
	PHYSFSX_writeFix(f, obj->shields);

	PHYSFSX_writeVector(f, &obj->last_pos);

	PHYSFSX_writeU8(f, obj->contains_type);
	PHYSFSX_writeU8(f, obj->contains_id);
	PHYSFSX_writeU8(f, obj->contains_count);

	switch (obj->movement_type) {

		case MT_PHYSICS:

	 		PHYSFSX_writeVector(f, &obj->mtype.phys_info.velocity);
			PHYSFSX_writeVector(f, &obj->mtype.phys_info.thrust);

			PHYSFSX_writeFix(f, obj->mtype.phys_info.mass);
			PHYSFSX_writeFix(f, obj->mtype.phys_info.drag);
			PHYSFSX_writeFix(f, obj->mtype.phys_info.brakes);

			PHYSFSX_writeVector(f, &obj->mtype.phys_info.rotvel);
			PHYSFSX_writeVector(f, &obj->mtype.phys_info.rotthrust);

			PHYSFSX_writeFixAng(f, obj->mtype.phys_info.turnroll);
			PHYSFS_writeSLE16(f, obj->mtype.phys_info.flags);

			break;

		case MT_SPINNING:

			PHYSFSX_writeVector(f, &obj->mtype.spin_rate);
			break;

		case MT_NONE:
			break;

		default:
			Int3();
	}

	switch (obj->control_type) {

		case CT_AI: {
			int i;

			PHYSFSX_writeU8(f, obj->ctype.ai_info.behavior);

			for (i = 0; i < MAX_AI_FLAGS; i++)
				PHYSFSX_writeU8(f, obj->ctype.ai_info.flags[i]);

			PHYSFS_writeSLE16(f, obj->ctype.ai_info.hide_segment);
			PHYSFS_writeSLE16(f, obj->ctype.ai_info.hide_index);
			PHYSFS_writeSLE16(f, obj->ctype.ai_info.path_length);
			PHYSFS_writeSLE16(f, obj->ctype.ai_info.cur_path_index);

			if (version <= 25)
			{
				PHYSFS_writeSLE16(f, -1);	//obj->ctype.ai_info.follow_path_start_seg
				PHYSFS_writeSLE16(f, -1);	//obj->ctype.ai_info.follow_path_end_seg
			}

			break;
		}

		case CT_EXPLOSION:

			PHYSFSX_writeFix(f, obj->ctype.expl_info.spawn_time);
			PHYSFSX_writeFix(f, obj->ctype.expl_info.delete_time);
			PHYSFS_writeSLE16(f, obj->ctype.expl_info.delete_objnum);

			break;

		case CT_WEAPON:

			//do I really need to write these objects?

			PHYSFS_writeSLE16(f, obj->ctype.laser_info.parent_type);
			PHYSFS_writeSLE16(f, obj->ctype.laser_info.parent_num);
			PHYSFS_writeSLE32(f, obj->ctype.laser_info.parent_signature);

			break;

		case CT_LIGHT:

			PHYSFSX_writeFix(f, obj->ctype.light_info.intensity);
			break;

		case CT_POWERUP:

			if (version >= 25)
				PHYSFS_writeSLE32(f, obj->ctype.powerup_info.count);
			break;

		case CT_NONE:
		case CT_FLYING:
		case CT_DEBRIS:
			break;

		case CT_SLEW:		//the player is generally saved as slew
			break;

		case CT_CNTRLCEN:
			break;			//control center object.

		case CT_MORPH:
		case CT_REPAIRCEN:
		case CT_FLYTHROUGH:
		default:
			Int3();
	
	}

	switch (obj->render_type) {

		case RT_NONE:
			break;

		case RT_MORPH:
		case RT_POLYOBJ: {
			int i;

			PHYSFS_writeSLE32(f, obj->rtype.pobj_info.model_num);

			for (i = 0; i < MAX_SUBMODELS; i++)
				PHYSFSX_writeAngleVec(f, &obj->rtype.pobj_info.anim_angles[i]);

			PHYSFS_writeSLE32(f, obj->rtype.pobj_info.subobj_flags);

			PHYSFS_writeSLE32(f, obj->rtype.pobj_info.tmap_override);

			break;
		}

		case RT_WEAPON_VCLIP:
		case RT_HOSTAGE:
		case RT_POWERUP:
		case RT_FIREBALL:

			PHYSFS_writeSLE32(f, obj->rtype.vclip_info.vclip_num);
			PHYSFSX_writeFix(f, obj->rtype.vclip_info.frametime);
			PHYSFSX_writeU8(f, obj->rtype.vclip_info.framenum);

			break;

		case RT_LASER:
			break;

		default:
			Int3();

	}

}
#endif

extern int remove_trigger_num(int trigger_num);

// --------------------------------------------------------------------
// Load game 
// Loads all the relevant data for a level.
// If level != -1, it loads the filename with extension changed to .min
// Otherwise it loads the appropriate level mine.
// returns 0=everything ok, 1=old version, -1=error
int load_game_data(CFILE *LoadFile)
{
	int i,j;

	short game_top_fileinfo_version;
	int object_offset;
	int gs_num_objects;
	int num_delta_lights;
	int trig_size;

	//===================== READ FILE INFO ========================

#if 0
	cfread(&game_top_fileinfo, sizeof(game_top_fileinfo), 1, LoadFile);
#endif

	// Check signature
	if (cfile_read_short(LoadFile) != 0x6705)
		return -1;

	// Read and check version number
	game_top_fileinfo_version = cfile_read_short(LoadFile);
	if (game_top_fileinfo_version < GAME_COMPATIBLE_VERSION )
		return -1;

	// We skip some parts of the former game_top_fileinfo
	cfseek(LoadFile, 31, SEEK_CUR);

	object_offset = cfile_read_int(LoadFile);
	gs_num_objects = cfile_read_int(LoadFile);
	cfseek(LoadFile, 8, SEEK_CUR);

	Num_walls = cfile_read_int(LoadFile);
	cfseek(LoadFile, 20, SEEK_CUR);

	Num_triggers = cfile_read_int(LoadFile);
	cfseek(LoadFile, 24, SEEK_CUR);

	trig_size = cfile_read_int(LoadFile);
	Assert(trig_size == sizeof(ControlCenterTriggers));
	cfseek(LoadFile, 4, SEEK_CUR);

	Num_robot_centers = cfile_read_int(LoadFile);
	cfseek(LoadFile, 4, SEEK_CUR);

	if (game_top_fileinfo_version >= 29) {
		cfseek(LoadFile, 4, SEEK_CUR);
		Num_static_lights = cfile_read_int(LoadFile);
		cfseek(LoadFile, 8, SEEK_CUR);
		num_delta_lights = cfile_read_int(LoadFile);
		cfseek(LoadFile, 4, SEEK_CUR);
	} else {
		Num_static_lights = 0;
		num_delta_lights = 0;
	}

	if (game_top_fileinfo_version >= 31) //load mine filename
		// read newline-terminated string, not sure what version this changed.
		cfgets(Current_level_name,sizeof(Current_level_name),LoadFile);
	else if (game_top_fileinfo_version >= 14) { //load mine filename
		// read null-terminated string
		char *p=Current_level_name;
		//must do read one char at a time, since no cfgets()
		do *p = cfgetc(LoadFile); while (*p++!=0);
	}
	else
		Current_level_name[0]=0;

	if (game_top_fileinfo_version >= 19) {	//load pof names
		N_save_pof_names = cfile_read_short(LoadFile);
		if (N_save_pof_names != 0x614d && N_save_pof_names != 0x5547) { // "Ma"de w/DMB beta/"GU"ILE
			Assert(N_save_pof_names < MAX_POLYGON_MODELS);
			cfread(Save_pof_names,N_save_pof_names,FILENAME_LEN,LoadFile);
		}
	}

	//===================== READ PLAYER INFO ==========================
	Object_next_signature = 0;

	//===================== READ OBJECT INFO ==========================

	Gamesave_num_org_robots = 0;
	Gamesave_num_players = 0;

	if (object_offset > -1) {
		if (cfseek( LoadFile, object_offset, SEEK_SET ))
			Error( "Error seeking to object_offset in gamesave.c" );

		for (i = 0; i < gs_num_objects; i++) {

			read_object(&Objects[i], LoadFile, game_top_fileinfo_version);

			Objects[i].signature = Object_next_signature++;
			verify_object( &Objects[i] );
		}

	}

	//===================== READ WALL INFO ============================

	for (i = 0; i < Num_walls; i++) {
		if (game_top_fileinfo_version >= 20)
			wall_read(&Walls[i], LoadFile); // v20 walls and up.
		else if (game_top_fileinfo_version >= 17) {
			v19_wall w;
			v19_wall_read(&w, LoadFile);
			Walls[i].segnum	        = w.segnum;
			Walls[i].sidenum	= w.sidenum;
			Walls[i].linked_wall	= w.linked_wall;
			Walls[i].type		= w.type;
			Walls[i].flags		= w.flags;
			Walls[i].hps		= w.hps;
			Walls[i].trigger	= w.trigger;
			Walls[i].clip_num	= w.clip_num;
			Walls[i].keys		= w.keys;
			Walls[i].state		= WALL_DOOR_CLOSED;
		} else {
			v16_wall w;
			v16_wall_read(&w, LoadFile);
			Walls[i].segnum = Walls[i].sidenum = Walls[i].linked_wall = -1;
			Walls[i].type		= w.type;
			Walls[i].flags		= w.flags;
			Walls[i].hps		= w.hps;
			Walls[i].trigger	= w.trigger;
			Walls[i].clip_num	= w.clip_num;
			Walls[i].keys		= w.keys;
		}
	}

#if 0
	//===================== READ DOOR INFO ============================

	if (game_fileinfo.doors_offset > -1)
	{
		if (!cfseek( LoadFile, game_fileinfo.doors_offset,SEEK_SET ))	{

			for (i=0;i<game_fileinfo.doors_howmany;i++) {

				if (game_top_fileinfo_version >= 20)
					active_door_read(&ActiveDoors[i], LoadFile); // version 20 and up
				else {
					v19_door d;
					int p;

					v19_door_read(&d, LoadFile);

					ActiveDoors[i].n_parts = d.n_parts;

					for (p=0;p<d.n_parts;p++) {
						int cseg,cside;

						cseg = Segments[d.seg[p]].children[d.side[p]];
						cside = find_connect_side(&Segments[d.seg[p]],&Segments[cseg]);

						ActiveDoors[i].front_wallnum[p] = Segments[d.seg[p]].sides[d.side[p]].wall_num;
						ActiveDoors[i].back_wallnum[p] = Segments[cseg].sides[cside].wall_num;
					}
				}

			}
		}
	}
#endif // 0

	//==================== READ TRIGGER INFO ==========================


// for MACINTOSH -- assume all triggers >= verion 31 triggers.

	for (i = 0; i < Num_triggers; i++)
	{
		if (game_top_fileinfo_version < 31)
		{
			v30_trigger trig;
			int t,type;
			type=0;

			if (game_top_fileinfo_version < 30) {
				v29_trigger trig29;
				int t;
				v29_trigger_read(&trig29, LoadFile);
				trig.flags	= trig29.flags;
				trig.num_links	= trig29.num_links;
				trig.num_links	= trig29.num_links;
				trig.value	= trig29.value;
				trig.time	= trig29.time;

				for (t=0;t<trig.num_links;t++) {
					trig.seg[t]  = trig29.seg[t];
					trig.side[t] = trig29.side[t];
				}
			}
			else
				v30_trigger_read(&trig, LoadFile);

			//Assert(trig.flags & TRIGGER_ON);
			trig.flags &= ~TRIGGER_ON;

			if (trig.flags & TRIGGER_CONTROL_DOORS)
				type = TT_OPEN_DOOR;
			else if (trig.flags & TRIGGER_SHIELD_DAMAGE)
				Int3();
			else if (trig.flags & TRIGGER_ENERGY_DRAIN)
				Int3();
			else if (trig.flags & TRIGGER_EXIT)
				type = TT_EXIT;
			else if (trig.flags & TRIGGER_ONE_SHOT)
				Int3();
			else if (trig.flags & TRIGGER_MATCEN)
				type = TT_MATCEN;
			else if (trig.flags & TRIGGER_ILLUSION_OFF)
				type = TT_ILLUSION_OFF;
			else if (trig.flags & TRIGGER_SECRET_EXIT)
				type = TT_SECRET_EXIT;
			else if (trig.flags & TRIGGER_ILLUSION_ON)
				type = TT_ILLUSION_ON;
			else if (trig.flags & TRIGGER_UNLOCK_DOORS)
				type = TT_UNLOCK_DOOR;
			else if (trig.flags & TRIGGER_OPEN_WALL)
				type = TT_OPEN_WALL;
			else if (trig.flags & TRIGGER_CLOSE_WALL)
				type = TT_CLOSE_WALL;
			else if (trig.flags & TRIGGER_ILLUSORY_WALL)
				type = TT_ILLUSORY_WALL;
			else
				Int3();
			Triggers[i].type        = type;
			Triggers[i].flags       = 0;
			Triggers[i].num_links   = trig.num_links;
			Triggers[i].num_links   = trig.num_links;
			Triggers[i].value       = trig.value;
			Triggers[i].time        = trig.time;
			for (t=0;t<trig.num_links;t++) {
				Triggers[i].seg[t] = trig.seg[t];
				Triggers[i].side[t] = trig.side[t];
			}
		}
		else
			trigger_read(&Triggers[i], LoadFile);
	}

	//================ READ CONTROL CENTER TRIGGER INFO ===============

#if 0
	if (game_fileinfo.control_offset > -1)
		if (!cfseek(LoadFile, game_fileinfo.control_offset, SEEK_SET))
		{
			Assert(game_fileinfo.control_sizeof == sizeof(control_center_triggers));
#endif // 0
	control_center_triggers_read_n(&ControlCenterTriggers, 1, LoadFile);

	//================ READ MATERIALOGRIFIZATIONATORS INFO ===============

	// mprintf((0, "Reading %i materialization centers.\n", game_fileinfo.matcen_howmany));
	for (i = 0; i < Num_robot_centers; i++) {
		if (game_top_fileinfo_version < 27) {
			old_matcen_info m;
			old_matcen_info_read(&m, LoadFile);
			RobotCenters[i].robot_flags[0] = m.robot_flags;
			RobotCenters[i].robot_flags[1] = 0;
			RobotCenters[i].hit_points = m.hit_points;
			RobotCenters[i].interval = m.interval;
			RobotCenters[i].segnum = m.segnum;
			RobotCenters[i].fuelcen_num = m.fuelcen_num;
		}
		else
			matcen_info_read(&RobotCenters[i], LoadFile);
			//	Set links in RobotCenters to Station array
			for (j = 0; j <= Highest_segment_index; j++)
			if (Segment2s[j].special == SEGMENT_IS_ROBOTMAKER)
				if (Segment2s[j].matcen_num == i)
					RobotCenters[i].fuelcen_num = Segment2s[j].value;
			// mprintf((0, "   %i: flags = %08x\n", i, RobotCenters[i].robot_flags));
	}

	//================ READ DL_INDICES INFO ===============

	for (i = 0; i < Num_static_lights; i++) {
		if (game_top_fileinfo_version < 29) {
			mprintf((0, "Warning: Old mine version.  Not reading Dl_indices info.\n"));
			Int3();	//shouldn't be here!!!
		} else
			dl_index_read(&Dl_indices[i], LoadFile);
	}

	//	Indicate that no light has been subtracted from any vertices.
	clear_light_subtracted();

	//================ READ DELTA LIGHT INFO ===============

	for (i = 0; i < num_delta_lights; i++) {
		if (game_top_fileinfo_version < 29) {
			mprintf((0, "Warning: Old mine version.  Not reading delta light info.\n"));
		} else
			delta_light_read(&Delta_lights[i], LoadFile);
	}

	//========================= UPDATE VARIABLES ======================

	reset_objects(gs_num_objects);

	for (i=0; i<MAX_OBJECTS; i++) {
		Objects[i].next = Objects[i].prev = -1;
		if (Objects[i].type != OBJ_NONE) {
			int objsegnum = Objects[i].segnum;

			if (objsegnum > Highest_segment_index)		//bogus object
				Objects[i].type = OBJ_NONE;
			else {
				Objects[i].segnum = -1;			//avoid Assert()
				obj_link(i,objsegnum);
			}
		}
	}

	clear_transient_objects(1);		//1 means clear proximity bombs

	// Make sure non-transparent doors are set correctly.
	for (i=0; i< Num_segments; i++)
		for (j=0;j<MAX_SIDES_PER_SEGMENT;j++) {
			side	*sidep = &Segments[i].sides[j];
			if ((sidep->wall_num != -1) && (Walls[sidep->wall_num].clip_num != -1)) {
				//mprintf((0, "Checking Wall %d\n", Segments[i].sides[j].wall_num));
				if (WallAnims[Walls[sidep->wall_num].clip_num].flags & WCF_TMAP1) {
					//mprintf((0, "Fixing non-transparent door.\n"));
					sidep->tmap_num = WallAnims[Walls[sidep->wall_num].clip_num].frames[0];
					sidep->tmap_num2 = 0;
				}
			}
		}


	reset_walls();

#if 0
	Num_open_doors = game_fileinfo.doors_howmany;
#endif // 0
	Num_open_doors = 0;

	//go through all walls, killing references to invalid triggers
	for (i=0;i<Num_walls;i++)
		if (Walls[i].trigger >= Num_triggers) {
			mprintf((0,"Removing reference to invalid trigger %d from wall %d\n",Walls[i].trigger,i));
			Walls[i].trigger = -1;	//kill trigger
		}

	//go through all triggers, killing unused ones
	for (i=0;i<Num_triggers;) {
		int w;

		//	Find which wall this trigger is connected to.
		for (w=0; w<Num_walls; w++)
			if (Walls[w].trigger == i)
				break;

	#ifdef EDITOR
		if (w == Num_walls) {
			mprintf((0,"Removing unreferenced trigger %d\n",i));
			remove_trigger_num(i);
		}
		else
	#endif
			i++;
	}

	//	MK, 10/17/95: Make walls point back at the triggers that control them.
	//	Go through all triggers, stuffing controlling_trigger field in Walls.
	{	int t;

	for (i=0; i<Num_walls; i++)
		Walls[i].controlling_trigger = -1;

	for (t=0; t<Num_triggers; t++) {
		int	l;
		for (l=0; l<Triggers[t].num_links; l++) {
			int	seg_num, side_num, wall_num;

			seg_num = Triggers[t].seg[l];
			side_num = Triggers[t].side[l];
			wall_num = Segments[seg_num].sides[side_num].wall_num;

			// -- if (Walls[wall_num].controlling_trigger != -1)
			// -- 	Int3();

			//check to see that if a trigger requires a wall that it has one,
			//and if it requires a matcen that it has one

			if (Triggers[t].type == TT_MATCEN) {
				if (Segment2s[seg_num].special != SEGMENT_IS_ROBOTMAKER)
					Int3();		//matcen trigger doesn't point to matcen
			}
			else if (Triggers[t].type != TT_LIGHT_OFF && Triggers[t].type != TT_LIGHT_ON) {	//light triggers don't require walls
				if (wall_num == -1)
					Int3();	//	This is illegal.  This trigger requires a wall
				else
					Walls[wall_num].controlling_trigger = t;
			}
		}
	}
	}

	//fix old wall structs
	if (game_top_fileinfo_version < 17) {
		int segnum,sidenum,wallnum;

		for (segnum=0; segnum<=Highest_segment_index; segnum++)
			for (sidenum=0;sidenum<6;sidenum++)
				if ((wallnum=Segments[segnum].sides[sidenum].wall_num) != -1) {
					Walls[wallnum].segnum = segnum;
					Walls[wallnum].sidenum = sidenum;
				}
	}

	#ifndef NDEBUG
	{
		int	sidenum;
		for (sidenum=0; sidenum<6; sidenum++) {
			int	wallnum = Segments[Highest_segment_index].sides[sidenum].wall_num;
			if (wallnum != -1)
				if ((Walls[wallnum].segnum != Highest_segment_index) || (Walls[wallnum].sidenum != sidenum))
					Int3();	//	Error.  Bogus walls in this segment.
								// Consult Yuan or Mike.
		}
	}
	#endif

	//create_local_segment_data();

	fix_object_segs();

	#ifndef NDEBUG
	dump_mine_info();
	#endif

	if (game_top_fileinfo_version < GAME_VERSION
	    && !(game_top_fileinfo_version == 25 && GAME_VERSION == 26))
		return 1;		//means old version
	else
		return 0;
}


int check_segment_connections(void);

extern void	set_ambient_sound_flags(void);

// ----------------------------------------------------------------------------

#define LEVEL_FILE_VERSION      8
//1 -> 2  add palette name
//2 -> 3  add control center explosion time
//3 -> 4  add reactor strength
//4 -> 5  killed hostage text stuff
//5 -> 6  added Secret_return_segment and Secret_return_orient
//6 -> 7  added flickering lights
//7 -> 8  made version 8 to be not compatible with D2 1.0 & 1.1

#ifndef RELEASE
const char *Level_being_loaded=NULL;
#endif

#ifdef COMPACT_SEGS
extern void ncache_flush();
#endif

extern int Slide_segs_computed;
extern int d1_pig_present;

int no_old_level_file_error=0;

//loads a level (.LVL) file from disk
//returns 0 if success, else error code
int load_level(const char * filename_passed)
{
#ifdef EDITOR
	int use_compiled_level=1;
#endif
	CFILE * LoadFile;
	char filename[PATH_MAX];
	int sig, minedata_offset, gamedata_offset;
	int mine_err, game_err;
#ifdef NETWORK
	int i;
#endif

	Slide_segs_computed = 0;

#ifdef NETWORK
   if (Game_mode & GM_NETWORK)
	 {
	  for (i=0;i<MAX_POWERUP_TYPES;i++)
		{
			MaxPowerupsAllowed[i]=0;
			PowerupsInMine[i]=0;
		}
	 }
#endif

	#ifdef COMPACT_SEGS
	ncache_flush();
	#endif

	#ifndef RELEASE
	Level_being_loaded = filename_passed;
	#endif

	strcpy(filename,filename_passed);

	#ifdef EDITOR
		//if we have the editor, try the LVL first, no matter what was passed.
		//if we don't have an LVL, try what was passed or RL2  
		//if we don't have the editor, we just use what was passed
	
		change_filename_extension(filename,filename_passed,".lvl");
		use_compiled_level = 0;
	
		if (!cfexist(filename))
		{
			char *p = strrchr(filename_passed, '.');

			if (stricmp(p, ".lvl"))
				strcpy(filename, filename_passed);	// set to what was passed
			else
				change_filename_extension(filename, filename, ".rl2");
			use_compiled_level = 1;
		}		
	#endif

	LoadFile = cfopen( filename, "rb" );

	if (!LoadFile)	{
		#ifdef EDITOR
			mprintf((0,"Can't open level file <%s>\n", filename));
			return 1;
		#else
			Error("Can't open file <%s>\n",filename);
		#endif
	}

	strcpy( Gamesave_current_filename, filename );

//	#ifdef NEWDEMO
//	if ( Newdemo_state == ND_STATE_RECORDING )
//		newdemo_record_start_demo();
//	#endif

	sig                      = cfile_read_int(LoadFile);
	Gamesave_current_version = cfile_read_int(LoadFile);
	mprintf((0, "Gamesave_current_version = %d\n", Gamesave_current_version));
	minedata_offset          = cfile_read_int(LoadFile);
	gamedata_offset          = cfile_read_int(LoadFile);

	Assert(sig == MAKE_SIG('P','L','V','L'));

	if (Gamesave_current_version >= 8) {    //read dummy data
		cfile_read_int(LoadFile);
		cfile_read_short(LoadFile);
		cfile_read_byte(LoadFile);
	}

	if (Gamesave_current_version < 5)
		cfile_read_int(LoadFile);       //was hostagetext_offset

	if (Gamesave_current_version > 1)
		cfgets(Current_level_palette,sizeof(Current_level_palette),LoadFile);
	if (Gamesave_current_version <= 1 || Current_level_palette[0]==0) // descent 1 level
		strcpy(Current_level_palette, DEFAULT_LEVEL_PALETTE);

	if (Gamesave_current_version >= 3)
		Base_control_center_explosion_time = cfile_read_int(LoadFile);
	else
		Base_control_center_explosion_time = DEFAULT_CONTROL_CENTER_EXPLOSION_TIME;

	if (Gamesave_current_version >= 4)
		Reactor_strength = cfile_read_int(LoadFile);
	else
		Reactor_strength = -1;  //use old defaults

	if (Gamesave_current_version >= 7) {
		int i;

		Num_flickering_lights = cfile_read_int(LoadFile);
		Assert((Num_flickering_lights >= 0) && (Num_flickering_lights < MAX_FLICKERING_LIGHTS));
		for (i = 0; i < Num_flickering_lights; i++)
			flickering_light_read(&Flickering_lights[i], LoadFile);
	}
	else
		Num_flickering_lights = 0;

	if (Gamesave_current_version < 6) {
		Secret_return_segment = 0;
		Secret_return_orient.rvec.x = F1_0;
		Secret_return_orient.rvec.y = 0;
		Secret_return_orient.rvec.z = 0;
		Secret_return_orient.fvec.x = 0;
		Secret_return_orient.fvec.y = F1_0;
		Secret_return_orient.fvec.z = 0;
		Secret_return_orient.uvec.x = 0;
		Secret_return_orient.uvec.y = 0;
		Secret_return_orient.uvec.z = F1_0;
	} else {
		Secret_return_segment = cfile_read_int(LoadFile);
		Secret_return_orient.rvec.x = cfile_read_int(LoadFile);
		Secret_return_orient.rvec.y = cfile_read_int(LoadFile);
		Secret_return_orient.rvec.z = cfile_read_int(LoadFile);
		Secret_return_orient.fvec.x = cfile_read_int(LoadFile);
		Secret_return_orient.fvec.y = cfile_read_int(LoadFile);
		Secret_return_orient.fvec.z = cfile_read_int(LoadFile);
		Secret_return_orient.uvec.x = cfile_read_int(LoadFile);
		Secret_return_orient.uvec.y = cfile_read_int(LoadFile);
		Secret_return_orient.uvec.z = cfile_read_int(LoadFile);
	}

	cfseek(LoadFile,minedata_offset,SEEK_SET);
	#ifdef EDITOR
	if (!use_compiled_level) {
		mine_err = load_mine_data(LoadFile);
#if 0 // get from d1src if needed
		// Compress all uv coordinates in mine, improves texmap precision. --MK, 02/19/96
		compress_uv_coordinates_all();
#endif
	} else
	#endif
		//NOTE LINK TO ABOVE!!
		mine_err = load_mine_data_compiled(LoadFile);

	if (mine_err == -1) {   //error!!
		cfclose(LoadFile);
		return 2;
	}

	cfseek(LoadFile,gamedata_offset,SEEK_SET);
	game_err = load_game_data(LoadFile);

	if (game_err == -1) {   //error!!
		cfclose(LoadFile);
		return 3;
	}

	//======================== CLOSE FILE =============================

	cfclose( LoadFile );

	set_ambient_sound_flags();

	#if 0	//def EDITOR
	write_game_text_file(filename);
	if (Errors_in_mine) {
		if (is_real_level(filename)) {
			char  ErrorMessage[200];

			sprintf( ErrorMessage, "Warning: %i errors in %s!\n", Errors_in_mine, Level_being_loaded );
			stop_time();
			gr_palette_load(gr_palette);
			nm_messagebox( NULL, 1, "Continue", ErrorMessage );
			start_time();
		} else
			mprintf((1, "Error: %i errors in %s.\n", Errors_in_mine, Level_being_loaded));
	}
	#endif

	#ifdef EDITOR
	//If a Descent 1 level and the Descent 1 pig isn't present, pretend it's a Descent 2 level.
	if ((Function_mode == FMODE_EDITOR) && (Gamesave_current_version <= 3) && !d1_pig_present)
	{
		if (!no_old_level_file_error)
			Warning("A Descent 1 level was loaded,\n"
					"and there is no Descent 1 texture\n"
					"set available. Saving it will\n"
					"convert it to a Descent 2 level.");

		Gamesave_current_version = LEVEL_FILE_VERSION;
	}
	#endif

	#ifdef EDITOR
	if (Function_mode == FMODE_EDITOR)
		editor_status("Loaded NEW mine %s, \"%s\"",filename,Current_level_name);
	#endif

	#if !defined(NDEBUG) && !defined(COMPACT_SEGS)
	if (check_segment_connections())
		nm_messagebox( "ERROR", 1, "Ok", 
				"Connectivity errors detected in\n"
				"mine.  See monochrome screen for\n"
				"details, and contact Matt or Mike." );
	#endif

	return 0;
}

#ifdef EDITOR
int get_level_name()
{
//NO_UI!!!	UI_WINDOW 				*NameWindow = NULL;
//NO_UI!!!	UI_GADGET_INPUTBOX	*NameText;
//NO_UI!!!	UI_GADGET_BUTTON 		*QuitButton;
//NO_UI!!!
//NO_UI!!!	// Open a window with a quit button
//NO_UI!!!	NameWindow = ui_open_window( 20, 20, 300, 110, WIN_DIALOG );
//NO_UI!!!	QuitButton = ui_add_gadget_button( NameWindow, 150-24, 60, 48, 40, "Done", NULL );
//NO_UI!!!
//NO_UI!!!	ui_wprintf_at( NameWindow, 10, 12,"Please enter a name for this mine:" );
//NO_UI!!!	NameText = ui_add_gadget_inputbox( NameWindow, 10, 30, LEVEL_NAME_LEN, LEVEL_NAME_LEN, Current_level_name );
//NO_UI!!!
//NO_UI!!!	NameWindow->keyboard_focus_gadget = (UI_GADGET *)NameText;
//NO_UI!!!	QuitButton->hotkey = KEY_ENTER;
//NO_UI!!!
//NO_UI!!!	ui_gadget_calc_keys(NameWindow);
//NO_UI!!!
//NO_UI!!!	while (!QuitButton->pressed && last_keypress!=KEY_ENTER) {
//NO_UI!!!		ui_mega_process();
//NO_UI!!!		ui_window_do_gadgets(NameWindow);
//NO_UI!!!	}
//NO_UI!!!
//NO_UI!!!	strcpy( Current_level_name, NameText->text );
//NO_UI!!!
//NO_UI!!!	if ( NameWindow!=NULL )	{
//NO_UI!!!		ui_close_window( NameWindow );
//NO_UI!!!		NameWindow = NULL;
//NO_UI!!!	}
//NO_UI!!!

	newmenu_item m[2];

	m[0].type = NM_TYPE_TEXT; m[0].text = "Please enter a name for this mine:";
	m[1].type = NM_TYPE_INPUT; m[1].text = Current_level_name; m[1].text_len = LEVEL_NAME_LEN;

	return newmenu_do( NULL, "Enter mine name", 2, m, NULL ) >= 0;

}
#endif


#ifdef EDITOR

int	Errors_in_mine;

// -----------------------------------------------------------------------------
int compute_num_delta_light_records(void)
{
	int	i;
	int	total = 0;

	for (i=0; i<Num_static_lights; i++) {
		total += Dl_indices[i].count;
	}

	return total;

}

// -----------------------------------------------------------------------------
// Save game
int save_game_data(PHYSFS_file *SaveFile)
{
	short game_top_fileinfo_version = Gamesave_current_version >= 5 ? 31 : 25;
	int player_offset = 0, object_offset = 0, walls_offset = 0, doors_offset = 0, triggers_offset = 0, control_offset = 0, matcen_offset = 0; //, links_offset;
	int dl_indices_offset = 0, delta_light_offset = 0;
	int offset_offset = 0, end_offset = 0;
	int num_delta_lights = 0;
	int i;

	//===================== SAVE FILE INFO ========================

	PHYSFS_writeSLE16(SaveFile, 0x6705);	// signature
	PHYSFS_writeSLE16(SaveFile, game_top_fileinfo_version);
	PHYSFS_writeSLE32(SaveFile, sizeof(game_fileinfo));
	PHYSFS_write(SaveFile, Current_level_name, 15, 1);
	PHYSFS_writeSLE32(SaveFile, Current_level_num);
	offset_offset = (int)PHYSFS_tell(SaveFile);	// write the offsets later
	PHYSFS_writeSLE32(SaveFile, -1);
	PHYSFS_writeSLE32(SaveFile, sizeof(player));

#define WRITE_HEADER_ENTRY(t, n) do { PHYSFS_writeSLE32(SaveFile, -1); PHYSFS_writeSLE32(SaveFile, n); PHYSFS_writeSLE32(SaveFile, sizeof(t)); } while(0)

	WRITE_HEADER_ENTRY(object, Highest_object_index + 1);
	WRITE_HEADER_ENTRY(wall, Num_walls);
	WRITE_HEADER_ENTRY(active_door, Num_open_doors);
	WRITE_HEADER_ENTRY(trigger, Num_triggers);
	WRITE_HEADER_ENTRY(0, 0);		// links (removed by Parallax)
	WRITE_HEADER_ENTRY(control_center_triggers, 1);
	WRITE_HEADER_ENTRY(matcen_info, Num_robot_centers);

	if (game_top_fileinfo_version >= 29)
	{
		WRITE_HEADER_ENTRY(dl_index, Num_static_lights);
		WRITE_HEADER_ENTRY(delta_light, num_delta_lights = compute_num_delta_light_records());
	}

	// Write the mine name
	if (game_top_fileinfo_version >= 31)
		PHYSFSX_printf(SaveFile, "%s\n", Current_level_name);
	else if (game_top_fileinfo_version >= 14)
		PHYSFSX_writeString(SaveFile, Current_level_name);

	if (game_top_fileinfo_version >= 19)
	{
		PHYSFS_writeSLE16(SaveFile, N_polygon_models);
		PHYSFS_write(SaveFile, Pof_names, sizeof(*Pof_names), N_polygon_models);
	}

	//==================== SAVE PLAYER INFO ===========================

	player_offset = (int)PHYSFS_tell(SaveFile);
	PHYSFS_write(SaveFile, &Players[Player_num], sizeof(player), 1); // not endian friendly, but not used either

	//==================== SAVE OBJECT INFO ===========================

	object_offset = (int)PHYSFS_tell(SaveFile);
	//fwrite( &Objects, sizeof(object), game_fileinfo.object_howmany, SaveFile );
	{
		for (i = 0; i <= Highest_object_index; i++)
			write_object(&Objects[i], game_top_fileinfo_version, SaveFile);
	}

	//==================== SAVE WALL INFO =============================

	walls_offset = (int)PHYSFS_tell(SaveFile);
	for (i = 0; i < Num_walls; i++)
		wall_write(&Walls[i], game_top_fileinfo_version, SaveFile);

	//==================== SAVE DOOR INFO =============================

#if 0
	doors_offset = PHYSFS_tell(SaveFile);
	for (i = 0; i < Num_open_doors; i++)
		door_write(&ActiveDoors[i], game_top_fileinfo_version, SaveFile);
#else
	doors_offset = 0; // kill warning
#endif

	//==================== SAVE TRIGGER INFO =============================

	triggers_offset = (int)PHYSFS_tell(SaveFile);
	for (i = 0; i < Num_triggers; i++)
		trigger_write(&Triggers[i], game_top_fileinfo_version, SaveFile);

	//================ SAVE CONTROL CENTER TRIGGER INFO ===============

	control_offset = (int)PHYSFS_tell(SaveFile);
	control_center_triggers_write(&ControlCenterTriggers, SaveFile);


	//================ SAVE MATERIALIZATION CENTER TRIGGER INFO ===============

	matcen_offset = (int)PHYSFS_tell(SaveFile);
	// mprintf((0, "Writing %i materialization centers\n", game_fileinfo.matcen_howmany));
	// { int i;
	// for (i=0; i<game_fileinfo.matcen_howmany; i++)
	// 	mprintf((0, "   %i: robot_flags = %08x\n", i, RobotCenters[i].robot_flags));
	// }
	for (i = 0; i < Num_robot_centers; i++)
		matcen_info_write(&RobotCenters[i], game_top_fileinfo_version, SaveFile);

	//================ SAVE DELTA LIGHT INFO ===============
	if (game_top_fileinfo_version >= 29)
	{
		dl_indices_offset = (int)PHYSFS_tell(SaveFile);
		for (i = 0; i < Num_static_lights; i++)
			dl_index_write(&Dl_indices[i], SaveFile);

		delta_light_offset = (int)PHYSFS_tell(SaveFile);
		for (i = 0; i < num_delta_lights; i++)
			delta_light_write(&Delta_lights[i], SaveFile);
	}

	//============= SAVE OFFSETS ===============

	end_offset = (int)PHYSFS_tell(SaveFile);

	// Update the offset fields

#define WRITE_OFFSET(o, n) do { PHYSFS_seek(SaveFile, offset_offset); PHYSFS_writeSLE32(SaveFile, o ## _offset); offset_offset += sizeof(int)*n; } while (0)

	WRITE_OFFSET(player, 2);
	WRITE_OFFSET(object, 3);
	WRITE_OFFSET(walls, 3);
	WRITE_OFFSET(doors, 3);
	WRITE_OFFSET(triggers, 6);
	WRITE_OFFSET(control, 3);
	WRITE_OFFSET(matcen, 3);
	if (game_top_fileinfo_version >= 29)
	{
		WRITE_OFFSET(dl_indices, 3);
		WRITE_OFFSET(delta_light, 0);
	}

	// Go back to end of data
	PHYSFS_seek(SaveFile, end_offset);

	return 0;
}

int save_mine_data(FILE * SaveFile);

// -----------------------------------------------------------------------------
// Save game
int save_level_sub(char * filename, int compiled_version)
{
	PHYSFS_file * SaveFile;
	char temp_filename[PATH_MAX];
	int minedata_offset=0,gamedata_offset=0;

//	if ( !compiled_version )
	{
		write_game_text_file(filename);

		if (Errors_in_mine) {
			if (is_real_level(filename)) {
				char  ErrorMessage[200];
	
				sprintf( ErrorMessage, "Warning: %i errors in this mine!\n", Errors_in_mine );
				stop_time();
				gr_palette_load(gr_palette);
	 
				if (nm_messagebox( NULL, 2, "Cancel Save", "Save", ErrorMessage )!=1)	{
					start_time();
					return 1;
				}
				start_time();
			} else
				mprintf((1, "Error: %i errors in this mine.  See the 'txm' file.\n", Errors_in_mine));
		}
//		change_filename_extension(temp_filename,filename,".LVL");
	}
//	else
	{
		if (Gamesave_current_version <= 3)
			change_filename_extension(temp_filename, filename, ".RDL");
		else
			change_filename_extension(temp_filename, filename, ".RL2");
	}

	SaveFile = PHYSFSX_openWriteBuffered(temp_filename);
	if (!SaveFile)
	{
		char ErrorMessage[256];

		char fname[20];
		_splitpath( temp_filename, NULL, NULL, fname, NULL );

		sprintf( ErrorMessage, \
			"ERROR: Cannot write to '%s'.\nYou probably need to check out a locked\nversion of the file. You should save\nthis under a different filename, and then\ncheck out a locked copy by typing\n\'co -l %s.lvl'\nat the DOS prompt.\n" 
			, temp_filename, fname );
		stop_time();
		gr_palette_load(gr_palette);
		nm_messagebox( NULL, 1, "Ok", ErrorMessage );
		start_time();
		return 1;
	}

	if (Current_level_name[0] == 0)
		strcpy(Current_level_name,"Untitled");

	clear_transient_objects(1);		//1 means clear proximity bombs

	compress_objects();		//after this, Highest_object_index == num objects

	//make sure player is in a segment
	if (update_object_seg(&Objects[Players[0].objnum]) == 0) {
		if (ConsoleObject->segnum > Highest_segment_index)
			ConsoleObject->segnum = 0;
		compute_segment_center(&ConsoleObject->pos,&(Segments[ConsoleObject->segnum]));
	}
 
	fix_object_segs();

	//Write the header

	PHYSFS_writeSLE32(SaveFile, MAKE_SIG('P','L','V','L'));
	PHYSFS_writeSLE32(SaveFile, Gamesave_current_version);

	//save placeholders
	PHYSFS_writeSLE32(SaveFile, minedata_offset);
	PHYSFS_writeSLE32(SaveFile, gamedata_offset);

	//Now write the damn data

	if (Gamesave_current_version >= 8)
	{
		//write the version 8 data (to make file unreadable by 1.0 & 1.1)
		PHYSFS_writeSLE32(SaveFile, GameTime);
		PHYSFS_writeSLE16(SaveFile, FrameCount);
		PHYSFSX_writeU8(SaveFile, FrameTime);
	}

	if (Gamesave_current_version < 5)
		PHYSFS_writeSLE32(SaveFile, -1);       //was hostagetext_offset

	// Write the palette file name
	if (Gamesave_current_version > 1)
		PHYSFSX_printf(SaveFile, "%s\n", Current_level_palette);

	if (Gamesave_current_version >= 3)
		PHYSFS_writeSLE32(SaveFile, Base_control_center_explosion_time);
	if (Gamesave_current_version >= 4)
		PHYSFS_writeSLE32(SaveFile, Reactor_strength);

	if (Gamesave_current_version >= 7)
	{
		int i;

		PHYSFS_writeSLE32(SaveFile, Num_flickering_lights);
		for (i = 0; i < Num_flickering_lights; i++)
			flickering_light_write(&Flickering_lights[i], SaveFile);
	}

	if (Gamesave_current_version >= 6)
	{
		PHYSFS_writeSLE32(SaveFile, Secret_return_segment);
		PHYSFSX_writeVector(SaveFile, &Secret_return_orient.rvec);
		PHYSFSX_writeVector(SaveFile, &Secret_return_orient.fvec);
		PHYSFSX_writeVector(SaveFile, &Secret_return_orient.uvec);
	}

	minedata_offset = (int)PHYSFS_tell(SaveFile);
#if 0	// only save compiled mine data
	if ( !compiled_version )	
		save_mine_data(SaveFile);
	else
#endif
		save_mine_data_compiled(SaveFile);
	gamedata_offset = (int)PHYSFS_tell(SaveFile);
	save_game_data(SaveFile);

	PHYSFS_seek(SaveFile, sizeof(int) + sizeof(Gamesave_current_version));
	PHYSFS_writeSLE32(SaveFile, minedata_offset);
	PHYSFS_writeSLE32(SaveFile, gamedata_offset);

	if (Gamesave_current_version < 5)
		PHYSFS_writeSLE32(SaveFile, (PHYSFS_sint32)PHYSFS_fileLength(SaveFile));

	//==================== CLOSE THE FILE =============================
	PHYSFS_close(SaveFile);

//	if ( !compiled_version )
	{
		if (Function_mode == FMODE_EDITOR)
			editor_status("Saved mine %s, \"%s\"",filename,Current_level_name);
	}

	return 0;

}

#if 0 //dunno - 3rd party stuff?
extern void compress_uv_coordinates_all(void);
#endif

int save_level(char * filename)
{
	int r1;

	// Save normal version...
	//save_level_sub(filename, 0);	// just save compiled one

	// Save compiled version...
	r1 = save_level_sub(filename, 1);

	return r1;
}

#endif	//EDITOR

#ifndef NDEBUG
void dump_mine_info(void)
{
	int	segnum, sidenum;
	fix	min_u, max_u, min_v, max_v, min_l, max_l, max_sl;

	min_u = F1_0*1000;
	min_v = min_u;
	min_l = min_u;

	max_u = -min_u;
	max_v = max_u;
	max_l = max_u;

	max_sl = 0;

	for (segnum=0; segnum<=Highest_segment_index; segnum++) {
		for (sidenum=0; sidenum<MAX_SIDES_PER_SEGMENT; sidenum++) {
			int	vertnum;
			side	*sidep = &Segments[segnum].sides[sidenum];

			if (Segment2s[segnum].static_light > max_sl)
				max_sl = Segment2s[segnum].static_light;

			for (vertnum=0; vertnum<4; vertnum++) {
				if (sidep->uvls[vertnum].u < min_u)
					min_u = sidep->uvls[vertnum].u;
				else if (sidep->uvls[vertnum].u > max_u)
					max_u = sidep->uvls[vertnum].u;

				if (sidep->uvls[vertnum].v < min_v)
					min_v = sidep->uvls[vertnum].v;
				else if (sidep->uvls[vertnum].v > max_v)
					max_v = sidep->uvls[vertnum].v;

				if (sidep->uvls[vertnum].l < min_l)
					min_l = sidep->uvls[vertnum].l;
				else if (sidep->uvls[vertnum].l > max_l)
					max_l = sidep->uvls[vertnum].l;
			}

		}
	}

//	mprintf((0, "Smallest uvl = %7.3f %7.3f %7.3f.  Largest uvl = %7.3f %7.3f %7.3f\n", f2fl(min_u), f2fl(min_v), f2fl(min_l), f2fl(max_u), f2fl(max_v), f2fl(max_l)));
//	mprintf((0, "Static light maximum = %7.3f\n", f2fl(max_sl)));
//	mprintf((0, "Number of walls: %i\n", Num_walls));

}

#endif

#ifdef EDITOR

//read in every level in mission and save out compiled version 
void save_all_compiled_levels(void)
{
	do_load_save_levels(1);
}

//read in every level in mission
void load_all_levels(void)
{
	do_load_save_levels(0);
}


void do_load_save_levels(int save)
{
	int level_num;

	if (! SafetyCheck())
		return;

	no_old_level_file_error=1;

	for (level_num=1;level_num<=Last_level;level_num++) {
		load_level(Level_names[level_num-1]);
		load_palette(Current_level_palette,1,1);		//don't change screen
		if (save)
			save_level_sub(Level_names[level_num-1],1);
	}

	for (level_num=-1;level_num>=Last_secret_level;level_num--) {
		load_level(Secret_level_names[-level_num-1]);
		load_palette(Current_level_palette,1,1);		//don't change screen
		if (save)
			save_level_sub(Secret_level_names[-level_num-1],1);
	}

	no_old_level_file_error=0;

}

#endif
