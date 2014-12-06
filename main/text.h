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
COPYRIGHT 1993-1998 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/
/*
 * $Source: Smoke:miner:source:main::RCS:text.h $
 * $Revision: 1.3 $
 * $Author: allender $
 * $Date: 1995/10/18 23:30:29 $
 * 
 * Header for text.c, including symbolics for text messages
 * 
 * $Log: text.h $
 * Revision 1.3  1995/10/18  23:30:29  allender
 * fixed horrendous misspelling
 *
 * Revision 1.2  1995/10/18  23:27:33  allender
 * added new printscreen line to end of text string
 *
 * Revision 1.1  1995/05/16  16:04:11  allender
 * Initial revision
 *
 * Revision 2.2  1995/03/30  16:36:22  mike
 * text localization.
 * 
 * Revision 2.1  1995/03/08  15:33:24  allender
 * *** empty log message ***
 * 
 * Revision 2.0  1995/02/27  11:30:21  john
 * New version 2.0, which has no anonymous unions, builds with
 * Watcom 10.0, and doesn't require parsing BITMAPS.TBL.
 * 
 * Revision 1.63  1995/02/11  14:59:42  rob
 * Added string for controlcen invul.
 * 
 * Revision 1.62  1995/02/10  15:19:38  allender
 * added text for bad write on demo write
 * 
 * Revision 1.61  1995/02/08  10:59:11  rob
 * Added string used in kill list view.
 * 
 * Revision 1.60  1995/02/07  20:38:40  rob
 * Added new localized strings for new multi options.
 * 
 * Revision 1.59  1995/02/02  15:29:17  matt
 * Changed & localized secret level text
 * 
 * Revision 1.58  1995/02/02  12:24:14  adam
 * added automap string
 * 
 * Revision 1.57  1995/02/01  18:06:50  rob
 * Localize default macros, other stuff frm network.c
 * 
 * Revision 1.56  1995/01/30  15:29:53  rob
 * Localizing.
 * 
 * Revision 1.55  1995/01/30  13:54:25  allender
 * added mission/demo text
 * 
 * Revision 1.54  1995/01/24  22:03:22  mike
 * text localization.
 * 
 * Revision 1.53  1995/01/23  13:05:59  adam
 * added save/load game text
 * 
 * Revision 1.52  1995/01/22  15:58:42  mike
 * localization
 * 
 * Revision 1.51  1994/12/29  12:08:47  rob
 * removed new text strings.
 * 
 * Revision 1.50  1994/12/28  16:47:14  rob
 * Added new com setup strings.
 * 
 * Revision 1.49  1994/12/15  23:24:22  rob
 * Added string for rejoin failure.
 * 
 * Revision 1.48  1994/12/15  10:16:06  adam
 * added lame trademark notice
 * 
 * Revision 1.47  1994/12/14  18:21:23  yuan
 * *** empty log message ***
 * 
 * Revision 1.46  1994/12/14  16:57:01  matt
 * Added text for hat2
 * 
 * Revision 1.45  1994/12/14  08:50:12  allender
 * *** empty log message ***
 * 
 * Revision 1.44  1994/12/13  19:43:29  rob
 * Added string.
 * 
 * Revision 1.43  1994/12/13  12:54:45  mike
 * add strings for hostages on board when you die.
 * 
 * Revision 1.42  1994/12/13  10:04:09  allender
 * *** empty log message ***
 * 
 * Revision 1.41  1994/12/12  22:20:23  rob
 * Added two new localized strings for modem.c
 * 
 * Revision 1.40  1994/12/09  19:46:26  yuan
 * Localized the sucker.
 * 
 * Revision 1.39  1994/12/09  14:35:30  yuan
 * Added localization string for Rob.
 * 
 * Revision 1.38  1994/12/09  14:08:16  mike
 * text message for remaining ships at end bonus.
 * 
 * Revision 1.37  1994/12/09  00:28:26  rob
 * Added "died in mine" string.
 * 
 * Revision 1.36  1994/12/08  14:30:31  yuan
 * Fixed off by one problem.
 * 
 * Revision 1.35  1994/12/08  11:18:56  allender
 * demo text string
 * 
 * Revision 1.33  1994/12/07  19:14:33  matt
 * Cleaned up command-line options and command-line help message
 * 
 * Revision 1.32  1994/12/07  17:20:34  yuan
 * Localization
 * 
 * Revision 1.31  1994/12/07  16:49:15  yuan
 * More localization
 * 
 * Revision 1.30  1994/12/07  14:52:11  yuan
 * Localization 492
 * 
 * Revision 1.29  1994/12/07  11:28:12  matt
 * Did a little localization support
 * 
 * Revision 1.28  1994/12/06  18:29:31  mike
 * Add text for cheat: Start at any level.
 * 
 * Revision 1.27  1994/12/06  16:30:39  yuan
 * Localization
 * 
 * Revision 1.26  1994/12/06  15:14:55  yuan
 * Fixed number of textstrings.
 * 
 * Revision 1.25  1994/12/06  15:13:52  yuan
 * Localization
 * 
 * Revision 1.24  1994/12/06  13:46:24  adam
 * *** empty log message ***
 * 
 * Revision 1.23  1994/12/05  23:17:28  yuan
 * Localization bugs fixed.
 * 
 * Revision 1.22  1994/12/05  23:05:41  yuan
 * Localization 425
 * 
 * Revision 1.21  1994/12/05  22:42:27  rob
 * Added new connect status strings
 * 
 * Revision 1.20  1994/12/05  16:47:30  rob
 * Added a new string.
 * 
 * 
 * Revision 1.19  1994/12/03  22:35:17  yuan
 * Localization 412
 * 
 * Revision 1.18  1994/12/03  17:53:42  yuan
 * 382.
 * 
 * 
 */



#ifndef _TEXT_H
#define _TEXT_H

//Array of pointers to text
extern char *Text_string[];

//Symbolic constants for all the strings

