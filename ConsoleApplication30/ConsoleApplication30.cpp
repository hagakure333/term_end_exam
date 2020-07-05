#include "DxLib.h"
#include <stdio.h>

//キー取得用の配列
char buf[256] = { 0 };
int keyState[256] = { 0 };


//キー入力状態を更新する関数
void KeyUpdate()
{
	GetHitKeyStateAll(buf);
	for (int i = 0; i < 256; i++)
	{
		if (buf[i] == 1) keyState[i]++;
		else keyState[i] = 0;
	}
}

int x = 100;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	ChangeWindowMode(TRUE);//非全画面にセット
	SetGraphMode(640, 480, 32);//画面サイズ指定
	SetOutApplicationLogValidFlag(FALSE);//Log.txtを生成しないように設定
	if (DxLib_Init() == 1) { return -1; }//初期化に失敗時にエラーを吐かせて終了

	while (ProcessMessage() == 0)
	{
		ClearDrawScreen();//裏画面消す
		SetDrawScreen(DX_SCREEN_BACK);//描画先を裏画面に

		KeyUpdate();//キー入力状態を更新する（自作関数）

		//keyState配列には、各キーが何フレーム押され続けているかが入っている
		DrawFormatString(100, 100, GetColor(255, 255, 255), "Z KEY %d", keyState[KEY_INPUT_Z]);
		DrawFormatString(100, 120, GetColor(255, 255, 255), "X KEY %d", keyState[KEY_INPUT_X]);

		if (keyState[KEY_INPUT_Z] > 0) {
			x += 1;
		}
		if (keyState[KEY_INPUT_X] == 1) {
			x += 30;
		}

		DrawCircle(x, 200, 32, GetColor(255, 0, 0));

		ScreenFlip();//裏画面を表画面にコピー
	}

	DxLib_End();
	return 0;
}