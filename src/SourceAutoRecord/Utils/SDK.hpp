#pragma once
#include <cmath>

struct Vector {
    float x, y, z;
    inline float Length()
    {
        return sqrt(x * x + y * y + z * z);
    }
    inline float Length2D()
    {
        return sqrt(x * x + y * y);
    }
    inline Vector operator*(float fl)
    {
        Vector res;
        res.x = x * fl;
        res.y = y * fl;
        res.z = z * fl;
        return res;
    }
    inline float& operator[](int i)
    {
        return ((float*)this)[i];
    }
    inline float operator[](int i) const
    {
        return ((float*)this)[i];
    }
};

struct QAngle {
    float x, y, z;
};

struct Color {
    Color()
    {
        *((int*)this) = 255;
    }
    Color(int _r, int _g, int _b)
    {
        SetColor(_r, _g, _b, 255);
    }
    Color(int _r, int _g, int _b, int _a)
    {
        SetColor(_r, _g, _b, _a);
    }
    void SetColor(int _r, int _g, int _b, int _a = 255)
    {
        _color[0] = (unsigned char)_r;
        _color[1] = (unsigned char)_g;
        _color[2] = (unsigned char)_b;
        _color[3] = (unsigned char)_a;
    }
    inline int r() const { return _color[0]; }
    inline int g() const { return _color[1]; }
    inline int b() const { return _color[2]; }
    inline int a() const { return _color[3]; }
    unsigned char _color[4];
};

enum SignonState {
    None = 0,
    Challenge = 1,
    Connected = 2,
    New = 3,
    Prespawn = 4,
    Spawn = 5,
    Full = 6,
    Changelevel = 7
};

struct CUserCmd {
    void* VMT; // 0
    int command_number; // 4
    int tick_count; // 8
    QAngle viewangles; // 12, 16, 20
    float forwardmove; // 24
    float sidemove; // 28
    float upmove; // 32
    int buttons; // 36
    unsigned char impulse; // 40
    int weaponselect; // 44
    int weaponsubtype; // 48
    int random_seed; // 52
    short mousedx; // 56
    short mousedy; // 58
    bool hasbeenpredicted; // 60
};

class CMoveData {
public:
    bool m_bFirstRunOfFunctions : 1; // 0
    bool m_bGameCodeMovedPlayer : 1; // 2
    void* m_nPlayerHandle; // 4
    int m_nImpulseCommand; // 8
    QAngle m_vecViewAngles; // 12, 16, 20
    QAngle m_vecAbsViewAngles; // 24, 28, 32
    int m_nButtons; // 36
    int m_nOldButtons; // 40
    float m_flForwardMove; // 44
    float m_flSideMove; // 48
    float m_flUpMove; // 52
    float m_flMaxSpeed; // 56
    float m_flClientMaxSpeed; // 60
    Vector m_vecVelocity; // 64, 68, 72
    QAngle m_vecAngles; // 76, 80, 84
    QAngle m_vecOldAngles; // 88, 92, 96
    float m_outStepHeight; // 100
    Vector m_outWishVel; // 104, 108, 112
    Vector m_outJumpVel; // 116, 120, 124
    Vector m_vecConstraintCenter; // 128, 132, 136
    float m_flConstraintRadius; // 140
    float m_flConstraintWidth; // 144
    float m_flConstraintSpeedFactor; // 148
    void SetAbsOrigin(const Vector& vec);
    const Vector& GetAbsOrigin() const;

private:
    Vector m_vecAbsOrigin;
};

class CHLMoveData : public CMoveData {
public:
    bool m_bIsSprinting;
};

typedef enum {
    HS_NEW_GAME = 0,
    HS_LOAD_GAME = 1,
    HS_CHANGE_LEVEL_SP = 2,
    HS_CHANGE_LEVEL_MP = 3,
    HS_RUN = 4,
    HS_GAME_SHUTDOWN = 5,
    HS_SHUTDOWN = 6,
    HS_RESTART = 7
} HOSTSTATES;

struct CHostState {
    HOSTSTATES m_currentState; // 0
    HOSTSTATES m_nextState; // 4
    Vector m_vecLocation; // 8, 12, 16
    QAngle m_angLocation; // 20, 24, 28
    char m_levelName[256]; // 32
    char m_landmarkName[256]; // 288
    char m_saveName[256]; // 544
    float m_flShortFrameTime; // 800
    bool m_activeGame; // 804
    bool m_bRememberLocation; // 805
    bool m_bBackgroundLevel; // 806
    bool m_bWaitingForConnection; // 807
};

#define INTERFACEVERSION_ISERVERPLUGINCALLBACKS "ISERVERPLUGINCALLBACKS002"

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);
typedef void* (*InstantiateInterfaceFn)();

struct InterfaceReg {
    InstantiateInterfaceFn m_CreateFn;
    const char* m_pName;
    InterfaceReg* m_pNext;
    static InterfaceReg* s_pInterfaceRegs;

