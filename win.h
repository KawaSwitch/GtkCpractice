//チーム８
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>

#define DROP_SPAN 8 // 雫の落ちる時間(ms)
#define WAVE_SPAN 50 // 波が広がる時間(ms)

extern gboolean timeout_callback();
extern gboolean draw_expose_callback(GtkWidget *w);
extern void value_update_callback(GtkSpinButton *s);

extern void DrawDropAndCircles(int x,int y,int lim,GdkGC* gc);
extern void GetRandomColorGC(GdkGC **gc);
extern int CalcNextParam(int color_ratio, int ini_param);
extern void GetRandomColorByTimer(GdkColor* color);

extern int wnd_height; // ウィンドウの高さ
extern int wnd_width; // ウィンドウの幅
extern int val; // スピンウィジェットの値
extern int timer; // 描画更新回数
extern int dummyA, dummyB, dummyC, dummyD;
extern GtkWidget *l1; // スピンウィジェット
extern GtkWidget *l2; // 描画ウィジェット
extern const int draw_span; // 1つの雫/波 描画時間(ms)
extern const int MAX_RGB; // RGBの最大値