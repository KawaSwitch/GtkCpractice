// チーム８
#include "win.h"

int wnd_height = 200; // ウィンドウの高さ
int wnd_width = 400; // ウィンドウの幅
int val=10; // スピンウィジェットの値
int timer=0; // 描画更新回数
GtkWidget *wl1, *wl2; // 天気ラベルウィジェット
GtkWidget *l2; // 描画ウィジェット
const int draw_span = DROP_SPAN + WAVE_SPAN; // 1つの雫/波 描画時間(ms)
const int MAX_RGB = 65535; // RGBの最大値
char buf[256] = "";
char *weather = NULL;
int reverse=0;

int main(int argc, char **argv)
{
  GtkWidget *win, *table, *button1, *exit, *value;
  GtkWidget *ch, *button5; 

  int interval = 40; // タイマー処理間隔(ms)

  gtk_init(&argc, &argv);
  srand(time(NULL));

  /* make a window */
  win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(win,"destroy",G_CALLBACK(gtk_main_quit),NULL);

  // 天気ウィジェット設定
  wl1 = gtk_label_new("Label1");
  wl2 = gtk_label_new("Label2");

  /* make a draw area */
  l2 = gtk_drawing_area_new();
  g_signal_connect(l2,"expose_event",G_CALLBACK(draw_expose_callback),NULL);

  exit = gtk_label_new("EXIT");

  button1 = gtk_button_new();
  gtk_container_add(GTK_CONTAINER(button1), exit);

  // スピンボタン設定
  value = gtk_spin_button_new_with_range(1,20,1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(value),10);

  // テーブル設定
  table = gtk_table_new(100,100,0);
  gtk_container_add(GTK_CONTAINER(win),table);

  //GTK_EXPAND|GTK_SHRINK
  gtk_table_attach(GTK_TABLE(table),wl1,24,25,0,1,0,0,0,0);
  gtk_table_attach(GTK_TABLE(table),wl2,74,75,0,1,0,0,0,0);
  //gtk_table_attach(GTK_TABLE(table),value,15,16,99,100,0,0,0,0);
  gtk_table_attach(GTK_TABLE(table),l2,0,100,30,60,0,0,0,0);
  gtk_table_attach(GTK_TABLE(table),button1,74,75,99,100,0,0,0,0);
  
  //Signals
  g_signal_connect(button1,"clicked",G_CALLBACK(gtk_main_quit),NULL);
  g_signal_connect(value,"value-changed",G_CALLBACK(value_update_callback),NULL);

  //追加
  ch = gtk_label_new("Reverse");
  button5 = gtk_button_new();
  gtk_container_add(GTK_CONTAINER(button5), ch);
  gtk_table_attach(GTK_TABLE(table),button5,45,46,99,100,0,0,0,0);
  g_signal_connect(button5,"clicked",G_CALLBACK(reverse_clicked_callback),NULL);
  


  //Show
  gtk_widget_show(win);
  gtk_widget_show(table);
  gtk_widget_show(wl1);
  gtk_widget_show(wl2);
  gtk_widget_set_size_request(l2,wnd_width,wnd_height);
  gtk_widget_show(l2);
  gtk_widget_show(button1);
  gtk_widget_show(ch);
  gtk_widget_show(button5);
  gtk_widget_show(exit);
  gtk_widget_show(value);
  
  g_timeout_add(interval,timeout_callback,NULL);

  gtk_main();
  return 0;
}

// タイムアウトコールバック
gboolean timeout_callback()
{
  int i;
  char* cbuf;
  static int start_time[48][2]; // 各波(雫&波)の描画開始時間
  static int cur_start_idx=0, cur_end_idx=0; // 現在の描画インデックス(開始/終了用)
  static int centerX[48], centerY[48]; // 中心座標群

  if (timer == 1)
    {
      int buf_size_max = 1024;
      weather = (char *)malloc(sizeof(char) * buf_size_max);
      GetTomorrowWeather(buf, &weather);

      // 不要な最後の改行を削除
      cbuf = strchr(weather, '\n');
      if (cbuf != NULL) *cbuf = '\0';
      
      // 天気ラベル設定
      gtk_label_set_text(GTK_LABEL(wl1), "長崎市の明日の天気");
      gtk_label_set_text(GTK_LABEL(wl2), weather);

      // 天気から時間間隔を決定
      val = ConvertWeatherToWaitSpan(weather);

      /* switch(atoi(weather)) */
      /* 	{ */
      /* 	case 1: */
      /* 	  gtk_label_set_text(GTK_LABEL(wl2), "晴れ"); */
      /* 	  break; */
      /* 	case 2: */
      /* 	  gtk_label_set_text(GTK_LABEL(wl2), "曇り"); */
      /* 	  break; */
      /* 	case 3: */
      /* 	  gtk_label_set_text(GTK_LABEL(wl2), "雨"); */
      /* 	  break; */
      /* 	case 4: */
      /* 	  gtk_label_set_text(GTK_LABEL(wl2), "曇り"); */
      /* 	  break; */
      /* 	default: */
      /* 	  gtk_label_set_text(GTK_LABEL(wl2), "定義なし"); */
      /* 	  break; */
      /* 	} */

      free(weather);
    }

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
  if(timer%val==0 && start_time[cur_start_idx][0]==0)
    {
      start_time[cur_start_idx][0] = timer;
      start_time[cur_start_idx][1] = reverse;
      if(++cur_start_idx == draw_span) 
	cur_start_idx = 0;
    }

  // 色取得
  static GdkGC* gc = NULL;
  GetRandomColorGC(&gc);  

  // 波を描画
  for(i = 0; i < draw_span; i++)
    {
      if(start_time[i][1]%2==0)
      	{DrawDropAndCircles(centerX[i],centerY[i],start_time[i][0],gc);}
      if(start_time[i][1]%2==1)
	{DrawReverseCircles(centerX[i],centerY[i],start_time[i][0],gc);}
    }
  
  // 描画開始から時間の経った波をクリア
  if(start_time[cur_end_idx][0]+draw_span == timer)
    { 
      start_time[cur_end_idx][0] = 0;
      if(++cur_end_idx == draw_span)
	cur_end_idx = 0;
    }

  timer++;
  return TRUE;
}

// 露出コールバック
gboolean draw_expose_callback(GtkWidget *w)
{
  // 描画エリアを背景色でクリア(背景色:黒)
  gdk_draw_rectangle(l2->window, 
		     l2->style->fg_gc[GTK_WIDGET_STATE(l2)],
		     TRUE,
		     0, 0, wnd_width, wnd_height);

  return TRUE;
}

// スピンボタン更新コールバック
void value_update_callback(GtkSpinButton *s)
{
  //val = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(s));
}

// Reverseボタンクリックコールバック
void reverse_clicked_callback()
{
  reverse++;
}