    InterfaceReg(InstantiateInterfaceFn fn, const char* pName)
        : m_pName(pName)
    {
        m_CreateFn = fn;
        m_pNext = s_pInterfaceRegs;
        s_pInterfaceRegs = this;
    }
};

class IServerPluginCallbacks {
public:
    virtual bool Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) = 0;
    virtual void Unload() = 0;
    virtual void Pause() = 0;
    virtual void UnPause() = 0;
    virtual const char* GetPluginDescription() = 0;
    virtual void LevelInit(char const* pMapName) = 0;
    virtual void ServerActivate(void* pEdictList, int edictCount, int clientMax) = 0;
    virtual void GameFrame(bool simulating) = 0;
    virtual void LevelShutdown() = 0;
    virtual void ClientFullyConnect(void* pEdict) = 0;
    virtual void ClientActive(void* pEntity) = 0;
    virtual void ClientDisconnect(void* pEntity) = 0;
    virtual void ClientPutInServer(void* pEntity, char const* playername) = 0;
    virtual void SetCommandClient(int index) = 0;
    virtual void ClientSettingsChanged(void* pEdict) = 0;
    virtual int ClientConnect(bool* bAllowConnect, void* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) = 0;
    virtual int ClientCommand(void* pEntity, const void*& args) = 0;
    virtual int NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) = 0;
    virtual void OnQueryCvarValueFinished(int iCookie, void* pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue) = 0;
    virtual void OnEdictAllocated(void* edict) = 0;
    virtual void OnEdictFreed(const void* edict) = 0;
};

struct CPlugin {
    char m_szName[128];
    bool m_bDisable;
    IServerPluginCallbacks* m_pPlugin;
    int m_iPluginInterfaceVersion;
    void* m_pPluginModule;
};

#define EXPOSE_INTERFACE_FN(functionName, interfaceName, versionName) \
    static InterfaceReg __g_Create##interfaceName##_reg(functionName, versionName);

#define EXPOSE_INTERFACE(className, interfaceName, versionName)                                           \
    static void* __Create##className##_interface() { return static_cast<interfaceName*>(new className); } \
    static InterfaceReg __g_Create##className##_reg(__Create##className##_interface, versionName);

#define EXPOSE_SINGLE_INTERFACE_GLOBALVAR(className, interfaceName, versionName, globalVarName)                           \
    static void* __Create##className##interfaceName##_interface() { return static_cast<interfaceName*>(&globalVarName); } \
    static InterfaceReg __g_Create##className##interfaceName##_reg(__Create##className##interfaceName##_interface, versionName);

