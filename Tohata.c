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
