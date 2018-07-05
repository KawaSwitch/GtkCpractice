//チーム８
#include "win.h"
/* #include <gtk/gtk.h> */
/* #include <math.h> */
/* #include <stdlib.h> */

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