#define EXPOSE_SINGLE_INTERFACE(className, interfaceName, versionName) \
    static className __g_##className##_singleton;                      \
    EXPOSE_SINGLE_INTERFACE_GLOBALVAR(className, interfaceName, versionName, __g_##className##_singleton)

struct CEventAction {
    const char* m_iTarget; // 0
    const char* m_iTargetInput; // 4
    const char* m_iParameter; // 8
    float m_flDelay; // 12
    int m_nTimesToFire; // 16
    int m_iIDStamp; //20
    CEventAction* m_pNext; // 24
};

struct EventQueuePrioritizedEvent_t {
    float m_flFireTime; // 0
    char* m_iTarget; // 4
    char* m_iTargetInput; // 8
    void* m_pActivator; // 12
    void* m_pCaller; // 16
    int m_iOutputID; // 20
    void* m_pEntTarget; // 24
    char m_VariantValue[20]; // 28
    EventQueuePrioritizedEvent_t* m_pNext; // 48
    EventQueuePrioritizedEvent_t* m_pPrev; // 52
};

struct CEventQueue {
    EventQueuePrioritizedEvent_t m_Events; // 0
    int m_iListCount; // 56
};

enum MapLoadType_t {
    MapLoad_NewGame = 0,
    MapLoad_LoadGame = 1,
    MapLoad_Transition = 2,
    MapLoad_Background = 3
};

struct CGlobalVarsBase {
    float realtime; // 0
    int framecount; // 4
    float absoluteframetime; // 8
    float curtime; // 12
    float frametime; // 16
    int maxClients; // 20
    int tickcount; // 24
    float interval_per_tick;
    float interpolation_amount;
    int simTicksThisFrame;
    int network_protocol;
    void* pSaveData;
    bool m_bClient;
    int nTimestampNetworkingBase;
    int nTimestampRandomizeWindow;
};

struct CGlobalVars : CGlobalVarsBase {
    char* mapname;
    int mapversion;
    char* startspot;
    MapLoadType_t eLoadType;
    bool bMapLoadFailed;
    bool deathmatch;
    bool coop;
    bool teamplay;
    int maxEntities;
};

class IGameEvent {
public:
    virtual ~IGameEvent() {};
    virtual const char* GetName() const = 0;
    virtual bool IsReliable() const = 0;
    virtual bool IsLocal() const = 0;
    virtual bool IsEmpty(const char* key = 0) = 0;
    virtual bool GetBool(const char* key = 0, bool default_value = false) = 0;
    virtual int GetInt(const char* key = 0, int default_value = 0) = 0;
    virtual float GetFloat(const char* key = 0, float default_value = 0.0f) = 0;
    virtual const char* GetString(const char* key = 0, const char* default_value = "") = 0;
    virtual void SetBool(const char* key, bool value) = 0;
    virtual void SetInt(const char* key, int value) = 0;
    virtual void SetFloat(const char* key, float value) = 0;
    virtual void SetString(const char* key, const char* value) = 0;
};

class IGameEventListener2 {
public:
    virtual ~IGameEventListener2() {};
    virtual void FireGameEvent(IGameEvent* event) = 0;
    virtual int GetEventDebugID() = 0;
};

// TODO: test these
#define EVENTS                               \
    {                                        \
        "server_spawn",                      \
            "server_shutdown",               \
            "server_cvar",                   \
            "server_message",                \
            "server_addban",                 \
            "server_removeban",              \
            "player_connect",                \
            "player_info",                   \
            "player_disconnect",             \
            "player_activate",               \
            "player_say",                    \
            "team_info",                     \
            "team_score",                    \
            "teamplay_broadcast_audio",      \
            "player_team",                   \
            "player_class",                  \
            "player_death",                  \
            "player_hurt",                   \
            "player_chat",                   \
            "player_score",                  \
            "player_spawn",                  \
            "player_shoot",                  \
            "player_use",                    \
            "player_drop",                   \
            "player_changename",             \
            "player_hintmessage",            \
            "game_init",                     \
            "game_newmap",                   \
            "game_start",                    \
            "game_end",                      \
            "round_start",                   \
            "round_end",                     \
            "game_message",                  \
            "break_breakable",               \
            "break_prop",                    \
            "entity_killed",                 \
            "bonus_updated",                 \
            "achievement_event",             \
            "physgun_pickup",                \
            "flare_ignite_npc",              \
            "helicopter_grenade_punt_miss",  \
            "user_data_downloaded",          \
            "ragdoll_dissolved",             \
            "gameinstructor_draw",           \
            "gameinstructor_nodraw",         \
            "map_transition",                \
            "entity_visible",                \
            "set_instructor_group_enabled",  \
            "instructor_server_hint_create", \
            "instructor_server_hint_stop",   \
            "portal_player_touchedground",   \
            "portal_player_ping",            \
            "portal_player_portaled",        \
            "turret_hit_turret",             \
            "security_camera_detached",      \
            "challenge_map_complete",        \
            "advanced_map_complete",         \
            "quicksave",                     \
            "autosave",                      \
            "slowtime",                      \
            "portal_enabled",                \
            "portal_fired",                  \
            "gesture_earned",                \
            "player_gesture",                \
            "player_zoomed",                 \
            "player_unzoomed",               \
            "player_countdown",              \
            "player_touched_ground",         \
            "player_long_fling",             \
            "remote_view_activated",         \
            "touched_paint",                 \
            "player_paint_jumped",           \
            "move_hint_visible",             \
            "movedone_hint_visible",         \
            "counter_hint_visible",          \
            "zoom_hint_visible",             \
            "jump_hint_visible",             \
            "partnerview_hint_visible",      \
            "paint_cleanser_visible",        \
            "paint_cleanser_not_visible",    \
            "player_touch_paint_cleanser",   \
            "bounce_count",                  \
            "player_landed",                 \
            "player_suppressed_bounce",      \
            "OpenRadialMenu",                \
            "AddLocator",                    \
            "player_spawn_blue",             \
            "player_spawn_orange",           \
            "map_already_completed",         \
            "achievement_earned",            \
            "replay_status",                 \
            "spec_target_updated",           \
            "player_fullyjoined",            \
            "achievement_write_failed",      \
            "player_stats_updated",          \
            "round_start_pre_entity",        \
            "teamplay_round_start",          \
            "client_disconnect",             \
            "server_pre_shutdown",           \
            "difficulty_changed",            \
            "finale_start",                  \
            "finale_win",                    \
            "vote_passed",                   \
            "portal_stats_ui_enable",        \
            "portal_stats_update",           \
            "puzzlemaker_fully_hidden",      \
            "puzzlemaker_showing",           \
            "ss_control_transferred",        \
            "inventory_updated",             \
            "cart_updated",                  \
            "store_pricesheet_updated",      \
            "gc_connected",                  \
            "item_schema_initialized",       \
            "client_loadout_changed",        \
            "gameui_activated",              \
            "gameui_hidden",                 \
            "hltv_status",                   \
            "hltv_cameraman",                \
            "hltv_rank_camera",              \
            "hltv_rank_entity",              \
            "hltv_fixed",                    \
            "hltv_chase",                    \
            "hltv_message",                  \
            "hltv_title",                    \
            "hltv_chat",                     \
    }
