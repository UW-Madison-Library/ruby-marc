#include <ruby.h>
#include <ruby/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#define BBUFSZ    99999
#define MAX_FILES    64

struct RECORD {
    int len;
    char txt[BBUFSZ];
};

static VALUE mMARC = Qnil;
static VALUE cMARCReader = Qnil;
static VALUE MARC_nextmarc(VALUE self, VALUE file);
static VALUE MARC_size(VALUE self);
static VALUE MARC_test(VALUE self, VALUE str);
char tt[100];

void Init_marc();
static struct RECORD *next_marc(FILE *fp);
static char *buf_read(FILE *fp, long off, int flen);
static char *test_string(char *s);

/* Ruby bindings */

void Init_readmarc() {
    mMARC = rb_define_module("MARC");
    cMARCReader = rb_define_class_under(mMARC, "Reader", rb_cObject);
    rb_define_method(cMARCReader, "nextmarc", MARC_nextmarc, 1);
    rb_define_method(cMARCReader, "marcsize", MARC_size, 0);
    rb_define_method(cMARCReader, "test", MARC_test, 1);
}

static VALUE MARC_nextmarc(VALUE self, VALUE file) {
    struct RECORD *r = NULL;
    rb_io_t *fptr;
    FILE *fp;
    VALUE obj;

    if (TYPE (file) != T_FILE) {
        rb_raise (rb_eTypeError, "must provide a file handle.");
    }
    GetOpenFile(file, fptr);
    fp = rb_io_stdio_file(fptr);

    obj = Data_Wrap_Struct(self, NULL, -1, r);
    r = next_marc(fp);

    return obj;
}

static VALUE MARC_size(VALUE self) {
    struct RECORD *r;
    Data_Get_Struct(self, struct RECORD, r);

    return INT2NUM(r->len);
}

static VALUE MARC_test(VALUE self, VALUE str) {
    char *s;
    char t[100];
    s = StringValuePtr(str);
    memcpy(t, test_string(s), 99);

    return rb_str_new2(t);
}

/* Native C implementation */
static struct RECORD *next_marc(FILE *fp) {
    static long boff = 0;
    static struct RECORD rec;
    char lrecl[6];
    char *b;
    lrecl[5] = '\0';

    while((b = buf_read(fp, boff, 5)) != NULL) {
	    if (!isdigit(*b))
	       break;
	    memcpy(lrecl, b, 5);
	    rec.len = atoi(lrecl);
	    if (rec.len == 0) break;
	    if ((b = buf_read(fp, boff, rec.len)) == NULL) {
            rb_raise(rb_eIOError, "FATAL:  marc read failure (loc %08lx: length %d)",
		            boff, rec.len);
	        return(NULL);
	    }
	    memcpy(rec.txt, b, rec.len);
	    rec.txt[rec.len] = '\0';
	    boff += rec.len;
	    return(&rec);
	}

    boff=0;
    return((struct RECORD *)NULL);
}

static char *buf_read(FILE *fp, long off, int flen) {
    /************************************************************/
    /** Buffered read -- will handle high level (fseek, fread) **/
    /** read requests against a limited number (MAX_FILES) of  **/
    /** open files.                                            **/
    /************************************************************/
    static int num_files = 0;
    static FILE *FP[MAX_FILES];
    static char *ibuf[MAX_FILES];
    static long lo_off[MAX_FILES];
    static long hi_off[MAX_FILES];
    long end_off;
    char *p;
    int i, j;

    for (i = 0; i < num_files; i++) {
        if (FP[i] == fp)
            break;
    }
    if (i == num_files) {
        num_files++;
        FP[i] = fp;
        lo_off[i] = hi_off[i] = 0;
        if ((ibuf[i] = (char *) malloc(BBUFSZ)) == NULL) {
            rb_raise (rb_eNoMemError, "FATAL: buffer malloc failure");
            return(NULL);
        }
    }
    end_off = off + flen;
    if ((off >= lo_off[i]) && (end_off < hi_off[i])) {
        p = &ibuf[i][off - lo_off[i]];
    } else {
        if (fseek(FP[i], off, SEEK_SET) != 0) {
            rb_raise(rb_eIOError, "FATAL:  fseek error");
            return(NULL);
        }
        j = fread(ibuf[i], sizeof(char), BBUFSZ, FP[i]);
        if (j == 0) {
            p = NULL;
        } else if (j < flen) {
            lo_off[i] = off;
            hi_off[i] = off + j;
            p = &ibuf[i][0];
        } else {
            lo_off[i] = off;
            hi_off[i] = off + j;
            p = &ibuf[i][0];
        }
    }
    return (p);
}

static char *test_string(char *s) {
    snprintf(tt, sizeof(tt)-1, "Hello, %s!", s);
    return tt;
} 
