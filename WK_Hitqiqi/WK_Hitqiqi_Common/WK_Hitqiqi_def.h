#pragma once
#include <string.h>
#include <atltime.h>
#include "WK_Common_Define.h"

#define  HITQIQI_VERSION                                7
#define  HITQIQI_ACCESSCODE                             95960795
//#define  HITQIQI_SERVER_DOMAIN                          "www.wkfty.com"
#define  HITQIQI_SERVER_DOMAIN                        "127.0.0.1"
#define  HITQIQI_SERVER_PORT                            11057
#define  HITQIQ_MAX_BUF                                 1024
#define  HITQIQ_MAX_RECONNECT                           3
#define  HITQIQI_DATA_FILENAME                          "data.ini"

#define WK_MSG_HITQIQI_UPDATAQIQIDATA                   WM_USER + 2 
#define WK_MSG_HITQIQI_UPDATAUSERENERGY                 WM_USER + 3
#define WK_MSG_HITQIQI_SHOWHITTEXT                      WM_USER + 4
#define WK_MSG_HITQIQI_SHOWNOTIFY                       WM_USER + 5
#define WK_MSG_HITQIQI_REFERSH                          WM_USER + 6

enum HITQIQIMSG_EVENT
{
  HITQIQIMSG_EVENT_ERROR = 0,
  HITQIQIMSG_EVENT_ASKQIQIDATA,
  HITQIQIMSG_EVENT_UPDATAQIQIDATA,
  HITQIQIMSG_EVENT_NOTIFY,
  HITQIQIMSG_EVENT_HIT,
  HITQIQIMSG_EVENT_LVLUPGET,
  HITQIQIMSG_EVENT_ROT_SUCCESS,
  HITQIQIMSG_EVENT_ASKALLON,
  HITQIQIMSG_EVENT_CHARGER,
  HITQIQIMSG_EVENT_COMMAND,
  HITQIQIMSG_EVENT_BETRAIER,
  HITQIQIMSG_EVENT_BETRAIER_OBTAIN,
  HITQIQIMSG_EVENT_TRY_BROKEN,
  HITQIQIMSG_EVENT_UPDATA_BROKEN_TIME,
  HITQIQIMSG_EVENT_BLADEAMMOR_ENERGY_MIN,
  HITQIQIMSG_EVENT_SMOKE_EXP_MIN,
  HITQIQIMSG_EVENT_WRONG_CLIENT_VERSION,
  HITQIQIMSG_EVENT_SENTRY_SUCCESS,
  HITQIQIMSG_EVENT_CHANGEABLE_REWARD,
};

enum HITQIQIMSG_SUBEVENT
{
  HITQIQIMSG_SUBEVENT_ERROR = 0,
  HITQIQIMSG_SUBEVENT_FLAG_FALSE,
  HITQIQIMSG_SUBEVENT_FLAG_TRUE,
  HITQIQIMSG_SUBEVENT_DATAINIT,
  HITQIQIMSG_SUBEVENT_LVLUPGET_SIGIN,
  HITQIQIMSG_SUBEVENT_LVLUPGET_SUBSTANT,
  HITQIQIMSG_SUBEVENT_LVLUPGET_HUGE,
  HITQIQIMSG_SUBEVENT_WARLOCK_HIT,
  HITQIQIMSG_SUBEVENT_SENTRY_HIT,
  HITQIQIMSG_SUBEVENT_OBSERVER_HIT,
};

enum HITQIQIMSG_SUBSUBEVENT
{
  HITQIQIMSG_SUBSUBEVENT_ERROR = 0,
  HITQIQIMSG_SUBSUBEVENT_COMMAND_ONLINE,
};

