#!/usr/bin/env python
#encoding:utf-8
import requests as r
import json as j

# パッケージ読み込み
url = 'http://weather.livedoor.com/forecast/webservice/'
loc = '420010'
# 地域番号
res = r.get(url+'json/v1?city='+loc).text
#print(url+'json/v1?city='+loc)

# 読み込んだJSONフォーマットの文字列を辞書型に変換
d = j.loads(res) # パッケージ.関数 〜 オブジェクト.関数
print(d)
# print(d['title'])
# いろいろ表示
for f in d['forecasts']:
    # f(orecast)
    if f['dateLabel'] == u"明日":
        if f['telop'] == u"晴れ":
            print 0,
        else:
            print 1,
    
