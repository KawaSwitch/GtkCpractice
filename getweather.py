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
#print(d)
# print(d['title'])
# いろいろ表示
for f in d['forecasts']:
    # f(orecast)
    if f['dateLabel'] == u"明日":
        print f['telop'].encode('utf-8')
        # if u"晴" in f['telop']:
        #     print u"晴れ".encode('utf-8')
        # elif f['telop'] == u"曇り":
        #     print 2
        # elif f['telop'] == u"雨":
        #     print 3
        # elif f['telop'] == u"晴":
        #     print 4
        # elif f['telop'] == u"晴のち曇":
        #     print 5,
        # elif f['telop'] == u"曇のち晴":
        #     print 6,
        # elif f['telop'] == u"晴れ":
        #     print 2,
                            
        # else: #定義なし
        #     print 0