#define  HITQIQI_GIFT_CNT     13
static const wchar_t* g_GiftDes[] =
{
  L"博学者：获得的最终经验值增加。" ,
  L"腐蚀之刃：成功命中的攻击有概率永久削弱琦琦的护甲。" ,
  L"精力充沛：精力越少，回复速度越快。" ,
  L"狂暴：攻击命中后，大幅提高攻击力，但是会显著降低命中率，可叠加。" ,
  //L"破坏者：琦琦狂暴时，巨额的提升精力恢复速度。" ,
  L"独行侠：十分钟内，如果没有其他玩家攻击琦琦，最终伤害翻倍。" ,
  L"蓄力：攻击力随时间持续上升，直到发出攻击。" ,
  L"回春术：每次攻击都有几率为在线玩家回复精力。" ,
  L"指挥官：在线时，提升所有玩家的整体属性。" ,
  L"背叛者（慎选）：只要背叛者在线，就会持续夺取其他玩家的精力。" ,
  L"术士（高难)：你的非暴击命中会为琦琦恢复生命值。升级奖励选项增加2" ,
  L"哨兵：无法暴击。若你的攻击命中了恰好受到暴击的琦琦，获得一次经验飞升" ,
  L"观察者：无视琦琦的护甲。可以看到琦琦的属性" ,
  L"百变怪：其他玩家获得稀有(蓝色)及以上升级奖励时，自身获得同样的加成" ,
};

enum HITQIQI_PLAYER_GIFT
{
  HITQIQI_PLAYER_GIFT_ERROR = 0,
  HITQIQI_PLAYER_GIFT_KNOWLEDGE,
  HITQIQI_PLAYER_GIFT_ROT,
  HITQIQI_PLAYER_GIFT_REENERGY,
  HITQIQI_PLAYER_GIFT_WILD,
  //HITQIQI_PLAYER_GIFT_BROKEN,
  HITQIQI_PLAYER_GIFT_ALONE,
  HITQIQI_PLAYER_GIFT_ACCUMULATE,
  HITQIQI_PLAYER_GIFT_CHARGER,
  HITQIQI_PLAYER_GIFT_COMMAND,
  HITQIQI_PLAYER_GIFT_BETRAIER,
  HITQIQI_PLAYER_GIFT_WARLOCK,
  HITQIQI_PLAYER_GIFT_SENTRY,
  HITQIQI_PLAYER_GIFT_OBSERVER,
  HITQIQI_PLAYER_GIFT_CHANGEABLE,
};

#define HITQIQI_PLAYER_GIFT_VALUE_KNOWLEDGE                         1.90

#define HITQIQI_PLAYER_GIFT_VALUE_ROT                               0.02
#define HITQIQI_PLAYER_GIFT_VALUE_ROT_AMMOR                         0.2

#define HITQIQI_PLAYER_GIFT_VALUE_REENERGY                          1.50

#define HITQIQI_PLAYER_GIFT_VALUE_BROKEN                             7.5

#define HITQIQI_PLAYER_GIFT_VALUE_ALLON                             2.0

#define HITQIQI_PLAYER_GIFT_VALUE_ACCUMULATE                        0.12
#define HITQIQI_PLAYER_GIFT_VALUE_ACCUMULATE_MAX                    640

#define HITQIQI_PLAYER_GIFT_VALUE_CHARGER_RATE                      0.2
#define HITQIQI_PLAYER_GIFT_VALUE_CHARGER_AMONT                     200.0

#define HITQIQI_PLAYER_GIFT_VALUE_COMMAND_ATTACK                    0.3
#define HITQIQI_PLAYER_GIFT_VALUE_COMMAND_HIT_RATE                  0.07
#define HITQIQI_PLAYER_GIFT_VALUE_COMMAND_CRIC_RATE                 0.12
#define HITQIQI_PLAYER_GIFT_VALUE_COMMAND_ENERGY_OBTAIN             0.3
#define HITQIQI_PLAYER_GIFT_VALUE_COMMAND_EXP_GET                   0.3

#define HITQIQI_PLAYER_GIFT_VALUE_BETRAIER_TIME                     120
#define HITQIQI_PLAYER_GIFT_VALUE_BETRAIER_AMONT                    150

#define HITQIQI_PLAYER_GIFT_VALUE_WARLOCK_REWARD_ADD                2

