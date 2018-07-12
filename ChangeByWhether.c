#include "win.h"

void water_cutter_callback()
{

}

// タイマーの値からランダム色を生成
void GetRandomColorByTimer(GdkColor* color)
{
  // RGB初期値
  static int ini_red, ini_green, ini_blue;
  // 速度調整用(同じ割合で変化しないように互いに素)
  static int c_red = 2, c_green = 3, c_blue = 5;

  int t = abs(timer); // 補正 絶対値をとっておく

  // 初期値をランダムで決める
  if (t == 0)
    {
      color->red = ini_red = rand() % MAX_RGB;
      color->green = ini_green = rand() % MAX_RGB;
      color->blue = ini_blue = rand() % MAX_RGB;
    }

  // RGB値をばらばらの速度で更新する
  color->red = CalcNextParam(c_red, ini_red);
  color->green = CalcNextParam(c_green, ini_green);
  color->blue = CalcNextParam(c_blue, ini_blue);
  // printf("%d %d %d\n", color->red, color->green, color->blue); // デバグ用
}

// ランダム色のグラフィックコンテクスト取得
void GetRandomColorGC(GdkGC **gc)
{
  static GdkColor color; // 色構造体

  if (*gc == NULL)
    *gc = gdk_gc_new(l2->window);

  GetRandomColorByTimer(&color);
  gdk_gc_set_rgb_fg_color(*gc, &color);
}

// 次のパラメータを計算する
// color_ratio:速度比率
int CalcNextParam(int color_ratio, int ini_param)
{
  static const int r = 5000; // 速度調整用定数
  int param = (timer / color_ratio) * r + ini_param;

  // スペクトル幅を綺麗に折り返すよう計算
  if (MAX_RGB - param % (2 * MAX_RGB) > 0)
    return param % MAX_RGB;
  else
    return MAX_RGB - param % MAX_RGB;
}