#define TXT_NEW_GAME				Text_string[ 0]
#define TXT_VIEW_SCORES			Text_string[ 1]
#define TXT_QUIT					Text_string[ 2]
#define TXT_CANNOT_SET_SCREEN	Text_string[ 3]
#define TXT_NO_JOYSTICK			Text_string[ 4]
#define TXT_REQUIRES_VGA		Text_string[ 5]
#define TXT_HELP					Text_string[ 6]
#define TXT_THANKS				Text_string[ 7]
#define TXT_SOUND_DISABLED		Text_string[ 8]
#define TXT_CANT_INIT_GFX		Text_string[ 9]
#define TXT_EXTRA_LIFE			Text_string[10]
#define TXT_COPYRIGHT			Text_string[11]
#define TXT_BLUE					Text_string[12]
#define TXT_RED					Text_string[13]
#define TXT_YELLOW				Text_string[14]
#define TXT_ACCESS_DENIED		Text_string[15]
#define TXT_ACCESS_GRANTED		Text_string[16]
#define TXT_BOOSTED_TO 			Text_string[17]
#define TXT_ENERGY 				Text_string[18]
#define TXT_SHIELD 				Text_string[19]
#define TXT_LASER 				Text_string[20]
#define TXT_MAXED_OUT			Text_string[21]
#define TXT_QUAD_LASERS			Text_string[22]
#define TXT_ALREADY_HAVE		Text_string[23]
#define TXT_VULCAN_AMMO			Text_string[24]
#define TXT_VULCAN_ROUNDS		Text_string[25]
#define TXT_ALREADY_ARE			Text_string[26]
#define TXT_CLOAKED				Text_string[27]
#define TXT_CLOAKING_DEVICE	Text_string[28]
#define TXT_INVULNERABLE		Text_string[29]
#define TXT_INVULNERABILITY	Text_string[30]
#define TXT_CREATE_NEW			Text_string[31]
#define TXT_YES					Text_string[32]
#define TXT_NO						Text_string[33]
#define TXT_OK						Text_string[34]
#define TXT_NO_DEMO_FILES		Text_string[35]
#define TXT_USE_F5				Text_string[36]
#define TXT_TO_CREATE_ONE		Text_string[37]
#define TXT_NO_FILES_MATCHING Text_string[38]
#define TXT_WERE_FOUND			Text_string[39]
#define TXT_DELETE_PILOT		Text_string[40]
#define TXT_DELETE_DEMO			Text_string[41]
#define TXT_COULDNT				Text_string[42]
#define TXT_EXIT_SEQUENCE		Text_string[43]
#define TXT_WARNING				Text_string[44]
#define TXT_UNABLE_TO_OPEN		Text_string[45]
#define TXT_COOL_SAYING			Text_string[46]
#define TXT_HIGH_SCORE			Text_string[47]
#define TXT_YOU_PLACED			Text_string[48]
#define TXT_YOU_PLACED_1ST		Text_string[49]
#define TXT_HIGH_SCORES			Text_string[50]
#define TXT_NAME					Text_string[51]
#define TXT_SCORE					Text_string[52]
#define TXT_SKILL					Text_string[53]
#define TXT_LEVELS				Text_string[54]
#define TXT_TIME					Text_string[55]
#define TXT_REGISTER_DESCENT	Text_string[56]
#define TXT_1ST					Text_string[57]
#define TXT_2ND					Text_string[58]
#define TXT_3RD					Text_string[59]
#define TXT_4TH					Text_string[60]
#define TXT_5TH					Text_string[61]
#define TXT_6TH					Text_string[62]
#define TXT_7TH					Text_string[63]
#define TXT_8TH					Text_string[64]
#define TXT_9TH					Text_string[65]
#define TXT_10TH					Text_string[66]
#define TXT_EMPTY					Text_string[67]
#define TXT_KILLED				Text_string[68]
#define TXT_SUICIDE				Text_string[69]
#define TXT_YOU					Text_string[70]
#define TXT_YOURSELF				Text_string[71]
#define TXT_NO_MACRO				Text_string[72]
#define TXT_SENDING				Text_string[73]
#define TXT_SEND_MESSAGE		Text_string[74]
#define TXT_SAYS					Text_string[75]
#define TXT_TELLS_YOU			Text_string[76]
#define TXT_HAS_DEST_CONTROL	Text_string[77]
#define TXT_CONTROL_DESTROYED Text_string[78]
#define TXT_HAS_ESCAPED			Text_string[79]
#define TXT_HAS_FOUND_SECRET	Text_string[80]
#define TXT_HAS_LEFT_THE_GAME	Text_string[81]
#define TXT_YOU_ARE_ONLY		Text_string[82]
#define TXT_OPPONENT_LEFT 		Text_string[83]
#define TXT_YOU_DEST_CONTROL	Text_string[84]
#define TXT_DEFINE_MACRO		Text_string[85]
#define TXT_MESSAGE_SENT_TO	Text_string[86]
#define TXT_NOBODY				Text_string[87]
#define TXT_PAUSE					Text_string[88]
#define TXT_CANT_PAUSE			Text_string[89]
#define TXT_HELP_ESC				Text_string[90]
#define TXT_HELP_F2				Text_string[91]
#define TXT_HELP_F3				Text_string[92]
#define TXT_HELP_F4				Text_string[93]
#define TXT_HELP_F5				Text_string[94]
#define TXT_HELP_PAUSE			Text_string[95]
#define TXT_HELP_MINUSPLUS		Text_string[96]
#define TXT_HELP_PRTSCN			Text_string[97]
#define TXT_HELP_1TO5			Text_string[98]
#define TXT_HELP_6TO10			Text_string[99]
#define TXT_HELP_TO_VIEW		Text_string[100]
#define TXT_KEYS					Text_string[101]
#define TXT_ABORT_AUTODEMO		Text_string[102]
#define TXT_ABORT_GAME			Text_string[103]
#define TXT_W_LASER				Text_string[104]
#define TXT_W_VULCAN				Text_string[105]
#define TXT_W_SPREADFIRE		Text_string[106]
#define TXT_W_PLASMA				Text_string[107]
#define TXT_W_FUSION				Text_string[108]
#define TXT_W_C_MISSILE			Text_string[109]
#define TXT_W_H_MISSILE			Text_string[110]
#define TXT_W_P_BOMB				Text_string[111]
#define TXT_W_S_MISSILE			Text_string[112]
#define TXT_W_M_MISSILE			Text_string[113]
#define TXT_W_LASER_S			Text_string[114]
#define TXT_W_VULCAN_S			Text_string[115]
#define TXT_W_SPREADFIRE_S		Text_string[116]
#define TXT_W_PLASMA_S			Text_string[117]
#define TXT_W_FUSION_S			Text_string[118]
#define TXT_W_C_MISSILE_S		Text_string[119]
#define TXT_W_H_MISSILE_S		Text_string[120]
#define TXT_W_P_BOMB_S			Text_string[121]
#define TXT_W_S_MISSILE_S		Text_string[122]
#define TXT_W_M_MISSILE_S		Text_string[123]
#define TXT_SELECTED				Text_string[124]
#define TXT_DONT_HAVE			Text_string[125]
#define TXT_DONT_HAVE_AMMO		Text_string[126]
#define TXT_HAVE_NO				Text_string[127]
#define TXT_S						Text_string[128]
#define TXT_SX						Text_string[129]
#define TXT_NO_PRIMARY			Text_string[130]
#define TXT_ALREADY_HAVE_THE	Text_string[131]
#define TXT_CANT_OPEN_DOOR		Text_string[132]
#define TXT_MOVE_JOYSTICK		Text_string[133]
#define TXT_TO_UL					Text_string[134]
#define TXT_ANY_BUTTON			Text_string[135]
#define TXT_JOYSTICK				Text_string[136]
#define TXT_UPPER_LEFT			Text_string[137]
#define TXT_TO_LR					Text_string[138]
#define TXT_LOWER_RIGHT			Text_string[139]
#define TXT_TO_C					Text_string[140]
#define TXT_CENTER				Text_string[141]
#define TXT_CONTROL_KEYBOARD	Text_string[142]
#define TXT_CONTROL_JOYSTICK	Text_string[143]
#define TXT_CONTROL_FSTICKPRO	Text_string[144]
#define TXT_CONTROL_THRUSTFCS	Text_string[145]
#define TXT_CONTROL_GGAMEPAD 	Text_string[146]
#define TXT_CONTROL_MOUSE	 	Text_string[147]
#define TXT_CONTROL_CYBERMAN	Text_string[148]
#define TXT_CUST_ABOVE			Text_string[149]
#define TXT_CUST_KEYBOARD		Text_string[150]
#define TXT_CONTROLS				Text_string[151]
#define TXT_KEYBOARD				Text_string[152]
#define TXT_COMMAND_LINE_0		Text_string[153]
#define TXT_COMMAND_LINE_1		Text_string[154]
#define TXT_COMMAND_LINE_2		Text_string[155]
#define TXT_COMMAND_LINE_3		Text_string[156]
#define TXT_COMMAND_LINE_4		Text_string[157]
#define TXT_COMMAND_LINE_5		Text_string[158]
#define TXT_COMMAND_LINE_6		Text_string[159]
#define TXT_COMMAND_LINE_7		Text_string[160]
#define TXT_COMMAND_LINE_8		Text_string[161]
#define TXT_COMMAND_LINE_9		Text_string[162]
#define TXT_COMMAND_LINE_10	Text_string[163]
#define TXT_COMMAND_LINE_11	Text_string[164]
#define TXT_COMMAND_LINE_12	Text_string[165]
#define TXT_COMMAND_LINE_13	Text_string[166]
#define TXT_COMMAND_LINE_14	Text_string[167]
#define TXT_COMMAND_LINE_15	Text_string[168]
#define TXT_COMMAND_LINE_16	Text_string[169]
#define TXT_COMMAND_LINE_17	Text_string[170]
#define TXT_COMMAND_LINE_18	Text_string[171]
#define TXT_REGISTRATION		Text_string[172]
#define TXT_ERROR_SELECTOR		Text_string[173]
#define TXT_ERROR_GRAPHICS		Text_string[174]
#define TXT_CALIBRATE			Text_string[175]
#define TXT_SKIP					Text_string[176]
#define TXT_JOYSTICK_NOT_CEN	Text_string[177]
#define TXT_CHOOSE_INPUT		Text_string[178]
#define TXT_ERROR					Text_string[179]
#define TXT_ERROR_PLR_VERSION Text_string[180]
#define TXT_DEMO_PLAYBACK		Text_string[181]
#define TXT_DEMO_RECORDING		Text_string[182]
#define TXT_CRUISE				Text_string[183]
#define TXT_DUMPING_SCREEN		Text_string[184]
#define TXT_CHEATS_ENABLED		Text_string[185]
#define TXT_NET_GAME_CLOSED	Text_string[186]
#define TXT_NET_GAME_FULL		Text_string[187]
#define TXT_NET_GAME_BETWEEN	Text_string[188]
#define TXT_NET_GAME_NSELECT	Text_string[189]
#define TXT_NET_GAME_NSTART	Text_string[190]
#define TXT_NET_GAME_CONNECT	Text_string[191]
#define TXT_NET_GAME_WRONGLEV Text_string[192]
#define TXT_KILLS					Text_string[193]
#define TXT_WAITING				Text_string[194]
#define TXT_SURE_LEAVE_GAME	Text_string[195]
#define TXT_JOINING				Text_string[196]
#define TXT_REJOIN				Text_string[197]
#define TXT_SORRY_ONLY			Text_string[198]
#define TXT_NETPLAYERS_IN		Text_string[199]
#define TXT_S_GAME				Text_string[200]
#define TXT_DESCRIPTION			Text_string[201]
#define TXT_LEVEL_				Text_string[202]
#define TXT_MODE					Text_string[203]
#define TXT_ANARCHY				Text_string[204]
#define TXT_TEAM_ANARCHY		Text_string[205]
#define TXT_ANARCHY_W_ROBOTS	Text_string[206]
#define TXT_COOPERATIVE			Text_string[207]
#define TXT_OPTIONS				Text_string[208]
#define TXT_CLOSED_GAME			Text_string[209]
#define TXT_NETGAME_SETUP		Text_string[210]
#define TXT_LEVEL_OUT_RANGE	Text_string[211]
#define TXT_SORRY					Text_string[212]
#define TXT_REGISTERED_ONLY	Text_string[213]
#define TXT_WAIT					Text_string[214]
#define TXT_FOUND					Text_string[215]
#define TXT_ACTIVE_GAMES		Text_string[216]
#define TXT_STARTING_NETGAME	Text_string[217]
#define TXT_NETLEVEL_NMATCH	Text_string[218]
#define TXT_TEAM					Text_string[219]
#define TXT_TEAM_SELECTION		Text_string[220]
#define TXT_TEAM_MUST_ONE		Text_string[221]
#define TXT_TEAM_SELECT			Text_string[222]
#define TXT_TEAM_PRESS_ENTER	Text_string[223]
#define TXT_TEAM_ATLEAST_TWO	Text_string[224]
#define TXT_IPX_NOT_FOUND		Text_string[225]
#define TXT_NET_FORMING			Text_string[226]
#define TXT_NET_PLAYERS			Text_string[227]
#define TXT_NET_JOIN				Text_string[228]
#define TXT_NET_CLOSED			Text_string[229]
#define TXT_NET_BETWEEN			Text_string[230]
#define TXT_NET_LEAVE			Text_string[231]
#define TXT_NET_WAITING			Text_string[232]
#define TXT_NET_TO_ENTER		Text_string[233]
#define TXT_NET_SEARCHING		Text_string[234]
#define TXT_INVALID_CHOICE		Text_string[235]
#define TXT_NET_GAME_BETWEEN2	Text_string[236]
#define TXT_VERSION_MISMATCH	Text_string[237]
#define TXT_GAME_FULL			Text_string[238]
#define TXT_IN_PROGRESS			Text_string[239]
#define TXT_DISCONNECTING		Text_string[240]
#define TXT_SERIAL_OPEN_ERROR Text_string[241]
#define TXT_CARRIER_LOST		Text_string[242]
#define TXT_ERROR_SERIAL_CFG	Text_string[243]
#define TXT_ERR_SER_SETTINGS	Text_string[244]
#define TXT_CONNECT_LOST		Text_string[245]
#define TXT_READY_DESCENT		Text_string[246]
#define TXT_CLOSED_LINK			Text_string[247]
#define TXT_DIAL_MODEM			Text_string[248]
#define TXT_ANSWER_MODEM		Text_string[249]
#define TXT_NULL_MODEM			Text_string[250]
#define TXT_COM_SETTINGS		Text_string[251]
#define TXT_START_GAME			Text_string[252]
#define TXT_SEND_MESSAGEP		Text_string[253]
#define TXT_HANGUP_MODEM		Text_string[254]
#define TXT_CLOSE_LINK			Text_string[255]
#define TXT_SERIAL				Text_string[256]
#define TXT_LINK_ACTIVE			Text_string[257]
#define TXT_MODEM					Text_string[258]
#define TXT_NOT_CONNECTED		Text_string[259]
#define TXT_SERIAL_GAME			Text_string[260]
#define TXT_EXIT_WILL_CLOSE	Text_string[261]
#define TXT_BAUD_RATE			Text_string[262]
#define TXT_MODEM_INIT_STRING	Text_string[263]
#define TXT_ACCEPT_SAVE			Text_string[264]
#define TXT_SERIAL_SETTINGS	Text_string[265]
#define TXT_WARNING_16550		Text_string[266]
#define TXT_DIFFICULTY			Text_string[267]
#define TXT_SERIAL_GAME_SETUP	Text_string[268]
#define TXT_ONLY_ANARCHY		Text_string[269]
#define TXT_SAVE					Text_string[270]
#define TXT_ACCEPT				Text_string[271]
#define TXT_SEL_NUMBER_EDIT	Text_string[272]
#define TXT_EDIT_PHONE_ENTRY	Text_string[273]
#define TXT_MANUAL_ENTRY		Text_string[274]
#define TXT_EDIT_PHONEBOOK		Text_string[275]
#define TXT_SEL_NUMBER_DIAL	Text_string[276]
#define TXT_ENTER_NUMBER_DIAL	Text_string[277]
#define TXT_NO_DIAL_TONE		Text_string[278]
#define TXT_BUSY					Text_string[279]
#define TXT_NO_ANSWER			Text_string[280]
#define TXT_NO_CARRIER			Text_string[281]
#define TXT_VOICE					Text_string[282]
#define TXT_ERR_MODEM_RETURN	Text_string[283]
#define TXT_CONNECT				Text_string[284]
#define TXT_BAUD_GREATER_9600 Text_string[285]
#define TXT_RING					Text_string[286]
#define TXT_NO_SERIAL_OPT		Text_string[287]
#define TXT_RESET_MODEM			Text_string[288]
#define TXT_NO_MODEM				Text_string[289]
#define TXT_NO_PHONENUM			Text_string[290]
#define TXT_DIALING				Text_string[291]
#define TXT_ESC_ABORT			Text_string[292]
#define TXT_WAITING_FOR_ANS	Text_string[293]
#define TXT_WAITING_FOR_CALL	Text_string[294]
#define TXT_WAITING_FOR_CARR	Text_string[295]
#define TXT_FAILED_TO_NEGOT	Text_string[296]
#define TXT_NEGOTIATION_FAIL	Text_string[297]
#define TXT_FATAL_ERROR_LEVEL	Text_string[298]
#define TXT_OTHER_LEVEL			Text_string[299]
#define TXT_YOUR_LEVEL			Text_string[300]
#define TXT_LVL_NO_MATCH		Text_string[301]
#define TXT_CHECK_VERSION		Text_string[302]
#define TXT_DESCENT_NO_MATCH	Text_string[303]
#define TXT_OPPONENT_NO_READY	Text_string[304]
#define TXT_WAIT_OPPONENT		Text_string[305]
#define TXT_LOCK					Text_string[306]
#define TXT_DEATHS				Text_string[307]
#define TXT_LIVES					Text_string[308]
#define TXT_LVL					Text_string[309]
#define TXT_QUAD 					Text_string[310]
#define TXT_REAR_VIEW			Text_string[311]
#define TXT_DIFFICULTY_1		Text_string[312]
#define TXT_DIFFICULTY_2		Text_string[313]
#define TXT_DIFFICULTY_3		Text_string[314]
#define TXT_DIFFICULTY_4		Text_string[315]
#define TXT_DIFFICULTY_5		Text_string[316]
#define TXT_DETAIL_1				Text_string[317]
#define TXT_DETAIL_2				Text_string[318]
#define TXT_DETAIL_3				Text_string[319]
#define TXT_DETAIL_4				Text_string[320]
#define TXT_DETAIL_5				Text_string[321]
#define TXT_DETAIL_CUSTOM_		Text_string[322]
#define TXT_LOAD_GAME  			Text_string[323]
#define TXT_MULTIPLAYER_		Text_string[324]
#define TXT_OPTIONS_				Text_string[325]
#define TXT_CHANGE_PILOTS		Text_string[326]
#define TXT_VIEW_DEMO			Text_string[327]
#define TXT_CREDITS				Text_string[328]
#define TXT_ORDERING_INFO		Text_string[329]
#define TXT_SELECT_DEMO			Text_string[330]
#define TXT_DIFFICULTY_LEVEL	Text_string[331]
#define TXT_SET_TO				Text_string[332]
#define TXT_DETAIL_LEVEL		Text_string[333]
#define TXT_OBJ_COMPLEXITY		Text_string[334]
#define TXT_OBJ_DETAIL			Text_string[335]
#define TXT_WALL_DETAIL			Text_string[336]
#define TXT_WALL_RENDER_DEPTH	Text_string[337]
#define TXT_DEBRIS_AMOUNT		Text_string[338]
#define TXT_SOUND_CHANNELS		Text_string[339]
#define TXT_LO_HI					Text_string[340]
#define TXT_DETAIL_CUSTOM		Text_string[341]
#define TXT_START_ANY_LEVEL	Text_string[342]
#define TXT_SELECT_START_LEV	Text_string[343]
#define TXT_ENTER_TO_CONT		Text_string[344]
#define TXT_INVALID_LEVEL		Text_string[345]
#define TXT_ERR_LOADING_GAME	Text_string[346]
#define TXT_SAVE_GAME_SLOTS	Text_string[347]
#define TXT_SAVE_ERROR			Text_string[348]
#define TXT_FX_VOLUME			Text_string[349]
#define TXT_MUSIC_VOLUME		Text_string[350]
#define TXT_REVERSE_STEREO		Text_string[351]
#define TXT_BRIGHTNESS			Text_string[352]
#define TXT_CONTROLS_			Text_string[353]
#define TXT_DETAIL_LEVELS		Text_string[354]
#define TXT_CAL_JOYSTICK		Text_string[355]
#define TXT_JOYS_SENSITIVITY	Text_string[356]
#define TXT_START_NET_GAME		Text_string[357]
#define TXT_JOIN_NET_GAME		Text_string[358]
#define TXT_MODEM_GAME			Text_string[359]
#define TXT_MULTIPLAYER			Text_string[360]
#define TXT_CONTINUE				Text_string[361]
#define TXT_CANT_PLAYBACK		Text_string[362]
#define TXT_DEMO_CORRUPT		Text_string[363]
#define TXT_DEMO_OLD				Text_string[364]
#define TXT_RECORDED				Text_string[365]
#define TXT_WITH_REGISTERED	Text_string[366]
#define TXT_WITH_SHAREWARE		Text_string[367]
#define TXT_OF_DESCENT			Text_string[368]
#define TXT_LEVEL_CANT_LOAD	Text_string[369]
#define TXT_DEMO_OLD_CORRUPT	Text_string[370]
#define TXT_DEMO_ERR_READING	Text_string[371]
#define TXT_SAVE_DEMO_AS		Text_string[372]
#define TXT_DEMO_USE_LETTERS	Text_string[373]
#define TXT_AUTOMAP				Text_string[374]
#define TXT_TURN_SHIP			Text_string[375]
#define TXT_SLIDE_UPDOWN 		Text_string[376]
#define TXT_LEVEL					Text_string[377]
#define TXT_PITCH_FORWARD		Text_string[378]
#define TXT_PITCH_BACKWARD		Text_string[379]
#define TXT_TURN_LEFT			Text_string[380]
#define TXT_TURN_RIGHT			Text_string[381]
#define TXT_SLIDE_ON				Text_string[382]
#define TXT_SLIDE_LEFT			Text_string[383]
#define TXT_SLIDE_RIGHT			Text_string[384]
#define TXT_SLIDE_UP				Text_string[385]
#define TXT_SLIDE_DOWN			Text_string[386]
#define TXT_BANK_ON				Text_string[387]
#define TXT_BANK_LEFT			Text_string[388]
#define TXT_BANK_RIGHT			Text_string[389]
#define TXT_FIRE_PRIMARY		Text_string[390]
#define TXT_FIRE_SECONDARY		Text_string[391]
#define TXT_FIRE_FLARE			Text_string[392]
#define TXT_ACCELERATE			Text_string[393]
#define TXT_REVERSE				Text_string[394]
#define TXT_DROP_BOMB			Text_string[395]
#define TXT_CRUISE_FASTER		Text_string[396]
#define TXT_CRUISE_SLOWER		Text_string[397]
#define TXT_CRUISE_OFF			Text_string[398]
#define TXT_PITCH_UD				Text_string[399]
#define TXT_TURN_LR				Text_string[400]
#define TXT_SLIDE_LR				Text_string[401]
#define TXT_SLIDE_UD				Text_string[402]
#define TXT_BANK_LR				Text_string[403]
#define TXT_THROTTLE				Text_string[404]
#define TXT_TEAM_ATLEAST_THREE Text_string[405]
#define TXT_NET_DISCONNECTED	Text_string[406]
#define TXT_NET_PLAYING			Text_string[[40]
#define TXT_NET_ESCAPED			Text_string[408]
#define TXT_NET_DIED				Text_string[409]
#define TXT_NET_FOUND_SECRET	Text_string[410]
#define TXT_NET_ESCAPE_TUNNEL	Text_string[411]
#define TXT_NET_RESERVED		Text_string[412]
#define TXT_WOWIE_ZOWIE			Text_string[413]
#define TXT_ALL_KEYS				Text_string[414]
#define TXT_CLOAK					Text_string[415]
#define TXT_FULL_SHIELDS		Text_string[416]
#define TXT_ON						Text_string[417]
#define TXT_OFF					Text_string[418]
#define TXT_NOT_IN_SHAREWARE	Text_string[419]
#define TXT_GAME_OVER			Text_string[420]
#define TXT_SELECT_PILOT		Text_string[421]
#define TXT_ENTER_PILOT_NAME	Text_string[422]
#define TXT_PLAYER				Text_string[423]
#define TXT_ALREADY_EXISTS		Text_string[424]
#define TXT_LOADING				Text_string[425]
#define TXT_FULL_RESCUE_BONUS	Text_string[426]
#define TXT_SHIELD_BONUS		Text_string[427]
#define TXT_ENERGY_BONUS		Text_string[428]
#define TXT_HOSTAGE_BONUS		Text_string[429]
#define TXT_SKILL_BONUS			Text_string[430]
#define TXT_TOTAL_BONUS			Text_string[431]
#define TXT_TOTAL_SCORE			Text_string[432]
#define TXT_SECRET_LEVEL		Text_string[433]
#define TXT_COMPLETE				Text_string[434]
#define TXT_DESTROYED			Text_string[435]
#define TXT_SAVE_GAME			Text_string[436]
#define TXT_PRESS_CTRL_R		Text_string[437]
#define TXT_RESET_HIGH_SCORES	Text_string[438]
#define TXT_YOU_WERE				Text_string[439]
#define TXT_WAS					Text_string[440]
#define TXT_KILLED_BY_NONPLAY	Text_string[441]
#define TXT_IMPORTANT_NOTE		Text_string[442]
#define TXT_FCS					Text_string[443]
#define TXT_PRESS_ANY_KEY		Text_string[444]
#define TXT_HOSTAGE_RESCUED	Text_string[445]
#define TXT_INIT_VICTOR			Text_string[446]
#define TXT_N						Text_string[447]
#define TXT_Y						Text_string[448]
#define TXT_ANY_LEVEL			Text_string[449]
#define TXT_SHAREWARE_DONE		Text_string[450]
#define TXT_PRESS_NEW_KEY		Text_string[451]
#define TXT_PRESS_NEW_JBUTTON	Text_string[452]
#define TXT_PRESS_NEW_MBUTTON	Text_string[453]
#define TXT_MOVE_NEW_JOY_AXIS	Text_string[454]
#define TXT_MOVE_NEW_MSE_AXIS	Text_string[455]
#define TXT_USING_VFX1			Text_string[456]
#define TXT_VFX1_ERROR1			Text_string[457]
#define TXT_VFX1_ERROR2			Text_string[458]
#define TXT_KCONFIG_STRING_1	Text_string[459]
#define TXT_BUTTONS				Text_string[460]
#define TXT_AXES					Text_string[461]
#define TXT_AXIS					Text_string[462]
#define TXT_INVERT				Text_string[463]
#define TXT_BTN_1					Text_string[464]
#define TXT_BTN_2					Text_string[465]
#define TXT_BTN_3					Text_string[466]
#define TXT_BTN_4					Text_string[467]
#define TXT_TRIG					Text_string[468]
#define TXT_HAT_L					Text_string[469]
#define TXT_HAT_R					Text_string[470]
#define TXT_HAT_U					Text_string[471]
#define TXT_HAT_D					Text_string[472]
#define TXT_LEFT					Text_string[473]
#define TXT_RIGHT					Text_string[474]
#define TXT_MID					Text_string[475]
#define TXT_UP						Text_string[476]
#define TXT_DOWN					Text_string[477]
#define TXT_X1						Text_string[478]
#define TXT_Y1						Text_string[479]
#define TXT_X2						Text_string[480]
#define TXT_Y2						Text_string[481]
#define TXT_L_R					Text_string[482]
#define TXT_F_B					Text_string[483]
#define TXT_FORWARD				Text_string[484]
#define TXT_MOVE_THROTTLE_F	Text_string[485]
#define TXT_MOVE_THROTTLE_R	Text_string[486]
#define TXT_MOVE_THROTTLE_C	Text_string[487]
#define TXT_REACTOR_EXPLODED	Text_string[488]
#define TXT_TIME_REMAINING		Text_string[489]
#define TXT_SECONDS				Text_string[490]
#define TXT_DEMO_WRITE_ERROR	Text_string[491]
#define TXT_DEMO_SIZE			Text_string[492]
#define TXT_DEMO_SAVE_BAD		Text_string[493]
#define TXT_BYTE_STR				Text_string[494]
#define TXT_DIED_IN_MINE		Text_string[495]
#define TXT_SHIP_BONUS			Text_string[496]
#define TXT_PHONE_NUM			Text_string[497]
#define TXT_KILL_MATRIX_TITLE	Text_string[498]
#define TXT_WAIT_FOR_OK			Text_string[499]
#define TXT_SURE_ABORT_SYNC	Text_string[500]
#define TXT_ERROR_WRITING_PLR	Text_string[501]
#define TXT_SHIP_DESTROYED_0	Text_string[502]
#define TXT_SHIP_DESTROYED_1	Text_string[503]
#define TXT_SHIP_DESTROYED_2	Text_string[504]
#define TXT_NET_FULL				Text_string[505]
#define TXT_DEMO_NO_SPACE		Text_string[506]
#define TXT_HAT2_L				Text_string[507]
#define TXT_HAT2_R				Text_string[508]
#define TXT_HAT2_U				Text_string[509]
#define TXT_HAT2_D				Text_string[510]
#define TXT_WARP_TO_LEVEL		Text_string[511]
#define TXT_TRADEMARK			Text_string[512]
#define TXT_NET_SYNC_FAILED	Text_string[513]
#define TXT_DONE					Text_string[514]
#define TXT_I_AM_A				Text_string[515]
#define TXT_CHEATER				Text_string[516]
#define TXT_LOADING_DATA		Text_string[517]
#define TXT_HELP_ALT_F2			Text_string[518]
#define TXT_HELP_ALT_F3			Text_string[519]
#define TXT_ONLY_REGISTERED	Text_string[520]
#define TXT_CONCUSSION			Text_string[521]
#define TXT_HOMING				Text_string[522]
#define TXT_PROXBOMB				Text_string[523]
#define TXT_SMART					Text_string[524]
#define TXT_MEGA					Text_string[525]
#define TXT_NOMISSION4DEMO		Text_string[526]
#define TXT_SHOW_IDS				Text_string[527]
#define TXT_DUPLICATE_NAME		Text_string[528]
#define TXT_ANARCHY_ONLY_MISSION		Text_string[529]
#define TXT_START_NOWAIT		Text_string[530]
#define TXT_QUITTING_NOW		Text_string[531]
#define TXT_MISSION_NOT_FOUND	Text_string[532]
#define TXT_MULTI_MISSION		Text_string[533]
#define TXT_MISSION_ERROR		Text_string[534]
#define TXT_COM_CUSTOM_SETTINGS	Text_string[535]
#define TXT_COM_BASE				Text_string[536]
#define TXT_COM_IRQ				Text_string[537]
#define TXT_RESET_DEFAULTS		Text_string[538]
#define TXT_VALID_IRQS			Text_string[539]
#define TXT_NO_UART				Text_string[540]
#define TXT_DEF_MACRO_1			Text_string[541]
#define TXT_DEF_MACRO_2			Text_string[542]
#define TXT_DEF_MACRO_3			Text_string[543]
#define TXT_DEF_MACRO_4			Text_string[544]
#define TXT_MISSION				Text_string[545]
#define TXT_VIEWING_DISTANCE	Text_string[546]
#define TXT_SECRET_EXIT			Text_string[547]
#define TXT_SHOW_ON_MAP			Text_string[548]
#define TXT_KILLED_BY_ROBOT	Text_string[549]
#define TXT_BAUD					Text_string[550]
#define TXT_CONSISTENCY_ERROR	Text_string[551]
#define TXT_PRESS_ANY_KEY2		Text_string[552]
#define TXT_DEMO_SAVE_NOSPACE	Text_string[553]
#define TXT_CNTRLCEN_INVUL		Text_string[554]
#define TXT_NO_DESTSAT_LVL		Text_string[555]
#define TXT_REACTOR_LIFE		Text_string[556]
#define TXT_MINUTES_ABBREV		Text_string[557]
#define TXT_CURRENT_IPX_SOCKET Text_string[558]
#define TXT_DOS_VERSION_1		Text_string[559]
#define TXT_DOS_VERSION_2		Text_string[560]
#define TXT_NOT_ENOUGH_HANDLES Text_string[561]
#define TXT_HANDLES_1			Text_string[562]
#define TXT_HANDLES_2			Text_string[563]
#define TXT_HANDLES_3			Text_string[564]
#define TXT_AVAILABLE_MEMORY	Text_string[565]
#define TXT_MEMORY_CONFIG 		Text_string[566]
#define TXT_RECONFIGURE_VMM	Text_string[567]
#define TXT_MORE_MEMORY			Text_string[568]
#define TXT_MORE_MEMORY_2		Text_string[569]
#define TXT_PHYSICAL_MEMORY	Text_string[570]
#define TXT_PHYSICAL_MEMORY_2	Text_string[571]
#define TXT_INITIALIZING_DPMI	Text_string[572]
#define TXT_INITIALIZING_CRIT	Text_string[573]
#define TXT_IGLASSES				Text_string[574]
#define TXT_VIOTRACK				Text_string[575]
#define TXT_KASAN					Text_string[576]
#define TXT_KASAN_2				Text_string[577]
#define TXT_3DMAX					Text_string[578]
#define TXT_PRESS_ANY_KEY3		Text_string[579]
#define TXT_SOCKET				Text_string[580]
#define TXT_NOFILECHECK			Text_string[581]
#define TXT_VERBOSE_1			Text_string[582]
#define TXT_VERBOSE_2			Text_string[583]
#define TXT_VERBOSE_3			Text_string[584]
#define TXT_VERBOSE_4			Text_string[585]
#define TXT_VERBOSE_5			Text_string[586]
#define TXT_VERBOSE_6			Text_string[587]
#define TXT_VERBOSE_7			Text_string[588]
#define TXT_VERBOSE_8			Text_string[589]
#define TXT_VERBOSE_9			Text_string[590]
#define TXT_VERBOSE_10			Text_string[591]
#define TXT_VERBOSE_11			Text_string[592]
#define TXT_INITIALIZING_NETWORK Text_string[593]
#define TXT_IPX_CHANNEL			Text_string[594]
#define TXT_NO_NETWORK			Text_string[595]
#define TXT_SOCKET_ERROR		Text_string[596]
#define TXT_MEMORY_IPX			Text_string[597]
#define TXT_ERROR_IPX			Text_string[598]
#define TXT_NETWORK_DISABLED	Text_string[599]
#define TXT_INITIALIZING_GRAPHICS Text_string[600]
#define TXT_SOUND_ERROR_OPEN	Text_string[601]
#define TXT_SOUND_ERROR_LOCK	Text_string[602]
#define TXT_SOUND_ERROR_HMI	Text_string[603]
#define TXT_SOUND_ERROR_LOCK_DRUMS Text_string[604]
#define TXT_SOUND_ERROR_MIDI	Text_string[605]
#define TXT_SOUND_ERROR_MIDI_CALLBACK Text_string[606]
#define TXT_EXTERNAL_CONTROL	Text_string[607]
#define TXT_IGLASSES_ERROR_1	Text_string[608]
#define TXT_IGLASSES_INIT		Text_string[609]
#define TXT_IGLASSES_ON			Text_string[610]
#define TXT_PRESS_ESC_TO_ABORT Text_string[611]
#define TXT_SERIAL_FAILURE		Text_string[612]
#define TXT_MESSAGE				Text_string[613]
#define TXT_MACRO					Text_string[614]
#define TXT_ERROR_SERIAL_LOCK	Text_string[615]
#define TXT_ERROR_SERIAL_LOCK_2	Text_string[616]
#define TXT_NO_LUNACY			Text_string[617]
#define TXT_LUNACY				Text_string[618]
#define TXT_ROBOT_PAINTING_OFF Text_string[619]
#define TXT_ROBOT_PAINTING_ON Text_string[620]
#define TXT_PRINTSCREEN1		Text_string[621]