#define HITQIQI_PLAYER_GIFT_VALUE_SENTRY_EXP_REWARD                0.35


#define  HITQIQI_USERDEFAULT_EXP_MAX                                150.0
#define  HITQIQI_USERDEFAULT_BASE_ATTACK                            15.0
#define  HITQIQI_USERDEFAULT_BASE_ATTACK_GROW                          2.0
#define  HITQIQI_USERDEFAULT_CRICITAL_RATE                          0.07
#define  HITQIQI_USERDEFAULT_CRICITAL_TIMES                         1.50
#define  HITQIQI_USERDEFAULT_ENERGY_MAX                             2000.0
#define HITQIQI_USERDEFAULT_EXP_GROWUP                              1.15
#define  HITQIQI_USERDEFAULT_ENERGY_OBTAIN                          4.10
#define  HITQIQI_USERDEFAULT_AMMOR_VOID                             0

#define HITQIQI_LVLUP_REWARD_CNT                                    3
#define HITQIQI_RANDOM_RETE_CNT                                     6

#define  HITQIQI_RATE_TINY                                          0.45                            //略微的
#define  HITQIQI_RATE_LITTLE                                        0.25                            //小幅的
#define  HITQIQI_RATE_CONSIDE                                       0.175                          //可观的
#define  HITQIQI_RATE_SIGIN                                         0.075                          //显著的
#define  HITQIQI_RATE_SUBSTANT                                      0.035                          //大幅的
#define  HITQIQI_RATE_HUGE                                          0.015                          //巨额的

static const double g_RandomRates[] =
{
100.0,
HITQIQI_RATE_TINY,
HITQIQI_RATE_LITTLE,
HITQIQI_RATE_CONSIDE,
HITQIQI_RATE_SIGIN,
HITQIQI_RATE_SUBSTANT,
HITQIQI_RATE_HUGE,
};

enum HITQIQI_RANDOM_TYPE {
  HITQIQI_RANDOM_TYPE_ERROR = 0,
  HITQIQI_RANDOM_TYPE_TINY,
  HITQIQI_RANDOM_TYPE_LITTLE,
  HITQIQI_RANDOM_TYPE_CONSIDE,
  HITQIQI_RANDOM_TYPE_SIGIN,
  HITQIQI_RANDOM_TYPE_SUBSTANT,
  HITQIQI_RANDOM_TYPE_HUGE,
};


static const wchar_t* g_RandomDes[] =
{
  L"ERROR",
  L"略微的",
  L"小幅的",
  L"可观的",
  L"显著的",
  L"大幅的",
  L"巨额的",
};

static const char* g_RandomTypeDes[] =
{
  "ERROR",
  "平庸",
  "普通",
  "优良",
  "稀有",
  "史诗",
  "传说",
};

enum HITQIQI_LVLUP_REWARD {
  HITQIQI_LVLUP_REWARD_ERROR = 0,
  HITQIQI_LVLUP_REWARD_ATTACK,
  HITQIQI_LVLUP_REWARD_CRIC_RATE,
  HITQIQI_LVLUP_REWARD_CRIC_TIME,
  HITQIQI_LVLUP_REWARD_ENERGY_MAX,
  HITQIQI_LVLUP_REWARD_ENERGY_OBTAIN,
  HITQIQI_LVLUP_REWARD_HIT_RATE,
  HITQIQI_LVLUP_REWARD_AMMOR_VOID,
};

static const wchar_t* g_LVLUPDes[] =
{
  L"ERROR",
  L"提高基础攻击力。" ,
  L"提高暴击率。" ,
  L"提高暴击倍率。" ,
  L"提高精力上限。" ,
  L"提高精力回复。" ,
  L"提高命中率。" ,
  L"提高护甲穿透。" ,
};

struct HITQIQI_LULUP_RANDOM
{
  HITQIQI_RANDOM_TYPE type;
  HITQIQI_LVLUP_REWARD reward;
  HITQIQI_LULUP_RANDOM() {
    memset(this, 0, sizeof(HITQIQI_LULUP_RANDOM));
  }
};

