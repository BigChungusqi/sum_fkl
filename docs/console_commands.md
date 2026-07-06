# Half-Life 控制台命令和变量

本文档整理了 Half-Life 游戏中的所有控制台命令和变量。

---

## 一、客户端命令 (Client Commands)

### 1.1 移动命令

| 命令 | 描述 |
|------|------|
| `+moveup` / `-moveup` | 向上移动 |
| `+movedown` / `-movedown` | 向下移动 |
| `+forward` / `-forward` | 向前移动 |
| `+back` / `-back` | 向后移动 |
| `+moveleft` / `-moveleft` | 向左平移 |
| `+moveright` / `-moveright` | 向右平移 |
| `+left` / `-left` | 向左转 |
| `+right` / `-right` | 向右转 |
| `+lookup` / `-lookup` | 向上看 |
| `+lookdown` / `-lookdown` | 向下看 |
| `+strafe` / `-strafe` | 平移模式 |
| `+speed` / `-speed` | 加速/奔跑 |
| `+duck` / `-duck` | 蹲下 |
| `+jump` / `-jump` | 跳跃 |
| `+klook` / `-klook` | 键盘视角 |
| `+mlook` / `-mlook` | 鼠标视角 |
| `+jlook` / `-jlook` | 操纵杆视角 |

### 1.2 动作命令

| 命令 | 描述 |
|------|------|
| `+attack` / `-attack` | 主攻击 |
| `+attack2` / `-attack2` | 次攻击 |
| `+use` / `-use` | 使用 |
| `+reload` / `-reload` | 装弹 |
| `+alt1` / `-alt1` | 备用功能 |
| `+break` / `-break` | 中断 |
| `impulse <num>` | 发送脉冲命令 |

### 1.3 计分板命令

| 命令 | 描述 |
|------|------|
| `+score` / `-score` | 显示计分板 |
| `+showscores` / `-showscores` | 显示计分板 |
| `+graph` / `-graph` | 显示网络图表 |
| `togglescores` | 切换计分板显示 |

### 1.4 观察者命令

| 命令 | 描述 |
|------|------|
| `spec_mode` | 切换观察模式 |
| `spec_decal` | 观察者喷图 |
| `spec_help` | 观察者帮助 |
| `spec_menu` | 观察者菜单 |

### 1.5 视角命令

| 命令 | 描述 |
|------|------|
| `centerview` | 居中视角 |
| `force_centerview` | 强制居中视角 |
| `thirdperson` | 第三人称视角 |
| `firstperson` | 第一人称视角 |
| `snapto` | 切换视角锁定 |

### 1.6 相机控制命令

| 命令 | 描述 |
|------|------|
| `+campitchup` / `-campitchup` | 相机向上 |
| `+campitchdown` / `-campitchdown` | 相机向下 |
| `+camyawleft` / `-camyawleft` | 相机左转 |
| `+camyawright` / `-camyawright` | 相机右转 |
| `+camin` / `-camin` | 相机推进 |
| `+camout` / `-camout` | 相机拉远 |
| `+cammousemove` / `-cammousemove` | 鼠标控制相机 |
| `+camdistance` / `-camdistance` | 相机距离控制 |
| `joyadvancedupdate` | 更新操纵杆高级设置 |

### 1.7 语音命令

| 命令 | 描述 |
|------|------|
| `voice_showbanned` | 显示被屏蔽的玩家列表 |

---

## 二、客户端变量 (Client Variables)

### 2.1 HUD 变量

| 变量 | 默认值 | 描述 |
|------|--------|------|
| `hud_draw` | 1 | 控制HUD显示 |
| `hud_capturemouse` | 1 | HUD鼠标捕获 |
| `hud_centerid` | 0 | 居中显示玩家ID |
| `hud_deathnotice_time` | 6 | 死亡通知显示时间 |
| `hud_drawhistory_time` | 2 | 历史记录显示时间 |
| `hud_fastswitch` | 0 | 快速武器切换 |
| `hud_takesshots` | 0 | 自动截图 |
| `hud_saytext` | 1 | 显示聊天文本 |
| `hud_saytext_time` | 5 | 聊天文本显示时间 |
| `hud_classautokill` | 1 | TF职业切换自动自杀 |