#define N_TEXT_STRINGS			622

// Constants for keyboard configuration
#define TNUM_REAR_VIEW			(319-8)
#define TNUM_AUTOMAP				(382-8)
#define TNUM_PITCH_FORWARD		(386-8)
#define TNUM_PITCH_BACKWARD	(387-8)
#define TNUM_TURN_LEFT			(388-8)
#define TNUM_TURN_RIGHT			(389-8)
#define TNUM_SLIDE_ON			(390-8)
#define TNUM_SLIDE_LEFT			(391-8)
#define TNUM_SLIDE_RIGHT		(392-8)
#define TNUM_SLIDE_UP			(393-8)
#define TNUM_SLIDE_DOWN			(394-8)
#define TNUM_BANK_ON				(395-8)
#define TNUM_BANK_LEFT			(396-8)
#define TNUM_BANK_RIGHT			(397-8)
#define TNUM_FIRE_PRIMARY		(398-8)
#define TNUM_FIRE_SECONDARY	(399-8)
#define TNUM_FIRE_FLARE			(400-8)
#define TNUM_ACCELERATE			(401-8)
#define TNUM_REVERSE				(402-8)
#define TNUM_DROP_BOMB			(403-8)
#define TNUM_CRUISE_FASTER		(404-8)
#define TNUM_CRUISE_SLOWER		(405-8)
#define TNUM_CRUISE_OFF			(406-8)
#define TNUM_PITCH_UD			(407-8)
#define TNUM_TURN_LR				(408-8)
#define TNUM_SLIDE_LR			(409-8)
#define TNUM_SLIDE_UD			(410-8)
#define TNUM_BANK_LR				(411-8)
#define TNUM_THROTTLE			(412-8)

