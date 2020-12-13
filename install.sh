rm errorhook.txt
rm errorfilter.txt
apxs -c -i -a mod_redirect.c >> errorhook.txt 2>&1
apxs -c -i -a mod_outputfilter.c >> errorfilter.txt 2>&1
systemctl restart apache2


