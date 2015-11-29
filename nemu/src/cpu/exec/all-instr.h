#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/xchg.h"
#include "data-mov/movzx.h"
#include "data-mov/movsx.h"
#include "data-mov/set.h"
#include "data-mov/pop.h"
#include "data-mov/cmovcc.h"

#include "arith/add.h"
#include "arith/adc.h"
#include "arith/sub.h"
#include "arith/sbb.h"
#include "arith/cmp.h"
#include "arith/call.h"
#include "arith/push.h"
#include "arith/test.h"
#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"

#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"

#include "string/rep.h"
#include "string/stos.h"
#include "string/cmps.h"
#include "string/movs.h"

#include "misc/misc.h"
#include "misc/jcc.h"
#include "misc/jmp.h"
#include "misc/leave.h"
#include "misc/ret.h"
#include "misc/ldt.h"

#include "special/special.h"
