echo
echo $(date)
echo
echo xiix coverage tests




#- ------ --- -- -- - -- -- - -- - - -- -- - -- - - - - -- - - -- - - -- -- - -- ---- -- -- ---
echo ' * small file' &&
../xiix localhost 8088 / > cmp &&
diff cmp t01.cmp &&
rm cmp &&

#- ------ --- -- -- - -- -- - -- - - -- -- - -- - - - - -- - - -- - - -- -- - -- ---- -- -- ---
echo ' * bigger file' &&
../xiix localhost 8088 /qa/t16.cmp > cmp &&
diff cmp t02.cmp &&
rm cmp &&

#- ------ --- -- -- - -- -- - -- - - -- -- - -- - - - - -- - - -- - - -- -- - -- ---- -- -- ---
echo ' * chunked small' &&
../xiix localhost 8088 /?chunked > cmp &&
diff cmp t03.cmp &&
rm cmp &&

#- ------ --- -- -- - -- -- - -- - - -- -- - -- - - - - -- - - -- - - -- -- - -- ---- -- -- ---
echo ' * chunked big' &&
../xiix localhost 8088 /?chunkedbig > cmp &&
diff cmp t04.cmp &&
rm cmp &&

#- ------ --- -- -- - -- -- - -- - - -- -- - -- - - - - -- - - -- - - -- -- - -- ---- -- -- ---
echo ' * chunked bigger' &&
../xiix localhost 8088 /?chunkedbigger > cmp &&
diff cmp t05.cmp &&
rm cmp &&

#- ------ --- -- -- - -- -- - -- - - -- -- - -- - - - - -- - - -- - - -- -- - -- ---- -- -- ---





echo
echo $(date)
