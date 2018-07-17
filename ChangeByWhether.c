#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "win.h"

static const int BUF_SIZE = 1024;
static const int READ = 0;
static const int WRITE = 1;

// ある地域の明日の天気を取得する
void GetTomorrowWeather(char* location, char **weather)
{
  int pipefd[2];
  int p_id;
  int status;
  FILE *fp;
  char buff[BUF_SIZE];

  if (pipe(pipefd) < 0) // パイプ生成
    {
      perror("GetTomorrwowWhether内");
      exit(EXIT_FAILURE);
    }
  if ((p_id = fork()) < 0) // 子プロセスの生成
    {
      perror("GetTomorrwowWeather内");

      //パイプを閉じて終了
      close(pipefd[READ]);
      close(pipefd[WRITE]);
      exit(EXIT_FAILURE);
    }

  // 子から親へ天気情報を送る
  if (p_id == 0) // 子プロセス
    {
      close(pipefd[READ]); // 子は読み込まないので閉じる

      // ファイル記述子を標準出力に割り当て
      dup2(pipefd[WRITE], 1);
      close(pipefd[WRITE]);

      // pythonファイルを実行しデータを書き込む
      if (execl("./getweather.py", "./getweather.py", (char *)0) < 0)
	{
	  perror("popen2");
	  return;
	}	

      close(pipefd[WRITE]);
    }
  else // 親プロセス
    {
      close(pipefd[WRITE]); // 親は書き込まないので閉じる

      read(pipefd[READ], buff, BUF_SIZE);
      strcpy(*weather,buff);    

      close(pipefd[READ]);

      // 子プロセス終了
      //kill(p_id, SIGINT);
      wait(&status);
    }
}

// 次の雫が落ちるまでの時間間隔を取得
int ConvertWeatherToWaitSpan(char* weather)
{
  if (!strcmp(weather, "晴れ"))
    return 30;
  else if (!strcmp(weather, "雨"))
    return 2;
  else if (!strcmp(weather, "曇り"))
    return 20;
  else
    return 1; // とりあえず
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
