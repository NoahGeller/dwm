/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static unsigned int borderpx  		= 2;        /* border pixel of windows */
static unsigned int snap      		= 32;       /* snap pixel */
static unsigned int gappih    		= 15;       /* horiz inner gap between windows */
static unsigned int gappiv    		= 15;       /* vert inner gap between windows */
static unsigned int gappoh    		= 15;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    		= 15;       /* vert outer gap between windows and screen edge */
static int smartgaps          		= 0;        /* 1 means no outer gap when there is only one window */
static int showbar            		= 1;        /* 0 means no bar */
static int topbar             		= 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=14" };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */

	/* class	instance	title		tags mask	floating	monitor */
	{ "Gimp",	NULL,		NULL,		0,			1,			-1 },
	{ "Steam",	NULL,		NULL,		1 << 8,		1,			-1 },
	{ "Steam",	"Steam",	"Steam",	1 << 8,		0,			-1 },
	{ "mpv",	NULL,		NULL,		0,			1,			-1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* Commands */

/* Application launchers */
static const char *roficmd[]	= { "rofi", "-modi", "drun", "-show", "drun", NULL };
static const char *termcmd[]	= { "st", NULL };
static const char *wwwcmd[]		= { "firefox", NULL };

/* Brightness controls */
static const char *brightup[]	= { "brightnessctl", "set", "5%+", NULL };
static const char *brightdown[]	= { "brightnessctl", "set", "5%-", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "color0",			STRING,		&normbgcolor },
		{ "color0",			STRING,		&normbordercolor },
		{ "color4",			STRING,		&normfgcolor },
		{ "color4",			STRING,		&selbgcolor },
		{ "color8",			STRING,		&selbordercolor },
		{ "color0",			STRING,		&selfgcolor },
		{ "borderpx",		INTEGER,	&borderpx },
		{ "snap",			INTEGER,	&snap },
		{ "showbar",		INTEGER,	&showbar },
		{ "topbar",			INTEGER,	&topbar },
		{ "nmaster",		INTEGER,	&nmaster },
		{ "resizehints",	INTEGER,	&resizehints },
		{ "mfact",			FLOAT,		&mfact },
		{ "gappih",			INTEGER,	&gappih },
		{ "gappiv",			INTEGER,	&gappiv },
		{ "gappoh",			INTEGER,	&gappoh },
		{ "gappov",			INTEGER,	&gappov },
		{ "smartgaps",		INTEGER,	&smartgaps },
};

static Key keys[] = {
	/* modifier						key							function		argument */
	{ MODKEY,						XK_p,						spawn,			{.v = roficmd } },
	{ MODKEY|ShiftMask,				XK_Return,					spawn,			{.v = termcmd } },
	{ MODKEY|ShiftMask,				XK_w,						spawn,			{.v = wwwcmd } },
	{ MODKEY,						XK_b,						togglebar,		{0} },
	{ MODKEY,						XK_j,						focusstack,		{.i = +1 } },
	{ MODKEY,						XK_k,						focusstack,		{.i = -1 } },
	{ MODKEY,						XK_i,						incnmaster,		{.i = +1 } },
	{ MODKEY,						XK_d,						incnmaster,		{.i = -1 } },
	{ MODKEY,						XK_h,						setmfact,		{.f = -0.05} },
	{ MODKEY,						XK_l,						setmfact,		{.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_equal,					incrgaps,       {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_minus,					incrgaps,       {.i = -5 } },
	{ MODKEY|ShiftMask,             XK_0,						togglegaps,     {0} },
	{ MODKEY,						XK_Return,					zoom,			{0} },
	{ MODKEY,						XK_Tab,						view,			{0} },
	{ MODKEY|ShiftMask,				XK_q,						killclient,		{0} },
	{ MODKEY,						XK_t,						setlayout,		{.v = &layouts[0]} },
	{ MODKEY,						XK_f,						setlayout,		{.v = &layouts[1]} },
	{ MODKEY,						XK_m,						setlayout,		{.v = &layouts[2]} },
	{ MODKEY,						XK_space,					setlayout,		{0} },
	{ MODKEY|ShiftMask,				XK_space,					togglefloating,	{0} },
	{ MODKEY,						XK_0,						view,			{.ui = ~0 } },
	{ MODKEY|ShiftMask,				XK_0,						tag,			{.ui = ~0 } },
	{ MODKEY,						XK_comma,					focusmon,		{.i = -1 } },
	{ MODKEY,						XK_period,					focusmon,		{.i = +1 } },
	{ MODKEY|ShiftMask,				XK_comma,					tagmon,			{.i = -1 } },
	{ MODKEY|ShiftMask,				XK_period,					tagmon,			{.i = +1 } },
	{ 0,							XF86XK_AudioLowerVolume,	spawn,			SHCMD("pamixer -d 5; pkill -RTMIN+1 dwmbar") },
	{ 0,							XF86XK_AudioRaiseVolume,	spawn,			SHCMD("pamixer -i 5; pkill -RTMIN+1 dwmbar") },
	{ 0,							XF86XK_AudioMute,			spawn,			SHCMD("pamixer -t; pkill -RTMIN+1 dwmbar") },
	{ 0,							XF86XK_MonBrightnessUp,		spawn,			{.v = brightup } },
	{ 0,							XF86XK_MonBrightnessDown,	spawn,			{.v = brightdown } },
	TAGKEYS(						XK_1,						0)
	TAGKEYS(						XK_2,						1)
	TAGKEYS(						XK_3,						2)
	TAGKEYS(						XK_4,						3)
	TAGKEYS(						XK_5,						4)
	TAGKEYS(						XK_6,						5)
	TAGKEYS(						XK_7,						6)
	TAGKEYS(						XK_8,						7)
	TAGKEYS(						XK_9,						8)
	{ MODKEY|ShiftMask,				XK_e,						quit,			{0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

