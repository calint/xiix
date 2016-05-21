#httperf --hog --server localhost --port 8088 --print-request --print-reply --uri=/?chunked --num-calls=1000 --num-conns=100
httperf --hog --server localhost --port 8088 --uri=/?chunked --num-calls=1000 --num-conns=10
httperf --hog --server localhost --port 8088 --uri=/?chunkedbig --num-calls=1000 --num-conns=10
httperf --hog --server localhost --port 8088 --uri=/?chunkedbigger --num-calls=1000 --num-conns=10