#define TNUM_N						(455-8)
#define TNUM_Y						(456-8)
#define TNUM_BTN_1				(472-8)
#define TNUM_BTN_2				(473-8)
#define TNUM_BTN_3				(474-8)
#define TNUM_BTN_4				(475-8)
#define TNUM_TRIG					(476-8)
#define TNUM_HAT_L				(477-8)
#define TNUM_HAT_R				(478-8)
#define TNUM_HAT_U				(479-8)
#define TNUM_HAT_D				(480-8)
#define TNUM_LEFT					(481-8)
#define TNUM_RIGHT				(482-8)
#define TNUM_MID					(483-8)
#define TNUM_UP					(484-8)
#define TNUM_DOWN					(485-8)
#define TNUM_X1					(486-8)
#define TNUM_Y1					(487-8)
#define TNUM_X2					(488-8)
#define TNUM_Y2					(489-8)
#define TNUM_L_R					(490-8)
#define TNUM_F_B					(491-8)
#define TNUM_HAT2_L				507
#define TNUM_HAT2_R				508
#define TNUM_HAT2_U				509
#define TNUM_HAT2_D				510

// Weapon names.
#define PRIMARY_WEAPON_NAMES(weapon_num) (*(&TXT_W_LASER + (weapon_num)))
#define SECONDARY_WEAPON_NAMES(weapon_num) (*(&TXT_W_C_MISSILE + (weapon_num)))
#define PRIMARY_WEAPON_NAMES_SHORT(weapon_num) (*(&TXT_W_LASER_S + (weapon_num)))
#define SECONDARY_WEAPON_NAMES_SHORT(weapon_num) (*(&TXT_W_C_MISSILE_S + (weapon_num)))

//	Control configuration strings.
#define CONTROL_TEXT(control_type) (*(&TXT_CONTROL_KEYBOARD + (control_type)))

// Network connect states
#define CONNECT_STATES(state_num) (*(&TXT_NET_DISCONNECTED + (state_num)))
// Network dump strings
#define NET_DUMP_STRINGS(dump_num) (*(&TXT_NET_GAME_CLOSED + (dump_num)))

#define MODE_NAMES(mode_num) (*(&TXT_ANARCHY + (mode_num)))

#define MODEM_ERROR_MESS(error_num) (*(&TXT_NO_DIAL_TONE + (error_num)))

#define MENU_DIFFICULTY_TEXT(difficulty) (*(&TXT_DIFFICULTY_1 + (difficulty)))
#define MENU_DETAIL_TEXT(detail) (*(&TXT_DETAIL_1 + (detail)))

//Load function

void load_text(void);

#endif