### 2.2 视角变量

| 变量 | 默认值 | 描述 |
|------|--------|------|
| `default_fov` | 90 | 默认视野角度 |
| `zoom_sensitivity_ratio` | 1.2 | 瞄准镜灵敏度比率 |
| `scr_ofsx` | 0 | 屏幕偏移X |
| `scr_ofsy` | 0 | 屏幕偏移Y |
| `scr_ofsz` | 0 | 屏幕偏移Z |

### 2.3 移动变量

| 变量 | 默认值 | 描述 |
|------|--------|------|
| `cl_forwardspeed` | 400 | 前进速度 |
| `cl_backspeed` | 400 | 后退速度 |
| `cl_sidespeed` | 400 | 平移速度 |
| `cl_upspeed` | 320 | 上下速度 |
| `cl_yawspeed` | 210 | 水平转向速度 |
| `cl_pitchspeed` | 225 | 垂直转向速度 |
| `cl_anglespeedkey` | 0.67 | 转向速度键 |
| `cl_movespeedkey` | 0.3 | 移动速度键 |
| `cl_pitchup` | 89 | 最大向上视角 |
| `cl_pitchdown` | 89 | 最大向下视角 |
| `cl_vsmoothing` | 0.05 | 视角平滑 |
| `cl_bob` | 0.01 | 武器晃动 |
| `cl_bobcycle` | 0.8 | 晃动周期 |
| `cl_bobup` | 0.5 | 晃动幅度 |
| `cl_waterdist` | 4 | 水中距离 |
| `cl_chasedist` | 112 | 追逐距离 |

### 2.4 鼠标变量

| 变量 | 默认值 | 描述 |
|------|--------|------|
| `sensitivity` | 3 | 鼠标灵敏度 |
| `m_pitch` | 0.022 | 鼠标垂直灵敏度 |
| `m_yaw` | 0.022 | 鼠标水平灵敏度 |
| `m_forward` | 1 | 鼠标前进灵敏度 |
| `m_side` | 0.8 | 鼠标平移灵敏度 |
| `m_filter` | 0 | 鼠标过滤 |
| `lookstrafe` | 0 | 鼠标平移 |
| `lookspring` | 0 | 鼠标弹簧 |

### 2.5 操纵杆变量

| 变量 | 默认值 | 描述 |
|------|--------|------|
| `joystick` | 0 | 启用操纵杆 |
| `joyname` | joystick | 操纵杆名称 |
| `joyadvanced` | 0 | 操纵杆高级模式 |
| `joyadvaxisx` | 0 | X轴映射 |
| `joyadvaxisy` | 0 | Y轴映射 |
| `joyadvaxisz` | 0 | Z轴映射 |
| `joyadvaxisr` | 0 | R轴映射 |
| `joyadvaxisu` | 0 | U轴映射 |
| `joyadvaxisv` | 0 | V轴映射 |
| `joyforwardthreshold` | 0.15 | 前进阈值 |
| `joysidethreshold` | 0.15 | 平移阈值 |
| `joypitchthreshold` | 0.15 | 俯仰阈值 |
| `joyyawthreshold` | 0.15 | 偏航阈值 |
| `joyforwardsensitivity` | -1.0 | 前进灵敏度 |
| `joysidesensitivity` | -1.0 | 平移灵敏度 |
| `joypitchsensitivity` | 1.0 | 俯仰灵敏度 |
| `joyyawsensitivity` | -1.0 | 偏航灵敏度 |
| `joywwhack1` | 0.0 | 操纵杆修正1 |
| `joywwhack2` | 0.0 | 操纵杆修正2 |

### 2.6 相机变量

