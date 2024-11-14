//ppvcu
#define MATCH_LDTILEA 0x300b
#define MASK_LDTILEA  0xfe00707f
#define MATCH_LDTILEB 0x800300b
#define MASK_LDTILEB  0xfe00707f
#define MATCH_LDTILEC 0x1000300b
#define MASK_LDTILEC  0xfe00707f
#define MATCH_LDTILED 0x1800300b
#define MASK_LDTILED  0xfe00707f
#define MATCH_LDTILEE 0x2000300b
#define MASK_LDTILEE  0xfe00707f
#define MATCH_LDTILEF 0x2800300b
#define MASK_LDTILEF  0xfe00707f
#define MATCH_LDTILEG 0x3000300b
#define MASK_LDTILEG  0xfe00707f
#define MATCH_LDTILEH 0x3800300b
#define MASK_LDTILEH  0xfe00707f
#define MATCH_LDTILEO 0x4000300b
#define MASK_LDTILEO  0xfe00707f
#define MATCH_AAMULA 0xa00200b
#define MASK_AAMULA  0xfe00707f
#define MATCH_AAMULB 0x1200200b
#define MASK_AAMULB  0xfe00707f
#define MATCH_AAMULC 0x1a00200b
#define MASK_AAMULC  0xfe00707f
#define MATCH_AAMULD 0x2200200b
#define MASK_AAMULD  0xfe00707f
#define MATCH_AAMULBC 0x2a00200b
#define MASK_AAMULBC  0xfe00707f
#define MATCH_TRIADDA 0xc00200b
#define MASK_TRIADDA  0xfe00707f
#define MATCH_TRIADDB 0x1400200b
#define MASK_TRIADDB  0xfe00707f
#define MATCH_OACC 0x1c00000b
#define MASK_OACC  0xfe00707f
#define MATCH_WBTILE 0x600200b
#define MASK_WBTILE  0xfe00707f
//mobile
#define MATCH_SETQ 0x90000037
#define MASK_SETQ  0xfe00707f
#define MATCH_ADDQ 0x10000037
#define MASK_ADDQ  0xfe00707f
#define MATCH_SUBQ 0x50000037
#define MASK_SUBQ  0xfe00707f
#define MATCH_MULQ 0x14000037
#define MASK_MULQ  0xfe00707f

//ppvcu
DECLARE_INSN(ldtilea, MATCH_LDTILEA, MASK_LDTILEA)
DECLARE_INSN(ldtileb, MATCH_LDTILEB, MASK_LDTILEB)
DECLARE_INSN(ldtilec, MATCH_LDTILEC, MASK_LDTILEC)
DECLARE_INSN(ldtiled, MATCH_LDTILED, MASK_LDTILED)
DECLARE_INSN(ldtilee, MATCH_LDTILEE, MASK_LDTILEE)
DECLARE_INSN(ldtilef, MATCH_LDTILEF, MASK_LDTILEF)
DECLARE_INSN(ldtileg, MATCH_LDTILEG, MASK_LDTILEG)
DECLARE_INSN(ldtileh, MATCH_LDTILEH, MASK_LDTILEH)
DECLARE_INSN(ldtileo, MATCH_LDTILEO, MASK_LDTILEO)
DECLARE_INSN(aamula, MATCH_AAMULA, MASK_AAMULA)
DECLARE_INSN(aamulb, MATCH_AAMULB, MASK_AAMULB)
DECLARE_INSN(aamulc, MATCH_AAMULC, MASK_AAMULC)
DECLARE_INSN(aamuld, MATCH_AAMULD, MASK_AAMULD)
DECLARE_INSN(aamulbc, MATCH_AAMULBC, MASK_AAMULBC)
DECLARE_INSN(triadda, MATCH_TRIADDA, MASK_TRIADDA)
DECLARE_INSN(triaddb, MATCH_TRIADDB, MASK_TRIADDB)
DECLARE_INSN(oacc, MATCH_OACC, MASK_OACC)
DECLARE_INSN(wbtile, MATCH_WBTILE, MASK_WBTILE)
//mobile
DECLARE_INSN(setq, MATCH_SETQ, MASK_SETQ)
DECLARE_INSN(addq, MATCH_ADDQ, MASK_ADDQ)
DECLARE_INSN(subq, MATCH_SUBQ, MASK_SUBQ)
DECLARE_INSN(mulq, MATCH_MULQ, MASK_MULQ)

