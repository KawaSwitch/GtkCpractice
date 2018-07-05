//チーム８
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>

#define DROP_SPAN 8 // 雫の落ちる時間(ms)
#define WAVE_SPAN 50 // 波が広がる時間(ms)

gboolean draw_expose_callback(GtkWidget *w);
void value_update_callback(GtkSpinButton *s);
gboolean timeout_callback();

void DrawDropAndCircles(int x,int y,int lim,GdkGC* gc);
void GetRandomColorGC(GdkGC **gc);
int CalcNextParam(int color_ratio, int ini_param);
void GetRandomColorByTimer(GdkColor* color);

int wnd_height = 200; // ウィンドウの高さ
int wnd_width = 400; // ウィンドウの幅
int val=10; // スピンウィジェットの値
int timer=0; // 描画更新回数
int dummyA=12, dummyB, dummyC, dummyD;
GtkWidget *l1; // スピンウィジェット
GtkWidget *l2; // 描画ウィジェット
const int draw_span = DROP_SPAN + WAVE_SPAN; // 1つの雫/波 描画時間(ms)
const int MAX_RGB = 65535; // RGBの最大値

int main(int argc, char **argv)
{
  /* printf("%p %p %p\n", &wnd_width, &wnd_height, &val); */
  /* printf("main:%p\n", main); */
  /* printf("timeout_callback:%p\n", timeout_callback); */
  /* printf("gtk_main:%p\n", gtk_main); */

  GtkWidget *win, *table, *button1, *exit, *value;
  int interval = 40; // タイマー処理間隔(ms)

  /* printf("interval変数:%p\n", &interval); */
  /* printf("win変数:%p\n", &win); */

  gtk_init(&argc, &argv);
  srand(time(NULL));

  /* make a window */
  win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  //printf("win変数がさしてる番地(初期化後):%p\n", win);
  g_signal_connect(win,"destroy",G_CALLBACK(gtk_main_quit),NULL);

  l1 = gtk_label_new("Label1");

  /* make a draw area */
  l2 = gtk_drawing_area_new();
  g_signal_connect(l2,"expose_event",G_CALLBACK(draw_expose_callback),NULL);

  exit = gtk_label_new("EXIT");

  button1 = gtk_button_new();
  gtk_container_add(GTK_CONTAINER(button1), exit);

  value = gtk_spin_button_new_with_range(1,20,1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(value),10);

  // テーブル設定
  table = gtk_table_new(100,100,0);
  gtk_container_add(GTK_CONTAINER(win),table);

  //GTK_EXPAND|GTK_SHRINK
  gtk_table_attach(GTK_TABLE(table),l1,24,25,0,1,0,0,0,0);
  gtk_table_attach(GTK_TABLE(table),value,74,75,0,1,0,0,0,0);
  gtk_table_attach(GTK_TABLE(table),l2,0,100,30,60,0,0,0,0);
  gtk_table_attach(GTK_TABLE(table),button1,74,75,99,100,0,0,0,0);

  //Signals
  g_signal_connect(button1,"clicked",G_CALLBACK(gtk_main_quit),NULL);
  g_signal_connect(value,"value-changed",G_CALLBACK(value_update_callback),NULL);

  
  //Show
  gtk_widget_show(win);
  gtk_widget_show(table);
  gtk_widget_show(l1);
  gtk_widget_set_size_request(l2,wnd_width,wnd_height);
  gtk_widget_show(l2);
  gtk_widget_show(button1);
  gtk_widget_show(exit);
  gtk_widget_show(value);

  g_timeout_add(interval,timeout_callback,NULL);

  gtk_main();
  return 0;
}

// 露出コールバック
gboolean draw_expose_callback(GtkWidget *w)
{
  char buffer[256];

  // 描画エリアを背景色でクリア(背景色:黒)
  gdk_draw_rectangle(l2->window, 
		     l2->style->fg_gc[GTK_WIDGET_STATE(l2)],
		     TRUE,
		     0, 0, wnd_width, wnd_height);

  // ラベル設定
  sprintf(buffer,"Value = %d", val);
  gtk_label_set_text(GTK_LABEL(l1),buffer);

  return TRUE;
}

// スピンボタン更新コールバック
void value_update_callback(GtkSpinButton *s)
{
  char buffer[256];
  val = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(s)); 

  // ラベルの値も更新
  sprintf(buffer,"Value = %d", val); 
  gtk_label_set_text(GTK_LABEL(l1),buffer); 
}

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

// 落ちる雫と波紋を描画する
// start:描画開始時間
void DrawDropAndCircles(int x,int y,int start,GdkGC* gc) 
{
  int j;
  int r1, r2; // 波紋の半径
  double pnt1_y, pnt2_y; // 直線描画用y座標
  static const int shift_span = 12; // 第2波目開始のずらす時間(ms)
  
  for(j=0; j <= wnd_width; j++)
    {
      if(timer < draw_span+start && start>0)
	{
	  if(timer < start+DROP_SPAN)
	    {
	      // 落ちる雫の描画
	      gdk_draw_arc(l2->window,gc,TRUE,
			   x + ((timer-start)%DROP_SPAN)*(15/7)-15,
			   y + ((timer-start)%DROP_SPAN)*(15/7)-15,
			   -(((30/7)) * ((timer-start)%DROP_SPAN)) + 30,
			   -(((30/7)) * ((timer-start)%DROP_SPAN)) + 30,
			   0,
			   23040);
	    }
	  else
	    {
	      // 雫が落ちた後の波紋を描画

	      // 第1波
	      r1 = 40 - pow((timer-start-DROP_SPAN)-40,2)/(double)40;
	      if(j<=x+r1-1&&j>=x-r1&&timer<48+start)
		{
		  pnt1_y = pow(r1,2)-pow(j-x,2);
		  pnt2_y = pow(r1,2)-pow(j-x+1,2);
		  gdk_draw_line(l2->window, gc,j,y-sqrt(pnt1_y),j+1,y-sqrt(pnt2_y));
		  gdk_draw_line(l2->window, gc,j,y+sqrt(pnt1_y),j+1,y+sqrt(pnt2_y));
		}

	      // 第2波(第1波より遅くずらす)
	      r2 = 40 - pow((timer-start-DROP_SPAN-shift_span)-40,2)/(double)40;
	      if(j<=x+r2-1 && j>=x-r2 && timer>20+start)
		{
		  pnt1_y = pow(r2,2)-pow(j-x,2);
		  pnt2_y = pow(r2,2)-pow(j-x+1,2);
		  gdk_draw_line(l2->window, gc,j,y-sqrt(pnt1_y),j+1,y-sqrt(pnt2_y));
		  gdk_draw_line(l2->window, gc,j,y+sqrt(pnt1_y),j+1,y+sqrt(pnt2_y));
		}
	    }
	}
    }
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
