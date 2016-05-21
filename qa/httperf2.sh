httperf --hog --server localhost --port 8088 --print-request --print-reply --uri=/?chunked --wsess=10,100,1 --rate=0 --session-cookie --burst-length=100