//ppvcu and mobile
{"ldtilea",         0, INSN_CLASS_I, "d,s,t",     MATCH_LDTILEA, MASK_LDTILEA, match_opcode, 0 },
{"ldtileb",         0, INSN_CLASS_I, "d,s,t",     MATCH_LDTILEB, MASK_LDTILEB, match_opcode, 0 },
{"ldtilec",         0, INSN_CLASS_I, "d,s,t",     MATCH_LDTILEC, MASK_LDTILEC, match_opcode, 0 },
{"ldtiled",         0, INSN_CLASS_I, "d,s,t",     MATCH_LDTILED, MASK_LDTILED, match_opcode, 0 },
{"ldtilee",         0, INSN_CLASS_I, "d,s,t",     MATCH_LDTILEE, MASK_LDTILEE, match_opcode, 0 },
{"ldtilef",         0, INSN_CLASS_I, "d,s,t",     MATCH_LDTILEF, MASK_LDTILEF, match_opcode, 0 },
{"ldtileg",         0, INSN_CLASS_I, "d,s,t",     MATCH_LDTILEG, MASK_LDTILEG, match_opcode, 0 },
{"ldtileh",         0, INSN_CLASS_I, "d,s,t",     MATCH_LDTILEH, MASK_LDTILEH, match_opcode, 0 },
{"ldtileo",         0, INSN_CLASS_I, "d,s,t",     MATCH_LDTILEO, MASK_LDTILEO, match_opcode, 0 },
{"aamula",         0, INSN_CLASS_I, "d,s,t",     MATCH_AAMULA, MASK_AAMULA, match_opcode, 0 },
{"aamulb",         0, INSN_CLASS_I, "d,s,t",     MATCH_AAMULB, MASK_AAMULB, match_opcode, 0 },
{"aamulc",         0, INSN_CLASS_I, "d,s,t",     MATCH_AAMULC, MASK_AAMULC, match_opcode, 0 },
{"aamuld",         0, INSN_CLASS_I, "d,s,t",     MATCH_AAMULD, MASK_AAMULD, match_opcode, 0 },
{"aamulbc",         0, INSN_CLASS_I, "d,s,t",     MATCH_AAMULBC, MASK_AAMULBC, match_opcode, 0 },
{"triadda",         0, INSN_CLASS_I, "d,s,t",     MATCH_TRIADDA, MASK_TRIADDA, match_opcode, 0 },
{"triaddb",         0, INSN_CLASS_I, "d,s,t",     MATCH_TRIADDB, MASK_TRIADDB, match_opcode, 0 },
{"oacc",         0, INSN_CLASS_I, "d,s,t",     MATCH_OACC, MASK_OACC, match_opcode, 0 },
{"wbtile",         0, INSN_CLASS_I, "d,s,t",     MATCH_WBTILE, MASK_WBTILE, match_opcode, 0 },
{"setq",       0, INSN_CLASS_I, "d,s,t",  MATCH_SETQ, MASK_SETQ, match_opcode, 0 },
{"addq",       0, INSN_CLASS_I, "d,s,t",  MATCH_ADDQ, MASK_ADDQ, match_opcode, 0 },
{"subq",       0, INSN_CLASS_I, "d,s,t",  MATCH_SUBQ, MASK_SUBQ, match_opcode, 0 },
{"mulq",       0, INSN_CLASS_I, "d,s,t",  MATCH_MULQ, MASK_MULQ, match_opcode, 0 },