struct GIFT_WILD_VALUE
{
  double wild_attack;
  double wild_hit_rate;
};

struct GIFT_ALLON_VALUE
{
  WK_Bool ALLON_FLAG;
  WK_Bool ALLON_UPDATA;
};



struct HITQIQI_USER_DATA
{
  char name[20];
  int level;
  double exp_max;
  double exp_now;
  double base_attack;
  double cricital_rate;
  double cricital_times;
  double energy_max;
  double energy_now;
  double energy_obtain;
  double hit_rate;
  double ammor_void;
  HITQIQI_PLAYER_GIFT gift;
  union
  {
    CTime broken_time;
    GIFT_WILD_VALUE wild_value;
    GIFT_ALLON_VALUE allon_value;
    double accumulate_value;
    double betraier_time;
  };
  HITQIQI_USER_DATA() {
    memset(this, 0, sizeof(HITQIQI_USER_DATA));
  }
};

#define QIQI_STATUS_CNT                             11
enum QIQI_STATUS
{
  QIQI_STATUS_ERROR = 0,
  QIQI_STATUS_NORMAL,
  QIQI_STATUS_TEA,                                      //饮茶 加回血 伤害覆盖
  QIQI_STATUS_RUN,                                      //加血 伤害总值
  QIQI_STATUS_FITNESS,                                  //锻炼 加护甲 100点伤害解除
  QIQI_STATUS_SHIELD,                                   //举盾 免疫伤害 1分钟内100次攻击解除
  QIQI_STATUS_SHADOW,                                   //影流 加闪避  连续3次命中 解除
  QIQI_STATUS_TATTOO,                                   //纹身 减伤 暴击次数
  QIQI_STATUS_FISH,                                     //玩小鱼 缩短时间 轮流攻击
  QIQI_STATUS_BLADEAMMOR,                               //刃甲 扣精力 伤害阶梯
  QIQI_STATUS_SMOKE,                                    //抽烟 扣经验 人数+收刀
  QIQI_STATUS_DEAD,
  QIQI_STATUS_SUCCESS,
};

#define QIQI_PIC_NORMAL                            _T("qiqi_pic_normal.png")
#define QIQI_PIC_TEA                               _T("qiqi_pic_tea.png")
#define QIQI_PIC_RUN                               _T("qiqi_pic_run.png")
#define QIQI_PIC_FITNESS                           _T("qiqi_pic_fitness.png")
#define QIQI_PIC_SHIELD                            _T("qiqi_pic_shield.png")
#define QIQI_PIC_SHADOW                            _T("qiqi_pic_shadow.png")
#define QIQI_PIC_TATTOO                            _T("qiqi_pic_tattoo.png")
#define QIQI_PIC_FISH                              _T("qiqi_pic_fish.png")
#define QIQI_PIC_BLADEAMMOR                        _T("qiqi_pic_bladeammor.png")
#define QIQI_PIC_SMOKE                             _T("qiqi_pic_smoke.png")
#define QIQI_PIC_DEAD                              _T("qiqi_pic_dead.png")
#define QIQI_PIC_SUCCESS                           _T("qiqi_pic_success.png")

static const char* g_StatusDes[] =
{
  "ERROR",
  "正常",
  "饮茶",
  "马拉松",
  "锻炼",
  "举盾",
  "影流",
  "纹身",
  "小鱼人",
  "刃甲",
  "抽烟",
  "死亡",
};

#define HITQIQI_ENERGY_VALUE_GROW                       1.15
#define HITQIQI_AMMOR_VALUE_GROW                      1.5
#define HITQIQI_DODGE_VALUE_GROW                       0.005

#define HITQIQI_DEFENSE_VALUE_TEA_TOTAL_TIME                       600
#define HITQIQI_DEFENSE_VALUE_TEA_DELAY_TIME                       120
#define HITQIQI_DEFENSE_VALUE_TEA_HP_OBTAIN                        0.01

