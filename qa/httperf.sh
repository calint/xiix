httperf --hog --server localhost --port 8088 --print-request --print-reply --uri=/?chunked --num-calls=1000 --num-conns=100
