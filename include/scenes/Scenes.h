#ifndef LOTUS_SCENES_H
#define LOTUS_SCENES_H

#define SCENE_TITLE "title"
#define SCENE_MENU "menu"
//#define SCENE_GAME "game"
#define SCENE_GAME "default"

#define UI_MAIN_MENU "root_main_menu"
#define UI_CONNECT_MENU "root_connect_menu"
#define UI_LOBBY_MENU "root_lobby_menu"
#define UI_GAME "root_game"
#define UI_VICTORY "root_victory"

void addMenuUI();
void addGameUI();

void addTitleScene();
void addMenuScene();
void addGameScene();

#endif //LOTUS_SCENES_H
