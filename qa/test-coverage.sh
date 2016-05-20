echo
echo $(date)
echo
echo xiix coverage tests




#- ------ --- -- -- - -- -- - -- - - -- -- - -- - - - - -- - - -- - - -- -- - -- ---- -- -- ---
echo ' * small file'
../xiix localhost 8088 / 1 -c > cmp &&
diff cmp t01.cmp &&
rm cmp &&

#- ------ --- -- -- - -- -- - -- - - -- -- - -- - - - - -- - - -- - - -- -- - -- ---- -- -- ---
echo ' * bigger file'
../xiix localhost 8088 /qa/t16.cmp 1 -c > cmp
diff cmp t02.cmp &&
rm cmp &&
#- ------ --- -- -- - -- -- - -- - - -- -- - -- - - - - -- - - -- - - -- -- - -- ---- -- -- ---





echo
echo $(date)