#define HITQIQI_DEFENSE_VALUE_RUN_TOTAL_TIME                       3600
#define HITQIQI_DEFENSE_VALUE_RUN_TOTAK_DAMAGE                     5000
#define HITQIQI_DEFENSE_VALUE_RUN_HP_OBTAIN                        0.05

#define HITQIQI_DEFENSE_VALUE_FITNESS_TOTAL_TIME                   1800
#define HITQIQI_DEFENSE_VALUE_FITNESS_BROKEN_LIMIT                 150
#define HITQIQI_DEFENSE_VALUE_FITNESS_AMMOR_OBTAIN                 1.5

#define HITQIQI_DEFENSE_VALUE_SHIELD_HIT_CNT                       100
#define HITQIQI_DEFENSE_VALUE_SHILED_HIT_INTIME                    60

#define HITQIQI_DEFENSE_VALUE_SHADOW_DODGE                         0.4
#define HITQIQI_DEFENSE_VALUE_SHADOW_HIT_CNT                       4

#define HITQIQI_DEFENSE_VALUE_TATTOO_DEFINE                        0.7
#define HITQIQI_DEFENSE_VALUE_TATTOO_CIRC_CNT                      150

#define HITQIQI_DEFENSE_VALUE_FISH_TIMEMIN                         9
#define HITQIQI_DEFENSE_VALUE_FISH_HITCNT                          4

#define HITQIQI_DEFENSE_VALUE_BLADEAMMOR_HITCNT                    4
#define HITQIQI_DEFENSE_VALUE_BLADEAMMOR_TIME                      60.0
#define HITQIQI_DEFENSE_VALUE_BLADEAMMOR_ENERGYMIN                 150

#define HITQIQI_DEFENSE_VALUE_SMOKE_PLAYER_CNT                     5
#define HITQIQI_DEFENSE_VALUE_SMOKE_TIME_MIN                       60.0
#define HITQIQI_DEFENSE_VALUE_SMOKE_TIME_CNT                       600.0
#define HITQIQI_DEFENSE_VALUE_SMOKE_EXP_MIN                        100.0

struct  QIQI_STATUS_TEA_VALUE
{
  double time_rest;
  double time_delay;
};

struct  QIQI_STATUS_RUN_VALUE
{
  double time_rest;
  double damage_sum;
};

struct  QIQI_STATUS_FITNESS_VALUE
{
  double time_rest;
};

struct  QIQI_STATUS_SHADOW_VALUE
{
  int nomiss_hit;
};

struct  QIQI_STATUS_TATTOO_VALUE
{
  int circ_cnt;
};

struct  QIQI_STATUS_FISH_VALUE
{
  CTime start_time;
};

struct  QIQI_STATUS_BLADEAMMOR_VALUE
{
  CTime start_time;
  double time_delay;
};

struct  QIQI_STATUS_SMOKE_VALUE
{
  double time_rest;
  double time_delay;
};

union HITQIQI_DEFENSE_VALUE
{
  QIQI_STATUS_TEA_VALUE tea_value;
  QIQI_STATUS_RUN_VALUE run_value;
  QIQI_STATUS_FITNESS_VALUE fitness_value;
  QIQI_STATUS_SHADOW_VALUE shadow_value;
  QIQI_STATUS_TATTOO_VALUE tattoo_value;
  QIQI_STATUS_FISH_VALUE fish_value;
  QIQI_STATUS_BLADEAMMOR_VALUE bladeammor_value;
  QIQI_STATUS_SMOKE_VALUE smoke_value;
  HITQIQI_DEFENSE_VALUE() {
    memset(this, 0, sizeof(HITQIQI_DEFENSE_VALUE));
  }
};

struct QIQI_DATA
{
  unsigned int id;
  QIQI_STATUS status;
  int hp_max;
  int hp_now;
  double energy;
  double energy_max;
  double ammor;
  double dodge_rate;
  int end_sec;

  QIQI_DATA() {
    memset(this, 0, sizeof(QIQI_DATA));
  }
};

