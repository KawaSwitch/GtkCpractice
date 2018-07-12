
#include "win.h"

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
      if(timer < draw_span+start && start>0 && reverse%2==0)
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
void DrawReverseCircles(int x,int y,int start,GdkGC* gc) 
{
  int j;
  int r1, r2; // 波紋の半径
  double pnt1_y, pnt2_y; // 直線描画用y座標
  static const int shift_span = 12; // 第2波目開始のずらす時間(ms)
  
  for(j=0; j <= wnd_width; j++)
    {
      if(timer < draw_span+start && start>0 && reverse%2==0)
	{
	  if(timer < start+DROP_SPAN)
	    {
	      // 落ちる雫の描画
	      gdk_draw_arc(l2->window,gc,TRUE,
			   x + ((-timer+start)%DROP_SPAN)*(15/7)-15,
			   y + ((-timer+start)%DROP_SPAN)*(15/7)-15,
			   -(((30/7)) * ((-timer+start)%DROP_SPAN)) + 30,
			   -(((30/7)) * ((-timer+start)%DROP_SPAN)) + 30,
			   0,
			   23040);
	    }
	  else
	    {
	      // 雫が落ちた後の波紋を描画

	      // 第1波
	      r1 = 40 - pow((timer-start-DROP_SPAN)-,2)/(double)40;
	      if(j<=x+r1-1&&j>=x-r1&&timer<48+start)
		{
		  pnt1_y = pow(r1,2)-pow(j-x,2);
		  pnt2_y = pow(r1,2)-pow(j-x+1,2);
		  gdk_draw_line(l2->window, gc,j,y-sqrt(pnt1_y),j+1,y-sqrt(pnt2_y));
		  gdk_draw_line(l2->window, gc,j,y+sqrt(pnt1_y),j+1,y+sqrt(pnt2_y));
		}

	      // 第2波(第1波より遅くずらす)
	      r2 = 40 - pow((timer-start-DROP_SPAN-shift_span),2)/(double)40;
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

//追加
  /* ch = gtk_label_new("反転"); */
  /* button1 = gtk_button_new(); */
  /* gtk_container_add(GTK_CONTAINER(button5), ch); */
  /* g_signal_connect(button5,"clicked",G_CALLBACK(change_ripple-shape),NULL); */
  /* gtk_table_attach(GTK_TABLE(table),button5,0,75,25,100,0,0,0,0); */