| 变量 | 默认值 | 描述 |
|------|--------|------|
| `cam_command` | 0 | 相机命令 |
| `cam_snapto` | 0 | 相机锁定 |
| `cam_idealyaw` | 90 | 相机理想偏航 |
| `cam_idealpitch` | 0 | 相机理想俯仰 |
| `cam_idealdist` | 64 | 相机理想距离 |
| `cam_contain` | 0 | 相机包含 |
| `c_maxpitch` | 90.0 | 最大俯仰 |
| `c_minpitch` | 0.0 | 最小俯仰 |
| `c_maxyaw` | 135.0 | 最大偏航 |
| `c_minyaw` | -135.0 | 最小偏航 |
| `c_maxdistance` | 200.0 | 最大距离 |
| `c_mindistance` | 30.0 | 最小距离 |

### 2.7 观察者变量

| 变量 | 默认值 | 描述 |
|------|--------|------|
| `spec_drawnames` | 1 | 显示玩家名称 |
| `spec_drawcone` | 1 | 显示观察锥 |
| `spec_drawstatus` | 1 | 显示状态 |
| `spec_autodirector` | 1 | 自动导演模式 |
| `spec_pip` | 1 | 画中画模式 |

### 2.8 其他客户端变量

| 变量 | 默认值 | 描述 |
|------|--------|------|
| `bitmapfonts` | 1 | 位图字体 |
| `color` | 255 0 0 | 颜色值 |
| `v_centermove` | 0.15 | 视角居中移动 |
| `v_centerspeed` | 500 | 视角居中速度 |

---

## 三、服务端变量 (Server Variables)

### 3.1 游戏规则变量

| 变量 | 默认值 | 描述 |
|------|--------|------|
| `mp_teamplay` | 0 | 团队游戏模式 |
| `mp_fraglimit` | 0 | 杀敌限制 |
| `mp_timelimit` | 0 | 时间限制 |
| `mp_friendlyfire` | 0 | 友军伤害 |
| `mp_falldamage` | 0 | 落地伤害 |
| `mp_weaponstay` | 0 | 武器保留 |
| `mp_forcerespawn` | 1 | 强制重生 |
| `mp_flashlight` | 0 | 手电筒 |
| `mp_autocrosshair` | 1 | 自动瞄准 |
| `mp_allowmonsters` | 0 | 允许怪物 |
| `mp_teamlist` | hgrunt;scientist | 团队列表 |
| `mp_teamoverride` | 1 | 团队覆盖 |
| `mp_defaultteam` | 0 | 默认团队 |
| `mp_chattime` | 10 | 聊天时间 |
| `decalfrequency` | 30 | 喷图频率 |
| `displaysoundlist` | 0 | 显示声音列表 |
| `mp_fragsleft` | 0 | 剩余杀敌数 |
| `mp_timeleft` | 0 | 剩余时间 |

---

## 四、技能变量 (Skill Variables)

技能变量用于调整游戏难度，分为三个等级：
- 等级1: 简单 (Easy)
- 等级2: 普通 (Normal)
- 等级3: 困难 (Hard)

### 4.1 玩家武器伤害

| 变量 | 描述 |
|------|------|
| `sk_plr_9mm_bullet1/2/3` | 9mm手枪子弹伤害 |
| `sk_plr_9mmAR_bullet1/2/3` | MP5子弹伤害 |
| `sk_plr_9mmAR_grenade1/2/3` | MP5榴弹伤害 |
| `sk_plr_buckshot1/2/3` | 霰弹枪伤害 |
| `sk_plr_xbow_bolt_client1/2/3` | 十字弓(客户端)伤害 |
| `sk_plr_xbow_bolt_monster1/2/3` | 十字弓(怪物)伤害 |
| `sk_plr_rpg1/2/3` | RPG伤害 |
| `sk_plr_gauss1/2/3` | Gauss枪伤害 |
| `sk_plr_egon_narrow1/2/3` | Egon窄束伤害 |
| `sk_plr_egon_wide1/2/3` | Egon宽束伤害 |
| `sk_plr_hand_grenade1/2/3` | 手榴弹伤害 |
| `sk_plr_satchel1/2/3` | 炸药包伤害 |
| `sk_plr_tripmine1/2/3` | 定位雷伤害 |

