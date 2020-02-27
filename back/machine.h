typedef enum {
    MODE_START, //to begin with
    MODE_SINGLE, //single item to eval
    MODE_OP, //wait a op to come
	//there are several innate function
    MODE_CONS,
	MODE_CAR,
	MODE_CDR,
	MODE_DEFINE,

    MODE_FUNCTION
}ModeType;

struct Mode {
	 ModeType type;
	 int stage;
	 int total_stage;
};

typedef struct Mode Mode;

typedef enum {
    INSTR_PUSHI,
    INSTR_PUSHC,
    INSTR_OP,
    INSTR_DOL,
    INSTR_POP,
    INSTR_CHECK
} InstrType;



typedef struct Instruction {
    InstrType type;
    union {
        int constant;
        char *id;
    }ex1; 
} Instruction;

typedef enum {
    METHOD_PASS,
    METHOD_EVAL,
    METHOD_NOT_EVAL,
    METHOD_APP
} Method;