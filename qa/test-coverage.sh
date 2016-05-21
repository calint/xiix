echo
echo $(date)
echo
echo xiix coverage tests




#- ------ --- -- -- - -- -- - -- - - -- -- - -- - - - - -- - - -- - - -- -- - -- ---- -- -- ---
echo ' * small file' &&
../xiix localhost 8088 / > cmp &&
diff -q cmp t01.cmp &&
rm cmp &&

#- ------ --- -- -- - -- -- - -- - - -- -- - -- - - - - -- - - -- - - -- -- - -- ---- -- -- ---
echo ' * bigger file' &&
../xiix localhost 8088 /qa/t16.cmp > cmp &&
diff -q cmp t02.cmp &&
rm cmp &&

#- ------ --- -- -- - -- -- - -- - - -- -- - -- - - - - -- - - -- - - -- -- - -- ---- -- -- ---
echo ' * chunked small' &&
../xiix localhost 8088 /?chunked > cmp &&
diff -q cmp t03.cmp &&
rm cmp &&

#- ------ --- -- -- - -- -- - -- - - -- -- - -- - - - - -- - - -- - - -- -- - -- ---- -- -- ---
echo ' * chunked big' &&
../xiix localhost 8088 /?chunkedbig > cmp &&
diff -q cmp t04.cmp &&
rm cmp &&

#- ------ --- -- -- - -- -- - -- - - -- -- - -- - - - - -- - - -- - - -- -- - -- ---- -- -- ---
echo ' * chunked bigger' &&
../xiix localhost 8088 /?chunkedbigger > cmp &&
gunzip -k t05.cmp.gz &&
diff -q cmp t05.cmp &&
rm cmp &&
rm t05.cmp &&

#- ------ --- -- -- - -- -- - -- - - -- -- - -- - - - - -- - - -- - - -- -- - -- ---- -- -- ---





echo&&echo $(date)&&echo
