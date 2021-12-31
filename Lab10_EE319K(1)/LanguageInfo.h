//
//
//*************LanguageInfo.c*****************
//
//
//This file contains all information regarding the languages avaialable to play in.
//

volatile uint8_t LangPnt;							// Keeps track of which language is being 'looked' at by the on screen arrow


typedef enum {English, French} Language_t;
volatile Language_t myL=English;
typedef enum {Title, Menu, Scroll, Play, Language, 
							ChangeLang, Okay, Back, Win, Lose, 
							Draw, HvH, HvM, RedWin, YellowWin, Pause, Resume, Return, ScoreRed, ScoreYellow} phrase_t;
const char Title_English[] ="Connect4!";
const char Title_French[] ="Connecter4!";
const char Menu_English[]="Menu";
const char Menu_French[] = "Menu";
const char Scroll_English[] = "Scroll";
const char Scroll_French[] = "D\x82" "filer";
const char Play_English[] = "Play";
const char Play_French[] = "Jouer";
const char Language_English[]="English";
const char Language_French[]="Fran\x87" "ais";
const char ChLang_English[]="Language";
const char ChLang_French[]="La langue";
const char Okay_English[]="Okay";
const char Okay_French[]="Le bien";
const char Back_English[]="Back";
const char Back_French[]="Retourner";
const char Win_English[]="You win!";
const char Win_French[]="Vous gagnez!";
const char Lose_English[]="You lose!";
const char Lose_French[]="Tu as perdu!";
const char Draw_English[]="Draw!";
const char Draw_French[]="Match nul!";
const char HH_English[] = "Human vs.\x0A" "   Human";
const char HH_French[] = "Humain contre\x0A" "   Humain";
const char HM_English[] = "Human vs.\x0A" "   Machine";
const char HM_French[] = "Humain contre\x0A" "   Machine";
const char RW_English[] = "Red wins!";
const char RW_French[] = "Le rouge\x0A" "       gagne!";
const char YW_English[] = "Yellow\x0A" "      wins!";
const char YW_French[] = "Le jaune\x0A" "       gagne!";
const char Pause_English[] = "Pause Menu";
const char Pause_French[] = "Suspendre le Menu";
const char Resume_English[] = "Resume";
const char Resume_French[] = "Reprendre";
const char Main_English[] = "Main Menu";
const char Main_French[] = "Menu Principal";
const char ScoreRed_English[] = "Score for Red: ";
const char ScoreRed_French[] = "Score pour Rouge: ";
const char ScoreYellow_English[] = "Score for Yellow: ";
const char ScoreYellow_French[] = "Score pour Jaune: ";
const char *Phrases[20][2]={
	{Title_English, Title_French},
	{Menu_English, Menu_French},
	{Scroll_English, Scroll_French},
	{Play_English, Play_French},
	{Language_English, Language_French},
	{ChLang_English, ChLang_French},
	{Okay_English, Okay_French},
	{Back_English, Back_French},
	{Win_English, Win_French},
	{Lose_English, Lose_French},
	{Draw_English, Draw_French},
	{HH_English, HH_French},
	{HM_English, HM_French},
	{RW_English, RW_French},
	{YW_English, YW_French},
	{Pause_English, Pause_French},
	{Resume_English, Resume_French},
	{Main_English, Main_French},
	{ScoreRed_English, ScoreRed_French},
	{ScoreYellow_English, ScoreYellow_French}
};