### 4.2 怪物属性

#### Alien Grunt (外星步兵)
| 变量 | 描述 |
|------|------|
| `sk_agrunt_health1/2/3` | 生命值 |
| `sk_agrunt_dmg_punch1/2/3` | 拳击伤害 |

#### Apache (直升机)
| 变量 | 描述 |
|------|------|
| `sk_apache_health1/2/3` | 生命值 |

#### Barney (警卫)
| 变量 | 描述 |
|------|------|
| `sk_barney_health1/2/3` | 生命值 |

#### Bullsquid (牛鲨)
| 变量 | 描述 |
|------|------|
| `sk_bullsquid_health1/2/3` | 生命值 |
| `sk_bullsquid_dmg_bite1/2/3` | 咬击伤害 |
| `sk_bullsquid_dmg_whip1/2/3` | 尾击伤害 |
| `sk_bullsquid_dmg_spit1/2/3` | 喷射伤害 |

#### Big Momma (巨型母兽)
| 变量 | 描述 |
|------|------|
| `sk_bigmomma_health_factor1/2/3` | 生命系数 |
| `sk_bigmomma_dmg_slash1/2/3` | 砍击伤害 |
| `sk_bigmomma_dmg_blast1/2/3` | 爆炸伤害 |
| `sk_bigmomma_radius_blast1/2/3` | 爆炸半径 |

#### Gargantua (巨型怪)
| 变量 | 描述 |
|------|------|
| `sk_gargantua_health1/2/3` | 生命值 |
| `sk_gargantua_dmg_slash1/2/3` | 砍击伤害 |
| `sk_gargantua_dmg_fire1/2/3` | 火焰伤害 |
| `sk_gargantua_dmg_stomp1/2/3` | 践踏伤害 |

#### Headcrab (头蟹)
| 变量 | 描述 |
|------|------|
| `sk_headcrab_health1/2/3` | 生命值 |
| `sk_headcrab_dmg_bite1/2/3` | 咬击伤害 |

#### Hgrunt (人类士兵)
| 变量 | 描述 |
|------|------|
| `sk_hgrunt_health1/2/3` | 生命值 |
| `sk_hgrunt_kick1/2/3` | 踢击伤害 |
| `sk_hgrunt_pellets1/2/3` | 弹丸数量 |
| `sk_hgrunt_gspeed1/2/3` | 手榴弹速度 |

#### Houndeye (犬眼)
| 变量 | 描述 |
|------|------|
| `sk_houndeye_health1/2/3` | 生命值 |
| `sk_houndeye_dmg_blast1/2/3` | 爆炸伤害 |

#### Islave (外星奴隶)
| 变量 | 描述 |
|------|------|
| `sk_islave_health1/2/3` | 生命值 |
| `sk_islave_dmg_claw1/2/3` |爪击伤害 |
| `sk_islave_dmg_clawrake1/2/3` |爪击扫射伤害 |

#### Controller (控制器)
| 变量 | 描述 |
|------|------|
| `sk_controller_health1/2/3` | 生命值 |
| `sk_controller_dmgzap1/2/3` | 电击伤害 |
| `sk_controller_speedball1/2/3` | 球速度 |
| `sk_controller_dmgball1/2/3` | 球伤害 |

#### Nihilanth (尼哈兰特)
| 变量 | 描述 |
|------|------|
| `sk_nihilanth_health1/2/3` | 生命值 |
| `sk_nihilanth_zap1/2/3` | 电击伤害 |

#### Scientist (科学家)
| 变量 | 描述 |
|------|------|
| `sk_scientist_health1/2/3` | 生命值 |

