#define PERL_NO_GET_CONTEXT

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "ppport.h"

#include "src/gost.c"

typedef gost_ctx *Digest__GOST;

MODULE = Digest::GOST    PACKAGE = Digest::GOST

BOOT:
    gost_init_table();

PROTOTYPES: ENABLE

SV *
gost (...)
PREINIT:
    gost_ctx ctx;
    int i;
    unsigned char *data;
    unsigned char result[32];
    STRLEN len;
CODE:
    gost_init(&ctx);
    for (i = 0; i < items; i++) {
        data = (unsigned char *)(SvPV(ST(i), len));
        gost_update(&ctx, data, len);
    }
    gost_final(&ctx, result);
    RETVAL = newSVpv(result, 32);
OUTPUT:
    RETVAL

Digest::GOST
new (class)
    SV *class
CODE:
    Newx(RETVAL, 1, gost_ctx);
    gost_init(RETVAL);
OUTPUT:
    RETVAL

Digest::GOST
clone (self)
    Digest::GOST self
CODE:
    Newx(RETVAL, 1, gost_ctx);
    Copy(self, RETVAL, 1, gost_ctx);
OUTPUT:
    RETVAL

void
reset (self)
    Digest::GOST self
PPCODE:
    gost_init(self);
    XSRETURN(1);

void
add (self, ...)
    Digest::GOST self
PREINIT:
    int i;
    unsigned char *data;
    STRLEN len;
PPCODE:
    for (i = 1; i < items; i++) {
        data = (unsigned char *)(SvPV(ST(i), len));
        gost_update(self, data, len);
    }
    XSRETURN(1);

SV *
digest (self)
    Digest::GOST self
PREINIT:
    unsigned char result[32];
CODE:
    gost_final(self, result);
    RETVAL = newSVpv(result, 32);
OUTPUT:
    RETVAL

void
DESTROY (self)
    Digest::GOST self
CODE:
    Safefree(self);
