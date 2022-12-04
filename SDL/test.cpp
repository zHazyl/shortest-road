#include "CommonFunction.h"
#include "BaseObject.h"
#include "game_map.h"
#include "TextObject.h"

BaseObject g_background;
TTF_Font* font = NULL;

bool InitData() {
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0)
		return false;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	g_window = SDL_CreateWindow("map_ToanRR",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (g_window == NULL)
		success = false;
	else {
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL)
			success = false;
		else {
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags))
				success = false;
		}

		if (TTF_Init() == -1)
			success = false;

		font = TTF_OpenFont("font//dlxfont_.ttf", 15);
		if (font == NULL)
			success = false;
	}

	return success;
}

bool LoadBackground() {
	bool ret = g_background.LoadImg("img//background.png", g_screen);
	if (ret == false)
		return false;

	return true;
}

void close() {
	g_background.Free();

	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	IMG_Quit();
	SDL_Quit();
}

struct PosRoad {
	int huong = 0;
	int chung = 0;
	int s = 0;
	int t = 0;
};

int r = 0;

int color = 2;

int main(int argc, char* argv[]) {

	int n = 100; // So dinh cua do thi
	int** G; // Ma tran trong so cua do thi
	NhapDoThi(n, G);
	int** D = new int* [n]; // Ma tran chua cac gia tri khoang cach ngan nhat tu i => j
	int** S = new int* [n]; // Ma tran chua gia tri phan tu ngay sau cua i tren duong di ngan nhat tu i => j
	Floyd(n, G, D, S);

	int s, t, _s;

	PosRoad pr[101][101]{};
	ifstream ifs("PosRoad.txt", ios::in);
	int edgeNum;
	ifs >> edgeNum;
	int sr, tr;
	for (int i = 0; i < edgeNum; ++i) {
		ifs >> sr >> tr;
		ifs >> pr[sr][tr].huong;
		ifs >> pr[sr][tr].chung;
		ifs >> pr[sr][tr].s;
		ifs >> pr[sr][tr].t;
	}
	ifs.close();


	if (InitData() == false)
		return -1;

	if (LoadBackground() == false)
		return -1;

	GameMap game_map;
	game_map.LoadMap("map/map01.dat");
	//game_map.LoadTiles(g_screen);
	int load = 0;


	//text
	const int kMenuItemNum = 64;
	SDL_Rect pos_arr[kMenuItemNum];
	char val[kMenuItemNum][2];
	FILE* fp = NULL;
	fopen_s(&fp, "pos_arr.txt", "rb");
	if (fp == NULL)
		return 0;

	for (int i = 0; i < kMenuItemNum; ++i) {
		fscanf_s(fp, "%d", &pos_arr[i].x);
		fscanf_s(fp, "%d", &pos_arr[i].y);
		pos_arr[i].w = 32;
		pos_arr[i].h = 32;
	}

	fclose(fp);

	TextObject text_menu[kMenuItemNum];
	char point[3];
	point[2] = '\0';

	TextObject dis;

	bool selected[kMenuItemNum]{};
	SDL_Event m_event;
	int xm = 0;
	int ym = 0;

	int x = -1;
	int y = -1;

	SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
	SDL_RenderClear(g_screen);

	g_background.Render(g_screen, NULL);

	bool is_quit = false;
	while (!is_quit) {
		//while (SDL_PollEvent(&g_event) != 0) {
		//	if (g_event.type == SDL_QUIT)
		//		is_quit = true;
		//	SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		//	SDL_RenderClear(g_screen);

		//	g_background.Render(g_screen, NULL);

		//	//game_map.DrawMap(g_screen);

		//	//Show text


		//}

		//for (int i = 0; i < kMenuItemNum; ++i) {
		//	point[0] = ((i + 1) / 10) + '0';
		//	point[1] = ((i + 1) % 10) + '0';
		//	text_menu[i].SetText(point);
		//	text_menu[i].LoadFromRenderText(font, g_screen);
		//	text_menu[i].RenderText(g_screen, pos_arr[i].x, pos_arr[i].y);
		//}

		while (SDL_PollEvent(&m_event)) {
			switch (m_event.type) {
			case SDL_QUIT:
				return 0;
			case SDL_MOUSEMOTION: {
				xm = m_event.motion.x;
				ym = m_event.motion.y;

				for (int i = 0; i < kMenuItemNum; ++i) {
					if (selected[i] == 0) {
						if (CheckFocusWithRect(xm, ym, pos_arr[i])) {
							/*if (selected[i] == false) {
								selected[i] = 1;*/
							text_menu[i].SetColor(TextObject::BLACK_TEXT);
							//}
						}
						else {
							/*if (selected[i] == true) {
								selected[i] = 0;*/
							text_menu[i].SetColor(TextObject::WHITE_TEXT);
							//}
						}
					}
					else {
						text_menu[i].SetColor(TextObject::BLACK_TEXT);
					}
				}
				break;
			}
			case SDL_MOUSEBUTTONDOWN: {
				xm = m_event.button.x;
				ym = m_event.button.y;
				for (int i = 0; i < kMenuItemNum; ++i) {
					if (CheckFocusWithRect(xm, ym, pos_arr[i])) {
						if (i <= 60) {
							if (selected[i] == 0) {
								selected[i] = 1;
								if (y == -1 && x != -1)
									y = i + 1;
								if (x == -1)
									x = i + 1;
							}
						}
						else {
							selected[i] = 1;
							if (i == 63) {
								x = -1;
								y = -1;
								r = 0;
								color = 2;
								SDL_RenderClear(g_screen);
								g_background.Render(g_screen, NULL);
								game_map.LoadMap("map/map01.dat");
								game_map.LoadTiles(g_screen);
								//SDL_RenderPresent(g_screen);
								for (int i = 0; i < kMenuItemNum; ++i)
									selected[i] = 0;
							}
						}
					}
				}
				break;
			}

			case SDL_KEYDOWN: {
				if (m_event.key.keysym.sym == SDLK_ESCAPE) {
					x = -1;
					y = -1;
					load = 0;
					color++;
				}
				break;
			}
						defalt: break;
			}
		}
		if (x != -1 && y != -1) {
			if (load == 0) {
				s = x;
				t = y;
				int spr, tpr;
				while (s != y)
				{
					t = S[s][y];
					spr = pr[s][t].s;
					tpr = pr[s][t].t;
					if (spr > tpr)
						swap(spr, tpr);
					for (int i = spr; i <= tpr; ++i) {
						if (pr[s][t].huong == 0) {
							game_map.GetGameMap().tile[i][pr[s][t].chung] = color;
						}
						else
							game_map.GetGameMap().tile[pr[s][t].chung][i] = color;
					}
					s = S[s][y];
				}
				game_map.LoadTiles(g_screen);
				load = 1;
				r++;
			}
			//game_map.DrawMap(g_screen);
		}

		if (SDL_PollEvent(&g_event) != 0 /*|| load == 1*/) {
			if (g_event.type == SDL_QUIT)
				is_quit = true;
			for (int i = 0; i < kMenuItemNum; ++i) {
				if (i <= 60) {
					point[0] = ((i + 1) / 10) + '0';
					point[1] = ((i + 1) % 10) + '0';
					text_menu[i].SetText(point);
					text_menu[i].LoadFromRenderText(font, g_screen);
					text_menu[i].RenderText(g_screen, pos_arr[i].x, pos_arr[i].y);
				}
				else {
					text_menu[i].SetText("O");
					text_menu[i].LoadFromRenderText(font, g_screen);
					text_menu[i].RenderText(g_screen, pos_arr[i].x, pos_arr[i].y);
				}
			}

			if (x != -1 && y != -1) {
				std::string d = "Distance: ";
				char p = (D[x][y] - 1) / 10 + '0';
				d = d + p;
				p = (D[x][y] - 1) % 10 + '0';
				d = d + p;
				dis.SetText(d);
				dis.SetColor(TextObject::BLACK_TEXT);
				dis.LoadFromRenderText(font, g_screen);
				dis.RenderText(g_screen, SCREEN_WIDTH - 190, 400 + r * 30);
			}

			game_map.DrawMap(g_screen);
			SDL_RenderPresent(g_screen);
		}
	}
	close();
	return 0;
}