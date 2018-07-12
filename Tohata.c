#include "win.h"

// タイムアウトコールバック
gboolean timeout_callback()
{
  int i;
  static int start_time[58]; // 各波(雫&波)の描画開始時間
  static int cur_start_idx=0, cur_end_idx=0; // 現在の描画インデックス(開始/終了用)
  static int centerX[58], centerY[58]; // 中心座標群

  // 新しい雫の着地点(波の中心)座標を設定する
  if(timer%val == 0)
    {
      // ウィンドウ内ランダム
      centerX[cur_start_idx] = rand() % wnd_width;
      centerY[cur_start_idx] = rand() % wnd_height;
    }

  // 描画エリアを背景色でクリア(背景色:黒)
  gdk_draw_rectangle(l2->window,
  		     l2->style->fg_gc[GTK_WIDGET_STATE(l2)],
  		     TRUE,
  		     0, 0, wnd_width, wnd_height);
 
  // 新しい波を追加する
  if(timer%val==0 && start_time[cur_start_idx]==0)
    {
      start_time[cur_start_idx] = timer;
      if(++cur_start_idx == draw_span) 
	cur_start_idx = 0;
    }

  // 色取得
  static GdkGC* gc = NULL;
  GetRandomColorGC(&gc);

  // 波を描画
  for(i = 0; i < draw_span; i++)
    DrawDropAndCircles(centerX[i],centerY[i],start_time[i],gc);
      
  // 描画開始から時間の経った波をクリア
  if(start_time[cur_end_idx]+draw_span == timer)
    { 
      start_time[cur_end_idx] = 0;
      if(++cur_end_idx == draw_span)
	cur_end_idx = 0;
    }

  timer++;
  return TRUE;
}

// タイマーの値からランダム色を生成
void GetBGColor(GdkColor* color)
{
  // RGB初期値
  static int ini_red, ini_green, ini_blue;
  // 速度調整用(同じ割合で変化)
  static int c_red = 5, c_green = 5, c_blue = 5;

  int t = abs(timer); // 補正 絶対値をとっておく

  // 初期値をランダムで決める
  if (t == 0)
    {
      color->red = ini_red = 0;
      color->green = ini_green = 0;
      color->blue = ini_blue = 0;
    }

  // RGB値をばらばらの速度で更新する
  color->red = CalcNextParam(c_red, ini_red);
  color->green = CalcNextParam(c_green, ini_green);
  color->blue = CalcNextParam(c_blue, ini_blue);
  // printf("%d %d %d\n", color->red, color->green, color->blue); // デバグ用
}

// 背景色のグラフィックコンテクスト取得
void GetBGColorGC(GdkGC **gc)
{
  static GdkColor color; // 色構造体

  if (*gc == NULL)
    *gc = gdk_gc_new(l2->window);

  GetBGColor(&color);
  gdk_gc_set_rgb_fg_color(*gc, &color);
}

void ChangeBGColor(GdkGC **gc)
{
  gdk_draw_rectangle(l2->window,
  		     l2->style->fg_gc[GTK_WIDGET_STATE(l2)],
  		     TRUE,
  		     0, 0, wnd_width, wnd_height);
}
