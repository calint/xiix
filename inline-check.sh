g++ -O3 -S src/main.cpp && cat main.s|grep call|sort -u && rm main.s