#### Snark (斯纳克)
| 变量 | 描述 |
|------|------|
| `sk_snark_health1/2/3` | 生命值 |
| `sk_snark_dmg_bite1/2/3` | 咬击伤害 |
| `sk_snark_dmg_pop1/2/3` | 爆炸伤害 |

#### Zombie (僵尸)
| 变量 | 描述 |
|------|------|
| `sk_zombie_health1/2/3` | 生命值 |
| `sk_zombie_dmg_one_slash1/2/3` | 单爪伤害 |
| `sk_zombie_dmg_both_slash1/2/3` | 双爪伤害 |

#### Turret (炮塔)
| 变量 | 描述 |
|------|------|
| `sk_turret_health1/2/3` | 生命值 |
| `sk_miniturret_health1/2/3` | 小炮塔生命值 |
| `sk_sentry_health1/2/3` |哨兵炮塔生命值 |

#### Leech (水蛭)
| 变量 | 描述 |
|------|------|
| `sk_leech_dmg_bite1/2/3` | 咬击伤害 |

### 4.3 世界武器伤害

| 变量 | 描述 |
|------|------|
| `sk_12mm_bullet1/2/3` | 12mm子弹伤害 |
| `sk_9mmAR_bullet1/2/3` | 9mmAR子弹伤害 |
| `sk_9mm_bullet1/2/3` | 9mm子弹伤害 |
| `sk_hornet_dmg1/2/3` | 黄蜂伤害 |

### 4.4 健康/充电

| 变量 | 描述 |
|------|------|
| `sk_suitcharger1/2/3` | HEV充电器 |
| `sk_battery1/2/3` | 电池 |
| `sk_healthcharger1/2/3` | 健康充电器 |
| `sk_healthkit1/2/3` | 健康包 |
| `sk_scientist_heal1/2/3` | 科学家治疗 |

### 4.5 伤害部位调整

#### 怪物伤害部位
| 变量 | 描述 |
|------|------|
| `sk_monster_head1/2/3` | 头部伤害倍数 |
| `sk_monster_chest1/2/3` |胸部伤害倍数 |
| `sk_monster_stomach1/2/3` | 腹部伤害倍数 |
| `sk_monster_arm1/2/3` | 手臂伤害倍数 |
| `sk_monster_leg1/2/3` | 腿部伤害倍数 |

#### 玩家伤害部位
| 变量 | 描述 |
|------|------|
| `sk_player_head1/2/3` | 头部伤害倍数 |
| `sk_player_chest1/2/3` |胸部伤害倍数 |
| `sk_player_stomach1/2/3` | 腹部伤害倍数 |
| `sk_player_arm1/2/3` | 手臂伤害倍数 |
| `sk_player_leg1/2/3` | 腿部伤害倍数 |

---

## 五、常用命令示例

### 5.1 游戏设置
```
// 设置鼠标灵敏度
sensitivity 5

// 设置视野角度
default_fov 90

// 启用快速武器切换
hud_fastswitch 1

// 居中显示玩家ID
hud_centerid 1
```

### 5.2 多人游戏设置
```
// 设置时间限制 (分钟)
mp_timelimit 30

// 设置杀敌限制
mp_fraglimit 50

// 启用友军伤害
mp_friendlyfire 1

// 启用手电筒
mp_flashlight 1
```

### 5.3 难度调整
```
// 设置玩家9mm子弹伤害
sk_plr_9mm_bullet1 8// 简单
sk_plr_9mm_bullet2 12// 普通
sk_plr_9mm_bullet3 15// 困难
```

---

## 六、变量标志说明

| 标志 | 描述 |
|------|------|
| `FCVAR_ARCHIVE` | 保存到配置文件 |
| `FCVAR_USERINFO` | 发送到服务器 |
| `FCVAR_SERVER` | 服务器变量，发送到客户端 |
| `FCVAR_UNLOGGED` | 不记录到日志 |

---

*文档生成时间: 2026-06-24*
*基于 Half-Life SDK 源代码整理*