enum HITQIQI_HITTYPE
{
  HITQIQI_HITTYPE_ERROR = -1,
  HITQIQI_HITTYPE_NORMAL,
  HITQIQI_HITTYPE_MISS,
  HITQIQI_HITTYPE_CRICITAL,
  HITQIQI_HITTYPE_BLOCK,
};

struct HIT_INFO
{
  char player_name[20];
  HITQIQI_HITTYPE hit_type;
  int hit_damage;

  HIT_INFO() {
    memset(this, 0, sizeof(HIT_INFO));
  }
};

struct HIT_TEXT_INFO
{
  HITQIQI_HITTYPE hit_type;
  int hit_damage;
  POINT ptMouse;
  WK_Bool is_warlock_hit;

  HIT_TEXT_INFO() {
    memset(this, 0, sizeof(HIT_TEXT_INFO));
  }
};

enum QIQI_NOTIFY_BACKGROUND {
  QIQI_NOTIFY_BACKGROUND_ERROR = 0,
  QIQI_NOTIFY_BACKGROUND_NORMAL,
  QIQI_NOTIFY_BACKGROUND_REAL,
  QIQI_NOTIFY_BACKGROUND_EXCELLENT,
  QIQI_NOTIFY_BACKGROUND_EPIC,
  QIQI_NOTIFY_BACKGROUND_GRODEN,
  QIQI_NOTIFY_BACKGROUND_RED,
  QIQI_NOTIFY_BACKGROUND_ROT,
};

#define QIQI_NOTIFY_BACKGROUND_NORMAL_BK1                  0xFF8e9eab
#define QIQI_NOTIFY_BACKGROUND_NORMAL_BK2                  0xFFeef2f3
#define QIQI_NOTIFY_BACKGROUND_REAL_BK1                    0xFFd4fc79
#define QIQI_NOTIFY_BACKGROUND_REAL_BK2                    0xFF96e6a1
#define QIQI_NOTIFY_BACKGROUND_EXCELLENT_BK1               0xFF2193b0
#define QIQI_NOTIFY_BACKGROUND_EXCELLENT_BK2               0xFF6dd5ed
#define QIQI_NOTIFY_BACKGROUND_EPIC_BK1                    0xFFFBD3E9
#define QIQI_NOTIFY_BACKGROUND_EPIC_BK2                    0xFFBB377D
#define QIQI_NOTIFY_BACKGROUND_GRODEN_BK1                  0xFFF7971E
#define QIQI_NOTIFY_BACKGROUND_GRODEN_BK2                  0xFFFFD200
#define QIQI_NOTIFY_BACKGROUND_RED_BK1                     0xFFC04848
#define QIQI_NOTIFY_BACKGROUND_RED_BK2                     0xFF480048
#define QIQI_NOTIFY_BACKGROUND_ROT_BK1                     0xFFFDFC47
#define QIQI_NOTIFY_BACKGROUND_ROT_BK2                     0xFF24FE41

struct QIQI_NOTIFY {
  QIQI_NOTIFY_BACKGROUND color;
  char notify_text[50];
};

struct HITQIQI_SERVER_HIT_RECORD
{
  CTime time;
  HIT_INFO hit_info;
};

struct  HITQIQIMSG_PACKAGE
{
  int version;
  int accesscode;
  CTime event_time;
  HITQIQIMSG_EVENT event;
  HITQIQIMSG_SUBEVENT subevent;
  HITQIQIMSG_SUBSUBEVENT subsubevent;
  union
  {
    QIQI_DATA qiqidata;
    HIT_INFO hit_info;
    QIQI_NOTIFY notify;
    HITQIQI_USER_DATA user_data;
    double betraier_obtain;
    CTime broken_time;
    HITQIQI_LULUP_RANDOM changeable_reward;
  };

  HITQIQIMSG_PACKAGE() {
    memset(this, 0, sizeof(HITQIQIMSG_PACKAGE));
    version = HITQIQI_VERSION;
    accesscode = HITQIQI_ACCESSCODE;
  }
};

#define HITQIQIMSG_PACKAGE_LEN sizeof(HITQIQIMSG_PACKAGE)
