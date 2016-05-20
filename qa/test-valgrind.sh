echo
echo $(date)
echo xiix memory leak check
echo
valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all ../xiix localhost 8088 / 5 -q
echo
echo $(date)