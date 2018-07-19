#include "win.h"

// タイマーの値からランダム色を生成
void GetBGColor(GdkColor* color)
{
  // RGB初期値
  static int ini_red, ini_green, ini_blue;
  // 速度調整用(同じ割合で変化)
  static int c_red = 2, c_green = 2, c_blue = 2;

  int t = abs(timer); // 補正 絶対値をとっておく

  // 初期値をランダムで決める
  if (t == 0)
    {
      color->red = ini_red = 0;
      color->green = ini_green = 0;
      color->blue = ini_blue = 0;
    }

  // RGB値をばらばらの速度で更新する
  color->red = CalcNextParamBG(c_red, ini_red);
  color->green = CalcNextParamBG(c_green, ini_green);
  color->blue = CalcNextParamBG(c_blue, ini_blue);
  // printf("%d %d %d\n", color->red, color->green, color->blue); // 背景色デバグ用
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

// 次のパラメータを計算する
int CalcNextParamBG(int color_ratio, int ini_param)
{
  static const int r = 500; // 速度調整用定数
  int param = (timer / color_ratio) * r + ini_param;

  // スペクトル幅を綺麗に折り返すよう計算
  if (MAX_RGB - param % (2 * MAX_RGB) > 0)
    return param % MAX_RGB;
  else
    return MAX_RGB - param % MAX_RGB;
}
