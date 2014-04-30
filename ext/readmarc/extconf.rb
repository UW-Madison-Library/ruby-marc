require 'mkmf'

dir_config('readmarc')
have_header('stdio.h') 
have_header('stdlib.h')
have_header('ctype.h')
have_header('string.h')
have_header('errno.h')
have_header('unistd.h')
abort 'missing isdigit()' unless have_func 'isdigit'
abort 'missing memcpy()' unless have_func 'memcpy'
abort 'missing malloc()' unless have_func 'malloc'
abort 'missing atoi()' unless have_func 'atoi'
abort 'missing fseek()' unless have_func 'fseek'
abort 'missing fread()' unless have_func 'fread'

create_makefile 'readmarc/readmarc'
